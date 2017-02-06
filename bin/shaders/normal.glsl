#version 140
uniform sampler2D screenBuffer; // screen buffer
uniform vec2 resolution;		// resolution
uniform float sampleCount;


void main()
{
	// multy sampling
	vec3 d = vec3(0.0);
	for(float s = 1.0 ; s <= sampleCount ; s ++)
	{
		float pixeldimx = 1.0 / resolution.x * s;
		float pixeldimy = 1.0 / resolution.y * s;
		
		vec2 tc0 = gl_TexCoord[0].xy; tc0.x += pixeldimx;
		vec2 tc1 = gl_TexCoord[0].xy; tc1.x -= pixeldimx;
		vec2 tc2 = gl_TexCoord[0].xy; tc2.y += pixeldimy;
		vec2 tc3 = gl_TexCoord[0].xy; tc3.y -= pixeldimy;
		
		float h0 = texture2D(screenBuffer , tc0).r;
		float h1 = texture2D(screenBuffer , tc1).r;
		float h2 = texture2D(screenBuffer , tc2).r;
		float h3 = texture2D(screenBuffer , tc3).r;
		
		/*h0 = 1.0 / (1.0 + h0 * h0 * 0.1);
		h1 = 1.0 / (1.0 + h1 * h1 * 0.1);
		h2 = 1.0 / (1.0 + h2 * h2 * 0.1);
		h3 = 1.0 / (1.0 + h3 * h3 * 0.1);*/
		
		d +=  vec3(h1-h0, 2 * pixeldimy, h3-h2);
	}
    vec3 n = normalize(d / sampleCount);
	
	gl_FragColor = vec4(n, 1.0);
}