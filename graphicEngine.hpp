#include <sys/types.h> // uint, ino_t

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>


/**
 * This macros displays OpenGL errors on stderr if in DEBUG mode.
 * GLC means  openGL Check error
 * GLCR means openGL Check error with Return values 
 */

#ifdef DEBUG
  #define GLC(stmt) {stmt;GraphicEngine::GLCheckError(#stmt, __FILE__,__LINE__);}
  #define GLCR(stmt) [=]()->decltype(stmt){auto tmp=stmt;GraphicEngine::GLCheckError(#stmt,__FILE__,__LINE__);return tmp;}()
#else
  #define GLC(stmt) stmt
  #define GLCR(stmt) stmt
#endif

  
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
 * 
 * 
 * 	VA
 * 	VBO
 * 	IBO
 * 	VAO
 * 	FBO
 * 	TBO //nan
 * 	UBO
 * 	PBO
 * 	
 * 
 * 	change GLEW
 * 	choose devil
 * 
 * 	Others:
 * 	GL_NV_texture_barrier
 * 	GL_EXT_direct_state_access
 *	GL_NVX_gpu_memory_info
 *	GL_ATI_meminfo
 * 
 * 	OpenGL 3.0
 * 	ARB_uniform_buffer_object
 * 	ARB_texture_buffer_object
 * 
 * 	OpenGL 3.1
 * 	ARB_bindable_uniform_buffer_object
 * 
 * 	OpenGL 3.2
 * 	ARB_sync
 * 	ARB_texture_query_lod
 * 	ARB_texture_gather
 * 	ARB_texture_cube_map_array
 * 	ARB_texture_seamless_cube_map
 * 
 * 	OpenGL 3.3:
 * 	ARB_explicit_attrib_location YES
 * 	ARB_sampler_objects
 * 	ARB_texture_swizzle
 * 	ARB_timer_query
 * 	ARB_vertex_type_2_10_10_10_rev
 * 
 * 
 * 	OpenGL 4.0
 * 	ARB_shader_subroutine
 * 	ARB_texture_buffer_object_rgb32
 * 	ARB_transform_feedback2
 * 	ARB_draw_indirect
 * 	ARB_tessellation_shader
 * 
 * 	OpenGL 4.1
 * 	ARB_separate_shader_objects
 * 	ARB_debug_output
 * 	
 * 
 * 	OpenGL 4.2
 * 	ARB_shading_language_420pack
 * 	ARB_map_buffer_alignment
 * 
 * 	OpenGL 4.3
 * 	ARB_vertex_attrib_binding
 * 	KHR_debug
 * 	ARB_program_interface_query
 * 	ARB_explicit_uniform_location YES
 * 	
 */

class GraphicEngine
{
public:
    GraphicEngine();

    void init(uint width,uint height);
    void resize(uint width,uint height);
    void render(uint m, uint p, uint t);

    // FS Resource loaders
    
    uint loadShader(string name, string filePath, GLenum type);
    void unloadShader(uint id);
    
    uint loadMesh(string name, string filePath);
    void unloadMesh(uint id);
    
    uint loadTexture(string name, string filePath);
    uint unloadTexture(uint id);

    // Shader program methods
    
    uint createProgram(string name);
    void addShaderToProgram(uint shader, uint program);
    void linkProgram(uint program);
    void deleteProgram(uint id);

    // Scene graph management
    
    uint createPiece(string name);
    void addTextureToPiece(uint texture, uint piece);
    void addProgramToPiece(uint program, uint piece);
    void addMeshToPiece(uint mesh, uint piece);
    void deletePiece(uint id);

    uint createObject(string name);
    void addPieceToObject(uint piece, uint object);
    void addObjectToObject(uint componant, uint composite);
    void deleteObject(uint id);
    
    
    // Memory management
    void collectGarbadges();

    /**
     * TODO
     * add properties
     * add lights
     */

private: // Methods
    
    uint getNextID()
    {
      return ++lastID;
    }
    
private: // Class methods

    void static inline GLCheckError(const char* stmt, const char* file, int line)
    {
	GLenum err = glGetError();
	
	if (err == GL_NO_ERROR)
	  return;
	
	std::cerr << "OpenGL error ";
	
	switch(err)
	{
	  case GL_INVALID_ENUM:
	    std::cerr << "GL_INVALID_ENUM";
	    break;
	  case GL_INVALID_VALUE:
	    std::cerr << "GL_INVALID_VALUE";
	    break;
	  case GL_INVALID_OPERATION:
	    std::cerr << "GL_INVALID_OPERATION";
	    break;
	  case GL_INVALID_FRAMEBUFFER_OPERATION:
	    std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION";
	    break;
	  case GL_OUT_OF_MEMORY:
	    std::cerr << "GL_OUT_OF_MEMORY";
	    break;
	  case GL_STACK_UNDERFLOW:
	    std::cerr << "GL_STACK_UNDERFLOW";
	    break;
	  case GL_STACK_OVERFLOW:
	    std::cerr << "GL_STACK_OVERFLOW";
	    break;
	  default:
	    std::cerr << "UNKNOWN_ERROR";
	}
	
	std::cerr << ", at " << file << ":" << line << ", for " << stmt << endl;
	
    }

private: //

    
private:
    uint lastID;
    float test;
    
    enum
    {
      VERTEX_POSITION_ATTRIB = 0,
      VERTEX_NORMAL_ATTRIB = 1,
      VERTEX_TEXTCOORD_ATTRIB = 2
    };
    
    struct Resource
    {
	string name;
	uint backrefs;
    };

    /** Filesystem level resources
     *
     *  Corresponds to one file on the file system.
     * 
     *  NOTE: We keep back reference count to desallocate
     *  resources not needed anymore.
     */
    
    struct FSResource : Resource
    {
	uint size;
    };

    struct mesh : FSResource
    {
        GLuint VBO;
        GLuint IBO;
	GLuint VAO;
	uint sizeOfIBO;
        // TODO bounding sphere
    };

    struct texture : FSResource
    {
        GLuint id;
        uint height, width;
        // TODO compression type
    };
    
    map<ino_t, uint> inodeToResource;

    map<uint, mesh>     meshs;
    map<uint, texture>  textures;
    
    /** Shaders
     */

    struct shader : FSResource
    {
        GLuint id;
        GLenum type;
    };

    struct program : Resource
    {
        GLuint id;
    };

    map<uint, shader>   shaders;
    map<uint, program>  programs;

    /** Scene level resources
    *
    *  Corresponds to one object in the scene graph.
    */
    
    struct piece : Resource
    {
        uint mesh;
        uint program;
        uint texture; // TODO allow multiple textures
    };

    struct object : Resource
    {
        uint piece;
        vector<uint> object;
	
	glm::mat4 modelMatrix;
    };
		
    map<string, uint> pieces;
    map<string, uint> objects;
    
    // --------------
    
    uint width, height;
    
};

