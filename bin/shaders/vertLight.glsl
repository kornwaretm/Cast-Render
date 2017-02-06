#version 140

uniform vec3 lightColor[2];
uniform vec3 lightPos[2];


void main()
{
	gl_Position = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
