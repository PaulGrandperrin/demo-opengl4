#version 430 core
#define ZNEAR 0.1f
#define ZFAR 1000.f
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

float sobelDepth(float step)
{
	float step_h = step;
	float step_v = step;
  
	float a = texture(depthTex, texCoord + vec2(- step_h, - step_v)).r;
	float b = texture(depthTex, texCoord + vec2(0.0     , - step_v)).r;
	float c = texture(depthTex, texCoord + vec2(+ step_h, - step_v)).r;
	float d = texture(depthTex, texCoord + vec2(- step_h, 0.0     )).r;
	float f = texture(depthTex, texCoord + vec2(+ step_h, 0.0     )).r;
	float g = texture(depthTex, texCoord + vec2(- step_h, + step_v)).r;
	float h = texture(depthTex, texCoord + vec2(0.0     , + step_v)).r;
	float i = texture(depthTex, texCoord + vec2(+ step_h, + step_v)).r;

	float fx = (- 1.0 * a + 1.0 * c - 2.0 * d + 2.0 * f - 1.0 * g + 1.0 * i) / 4.0;
	float fy = (+ 1.0 * a + 2.0 * b + 1.0 * c - 1.0 * g - 2.0 * h - 1.0 * i) / 4.0;
	
	return sqrt(fx * fx + fy * fy);
}

float sobelNormal(float step)
{
	float step_h = step;
	float step_v = step;

	vec3 a = texture(normalTex, texCoord + vec2(- step_h, - step_v)).rgb;
	vec3 b = texture(normalTex, texCoord + vec2(0.0     , - step_v)).rgb;
	vec3 c = texture(normalTex, texCoord + vec2(+ step_h, - step_v)).rgb;
	vec3 d = texture(normalTex, texCoord + vec2(- step_h, 0.0     )).rgb;
	vec3 f = texture(normalTex, texCoord + vec2(+ step_h, 0.0     )).rgb;
	vec3 g = texture(normalTex, texCoord + vec2(- step_h, + step_v)).rgb;
	vec3 h = texture(normalTex, texCoord + vec2(0.0     , + step_v)).rgb;
	vec3 i = texture(normalTex, texCoord + vec2(+ step_h, + step_v)).rgb;

	vec3 fx = (- 1.0 * a + 1.0 * c - 2.0 * d + 2.0 * f - 1.0 * g + 1.0 * i) / 4.0;
	vec3 fy = (+ 1.0 * a + 2.0 * b + 1.0 * c - 1.0 * g - 2.0 * h - 1.0 * i) / 4.0;
	
	vec3 edge = sqrt(fx * fx + fy * fy);
	
	return edge.r + edge.g + edge.b;
}


void main()
{
	
	if(sobelDepth(0.003) > 0.5 || sobelDepth(0.001) > 0.003 || sobelNormal(0.001) > 0.5)
	{
	  fragColor=vec4(0, 0, 0, 1);
	}
	
	else
	{
	  fragColor=texture(colorTex, texCoord);
	}
}
