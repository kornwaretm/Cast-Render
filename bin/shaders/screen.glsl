#version 140
uniform sampler2D depthBuffer;
uniform sampler2D screenBuffer;
uniform sampler2D shadowBuffer;
uniform vec4 fogColor;

uniform float sampleCount;
uniform vec2 resolution;


void main()
{
	vec4 color = texture2D(screenBuffer, gl_TexCoord[0].xy);
	float depth = texture2D(depthBuffer, gl_TexCoord[0].xy).r;
	float fogFactor = clamp(pow(0.5 + depth, 4.0),0.0 , 1.0);
	
	vec4 shadows = vec4(0.0);
	
	if(sampleCount <= 0)
	{
		shadows = texture2D(shadowBuffer, gl_TexCoord[0].xy);
	}
	else
	{
		for(float s = 1.0 ; s <= sampleCount ; s ++)
		{
			float pixeldimx = 1.0 / resolution.x * s;
			float pixeldimy = 1.0 / resolution.y * s;
			
			vec2 tc0 = gl_TexCoord[0].xy; tc0.x += pixeldimx;
			vec2 tc1 = gl_TexCoord[0].xy; tc1.x -= pixeldimx;
			vec2 tc2 = gl_TexCoord[0].xy; tc2.y += pixeldimy;
			vec2 tc3 = gl_TexCoord[0].xy; tc3.y -= pixeldimy;
			
			vec4 sh0 = texture2D(shadowBuffer , tc0);
			vec4 sh1 = texture2D(shadowBuffer , tc1);
			vec4 sh2 = texture2D(shadowBuffer , tc2);
			vec4 sh3 = texture2D(shadowBuffer , tc3);
			
			shadows += (sh0 + sh1 + sh2 + sh3) / 4.0 * fogFactor;
		}
		shadows = clamp(shadows / sampleCount , vec4(0.0), vec4(1.0));
	}
	
	vec4 final = (vec4(0.75) + shadows * 0.25) * color;
	
	gl_FragColor = (final * fogFactor) + (fogColor * (1.0 - fogFactor));
	gl_FragColor.a = 1.0;
}