#version 130
//invariant vec4 gl_Position;
// On recoie un vecteur
in vec3 vertexPosition,vertexNormal;
//in vec2 vertexTexture;

// On renvoie une normale
smooth out vec3 normal;
//smooth out vec2 texPos;



// Avec pour parametre deux matrices
//uniform mat4 modelMatrix,viewMatrix,modelViewMatrix, projectionMatrix,modelViewProjectionMatrix;
//uniform mat3 normalMatrix;

uniform mat4 projectionMatrix;


void main()
{
	gl_Position=projectionMatrix*vec4(vertexPosition,1.0);
	//gl_Position = vec4(vertexPosition.xy,1,1);
	//normal=normalMatrix*vertexNormal;

	normal = vertexNormal;
	//texPos=vertexTexture;
}