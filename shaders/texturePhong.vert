#version 430 core
precision highp float;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexture;

layout(location = 3) uniform mat4 modelViewProjectionMatrix;
layout(location = 7) uniform mat4 modelMatrix;
layout(location = 6) uniform mat3 normalMatrix;

layout(location = 4) uniform vec3 cameraPos;


smooth out vec2 texCoord;

smooth out vec3 normalVec;
smooth out vec3 lightVec;
smooth out vec3 cameraVec;


void main()
{
	vec3 lightPos = vec3(10,10,10);
	
	vec4 modelPosition = modelMatrix * vec4(vertexPosition,1.0);
	
	lightVec = normalize(lightPos - modelPosition.xyz);
	cameraVec = normalize(cameraPos - modelPosition.xyz);
	normalVec = normalize(normalMatrix * vertexNormal);
	
	gl_Position=modelViewProjectionMatrix*vec4(vertexPosition,1.0);
	texCoord=vertexTexture;
}