#include <string>
#include <iterator>
#include <iostream>
#include <cstdio>
#include <vector>

#include "monocle/Asset/ShaderProgram.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

using namespace mncl;






#define MNCL_SCALAR_FUNCTION_D(T) ShaderUniform<T>::GLFunc
#define MNCL_VECTOR_FUNCTION_D(T, S) ShaderUniformVector<T<S, glm::highp>, S>::GLFunc
#define MNCL_MATRIX_FUNCTION_D(T, S) ShaderUniformMatrix<T<S, glm::highp>, S>::GLFunc


void ShaderUniformBase::SetupGLFunctions() {
	MNCL_SCALAR_FUNCTION_D(float) = glUniform1f;
	MNCL_SCALAR_FUNCTION_D(int) = glUniform1i;
	MNCL_SCALAR_FUNCTION_D(unsigned int) = glUniform1ui;

	MNCL_VECTOR_FUNCTION_D(glm::tvec1, float) = glUniform1fv;
	MNCL_VECTOR_FUNCTION_D(glm::tvec2, float) = glUniform2fv;
	MNCL_VECTOR_FUNCTION_D(glm::tvec3, float) = glUniform3fv;
	MNCL_VECTOR_FUNCTION_D(glm::tvec4, float) = glUniform4fv;

	MNCL_VECTOR_FUNCTION_D(glm::tvec1, int) = glUniform1iv;
	MNCL_VECTOR_FUNCTION_D(glm::tvec2, int) = glUniform2iv;
	MNCL_VECTOR_FUNCTION_D(glm::tvec3, int) = glUniform3iv;
	MNCL_VECTOR_FUNCTION_D(glm::tvec4, int) = glUniform4iv;

	MNCL_VECTOR_FUNCTION_D(glm::tvec1, unsigned int) = glUniform1uiv;
	MNCL_VECTOR_FUNCTION_D(glm::tvec2, unsigned int) = glUniform2uiv;
	MNCL_VECTOR_FUNCTION_D(glm::tvec3, unsigned int) = glUniform3uiv;
	MNCL_VECTOR_FUNCTION_D(glm::tvec4, unsigned int) = glUniform4uiv;

	MNCL_MATRIX_FUNCTION_D(glm::tmat2x2, float) = glUniformMatrix2fv;
	MNCL_MATRIX_FUNCTION_D(glm::tmat2x3, float) = glUniformMatrix2x3fv;
	MNCL_MATRIX_FUNCTION_D(glm::tmat2x4, float) = glUniformMatrix2x4fv;
	MNCL_MATRIX_FUNCTION_D(glm::tmat3x2, float) = glUniformMatrix3x2fv;
	MNCL_MATRIX_FUNCTION_D(glm::tmat3x3, float) = glUniformMatrix3fv;
	MNCL_MATRIX_FUNCTION_D(glm::tmat3x4, float) = glUniformMatrix3x4fv;
	MNCL_MATRIX_FUNCTION_D(glm::tmat4x2, float) = glUniformMatrix4x2fv;
	MNCL_MATRIX_FUNCTION_D(glm::tmat4x3, float) = glUniformMatrix4x3fv;
	MNCL_MATRIX_FUNCTION_D(glm::tmat4x4, float) = glUniformMatrix4fv;
}


ShaderUniformBase::ShaderUniformBase() : uniformLocation(-1) {}

ShaderUniformBase::operator bool() const { return uniformLocation >= 0; }

bool ShaderUniformBase::isValid() const { return uniformLocation >= 0; }


ShaderModule::ShaderModule(GLenum shaderType, std::istream* inputStream) {
	std::string fileString = std::string(std::istreambuf_iterator<char>(*inputStream), std::istreambuf_iterator<char>());
	const char* src = fileString.c_str();
	shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &src, nullptr);
	glCompileShader(shaderId);

	GLint compSuccess = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compSuccess);
	if (!compSuccess)
	{
		//Log error
		/*GLint logSize = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
		char* errMsg = new char[(uint64_t)logSize + 1];
		glGetShaderInfoLog(shaderId, logSize, nullptr, errMsg);
		printf("Shader error:\n%s\n", errMsg);*/
		glDeleteShader(shaderId);
		throw new std::runtime_error("Shader compilation error");
	}
}


ShaderModule::~ShaderModule() 
{
	glDeleteShader(shaderId);
}

ShaderProgram::ShaderProgram(uint32_t moduleCount, ShaderModule** modules) {
	programId = glCreateProgram();

	for (uint32_t moduleIndex = 0; moduleIndex < moduleCount; moduleIndex++)
	{
		glAttachShader(programId, modules[moduleIndex]->shaderId);
	}

	glLinkProgram(programId);

	//Check for link error
	GLint linkSuccess = GL_FALSE;
	glGetProgramiv(programId, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess)
	{
		//Log error
		/*GLint logSize = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logSize);
		char* errMsg = new char[(uint64_t)logSize + 1];
		glGetProgramInfoLog(programID, logSize, nullptr, errMsg);
		printf("Program link error:\n%s\n", errMsg);*/
		glDeleteProgram(programId);
		throw new std::runtime_error("Shader link error");
	}
	for (uint32_t moduleIndex = 0; moduleIndex < moduleCount; moduleIndex++)
	{
		glDetachShader(programId, modules[moduleIndex]->shaderId);
	}
};


ShaderProgram::~ShaderProgram() {
	glDeleteProgram(programId);
}


void ShaderProgram::use() const {
	glUseProgram(programId);
}


