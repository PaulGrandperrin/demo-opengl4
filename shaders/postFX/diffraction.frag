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
	float strength= 0.01;
	fragColor.r=texture2D(colorTex,vec2(texCoord.s-strength,texCoord.t)).r;
	fragColor.g=texture2D(colorTex,vec2(texCoord.s,texCoord.t)).g;
	fragColor.b=texture2D(colorTex,vec2(texCoord.s+strength,texCoord.t)).b;
	fragColor.a = 1.;
}
