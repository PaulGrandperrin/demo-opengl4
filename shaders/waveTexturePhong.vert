#version 430 core
#define NBLIGHTS 2

precision highp float;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexture;

layout(location = 2) uniform mat4 modelViewProjectionMatrix;
layout(location = 0) uniform mat4 modelMatrix;
layout(location = 3) uniform mat3 normalMatrix;

layout(location = 4) uniform vec3 cameraPos;
layout(location = 5) uniform float time;

layout(std140) uniform lights
{
  vec4 lightsPos[NBLIGHTS];
  vec4 lightsColors[NBLIGHTS];
};

smooth out vec2 texCoord;

smooth out vec3 normalVec;
smooth out vec4 lightVec[NBLIGHTS];
smooth out vec3 cameraVec;


void main()
{
	vec3 pos = vertexPosition;
	pos.y = sin(pos.z*100.0+time)/100.0;
	
	vec3 norm = vertexNormal;
	norm.y = 1;
	norm.x = 0;//cos(pos.x*10.0+time) * norm.x;
	norm.z = cos(pos.z*100.0+time);
	norm=normalize(norm);
	
	vec4 modelPosition = modelMatrix * vec4(pos,1.0);
	
	cameraVec = normalize(cameraPos - modelPosition.xyz);
	normalVec = normalize(normalMatrix * norm);
	
	for(int i=0; i<NBLIGHTS; ++i)
	{
	  lightVec[i] = vec4(normalize(lightsPos[i].xyz - modelPosition.xyz), max(0,lightsPos[i][3]-distance(lightsPos[i].xyz,modelPosition.xyz))/lightsPos[i][3]);
	}
	
	
	
	gl_Position=modelViewProjectionMatrix*vec4(pos,1.0);
	texCoord=vertexTexture;
}