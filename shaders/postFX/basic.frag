#version 430 core
precision highp float;

layout(binding = 0) uniform sampler2D colorTex;
layout(binding = 1) uniform sampler2D normalTex;
layout(binding = 2) uniform sampler2D depthTex;

smooth in vec2 texCoord;

out vec4 fragColor;

void main()
{
    fragColor=texture(colorTex, texCoord);
}

