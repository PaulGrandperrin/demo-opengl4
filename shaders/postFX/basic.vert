#version 430 core

precision highp float;

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 vertexCoord;


smooth out vec2 texCoord;

void main()
{
	gl_Position=vec4(vertexPosition,1);
	texCoord=vertexCoord;
}