#version 130
#extension GL_EXT_geometry_shader4 : enable

smooth in vec3 normal[];
smooth out vec3 normal2;

smooth in vec2 texPos[];
smooth out vec2 texPos2;

void main(void)
{

    for(int i=0; i< 3; i++)
    {

        gl_Position =gl_PositionIn[i];
	normal2=normal[i];
	texPos2=texPos[i];
        EmitVertex();
    }

	EndPrimitive();

}