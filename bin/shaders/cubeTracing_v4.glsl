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

float projectToPlane(in vec3 p, vec3 norm , in vec3 size, in vec3 or, vec3 dir)
{
	vec3 A = or + p.xyz;
	vec3 B = A + dir;
	float dist = dot(norm, - A) / dot(norm, (B - A));
	vec3 final = A + dir * dist;
	
	if(final.x > size.x )return 32.0;
	if(final.x < -size.x)return 32.0;
	if(final.y > size.y )return 32.0;
	if(final.y < -size.y)return 32.0;
	if(final.z > size.z )return 32.0;
	if(final.z < -size.z)return 32.0;
	
	return dist;
}

float projectToSphere(vec3 pos, vec3 ro, vec3 dir, float r)
{
    vec3 oc = ro - pos;
    float b = dot( oc, dir );
    float c = dot( oc, oc ) - r * r;//( - r * r)
    float h = b*b - c;
    if( h<0.0 ) return -1.0;
    h = sqrt( h );
    return -b - h;
}

float rayTrace(in vec3 origin,  in vec3 direction, in vec4 pos, out vec2 uv)
{
	
	vec3 dir = normalize((vec4(direction, 0.1) * invObjectTransform).xyz);
	vec3 or = ((vec4(origin, 0.1) * invObjectTransform).xyz);
	vec3 size = objectScale;
	vec3 p = pos.xyz;
	float dist = projectToSphere(p, or, dir, 1.0);
	if( dist < 0.0 )return 32.0; // fail
	
	vec3 final = or + dir * dist;
	uv.x = 0.5 + atan(final.z / final.x) / 6.283185307179586476925286766559 * 2.0;
	uv.y = clamp(final.y * 0.5 + 0.5, 0.0,1.0);
	
	// red assist
	float nr = texture2D(tex0, uv).r;
	dist += nr; ///projectToSphere(p, or, dir, nr);
	final = or + dir * dist;
	uv.x = 0.5 + atan(final.z / final.x) / 6.283185307179586476925286766559 * 2.0;
	uv.y = clamp(final.y * 0.5 + 0.5, 0.0,1.0);
	
	/*
	// green assist
	nr -= texture2D(tex0, uv).g;
	dist = projectToSphere(p, or, dir, nr);
	final = or + dir * dist;
	uv.x = 0.5 + atan(final.z / final.x) / 6.283185307179586476925286766559 * 2.0;
	uv.y = clamp(final.y * 0.5 + 0.5, 0.0,1.0);
	
	// blue assist
	nr -= texture2D(tex0, uv).b;
	dist = projectToSphere(p, or, dir, nr);
	final = or + dir * dist;
	uv.x = 0.5 + atan(final.z / final.x) / 6.283185307179586476925286766559 * 2.0;
	uv.y = clamp(final.y * 0.5 + 0.5, 0.0,1.0);
	uv *= nr;*/
	
	/*
	// alpha assist
	nr -= (texture2D(tex0, uv).a - 0.5) * 2.0;
	dist = projectToSphere(p, or, dir, nr);
	final = or + dir * dist;
	uv.x = 0.5 + atan(final.z / final.x) / 6.283185307179586476925286766559 * 2.0;
	uv.y = clamp(final.y * 0.5 + 0.5, 0.0,1.0);*/
	
	
	if( dist > 32.0)dist = 32.0;
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
	
	//if(refdepth > vol) // compare depth with other existing object in the world
	//{
	//	gl_FragColor = ref;
	//}
	//else
	//{
		vec4 finalColor = col * col.a + refcolor * (1.0 - col.a);
		float colorFloat = DecodeFloatRGBA(finalColor);
		float newDepth = vol * col.a + refdepth * (1.0 - col.a);
		gl_FragColor = vec4(newDepth ,colorFloat , 0.0, 0.0);
	//}
}