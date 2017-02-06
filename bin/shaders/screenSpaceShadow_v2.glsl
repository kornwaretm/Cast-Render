#version 140
uniform vec3 lightPos[10];
uniform float radius[10];
uniform sampler2D depthReference;// height texture
uniform vec2 foVCount;		 // camera field of view and light count
uniform vec2 resolution;
uniform vec3 cameraPosition;	// camera position
uniform mat4 cameraMatrix;		// invers camera matrix

float rayMarch(in vec3 origin,  in vec3 direction, in vec3 pos, in float radius)
{
	float stepper = 0.0;
	float distance = 10000.0;
	float prec = 0.02;
	for(int i=0; i<64; i++)
	{
		// calculate new position using step size
		vec3 current = origin + direction * stepper;
		
		distance = length(current - pos)-radius;
		
		// proceed steps
		stepper += distance * 0.25;
		
		if( distance < prec || distance > 32.0) break;
	}
	
	if( distance > 32.0)stepper = 32.0;
	return stepper;
}

float smin( float a, float b, float k )
{
    float res = exp( -k*a ) + exp( -k*b );
    return -log( res )/k;
}

void main()
{
	vec2 uv = gl_TexCoord[0].xy; 	 	// uv coordinate
	uv.x *= resolution.x/resolution.y; 	// repair aspec ratio
	
	vec2 uvrev = gl_TexCoord[0].xy * vec2(0.5) + vec2(0.5); // reference
	uvrev.y = 1.0 - uvrev.y;	// device space to texture space coord
	vec4 ref = texture2D(depthReference, uvrev); // sampling 
	float refdepth = ref.r;	// depth
	
	vec3 direction = vec3(uv, foVCount.x); // create ray direction by first build starting point from the current screen coordinate
	vec4 transformedDirection = normalize(cameraMatrix * vec4(direction, 0.0)); //transfrom to follow actual camera position
	
	
	// do the actual ray tracing, result are depth, distance from camera
	float result;
	for(int i = 0 ; i < foVCount.y ; i++)
	{
		vec3 pos = lightPos[i];
		float cr = rayMarch(cameraPosition, transformedDirection.xyz, pos, radius[i] * 2); // the actual ray marching
		if(i <= 0)
		{
			result = cr;
		}
		else
		{
			result = min(cr, result);
		}
	}

	// distance factor
	float vol = 1.0 - clamp(result / 32.0, 0.0, 1.0);
	float factor = max(refdepth, vol);
	float fvol = 1.0 - 1.0 / (1.0 + result * result * 0.1);//clamp(result / 32.0, 0.0, 1.0);// convert resulting position to a storable value
	
	//gl_FragColor = vec4(factor);
	if(refdepth >= factor) // compare depth with other existing object in the world
	{
		gl_FragColor = vec4(1.0);
	}
	else
	{
		gl_FragColor = vec4(fvol * fvol);// * result * 2.0);// - factor);// * vec4(1.0, 0.0, 0.0,1.0);
	}
}