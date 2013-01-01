#version 430 core
precision highp float;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexture;

layout(location = 2) uniform mat4 modelViewProjectionMatrix;
layout(location = 3) uniform mat3 normalMatrix;

smooth out vec2 texCoord;
smooth out vec3 normalVec;

void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertexPosition,1.0);
	texCoord=vertexTexture;
	normalVec = normalize(normalMatrix * vertexNormal);
}