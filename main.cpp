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
    
    SolidLeaf* demon = new SolidLeaf(&Ge);
    demon->mesh=meshDemon;
    demon->program=program;
    demon->texture=textureDemon;
    
    SolidLeaf* cube = new SolidLeaf(&Ge);
    cube->mesh=meshCube;
    cube->program=program;
    cube->texture=textureCube;
    
    SolidComposite* scene = new SolidComposite(&Ge);
    scene->add(demon);
    scene->add(cube);
    
    float azimut = 0, elevation = 0, twist = 0;
    float camX = 0, camY = 0, camZ = 0;
    
    int mouseX = 0, mouseY = 0;
    
    bool mouseHidden = false;
    bool changeEscapeStatus = true;
    
    while( running )
    {
	//struct timespec time;
	//clock_gettime(CLOCK_MONOTONIC,&time);
        //double monoTime = time.tv_nsec / (double)1000000000.0f + time.tv_sec;
        
        
	double monoTime = glfwGetTime();
	if( glfwGetKey('D') ==  GLFW_PRESS )
	{
	  camZ -= cos(-azimut/360.*2.*3.1415927+3.1415927/2.)*0.1;
	  camX -= sin(-azimut/360.*2.*3.1415927+3.1415927/2.)*0.1;
	}
	
	if( glfwGetKey('Q') ==  GLFW_PRESS )
	{
	  camZ -= cos(-azimut/360.*2.*3.1415927-3.1415927/2.)*0.1;
	  camX -= sin(-azimut/360.*2.*3.1415927-3.1415927/2.)*0.1;
	} 
	
	if( glfwGetKey('S') ==  GLFW_PRESS )
	{
	  camZ -= cos(-azimut/360.*2.*3.1415927)*cos(elevation/360.*2.*3.1415927)*0.1;
	  camX -= sin(-azimut/360.*2.*3.1415927)*cos(elevation/360.*2.*3.1415927)*0.1;
	  camY -= sin(elevation/360.*2.*3.1415927)*0.1;
	}
	
	if( glfwGetKey('Z') ==  GLFW_PRESS )
	{
	  camZ += cos(-azimut/360.*2.*3.1415927)*cos(elevation/360.*2.*3.1415927)*0.1;
	  camX += sin(-azimut/360.*2.*3.1415927)*cos(elevation/360.*2.*3.1415927)*0.1;
	  camY += sin(elevation/360.*2.*3.1415927)*0.1;
	}
	
	if( glfwGetKey(GLFW_KEY_SPACE) ==  GLFW_PRESS )
	{
	  camY -= 0.1;
	}
	
	if( glfwGetKey(GLFW_KEY_LSHIFT) ==  GLFW_PRESS )
	{
	  camY += 0.1;
	}
	
	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
	{
	  glfwDisable(GLFW_MOUSE_CURSOR);
	  glfwGetMousePos(&mouseX, &mouseY);
	  mouseHidden = true;
	}
	
	if(glfwGetKey(GLFW_KEY_ESC))
	{
	  if(changeEscapeStatus)
	  {
	    if(mouseHidden)
	    {
	      glfwEnable(GLFW_MOUSE_CURSOR);
	      mouseHidden = false;
	    }
	    else
	    {
	      running = false;
	    }
	    changeEscapeStatus = false;
	  }
	}
	else
	{
	  changeEscapeStatus = true;
	}
	
	if(mouseHidden)
	{
	  int x, y;
	  glfwGetMousePos(&x, &y);
	  x -= mouseX;
	  y -= mouseY;
	  mouseX += x;
	  mouseY += y;
	  azimut += x/3.;
	  elevation += y/3.;
	}
	
	cout << "a: "<< azimut << " e: " << elevation << endl;
	
	Ge.camera.identity();
	Ge.camera.rotate(elevation,1,0,0);
	Ge.camera.rotate(azimut,0,1,0);
	Ge.camera.translate(camX,camY,camZ);
	
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
        running &= glfwGetWindowParam(GLFW_OPENED);
    }

    glfwCloseWindow();
    glfwTerminate();

    return EXIT_SUCCESS;

    failOWindow:
    glfwTerminate();

    failInit:
    return EXIT_FAILURE;
}
