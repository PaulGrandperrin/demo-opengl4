#include "graphicEngine.hpp"

#include <sys/types.h>
#include <sys/stat.h>


#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <IL/il.h>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

using namespace std;

using namespace DSGE;

GE::GE() : camera(this), lastID(0)
{}

void GE::init(uint width, uint height)
{
    glewExperimental=GL_TRUE;
    
    ilInit();
    
    if(GLIR(glewInit()) != GLEW_OK)
      exit(17);
    
    /*
    glEnable(GL_DEBUG_OUTPUT_ARB);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    glDebugMessageCallbackARB(&debugOutput, NULL);
    */
    
    resize(width, height);
}

void GE::resize(uint width, uint height)
{
    this->width = width;
    this->height = height;
    
    GLC(glViewport (0, 0, width, height));
    
    projectionMatrix = glm::perspective(75.0f, this->width / (float) this->height, 0.001f, 10000.f);
    GLC(glClearColor(0, 0, 0, 1)); 
}

void GE::clearDepth()
{
  GLC(glClear(GL_DEPTH_BUFFER_BIT));
}

void GE::render(SceneObject* o, double time)
{ 
    
    GLC(glEnable(GL_DEPTH_TEST));
    GLC(glDisable(GL_BLEND));

    
    o->draw(glm::mat4(1.f));

    GLC(glBindVertexArray(0));
    GLC(glUseProgram(0));
    GLC(glBindTexture(GL_TEXTURE_2D, 0));
    
    GLC(glFlush());

    return;
}


void GE::drawSolid(Solid* of, glm::mat4 mat)
{
    mesh m = meshs[of->mesh];
    texture t = textures[of->texture];
    program p = programs[of->program];
  
    GLC(glUseProgram(p.id));
    
    glm::vec4 camPos = glm::inverse(camera.transform) * glm::vec4(0, 0, 0, 1);
    
    glm::mat4 modelViewProjectionMatrix = projectionMatrix * camera.transform * mat;
    glm::mat3 normalMatrix =  glm::mat3(mat);

    
    GLI(glUniform3f(CAMERA_UNIFORM, camPos.x, camPos.y, camPos.z));
    GLC(glUniformMatrix4fv(MODELVIEWPROJECTIONMAT_UNIFORM, 1, 0, (GLfloat*) glm::value_ptr(modelViewProjectionMatrix)));
    GLI(glUniformMatrix4fv(MODELMAT_UNIFORM, 1, 0, (GLfloat*) glm::value_ptr(mat)));
    GLI(glUniformMatrix3fv(NORMALMAT_UNIFORM, 1, 0, (GLfloat*) glm::value_ptr(normalMatrix)));
    
    GLC(glBindVertexArray(m.VAO));
    
    GLC(glActiveTexture(GL_TEXTURE0));
    GLC(glBindTexture(GL_TEXTURE_2D, t.id));
    
    GLC(glDrawElements(GL_TRIANGLES, m.sizeOfIBO, GL_UNSIGNED_INT, 0));
  
}


void GE::positionLight(Light* l, glm::mat4 mat)
{
  
}


uint GE::loadMesh(string name, string filePath)
{

    struct point
    {
        float x,y,z;
    };
    
    struct normal
    {
        float x,y,z;
    };
    
    struct coord
    {
        float x,y;
    };

    struct vertex
    {
        uint pointId;
        uint normalId;
        uint coordId;

        bool operator<(const vertex& v) const
        {
            if(   this->pointId < v.pointId
                || ( this->pointId == v.pointId &&
                ( this->normalId < v.normalId || (this->normalId == v.normalId && this->coordId < v.coordId))))
                return true;
            else
                return false;
        }
    };

    struct triangle
    {
        uint ids[3];
    };

    
    vector<point>       points;
    vector<normal>      normals;
    vector<coord>       coords;
    map<vertex,uint>    vertices;
    vector<triangle>    triangles;

    uint vertCounter=0;
    
    ifstream file(filePath.c_str(),ios::in);
    if (!file)
        return 0;
    
    string buffer, key;
    while(getline(file, buffer))
    {
        istringstream line(buffer);
        key = "";
        line >> key;
        
        if(key == "v")
        {
            point tmp;
            line >> tmp.x >> tmp.y >> tmp.z;
            points.push_back ( tmp );
        }
        else if(key == "vn")
        {
            normal tmp;
            line >> tmp.x >> tmp.y >> tmp.z;
            normals.push_back ( tmp );
        }
        else if(key == "vt")
        {
            coord tmp;
            line >> tmp.x >> tmp.y;
            coords.push_back ( tmp );
        }
        else if(key == "g")
        {
            //TODO implement
        }
        else if(key == "f")
        {
            triangle t;
            vertex v;
            string sv;
            int c;
            
            for(int i = 0; i < 3; i++)
            {
                line >> sv;
                c = sv.find_first_of('/');
                v.pointId = atoi(sv.substr(0, c).c_str())-1;

                sv=sv.substr(c + 1, sv.length() - c - 1);
                c=sv.find_first_of('/');
                if (c == 0)
                    v.coordId = 0;
                v.coordId = atoi(sv.substr(0, c).c_str())-1;
                
                sv=sv.substr ( c+1,sv.length()-c-1 );
                c=sv.find_first_of('/');
                if (c == 0)
                    v.normalId = 0;
                v.normalId = atoi(sv.substr(0, c).c_str())-1;

                
                if(vertices.find(v) != vertices.end())
                    t.ids[i]=vertices[v];
                else
                {
                    vertices[v]=vertCounter;
                    t.ids[i]=vertCounter;
		    vertCounter++;
                }
            }
            triangles.push_back(t);
        }
    }
    
    file.close();

    // TODO calculate bounding sphere
    GLfloat* vertexArray = new GLfloat[vertices.size() * (3 + 3 + 2)];
    
    /* TODO Optimize the order of vertices */
    for(auto it=vertices.begin(); it!=vertices.end(); it++)
    {
        vertexArray[it->second*8]   = points[it->first.pointId].x;
        vertexArray[it->second*8+1] = points[it->first.pointId].y;
        vertexArray[it->second*8+2] = points[it->first.pointId].z;
        vertexArray[it->second*8+3] = normals[it->first.normalId].x;
        vertexArray[it->second*8+4] = normals[it->first.normalId].y;
        vertexArray[it->second*8+5] = normals[it->first.normalId].z;
        vertexArray[it->second*8+6] = coords[it->first.coordId].x;
        vertexArray[it->second*8+7] = coords[it->first.coordId].y;
/*
        cout << "px " << points[it->first.pointId].x << endl;
        cout << "py " << points[it->first.pointId].y << endl;
        cout << "pz " << points[it->first.pointId].z << endl;
        cout << "nx " << normals[it->first.pointId].x << endl;
        cout << "ny " << normals[it->first.pointId].y << endl;
        cout << "nz " << normals[it->first.pointId].z << endl;
        cout << "cx " << coords[it->first.pointId].x << endl;
        cout << "cy " << coords[it->first.pointId].y << endl;
        cout << endl;
	*/
    }


    GLuint* indexArray = new GLuint[triangles.size() * 3];

    for(uint i=0; i<triangles.size();++i)
    {
        indexArray[i*3]   = triangles[i].ids[0];
        indexArray[i*3+1] = triangles[i].ids[1];
        indexArray[i*3+2] = triangles[i].ids[2];
    }

    GLuint VBO;
    GLC(glGenBuffers(1, &VBO));
    GLC(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLC(glBufferData(GL_ARRAY_BUFFER, vertices.size() * (3 + 3 + 2) * sizeof(GLfloat), vertexArray, GL_STATIC_DRAW));
    GLC(glBindBuffer(GL_ARRAY_BUFFER, 0));
    
    GLuint IBO;
    GLC(glGenBuffers(1, &IBO));
    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO));
    GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER,triangles.size() * 3 * sizeof(GLuint), indexArray, GL_STATIC_DRAW));
    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    
    
    free(vertexArray);
    free(indexArray);

    cout << "vertexCount: " << vertices.size() << endl;
    cout << "triangleCount: " << triangles.size() << endl;
    
    
    GLuint VAO;
    GLC(glGenVertexArrays(1, &VAO));
    GLC(glBindVertexArray(VAO));
    
    GLC(glEnableVertexAttribArray(VERTEX_POSITION_ATTRIB));
    GLC(glEnableVertexAttribArray(VERTEX_NORMAL_ATTRIB));
    GLC(glEnableVertexAttribArray(VERTEX_TEXTCOORD_ATTRIB));
    
    GLC(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    
    GLC(glVertexAttribPointer (VERTEX_POSITION_ATTRIB, 3, GL_FLOAT,GL_FALSE, 32, ( char * ) NULL + 0 ));
    GLC(glVertexAttribPointer(VERTEX_NORMAL_ATTRIB, 3, GL_FLOAT,GL_TRUE, 32, ( char * ) NULL + 12 ));
    GLC(glVertexAttribPointer (VERTEX_TEXTCOORD_ATTRIB, 2, GL_FLOAT, GL_FALSE, 32, ( char * ) NULL + 24 ));

    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
    
    GLC(glBindVertexArray(0));

    // Get file inode number
    struct stat sb;
    stat(filePath.c_str(), &sb);
    cout << sb.st_ino << endl;
    
    // TODO save inode
    
    mesh m;
    m.IBO=IBO;
    m.VBO=VBO;
    m.VAO=VAO;
    m.size=vertices.size() * (3 + 3 + 2) + triangles.size() * 3;
    m.sizeOfIBO = triangles.size() * 3;
    m.backrefs=0;
    m.name=name;
    
    uint ID=getNextID();
    
    this->meshs.insert(pair<uint,mesh>(ID,m));
    return ID;
}

uint GE::loadShader(string name, string filePath, GLenum type)
{
  	GLuint shader;
	GLsizei logsize;
	GLint compile_status = GL_TRUE;
	char *log;

	shader = GLCR(glCreateShader(type));

	ifstream file(filePath,ios::in);
	string src,line;
	
	while(getline(file,line))
		src+="\n"+line; //TODO replace "\n" by endl
	
	file.close();

	const char* strchar=src.c_str();
	GLC(glShaderSource(shader, 1, (const GLchar**) &(strchar), NULL));
	GLC(glCompileShader(shader));

	GLC(glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status));
	if (compile_status != GL_TRUE)
	{
		GLC(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize));

		log = (char*)malloc(logsize + 1);

		GLC(glGetShaderInfoLog(shader, logsize, &logsize, log));
		cout << endl << "Error compiling " << filePath << " :" <<endl << log << endl;

		free (log);
		GLC(glDeleteShader(shader));

		return 0;
	}

	GE::shader s;
	uint ID=getNextID();
	s.id = shader;
	s.type = type;
	s.size=0; //TODO complete
	s.backrefs=0;
	s.name=name;
	
	this->shaders.insert(pair<uint,GE::shader>(ID,s));
	
	return ID;
}


uint GE::createProgram(string name)
{
    uint ID=getNextID();
    program p;
    p.id=GLCR(glCreateProgram());
    p.backrefs=0;
    p.name=name;
    
    this->programs.insert(pair<uint,program>(ID,p));
    return ID;
}


void GE::addShaderToProgram(uint shader, uint program)
{
	GLuint p, s;
	
	p = this->programs[program].id;
	s = this->shaders[shader].id;
	
	 if(!GLCR(glIsShader(s)))
	    exit (12);
	 
	GLC(glAttachShader(p, s));
	
	//TODO garbage collection ?
}
	
void GE::linkProgram(uint program)
{
	GLuint p;
	p = this->programs[program].id;
	
	/*
	GLC(glBindAttribLocation(p, VERTEX_POSITION_ATTRIB, "in_position"));
	GLC(glBindAttribLocation(p, VERTEX_NORMAL_ATTRIB, "in_normal"));
	GLC(glBindAttribLocation(p, VERTEX_TEXTCOORD_ATTRIB, "in_textCoord"));
	*/
	
	GLC(glLinkProgram(p));

	//this->programs[program].modelViewProjectionMatrixID =
	//    GLCR(glGetUniformLocation(p, "modelViewProjectionMatrix" ));
	
	GLint link_status = GL_TRUE;
	GLint logsize=200;
	char* log;
	
	GLC(glGetProgramiv(p, GL_LINK_STATUS, &link_status));
	if(link_status != GL_TRUE)
	{
		GLC(glGetProgramiv(p, GL_INFO_LOG_LENGTH, &logsize));
		log = (char*)malloc(logsize + 1);

		GLC(glGetProgramInfoLog(p, logsize, &logsize, log));

		cout << endl<< "Error while linking:"<< endl << log << endl;

		free ( log );
		GLC(glDeleteProgram(p));
		exit(-1);
	}

}

uint GE::loadTexture(string name, string filePath, bool withMimaps)
{
    
    ILuint ilID;
    GLuint glID;
    
    ilGenImages(1, &ilID);
    ilBindImage(ilID);
    
    if (!ilLoadImage(filePath.c_str()))
        exit (11);
    
    if (!ilConvertImage(IL_RGBA,IL_UNSIGNED_BYTE))
        exit (11);
    
    GLC(glGenTextures(1,&glID));
    GLC(glBindTexture(GL_TEXTURE_2D,glID));

    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    
    if(withMimaps)
    {
      GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
      GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
      GLC(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4));
    }
    else
    {
      GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
      GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }

    GLC(glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        ilGetInteger(IL_IMAGE_WIDTH),
        ilGetInteger(IL_IMAGE_HEIGHT),
        0,
        ilGetInteger(IL_IMAGE_FORMAT),
        GL_UNSIGNED_BYTE,
        ilGetData()
    ));
    
    if(withMimaps)
    {
      GLC(glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST));
      GLC(glGenerateMipmap(GL_TEXTURE_2D));
    }
    
    GE::texture t;
    t.id = glID;
    t.name = name;
    t.height = ilGetInteger(IL_IMAGE_HEIGHT);
    t.width = ilGetInteger(IL_IMAGE_WIDTH);
    t.size = ilGetInteger(IL_IMAGE_WIDTH) * ilGetInteger(IL_IMAGE_HEIGHT) * ilGetInteger(IL_IMAGE_BPP);
    t.backrefs = 0;
    
    ilDeleteImages(1, &ilID);
    
    uint ID = getNextID();
    this->textures.insert(pair<uint,texture>(ID,t));
    return ID;
}

// -----------------------

Object3D::Object3D(GE* ge): ge(ge) {}
SceneObject::SceneObject(GE* ge): Object3D(ge) {}
SceneComposite::SceneComposite(GE* ge): SceneObject(ge) {}
Solid::Solid(GE* ge): SceneObject(ge) {}
Light::Light(GE* ge): SceneObject(ge) {}


// ----------------


void Object3D::rotate(float angle, float x, float y, float z)
{
  transform = glm::rotate(transform,angle,glm::vec3(x, y, z));
}

void Object3D::translate(float x, float y, float z)
{
  transform = glm::translate(transform,glm::vec3(x, y, z));
}

void Object3D::scale(float x, float y, float z)
{
  transform = glm::scale(transform,glm::vec3(x, y, z));
}

void Object3D::identity()
{
  transform= glm::mat4(1.0f);
}

// -------------------------

void SceneComposite::add(SceneObject* o)
{
    children.push_front(o);
}

void SceneComposite::remove(SceneObject* o)
{
    children.remove(o);
}

//--------------------


void SceneComposite::draw(glm::mat4 mat)
{
  mat *= transform;
  for(auto it = children.begin(); it != children.end(); ++it)
  {
      (*it)->draw(mat);
  }
}


void Solid::draw(glm::mat4 mat)
{
    mat *= transform;
    ge->drawSolid(this, mat);
}

// -----------------

void SceneComposite::computeLightsPositions(glm::mat4 mat)
{
  mat *= transform;
  for(auto it = children.begin(); it != children.end(); ++it)
  {
      (*it)->computeLightsPositions(mat);
  }
}


void Light::computeLightsPositions(glm::mat4 mat)
{
  ge->positionLight(this, mat);
}
