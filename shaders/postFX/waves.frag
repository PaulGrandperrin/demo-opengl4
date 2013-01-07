#version 430 core
#define ZNEAR 0.1f
#define ZFAR 1000.f
precision highp float;


layout(binding = 0) uniform sampler2D colorTex;
layout(binding = 1) uniform sampler2D normalTex;
layout(binding = 2) uniform sampler2D depthTex;

layout(location = 5) uniform float time;

smooth in vec2 texCoord;

out vec4 fragColor;


void main()
{
	float size = 0.05;
	float strength= 0.5;
	fragColor=texture2D(colorTex,vec2(texCoord.s+strength*size*sin(texCoord.t*1./size+time/(1./size)),texCoord.t+strength*size*cos(texCoord.s*1./size+time/(1./size))));
}
