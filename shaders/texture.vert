#version 430 core
precision highp float;

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 vertexTexture;

layout(location = 2) uniform mat4 modelViewProjectionMatrix;

smooth out vec2 texCoord;

void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertexPosition,1.0);
	texCoord=vertexTexture;
}