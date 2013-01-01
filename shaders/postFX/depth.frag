#version 430 core
#define ZNEAR 0.1f
#define ZFAR 500.f
precision highp float;


layout(binding = 0) uniform sampler2D colorTex;
layout(binding = 1) uniform sampler2D normalTex;
layout(binding = 2) uniform sampler2D depthTex;

smooth in vec2 texCoord;

out vec4 fragColor;

float linearDepth(float depth)
{
    return (2.0 * ZNEAR) / (ZFAR + ZNEAR - depth * (ZFAR - ZNEAR));
}

void main()
{
	float d= linearDepth(texture(depthTex, texCoord));
	fragColor= vec4(d,d,d,1);
}
