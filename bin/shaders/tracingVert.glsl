#version 140
uniform mat4 cameraMatrix;		// invers camera matrix
uniform vec2 resolution;		// screen resolution

varying vec3 rayDirection;		// ray direction
varying vec2 uvrev;


void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vec2 screenUv = gl_MultiTexCoord0.xy;
	screenUv.x *= resolution.x/resolution.y;// repair aspec ratio
	
	uvrev = gl_MultiTexCoord0.xy * vec2(0.5) + vec2(0.5);
	uvrev.y = 1.0 - uvrev.y;
	
	// create a ray
	vec3 direction = vec3(screenUv, 1.0);
	vec4 transformedDirection = normalize(cameraMatrix * vec4(direction, 0.0));
	rayDirection = transformedDirection.xyz;
	
	gl_Position = gl_Vertex;
	
	
}
