#version 140
uniform mat4 invObjectTransform;// object transformation inversed
uniform vec3 objectScale;		// object scale
uniform vec3 objectPosition;	// object position
uniform vec2 uvMultiplier;		// object uv scalling

uniform vec2 resolution;		// screen resolution
uniform vec3 cameraPosition;	// camera position
uniform mat4 cameraMatrix;		// invers camera matrix

uniform sampler2D depthReference;// height texture
uniform sampler2D tex0;			 // height texture
uniform sampler2D tex1;			 // color texture
uniform float cameraFoV;		 // camera field of view


vec4 EncodeFloatRGBA( float v ) {
  vec4 enc = vec4(1.0, 255.0, 65025.0, 160581375.0) * v;
  enc = fract(enc);
  enc -= enc.yzww * vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0);
  return enc;
}

float DecodeFloatRGBA( vec4 rgba ) {
  return dot( rgba, vec4(1.0, 1/255.0, 1/65025.0, 1/160581375.0) );
}

// exponential smooth min (k = 32);
float smin( float a, float b, float k )
{
    float res = exp( -k*a ) + exp( -k*b );
    return -log( res )/k;
}


float rayTrace(in vec3 origin,  in vec3 direction, in vec4 pos, out vec2 uv)
{
	vec3 dir = normalize((vec4(direction, 0.1) * invObjectTransform).xyz);
	vec3 or = ((vec4(origin, 0.1) * invObjectTransform).xyz);
	vec3 norm = vec3(0.0, 1.0, 0.0);
	vec3 final;
	float error = 32.0;
	float minerror = 32.0;
	float dist = 32.0;
	float fdist = 32.0;
	float stc = objectScale.y / 0.05; // step count
	float mx = objectScale.y; // max y
	float sts = mx / stc; // step size;
	float cy;
	vec3 p = pos.xyz;

	vec3 A = or + p.xyz;
	vec3 B = A + dir;
	dist = dot(norm, - A) / dot(norm, (B - A));
	final = A + dir * dist;
	vec2 cuv = (final.xz / objectScale.xz) * 0.5 - vec2(0.5);
	uv = cuv;
	
	if(final.x > objectScale.x ) {dist = 32.0;}
	if(final.x < -objectScale.x) {dist = 32.0;}
	if(final.z > objectScale.z ) {dist = 32.0;}
	if(final.z < -objectScale.z) {dist = 32.0;}
		
	return dist;
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
	vec4 pos = -vec4(objectPosition,1.0); // flipped position, so compiler can use addmull
	float result = rayTrace(cameraPosition, transformedDirection.xyz, pos, uvtex); // the actual ray marching
	
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
		vec4 finalColor = col * col.a + refcolor * (1.0 - col.a);
		float colorFloat = DecodeFloatRGBA(finalColor);
		float newDepth = vol * col.a + refdepth * (1.0 - col.a);
		gl_FragColor = vec4(newDepth ,colorFloat , 0.0, 0.0);
	}
}