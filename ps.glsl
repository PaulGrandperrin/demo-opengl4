#version 430 core
precision highp float;
smooth in vec3 normal;
smooth in vec2 texPos;
//uniform vec3 lightDir,cameraDir;
layout(binding = 0) uniform sampler2D textureMap;

out vec4 fragColor;

void main()
{
	vec3 lightDir= vec3(1.0,1.0,0);
	vec3 cameraDir = vec3(0,1.0,1.0);
	vec4 color=texture(textureMap,texPos); //vec4(normalize(normal),1.0)
	vec3 normal=normalize(normal);

	float al=0.2;
	float dl= max(dot(lightDir,normal),0.0)*1.5;
	float sl= pow(max(dot(-normalize(cameraDir),reflect(-lightDir,normal)),0.0),16)*32;


	fragColor=color;//*(al+dl+sl);
}
