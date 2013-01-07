#version 430 core
#define ZNEAR 0.1f
#define ZFAR 1000.f
precision highp float;


layout(binding = 0) uniform sampler2D colorTex;
layout(binding = 1) uniform sampler2D normalTex;
layout(binding = 2) uniform sampler2D depthTex;

layout(location = 5) uniform float time;
layout(location = 6) uniform float height;
layout(location = 7) uniform float width;
layout(location = 8) uniform float param1;

smooth in vec2 texCoord;

out vec4 fragColor;

const float blurclamp = 6.0;  // max blur amount
const float bias = 4;	      //aperture - bigger values for shallower depth of field

void main()
{
	float aspectratio = width/height;
	vec2 aspectcorrect = vec2(1.0,aspectratio);
	
	vec4 depth1   = texture2D(depthTex,texCoord);

	float factor = ( depth1.x - param1/10);
	 
	vec2 dofblur = vec2 (clamp( factor * bias, -blurclamp, blurclamp ));


	vec4 col = vec4(0.0);
	
	col += texture2D(colorTex, texCoord);
	col += texture2D(colorTex, texCoord + (vec2( 0.0,0.4 )*aspectcorrect) * dofblur);
	col += texture2D(colorTex, texCoord + (vec2( 0.15,0.37 )*aspectcorrect) * dofblur);
	col += texture2D(colorTex, texCoord + (vec2( 0.29,0.29 )*aspectcorrect) * dofblur);
	col += texture2D(colorTex, texCoord + (vec2( -0.37,0.15 )*aspectcorrect) * dofblur);	
	col += texture2D(colorTex, texCoord + (vec2( 0.4,0.0 )*aspectcorrect) * dofblur);	
	col += texture2D(colorTex, texCoord + (vec2( 0.37,-0.15 )*aspectcorrect) * dofblur);	
	col += texture2D(colorTex, texCoord + (vec2( 0.29,-0.29 )*aspectcorrect) * dofblur);	
	col += texture2D(colorTex, texCoord + (vec2( -0.15,-0.37 )*aspectcorrect) * dofblur);
	col += texture2D(colorTex, texCoord + (vec2( 0.0,-0.4 )*aspectcorrect) * dofblur);	
	col += texture2D(colorTex, texCoord + (vec2( -0.15,0.37 )*aspectcorrect) * dofblur);
	col += texture2D(colorTex, texCoord + (vec2( -0.29,0.29 )*aspectcorrect) * dofblur);
	col += texture2D(colorTex, texCoord + (vec2( 0.37,0.15 )*aspectcorrect) * dofblur);	
	col += texture2D(colorTex, texCoord + (vec2( -0.4,0.0 )*aspectcorrect) * dofblur);	
	col += texture2D(colorTex, texCoord + (vec2( -0.37,-0.15 )*aspectcorrect) * dofblur);	
	col += texture2D(colorTex, texCoord + (vec2( -0.29,-0.29 )*aspectcorrect) * dofblur);	
	col += texture2D(colorTex, texCoord + (vec2( 0.15,-0.37 )*aspectcorrect) * dofblur);
	
	col += texture2D(colorTex, texCoord + (vec2( 0.15,0.37 )*aspectcorrect) * dofblur*0.9);
	col += texture2D(colorTex, texCoord + (vec2( -0.37,0.15 )*aspectcorrect) * dofblur*0.9);		
	col += texture2D(colorTex, texCoord + (vec2( 0.37,-0.15 )*aspectcorrect) * dofblur*0.9);		
	col += texture2D(colorTex, texCoord + (vec2( -0.15,-0.37 )*aspectcorrect) * dofblur*0.9);
	col += texture2D(colorTex, texCoord + (vec2( -0.15,0.37 )*aspectcorrect) * dofblur*0.9);
	col += texture2D(colorTex, texCoord + (vec2( 0.37,0.15 )*aspectcorrect) * dofblur*0.9);		
	col += texture2D(colorTex, texCoord + (vec2( -0.37,-0.15 )*aspectcorrect) * dofblur*0.9);	
	col += texture2D(colorTex, texCoord + (vec2( 0.15,-0.37 )*aspectcorrect) * dofblur*0.9);	
	
	col += texture2D(colorTex, texCoord + (vec2( 0.29,0.29 )*aspectcorrect) * dofblur*0.7);
	col += texture2D(colorTex, texCoord + (vec2( 0.4,0.0 )*aspectcorrect) * dofblur*0.7);	
	col += texture2D(colorTex, texCoord + (vec2( 0.29,-0.29 )*aspectcorrect) * dofblur*0.7);	
	col += texture2D(colorTex, texCoord + (vec2( 0.0,-0.4 )*aspectcorrect) * dofblur*0.7);	
	col += texture2D(colorTex, texCoord + (vec2( -0.29,0.29 )*aspectcorrect) * dofblur*0.7);
	col += texture2D(colorTex, texCoord + (vec2( -0.4,0.0 )*aspectcorrect) * dofblur*0.7);	
	col += texture2D(colorTex, texCoord + (vec2( -0.29,-0.29 )*aspectcorrect) * dofblur*0.7);	
	col += texture2D(colorTex, texCoord + (vec2( 0.0,0.4 )*aspectcorrect) * dofblur*0.7);
			 
	col += texture2D(colorTex, texCoord + (vec2( 0.29,0.29 )*aspectcorrect) * dofblur*0.4);
	col += texture2D(colorTex, texCoord + (vec2( 0.4,0.0 )*aspectcorrect) * dofblur*0.4);	
	col += texture2D(colorTex, texCoord + (vec2( 0.29,-0.29 )*aspectcorrect) * dofblur*0.4);	
	col += texture2D(colorTex, texCoord + (vec2( 0.0,-0.4 )*aspectcorrect) * dofblur*0.4);	
	col += texture2D(colorTex, texCoord + (vec2( -0.29,0.29 )*aspectcorrect) * dofblur*0.4);
	col += texture2D(colorTex, texCoord + (vec2( -0.4,0.0 )*aspectcorrect) * dofblur*0.4);	
	col += texture2D(colorTex, texCoord + (vec2( -0.29,-0.29 )*aspectcorrect) * dofblur*0.4);	
	col += texture2D(colorTex, texCoord + (vec2( 0.0,0.4 )*aspectcorrect) * dofblur*0.4);	
			
	fragColor = col/41.0;
	fragColor.a = 1.0;
}
