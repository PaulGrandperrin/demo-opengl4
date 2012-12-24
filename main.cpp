#include <iostream>

#include <time.h>

#include "graphicEngine.hpp"
#include <GL/glew.h>
#include <GL/glfw.h>

using namespace DSGE;

GE Ge;

void resize(int x, int y)
{
    Ge.resize(x, y);
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
    //glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    //glfwOpenWindowHint(GLFW_FSAA_SAMPLES,16);
    
    glfwEnable(GLFW_STICKY_KEYS);

    if(!glfwOpenWindow(640,480, 0,0,0,0,0,0,GLFW_WINDOW))
      //goto failOWindow;
    exit(2);
    
    cout << glGetString(GL_VERSION) <<endl;
    
    glfwSetWindowTitle("OpenGL Demo");
    glfwSetWindowSizeCallback(&resize);
    glfwSwapInterval(1);
    
    Ge.init(640,480);
    
    uint meshDemon = Ge.loadMesh("test","demon.obj");
    uint meshCube = Ge.loadMesh("test","cube.obj");
    
    uint textureDemon = Ge.loadTexture("test","demon.png");
    uint textureCube = Ge.loadTexture("test","tux.png");
    
    uint program = Ge.createProgram("test");
    uint ps = Ge.loadShader("test","ps.glsl",GL_FRAGMENT_SHADER);
    uint vs = Ge.loadShader("test","vs.glsl",GL_VERTEX_SHADER);
    Ge.addShaderToProgram(ps,program);
    Ge.addShaderToProgram(vs,program);
    Ge.linkProgram(program);
    
    ObjectLeaf* demon = new ObjectLeaf(&Ge);
    demon->mesh=meshDemon;
    demon->program=program;
    demon->texture=textureDemon;
    
    ObjectLeaf* cube = new ObjectLeaf(&Ge);
    cube->mesh=meshCube;
    cube->program=program;
    cube->texture=textureCube;
    
    ObjectComposite* scene = new ObjectComposite(&Ge);
    scene->add(demon);
    scene->add(cube);
    
    
    
    while( running )
    {
	//struct timespec time;
	//clock_gettime(CLOCK_MONOTONIC,&time);
        //double monoTime = time.tv_nsec / (double)1000000000.0f + time.tv_sec;
        
	double monoTime = glfwGetTime();
	
	demon->identity();
	demon->translate(0,0,0);
	
	cube->identity();
	cube->translate(0,0,1);
	cube->scale(0.2f,0.2f,0.2f);
	cube->rotate((float)monoTime*600, 1, 0, 0);
	
	scene->identity();
	scene->translate(0,0,-3);
	scene->rotate((float)monoTime*200, 0, 1, 0);
	
	
	Ge.render(scene, monoTime);
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
