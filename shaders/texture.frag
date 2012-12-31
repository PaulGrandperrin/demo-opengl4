#version 430 core
precision highp float;

layout(binding = 0) uniform sampler2D textureMap;

smooth in vec2 texCoord;

layout(location = 0, index = 0) out vec4 fragColor;
layout(location = 1, index = 0) out vec4 fragNormal;

void main()
{
	fragColor=texture(textureMap,texCoord);
	fragNormal=vec4(0,0,0,0);
}
