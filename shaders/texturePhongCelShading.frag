#version 430 core
#define NBLIGHTS 10
precision highp float;


layout(binding = 0) uniform sampler2D textureMap;

layout(std140) uniform lights
{
  vec4 lightsPos[NBLIGHTS];
  vec4 lightsColors[NBLIGHTS];
};

smooth in vec2 texCoord;

smooth in vec3 normalVec;
smooth in vec4 lightVec[NBLIGHTS];
smooth in vec3 cameraVec;


layout(location = 0, index = 0) out vec4 fragColor;
layout(location = 1, index = 0) out vec4 fragNormal;

void main()
{
	vec3 lightVecN[NBLIGHTS];
	vec3 normalVecN = normalize(normalVec);
	vec3 cameraVecN = normalize(cameraVec);
	
	vec4 color=texture(textureMap,texCoord);

	vec4 al = vec4(0.5,0.5,0.5,1);
	vec4 dl = vec4(0,0,0,1);
	vec4 sl = vec4(0,0,0,1);
	
	for(int i = 0;i<NBLIGHTS;++i)
	{
	
	  if(lightVec[i][3] > 0)
	  { 
	    lightVecN[i]= normalize(lightVec[i].xyz);
	    
	    if((max(dot(lightVecN[i],normalVecN),0.0)*1 * lightVec[i][3])>0)
	    {
	      dl=dl+lightsColors[i]/5;
	    }
	    
	    if((pow(max(dot(-cameraVecN,reflect(lightVecN[i],normalVecN)),0.0),64)*8*lightVec[i][3])>=0.9)
	    {
		sl = lightsColors[i];
	    }
	    
	  }
	}
	
	fragColor=color*(al+dl+sl);
	fragNormal=vec4((normalVecN+1)/2.,1);
}
