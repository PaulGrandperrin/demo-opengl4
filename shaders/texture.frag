#version 430 core
precision highp float;

layout(binding = 0) uniform sampler2D textureMap;

smooth in vec2 texCoord;

out vec4 fragColor;

void main()
{
	fragColor=texture(textureMap,texCoord);
}
