#version 130
uniform vec3 lightColor[10];
uniform vec3 lightPos[10];
uniform float radius[10];
uniform vec2 lightCount;

uniform sampler2D colorDepth;
uniform sampler2D normalTex;
uniform vec3 cameraPosition;
uniform vec2 resolution;
uniform float debugBuffers;
//uniform vec4 fogColor;
uniform vec4 ambientColor;

vec4 EncodeFloatRGBA( float v ) {
  vec4 enc = vec4(1.0, 255.0, 65025.0, 160581375.0) * v;
  enc = fract(enc);
  enc -= enc.yzww * vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0);
  return enc;
}
/*
float DecodeFloatRGBA( vec4 rgba ) {
  return dot( rgba, vec4(1.0, 1/255.0, 1/65025.0, 1/160581375.0) );
}*/

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	vec4 colorRaw = texture2D(colorDepth, uv);
	vec4 color = EncodeFloatRGBA(colorRaw.g);
	float depth = colorRaw.r;
	vec4 normal = texture2D(normalTex, uv);
	
	uv.x *= resolution.x/resolution.y;
	vec3 position = vec3(uv, depth);
	
	vec3 lighting = color.xyz * ambientColor.xyz;
	
	float shininess = 50.0;
	
	// loop for lights
	int count = int(lightCount.x);
	if(count > 0.0)
	for(int i = 0 ; i < count ; i++)
	{
		vec3 lp = lightPos[i];
		vec3 lightDir = normalize(position - lp);
		vec3 reflectVec = normalize(-reflect(lightDir, normal.xyz));
		
		vec3 diffuse = max(dot(normal.xyz, lightDir), 0.0) * color.xyz * lightColor[i];
		vec3 specular = lightColor[i] * pow(max(dot(reflectVec, lightDir), 0.0), shininess);
		
		float factor = radius[i] / length(position - lp);
		lighting += (diffuse + specular) * factor;
	}
	
	
	//float fogFactor = clamp(pow(0.5 + depth, 4.0),0.0 , 1.0);
	//lighting = (fogColor.rgb * (1.0 - fogFactor)) + (lighting * fogFactor);
	
	if(debugBuffers > 0)
    {
		if(gl_TexCoord[0].x < 0.5)
		{
			if(gl_TexCoord[0].y < 0.5)
			{
				gl_FragColor = vec4(normal.xyz, 1.0);
				
			}
			else
			{
				gl_FragColor = vec4(vec3(depth), 1.0);
			}
		}
		else
		{
			if(gl_TexCoord[0].y < 0.5)
			{
				gl_FragColor = vec4(lighting, 1.0);
			}
			else
			{
				gl_FragColor = vec4(color.xyz, 1.0);
				
			}
		}
	}
	else
	{
		gl_FragColor = vec4(lighting, 1.0);
	}
}