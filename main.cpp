#include <iostream>

#include <time.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/glfw.h>

#include "graphicEngine.hpp"


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
    
    uint meshDemon = Ge.loadMesh("test","meshes/demon.obj");
    uint meshIle = Ge.loadMesh("test","meshes/ile.obj");
    uint meshCube = Ge.loadMesh("test","meshes/cube.obj");
    
    uint textureDemon = Ge.loadTexture("test","textures/demon.png", true);
    uint textureIle = Ge.loadTexture("test","textures/ile.png", true);
    uint textureCube = Ge.loadTexture("test","textures/skybox/hi-quality/stormydays.tga", false);
    
    uint programTexture = Ge.createProgram("test");
    uint ps = Ge.loadShader("test","shaders/texture.frag",GL_FRAGMENT_SHADER);
    uint vs = Ge.loadShader("test","shaders/texture.vert",GL_VERTEX_SHADER);
    Ge.addShaderToProgram(ps,programTexture);
    Ge.addShaderToProgram(vs,programTexture);
    Ge.linkProgram(programTexture);
    
    
    uint programTexturePhong = Ge.createProgram("test");
    ps = Ge.loadShader("test","shaders/texturePhong.frag",GL_FRAGMENT_SHADER);
    vs = Ge.loadShader("test","shaders/texturePhong.vert",GL_VERTEX_SHADER);
    Ge.addShaderToProgram(ps,programTexturePhong);
    Ge.addShaderToProgram(vs,programTexturePhong);
    Ge.linkProgram(programTexturePhong);
    
    Solid* demon = new Solid(&Ge);
    demon->mesh=meshDemon;
    demon->program=programTexturePhong;
    demon->texture=textureDemon;
    
    Solid* ile = new Solid(&Ge);
    ile->mesh=meshIle;
    ile->program=programTexturePhong;
    ile->texture=textureIle;
    
    Solid* cube = new Solid(&Ge);
    cube->mesh=meshCube;
    cube->program=programTexture;
    cube->texture=textureCube;
    
    SceneComposite* scene = new SceneComposite(&Ge);
    scene->add(demon);
    scene->add(ile);
    
    float azimut = 0, elevation = 0, twist = 0;
    float camX = 0, camY = 0, camZ = 0;
    
    int mouseX = 0, mouseY = 0;
    
    bool mouseHidden = false;
    bool changeEscapeStatus = true;
    
    double oldMonotime = glfwGetTime();
    
    while( running )
    {
	//struct timespec time;
	//clock_gettime(CLOCK_MONOTONIC,&time);
        //double monoTime = time.tv_nsec / (double)1000000000.0f + time.tv_sec;
        double monoTime;
        oldMonotime = monoTime;
	monoTime = glfwGetTime();
	
	float distance = (monoTime - oldMonotime)*10;
	
	if( glfwGetKey('D') ==  GLFW_PRESS )
	{
	  camZ -= cos(-azimut/360.*2.*M_PI+M_PI/2.)*distance;
	  camX -= sin(-azimut/360.*2.*M_PI+M_PI/2.)*distance;
	}
	
	if( glfwGetKey('Q') ==  GLFW_PRESS )
	{
	  camZ -= cos(-azimut/360.*2.*M_PI-M_PI/2.)*distance;
	  camX -= sin(-azimut/360.*2.*M_PI-M_PI/2.)*distance;
	} 
	
	if( glfwGetKey('S') ==  GLFW_PRESS )
	{
	  camZ -= cos(-azimut/360.*2.*M_PI)*cos(elevation/360.*2.*M_PI)*distance;
	  camX -= sin(-azimut/360.*2.*M_PI)*cos(elevation/360.*2.*M_PI)*distance;
	  camY -= sin(elevation/360.*2.*M_PI)*distance;
	}
	
	if( glfwGetKey('Z') ==  GLFW_PRESS )
	{
	  camZ += cos(-azimut/360.*2.*M_PI)*cos(elevation/360.*2.*M_PI)*distance;
	  camX += sin(-azimut/360.*2.*M_PI)*cos(elevation/360.*2.*M_PI)*distance;
	  camY += sin(elevation/360.*2.*M_PI)*distance;
	}
	
	if( glfwGetKey(GLFW_KEY_SPACE) ==  GLFW_PRESS )
	{
	  camY -= distance;
	}
	
	if( glfwGetKey(GLFW_KEY_LSHIFT) ==  GLFW_PRESS )
	{
	  camY += distance;
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
	  if(elevation > 90)
	    elevation = 90;
	  else if (elevation < -90)
	    elevation = -90;
	}
	

	
	demon->identity();
	demon->translate(0,1,0);
	demon->rotate((float)monoTime*50, 0, 1, 0);
	
	ile->identity();
	ile->translate(0,0,0);
	
	cube->identity();
	cube->translate(0,0,0);
	cube->rotate(-90, 1, 0, 0);
	cube->rotate((float)monoTime, 0, 0, 1);
	
	scene->identity();
	
	Ge.clearDepth();
	Ge.camera.identity();
	Ge.camera.rotate(elevation,1,0,0);
	Ge.camera.rotate(azimut,0,1,0);
	Ge.render(cube, monoTime);
	
	
	Ge.clearDepth();
	Ge.camera.identity();
	Ge.camera.rotate(elevation,1,0,0);
	Ge.camera.rotate(azimut,0,1,0);
	Ge.camera.translate(camX,camY,camZ);
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
