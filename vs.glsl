#version 430 core
//invariant vec4 gl_Position;
// On recoie un vecteur
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexture;

uniform mat4 projectionMatrix;

// On renvoie une normale
smooth out vec3 normal;
smooth out vec2 texPos;



// Avec pour parametre deux matrices
//uniform mat4 modelMatrix,viewMatrix,modelViewMatrix, projectionMatrix,modelViewProjectionMatrix;
//uniform mat3 normalMatrix;




void main()
{
	gl_Position=projectionMatrix*vec4(vertexPosition,1.0);
	//gl_Position = vec4(vertexPosition.xy,1,1);
	//normal=normalMatrix*vertexNormal;

	normal = vertexNormal;
	texPos=vertexTexture;
}