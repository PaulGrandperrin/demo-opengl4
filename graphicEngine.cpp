#include "graphicEngine.hpp"

#include <sys/types.h>
#include <sys/stat.h>


#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void GraphicEngine::init(uint width, uint height)
{
    lastID=0;
    
    glViewport (0, 0, width, height);
    glClearColor(0, 0, 0, 0);
}


void GraphicEngine::render()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    
}


uint GraphicEngine::loadMesh(string path, string fileName)
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
                ||this->pointId == v.pointId &&
                ( this->normalId < v.normalId || this->normalId == v.normalId && this->coordId < v.coordId))
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

    ifstream file(fileName.c_str(),ios::in);
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
            uint vertCounter=0;
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
                    vertices[v]=vertCounter++;
                    t.ids[i]=vertCounter;
                }
            }
            triangles.push_back(t);
        }
    }
    
    file.close();

    // TODO calculate bounding sphere

    GLfloat* vertexArray = new GLfloat[vertices.size() * (3 + 3 + 2)];
    
    for(auto it=vertices.begin(); it!=vertices.end(); it++)
    {
        vertexArray[it->second*8]   = points[it->first.pointId].x;
        vertexArray[it->second*8+1] = points[it->first.pointId].y;
        vertexArray[it->second*8+2] = points[it->first.pointId].z;
        vertexArray[it->second*8+3] = normals[it->first.pointId].x;
        vertexArray[it->second*8+4] = normals[it->first.pointId].y;
        vertexArray[it->second*8+5] = normals[it->first.pointId].z;
        vertexArray[it->second*8+6] = coords[it->first.pointId].x;
        vertexArray[it->second*8+7] = coords[it->first.pointId].y;

        cout << "px " << points[it->first.pointId].x << endl;
        cout << "py " << points[it->first.pointId].y << endl;
        cout << "pz " << points[it->first.pointId].z << endl;
        cout << "nx " << normals[it->first.pointId].x << endl;
        cout << "ny " << normals[it->first.pointId].y << endl;
        cout << "nz " << normals[it->first.pointId].z << endl;
        cout << "cx " << coords[it->first.pointId].x << endl;
        cout << "cy " << coords[it->first.pointId].y << endl;
        cout << endl;
    }


    GLuint* indexArray = new GLuint[triangles.size() * 3];

    for(uint i=0; i<triangles.size();++i)
    {
        indexArray[i]   = triangles[i].ids[0];
        indexArray[i+1] = triangles[i].ids[1];
        indexArray[i+2] = triangles[i].ids[2];
    }

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * (3 + 3 + 2) * sizeof(GLfloat), vertexArray, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    GLuint IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,triangles.size() * sizeof(GLuint), indexArray, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    
    free(vertexArray);
    free(indexArray);

    cout << "vertexCount: " << vertices.size() << endl;
    cout << "triangleCount: " << triangles.size() << endl;

    // Get file inode number
    struct stat sb;
    stat(fileName.c_str(), &sb);
    cout << sb.st_ino << endl;
    
    // TODO save inode
    
    mesh m;
    m.IBO=IBO;
    m.VBO=VBO;
    m.memorySize=vertices.size() * (3 + 3 + 2) + triangles.size() * 3;
    m.backrefs=0;
    m.name=path;
    
    uint ID=getNextID();
    
    this->meshs.insert(pair<uint,mesh>(ID,m));
    
    return ID;
}

uint GraphicEngine::loadShader(string path, string fileName, GLenum type)
{
  	GLuint shader;
	GLsizei logsize;
	GLint compile_status = GL_TRUE;
	char *log;

	shader = glCreateShader(type);

	ifstream file(fileName,ios::in);
	string src,line;
	
	while(getline(file,line))
		src+="\n"+line; //TODO replace "\n" by endl
	
	file.close();

	const char* strchar=src.c_str();
	glShaderSource(shader, 1, (const GLchar**) &(strchar), NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);

		log = (char*)malloc(logsize + 1);

		glGetShaderInfoLog(shader, logsize, &logsize, log);
		cout << endl << "Error compiling " << fileName << " :" <<endl << log << endl;

		free (log);
		glDeleteShader(shader);

		return 0;
	}

	GraphicEngine::shader s;
	uint ID=getNextID();
	s.id = shader;
	s.type = type;
	s.memorySize=0;
	s.backrefs=0;
	s.name=path;
	
	this->shaders.insert(pair<uint,GraphicEngine::shader>(ID,s));
	
	return ID;
}


uint GraphicEngine::createProgram(string path)
{
    uint ID=getNextID();
    program p;
    p.id=glCreateProgram();
    p.backrefs=0;
    p.name=path;
    
    this->programs.insert(pair<uint,program>(ID,p));
    return ID;
}


void GraphicEngine::addShaderToProgram(uint shader, uint program)
{
	GLuint p, s;
	
	p = this->programs[program].id;
	s = this->shaders[shader].id;
	
	glAttachShader(p, s);
	
	//TODO garbage collection ?
}
	
void GraphicEngine::linkProgram(uint program)
{
	GLuint p;
	p = this->programs[program].id;
	
	glLinkProgram(p);

	
	GLint link_status = GL_TRUE;
	GLint logsize=200;
	char* log;
	
	glGetProgramiv(p, GL_LINK_STATUS, &link_status);
	if(link_status != GL_TRUE)
	{
		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &logsize);
		log = (char*)malloc(logsize + 1);

		glGetProgramInfoLog(p, logsize, &logsize, log);

		cout << endl<< "Error while linking:"<< endl << log << endl;

		free ( log );
		glDeleteProgram (p);
		exit(-1);
	}

}

