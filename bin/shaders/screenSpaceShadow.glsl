#version 140
uniform vec3 lightPos[10];
uniform float radius[10];

uniform vec2 resolution;
uniform vec2 count;
uniform sampler2D depthReference;


void main()
{
	float ratio = resolution.x/resolution.y;
	
	// uv coordinate in device space
	vec2 posxy = gl_TexCoord[0].xy;
	
	
	// texel position in projection space
	float depthTexel = texture2D(depthReference, posxy).r; // shift depth a bit. 
	
	// skip backgroud or far away objects
	if(depthTexel <= 0.0001)
	{
		gl_FragColor = vec4(1.0);
	}
	else
	{
		vec3 texelPos = vec3(posxy, depthTexel);
	
		float prec = 0.00001;
		int count = int(count.y);
		vec3 lightP;
		vec3 rayDirection;
		float stepper = 0.0;
		float rayLength;
		float minx = 1.0 / resolution.x;
		float maxx = 1.0 - minx;
		float miny = 1.0 / resolution.y;
		float maxy = 1.0 - miny;
		
		// resulting shadow
		float shadow = 0.0;
		for(int i = 0 ; i < count ; i++)
		{
			// light position transformed to screen space
			lightP = lightPos[i];
			lightP.x /= ratio;
			lightP.y *= ratio;
			lightP.z *= ratio;
			float rad = radius[i] * ratio;
			
			vec3 sub = lightP - texelPos;
			
			// ray direction
			rayDirection = normalize(sub);
			
			// max ray travel
			rayLength = length(sub);
			
			// ray casting stepper
			stepper = 0.0;
			float stepSize = length(rayDirection) / resolution.x * 16.0;
			//float stepSize = length(rayDirection) / resolution.x;// * 8.0;
			
			// current shadow, for current light
			float shad = 0.0;
			while(stepper < 1.0)
			{
				vec3 currentPosition = texelPos + rayDirection * stepper;
				// out of bound
				if(currentPosition.x < minx || currentPosition.x > maxx || currentPosition.y < miny || currentPosition.y > maxy) break;
				
				// first check distance with light radius
				float distFromLight = length(currentPosition - lightP);
				if(distFromLight > rad) break;
				
				// read all depth required
				float currentDepth = texture2D(depthReference, currentPosition.xy).r;
				float expectedDepth = currentPosition.z;
				
				// shadow acquired
				float ftd = expectedDepth + prec;
				if(currentDepth > ftd)
				{
					if(stepper > stepSize)
					{
						// light source inside volume
						shad = 1.0;
					}
					else
					{
						shad = 0.0;
					}
					break;
				}
				
				stepper += stepSize;
			}
			shadow += shad;
		}
		//float steppervol = 1.0 / (1.0 + stepper * stepper * 0.1);
		gl_FragColor = vec4(vec3(1.0-shadow), 1.0);
		//gl_FragColor = vec4(vec3(rayDirection), 1.0);
		//gl_FragColor = vec4(vec3(length(posxy - lightP.xy) / 2.0),1.0);
		//gl_FragColor = vec4(posxy,0.1, 1.0);
		//gl_FragColor = vec4(depthTexel, 0.1,0.1, 1.0);
		//gl_FragColor = vec4(texelPos, 1.0);
	}
}