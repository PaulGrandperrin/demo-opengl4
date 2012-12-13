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
    
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR,4);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR,3);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES,16);

    if(!glfwOpenWindow(640,480, 0,0,0,0,0,0,GLFW_WINDOW))
      //goto failOWindow;
    exit(2);
    
    glfwSetWindowTitle("OpenGL Demo");
    glfwSetWindowSizeCallback(&resize);
    
    GE.init(640,480);
    uint mesh = GE.loadMesh("test",argv[1]);
    uint texture = GE.loadTexture("test",argv[2]);
    uint program = GE.createProgram("test");
    uint ps = GE.loadShader("test","ps.glsl",GL_FRAGMENT_SHADER);
    uint vs = GE.loadShader("test","vs.glsl",GL_VERTEX_SHADER);
    GE.addShaderToProgram(ps,program);
    GE.addShaderToProgram(vs,program);
    GE.linkProgram(program);
    
    while( running )
    {
        GE.render(mesh, program, texture);
        glfwSwapBuffers();
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }

    glfwCloseWindow();
    glfwTerminate();

    return EXIT_SUCCESS;

    failOWindow:
    glfwTerminate();

    failInit:
    return EXIT_FAILURE;
}
