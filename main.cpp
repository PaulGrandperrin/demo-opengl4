#include <stdlib.h>

#include "graphicEngine.hpp"
#include <GL/glfw.h>




int main(int argc, char* argv[], char* env[])
{
    bool running=true;
    GraphicEngine GE;

    if(!glfwInit())
    goto failInit;

    if(!glfwOpenWindow(1280,800, 0,0,0,0,0,0,GLFW_WINDOW))
    goto failOWindow;

    glfwSetWindowTitle("OpenGL Demo");

    GE.init(1280,800);
    return GE.loadMesh("",argv[1]);
    while( running )
    {
        GE.render();
        glfwSwapBuffers();
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
    }



    return EXIT_SUCCESS;




    failOWindow:
    glfwTerminate();

    failInit:
    return EXIT_FAILURE;


}
