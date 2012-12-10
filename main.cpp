#include <iostream>

#include "graphicEngine.hpp"
#include <GL/glew.h>
#include <GL/glfw.h>

GraphicEngine GE;

void resize(int x, int y)
{
    GE.resize(x, y);
}

int main(int argc, char* argv[], char* env[])
{  
  
    bool running=true;

    if(!glfwInit())
     //goto failInit;
    exit(1);
    
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR,3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR,0);
    //glfwOpenWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    //glfwOpenWindowHint(GLFW_FSAA_SAMPLES,4);

    if(!glfwOpenWindow(640,480, 0,0,0,0,0,0,GLFW_WINDOW))
      //goto failOWindow;
    exit(2);
    
    glfwSetWindowTitle("OpenGL Demo");
    glfwSetWindowSizeCallback(&resize);
    
    GE.init(640,480);
    int mesh = GE.loadMesh("test",argv[1]);
    int program = GE.createProgram("test");
    int ps = GE.loadShader("test","ps.glsl",GL_FRAGMENT_SHADER);
    int vs = GE.loadShader("test","vs.glsl",GL_VERTEX_SHADER);
    GE.addShaderToProgram(ps,program);
    GE.addShaderToProgram(vs,program);
    GE.linkProgram(program);
    
    while( running )
    {
        GE.render(mesh, program);
        glfwSwapBuffers();
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
    }

    glfwCloseWindow();
    glfwTerminate();

    return EXIT_SUCCESS;

    failOWindow:
    glfwTerminate();

    failInit:
    return EXIT_FAILURE;
}
