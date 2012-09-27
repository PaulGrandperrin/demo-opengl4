#include <sys/types.h> // uint, ino_t

#include <map>
#include <vector>
#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

using namespace std;

/** TODO
 *  Fonctionnalities:
 *      Bling-Phonon lights
 *      Sample Distribution Shadow Maps
 *      Real-time cube map reflexions implemented with geometry shaders
 *      Key-Frame based animations
 *      Tesselation
 *      HDR pipeline and fragment shader tonemapping and blooming
 *      Normals maps / height maps / paralax mapping / subsurface scattering maps
 *      transparents faces
 *      fake refractions
 *      DOF
 *      screen space directionnal occlusion
 *      instanciated rendering
 *
 *  Optimisations:
 *      texture compression
 *      indexed triangles stripping with primitive restart
 *      Frustrum culling with octree and mesh bunding and occlusion queries
 */

class GraphicEngine
{
public:
    GraphicEngine(){}

    void init(uint width,uint height);
    void resize(uint width,uint height);
    void render();

    uint loadShader(string path, string fileName, GLenum type);
    void unloadShader(uint id);
    
    uint loadMesh(string path, string fileName);
    void unloadMesh(uint id);
    
    uint loadTexture(string path, string fileName);
    uint unloadTexture(uint id);

    
    uint createProgram(string path);
    void addShaderToProgram(uint shader, uint program);
    void deleteProgram(uint id);

    uint createPiece(string path);
    void addTextureToPiece(uint texture, uint piece);
    void addProgramToPiece(uint program, uint piece);
    void addMeshToPiece(uint mesh, uint piece);
    void deletePiece(uint id);

    uint createObject(string path);
    void addPieceToObject(uint piece, uint object);
    void addObjectToObject(uint object, uint object);
    void deleteObject(uint id);

    /**
     * TODO
     * add properties
     * add lights
     */

    
private:
    uint nextID;

    /** Filesystem level resources
     *
     *  Corresponds to one file on the file system.
     * 
     *  NOTE: We keep back reference count to desallocate
     *  resources not needed anymore.
     */

    struct FSResource
    {
        string name;
        uint backrefs;
        uint memorySize;
    };

    struct mesh : FSResource
    {
        GLuint VBO;
        GLuint IBO;
        // TODO bounding sphere
    };

    struct texture : FSResource
    {
        GLuint id;
        uint height, weight;
        // TODO compression type
    };

    struct shader : FSResource
    {
        GLuint id;
        GLenum type;
    };

    struct program : FSResource
    {
        GLuint id;
    };

    map<ino_t, uint> resourceInodes; // FS inode number -> resource ID
    
    map<uint, mesh>     meshs;
    map<uint, texture>  textures;
    map<uint, shader>   shaders;
    map<uint, program>  programs;

    /** Intermediate level resources
     *
     *  Does not correspond to one file on the filesystem neither
     *  do correspond to one object in the scene graph.
     */

    struct piece
    {
        uint mesh;
        uint program;
        uint texture; // TODO allow multiple textures
    };

    /** Scene level resources
     *
     *  Corresponds to one object in the scene graph.
     */

    struct object
    {
        vector<uint> piece;
        vector<uint> object;
    };

    struct light
    {
        // TODO fill
    };
    
    map<string, uint> resourceTree;


    map<ino_t,uint> loadedResources;
    
};
