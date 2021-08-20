#ifndef MONOCLE_SHADER_LOADER_H
#define MONOCLE_SHADER_LOADER_H
#include <vector>
#include <istream>
#include "glad/glad.h"
namespace mncl {
	//A wrapper for OpenGL programs
	class ShaderLoader 
	{
		//ShaderLoader(ShaderLoader&&) = delete;
		//ShaderLoader(const ShaderLoader&) = delete;

		GLuint programId;
		std::vector<GLuint> shaderIds;
		char state;
	public:
		// Adds a shader to the program
		// - shaderType: a uint specifying the shader type
		// - inputStream: the stream from which you are getting your shader code
		// Returns true on success.
		bool addShader(GLenum shaderType, std::istream *inputStream);

		// Links, clears, and destroys the shaders.
		// Returns true on success.
		// Multiple calls are legal.
		bool linkProgram();

		// Gets the programId.
		// Call linkProgram before running.
		GLuint getProgram();

		operator GLuint() {
			return programId;
		}


		//Returns false if in error state
		operator bool() {
			return state != 2;
		};

		ShaderLoader();
	};
};



#endif