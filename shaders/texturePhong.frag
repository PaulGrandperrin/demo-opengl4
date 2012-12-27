#version 430 core
precision highp float;


layout(binding = 0) uniform sampler2D textureMap;

smooth in vec2 texCoord;

smooth in vec3 normalVec;
smooth in vec3 lightVec;
smooth in vec3 cameraVec;

out vec4 fragColor;

void main()
{
	vec3 lightVecN = normalize(lightVec);
	vec3 normalVecN = normalize(normalVec);
	vec3 cameraVecN = normalize(cameraVec);
	
	vec4 color=texture(textureMap,texCoord);

	float al=0.1;
	float dl= max(dot(lightVecN,normalVecN),0.0)*1;
	float sl= pow(max(dot(-cameraVecN,reflect(lightVecN,normalVecN)),0.0),64)*8;


	fragColor=vec4(color*(al+dl+sl));
	//fragColor = vec4(normalVecN,1);
	//fragColor = vec4(lightVecN,1);
}
