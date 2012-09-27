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

    glViewport (0, 0, width, height);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}


void GraphicEngine::render()
{

    
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
    
    
    //TODO save mesh info
    
    free(vertexArray);
    free(indexArray);

    cout << "vertexCount: " << vertices.size() << endl;
    cout << "triangleCount: " << triangles.size() << endl;


    // Save infos

    // Get file inode number
    struct stat sb;
    stat(fileName.c_str(),&sb);
    cout << sb.st_ino <<endl ;
    
    return 1;
}
