#version 130

// constants
#define pi 3.1415926535897932384626433832795
#define pi2 6.283185307179586476925286766559

// one zero half neghalf
const vec4 ozhn = vec4(1.0, 0.0, 0.5, -0.5);

uniform vec4 objectQuaternion[200];// object transformation inversed
uniform vec3 objectScale[200];		// object scale
uniform vec3 objectPosition[200];	// object position
uniform vec2 uvMultiplier[200];		// object uv scalling
uniform vec2 uvShift[200];			// object uv shifting
uniform int objectType[200];		// object types
uniform float objectCount;			// number of composited object

uniform vec2 resolution;		// screen resolution
uniform vec3 cameraPosition;	// camera position
uniform mat4 cameraMatrix;		// invers camera matrix

uniform sampler2D depthReference;// height texture
uniform sampler2D tex0;			 // height texture
uniform sampler2D tex1;			 // color texture
uniform float cameraFoV;		 // camera field of view

vec4 EncodeFloatRGBA(in  float v ) {
  vec4 enc = vec4(1.0, 255.0, 65025.0, 160581375.0) * v;
  enc = fract(enc);
  enc -= enc.yzww * vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0);
  return enc;
}

float DecodeFloatRGBA(in  vec4 rgba ) {
  return dot( rgba, vec4(1.0, 1/255.0, 1/65025.0, 1/160581375.0) );
}

float mapSphere(in vec3 position, out vec2 uv, in vec2 uvmul, in vec2 uvsh)
{
	float spherex = 1.0 - atan(position.x / position.y) / pi;
	uv = vec2(spherex, position.z) +  ozhn.ww;
	uv = uv * uvmul + uvsh;
	
	float radius = texture2D(tex0, uv).r;
	return length(position)-radius;
}

float mapCilinder(in vec3 position, out vec2 uv, in vec2 uvmul, in vec2 uvsh)
{
	
	uv.x = atan(position.x / position.z) / pi2;
	uv.y = clamp(position.y * 0.5 + 0.5, 0.0,1.0);
	if(position.z > 0.0) uv.x += 0.5;
	uv.x = 1.0 - uv.x;
	if(uv.x < 0.0)uv.x += 1.0;
	if(uv.y < 0.0)uv.y += 1.0;
	if(uv.x > 1.0)uv.x -= 1.0;
	if(uv.y > 1.0)uv.y -= 1.0;
	uv = uv * uvmul + uvsh;
	
	float radius = texture2D(tex0, uv).r;
	vec2 d = abs(vec2(length(position.xz),position.y )) - vec2(radius, 1.0);
	return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float mapCube(in vec3 position, out vec2 uv, in vec2 uvmul, in vec2 uvsh, in vec3 scale)
{
	uv = (position.xz / scale.xz) * 0.5 + vec2(0.5);
	uv.x = 1.0 - uv.x;
	uv = uv * uvmul + uvsh;
	
	float height = texture2D(tex0, uv).r * scale.y;
	vec3 cubeDimension = vec3(scale.x, height,scale.z);
	return length(max(abs(position) - cubeDimension, 0.0));
}

vec3 rotateVecQuat( in vec4 quat, in vec3 vec )
{
	return vec + 2.0 * cross( cross( vec, quat.xyz ) + quat.w * vec, quat.xyz );
}

float rayTrace(in vec3 origin,  in vec3 direction, out vec2 uv)
{
	float stepper = 0.0;
	float dist;float nd;
	
	vec2 cuv;
	for(int i=0; i<128; i++)
	{
		// calculate new position using step size
		vec3 current = origin + direction * stepper;
		for(int c = 0 ; c < objectCount ; c ++)
		{
			
			int type = objectType[c];
			// cube
			if(type == 0)
			{
				vec3 pos = current - objectPosition[c];
				pos = rotateVecQuat(objectQuaternion[c], pos);
				nd = mapCube( pos, cuv, uvMultiplier[c], uvShift[c], objectScale[c]);
			}
			// cilinder
			else if(type == 1)
			{
				vec3 pos = current - objectPosition[c];
				pos = rotateVecQuat(objectQuaternion[c], pos) / objectScale[c];
				nd = mapCilinder( pos, cuv, uvMultiplier[c], uvShift[c]);
			}
			// sphere
			else if(type == 2)
			{
				vec3 pos = current - objectPosition[c];
				pos = rotateVecQuat(objectQuaternion[c], pos) / objectScale[c];
				nd = mapSphere( pos, cuv, uvMultiplier[c], uvShift[c]);
			}
			
			if(c == 0)
			{
				dist = nd;
				uv = cuv;
			}
			else
			{
				if(nd <= dist)uv = cuv;
			}
			// union operation
			dist = min(nd,dist);
		}
		
		// proceed steps
		stepper += dist * 0.25;
		
		if(stepper < 0.1 || stepper > 32.0) break;
	}
	// far radius
	if( dist > 32.0)stepper = 32.0;
	
	return stepper;
}

void main() 
{
	vec2 uv = gl_TexCoord[0].xy; 	 	// uv coordinate
	uv.x *= resolution.x/resolution.y; 	// repair aspec ratio
	
	vec2 uvrev = gl_TexCoord[0].xy * vec2(0.5) + vec2(0.5); // reference
	uvrev.y = 1.0 - uvrev.y;	// device space to texture space coord
	vec4 ref = texture2D(depthReference, uvrev); // sampling 
	float refdepth = ref.r;	// depth
	vec4 refcolor = EncodeFloatRGBA(ref.g); // encode to real colors
	
	vec3 direction = vec3(uv, cameraFoV); // create ray direction by first build starting point from the current screen coordinate
	vec4 transformedDirection = normalize(cameraMatrix * vec4(direction, 0.0)); //transfrom to follow actual camera position
	
	
	// do the actual ray tracing, result are depth, distance from camera
	vec2 uvtex = vec2(0.0); // this variable used to store the resulting uv coordinate
	float result = rayTrace(cameraPosition, transformedDirection.xyz, uvtex); // the actual ray marching
	
	// distance factor
	float vol = 1.0 - clamp(result / 32.0, 0.0, 1.0);//1.0 / (1.0 + result * result * 0.1); // convert resulting position to a storable value
	
	// color from texture
	vec4 col = texture2D(tex1, uvtex); // sampling texture to get the real color of the intersection point
	
	if(refdepth > vol) // compare depth with other existing object in the world
	{
		gl_FragColor = ref;
	}
	else
	{
		vec4 finalColor = mix(refcolor, col, col.a);
		float colorFloat = DecodeFloatRGBA(finalColor);
		float newDepth = mix(refdepth, vol, col.a);
		gl_FragColor = vec4(newDepth ,colorFloat , 0.0, 0.0);
	}
}