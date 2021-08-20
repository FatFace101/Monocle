#include "monocle/Shader/ShaderLoader.h"
#include <string>
#include <iterator>
#include <iostream>
#include <cstdio>

using namespace mncl;
bool ShaderLoader::addShader(GLenum shaderType, std::istream *inputStream) {
	if (state) {
		return 1;
	}

	//load shader
	
	std::string fileString = std::string(std::istreambuf_iterator<char>(*inputStream), std::istreambuf_iterator<char>());
	std::cout << fileString << "\n";
	const char* src = fileString.c_str();
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &src, nullptr);


	GLint compSuccess = GL_FALSE;

	// compile shader
	glCompileShader(shaderId);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compSuccess);
	if (!compSuccess) {
		GLint logSize = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> errMsg((uint64_t)logSize + 1);
		glGetShaderInfoLog(shaderId, logSize, nullptr, errMsg.data());
		printf("Shader error: %s\n", errMsg.data());
		glDeleteShader(shaderId);
		return false;
	}
	glAttachShader(programId, shaderId);
	shaderIds.push_back(shaderId);
	return true;
};

bool ShaderLoader::linkProgram() {
	if (state) {
		return (state == 1);
	}
	state = 1;
	glLinkProgram(programId);
	GLint linkSuccess = GL_FALSE;
	glGetProgramiv(programId, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess) {
		state = 2;
		GLint logSize = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> errMsg((uint64_t)logSize + 1);
		glGetProgramInfoLog(programId, logSize, nullptr, errMsg.data());
		printf("%s\n", errMsg.data());
		return 1;
	}
	for (GLuint shaderId:shaderIds)
	{
		glDetachShader(programId, shaderId);
		glDeleteShader(shaderId);
		
	}
	return linkSuccess == GL_TRUE;
};


GLuint ShaderLoader::getProgram() {
	return programId;
};

ShaderLoader::ShaderLoader() : programId(glCreateProgram()), state(0), shaderIds(std::vector<GLuint>()) {};