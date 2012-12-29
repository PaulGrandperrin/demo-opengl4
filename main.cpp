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
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES,16);
    
    glfwEnable(GLFW_STICKY_KEYS);

    if(!glfwOpenWindow(640,480, 0,0,0,0,0,0,GLFW_WINDOW))
      //goto failOWindow;
    exit(2);
    
    cout << glGetString(GL_VERSION) <<endl;
    
    glfwSetWindowTitle("OpenGL Demo");
    glfwSetWindowSizeCallback(&resize);
    glfwSwapInterval(1);
    
    Ge.init(640,480);
    
    uint meshDemon = Ge.loadMesh("test","meshes/skeleton.obj");
    uint meshIle = Ge.loadMesh("test","meshes/ile.obj");
    uint meshCube = Ge.loadMesh("test","meshes/cube.obj");
    
    uint textureDemon = Ge.loadTexture("test","textures/bone.tga", true);
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
    
    Light* light1 = new Light(&Ge);
    light1->identity();
    light1->translate(-15,-15,-10);
    light1->color=glm::vec4(1,0,0,1);
    light1->radius = 45;
    
    Light* light2 = new Light(&Ge);
    light2->identity();
    light2->translate(15,-15,-10);
    light2->color=glm::vec4(0,1,0,1);
    light2->radius=45;
    
    Light* light3 = new Light(&Ge);
    light3->identity();
    light3->translate(0,-15,15);
    light3->color=glm::vec4(0,0,1,1);
    light3->radius=45;
    
    
    Light* light4 = new Light(&Ge);
    light4->identity();
    light4->translate(-15,15,-10);
    light4->color=glm::vec4(1,0,0,1);
    light4->radius = 45;
    
    Light* light5 = new Light(&Ge);
    light5->identity();
    light5->translate(15,15,-10);
    light5->color=glm::vec4(0,1,0,1);
    light5->radius=45;
    
    Light* light6 = new Light(&Ge);
    light6->identity();
    light6->translate(0,15,15);
    light6->color=glm::vec4(0,0,1,1);
    light6->radius=45;
    
    SceneComposite* lightsA = new SceneComposite(&Ge);
    lightsA->identity();
    lightsA->add(light1);
    lightsA->add(light2);
    lightsA->add(light3);
    
    SceneComposite* lightsB = new SceneComposite(&Ge);
    lightsB->identity();
    lightsB->add(light4);
    lightsB->add(light5);
    lightsB->add(light6);
    
    Solid* skeleton = new Solid(&Ge);
    skeleton->identity();
    skeleton->mesh=meshDemon;
    skeleton->program=programTexturePhong;
    skeleton->texture=textureDemon;
    
    Solid* ile = new Solid(&Ge);
    ile->identity();
    ile->translate(0,-43.5,0);
    ile->mesh=meshIle;
    ile->program=programTexture;
    ile->texture=textureIle;
    
    Solid* cube = new Solid(&Ge);
    cube->identity();
    cube->mesh=meshCube;
    cube->program=programTexture;
    cube->texture=textureCube;
    
    SceneComposite* scene = new SceneComposite(&Ge);
    scene->identity();
    scene->add(skeleton);
    scene->add(lightsA);
    scene->add(lightsB);
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
	

	
	skeleton->identity();
	
	lightsA->identity();
	lightsA->rotate((float)monoTime*50, 0, 1, 0);
	
	lightsB->identity();
	lightsB->rotate(-(float)monoTime*50, 0, 1, 0);
	
	cube->identity();
	cube->translate(0,0,0);
	cube->rotate(-90, 1, 0, 0);
	cube->rotate((float)monoTime, 0, 0, 1);
	
	scene->identity();
	//scene->rotate((float)monoTime*50, 0, 1, 0);
	
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
