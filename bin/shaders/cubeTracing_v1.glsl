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

float map(vec3 position, out vec2 uv)
{
	uv = position.xz * 0.5 + vec2(0.5);
	uv.x = 1.0 - uv.x;
	uv = uv * uvMultiplier;
	
	float height = texture2D(tex0, uv).r;
	vec3 cubeDimension = vec3(1.0, height,1.0);
	return length(max(abs(position) - cubeDimension, 0.0));
}

float rayTrace(in vec4 origin,  in vec4 direction, in vec4 pos, out vec2 uv)
{
	float stepper = 0.0;
	float dist;
	for(int i=0; i<64; i++)
	{
		// calculate new position using step size
		vec4 current = origin + direction * stepper;
		
		// transform position to inverse object matrix
		vec4 tposition = (current + pos) * invObjectTransform / objectScale;
		
		// actual form calculation
		dist = map( tposition.xyz  , uv);
		
		// proceed steps
		stepper += dist * 0.5;
		
		if( dist > 32.0) return 32.0;
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
	vec4 pos = -vec4(objectPosition,1.0); // flipped position, so compiler can use addmull
	float result = rayTrace(vec4(cameraPosition, 1.0), transformedDirection, pos, uvtex); // the actual ray marching
	
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
		//finalColor *= 0.5;
		float colorFloat = DecodeFloatRGBA(finalColor);
		float newDepth = vol * col.a + refdepth * (1.0 - col.a);
		gl_FragColor = vec4(newDepth ,colorFloat , 0.0, 0.0);
	}
}