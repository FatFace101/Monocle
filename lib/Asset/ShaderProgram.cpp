#include <string>
#include <iterator>
#include <iostream>
#include <cstdio>
#include <vector>

#include "monocle/Asset/ShaderProgram.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

using namespace mncl;



#define MNCL_SCALAR_ACCEPT_TYPES(T) const GLenum ShaderUniform<T>::AcceptTypes[]
#define MNCL_VECTOR_ACCEPT_TYPES(T, S) const GLenum ShaderUniformVector<T<S, glm::highp>, S>::AcceptTypes[]
#define MNCL_MATRIX_ACCEPT_TYPES(T, S) const GLenum ShaderUniformMatrix<T<S, glm::highp>, S>::AcceptTypes[]

MNCL_SCALAR_ACCEPT_TYPES(float) = { GL_FLOAT, GL_DOUBLE };
MNCL_SCALAR_ACCEPT_TYPES(int) = { GL_INT };
MNCL_SCALAR_ACCEPT_TYPES(unsigned int) = { GL_UNSIGNED_INT };

MNCL_VECTOR_ACCEPT_TYPES(glm::tvec1, float) = { GL_FLOAT     , GL_DOUBLE };
MNCL_VECTOR_ACCEPT_TYPES(glm::tvec2, float) = { GL_FLOAT_VEC2, GL_DOUBLE_VEC2 };
MNCL_VECTOR_ACCEPT_TYPES(glm::tvec3, float) = { GL_FLOAT_VEC3, GL_DOUBLE_VEC3 };
MNCL_VECTOR_ACCEPT_TYPES(glm::tvec4, float) = { GL_FLOAT_VEC4, GL_DOUBLE_VEC4 };

MNCL_VECTOR_ACCEPT_TYPES(glm::tvec1, int) = { GL_INT };
MNCL_VECTOR_ACCEPT_TYPES(glm::tvec2, int) = { GL_INT_VEC2 };
MNCL_VECTOR_ACCEPT_TYPES(glm::tvec3, int) = { GL_INT_VEC3 };
MNCL_VECTOR_ACCEPT_TYPES(glm::tvec4, int) = { GL_INT_VEC4 };

MNCL_VECTOR_ACCEPT_TYPES(glm::tvec1, unsigned int) = { GL_UNSIGNED_INT };
MNCL_VECTOR_ACCEPT_TYPES(glm::tvec2, unsigned int) = { GL_UNSIGNED_INT_VEC2 };
MNCL_VECTOR_ACCEPT_TYPES(glm::tvec3, unsigned int) = { GL_UNSIGNED_INT_VEC3 };
MNCL_VECTOR_ACCEPT_TYPES(glm::tvec4, unsigned int) = { GL_UNSIGNED_INT_VEC4 };

MNCL_MATRIX_ACCEPT_TYPES(glm::tmat2x2, float) = { GL_FLOAT_MAT2  , GL_DOUBLE_MAT2 };
MNCL_MATRIX_ACCEPT_TYPES(glm::tmat2x3, float) = { GL_FLOAT_MAT2x3, GL_DOUBLE_MAT2x3 };
MNCL_MATRIX_ACCEPT_TYPES(glm::tmat2x4, float) = { GL_FLOAT_MAT2x4, GL_DOUBLE_MAT2x4 };
MNCL_MATRIX_ACCEPT_TYPES(glm::tmat3x2, float) = { GL_FLOAT_MAT3x2, GL_DOUBLE_MAT3x2 };
MNCL_MATRIX_ACCEPT_TYPES(glm::tmat3x3, float) = { GL_FLOAT_MAT3  , GL_DOUBLE_MAT3 };
MNCL_MATRIX_ACCEPT_TYPES(glm::tmat3x4, float) = { GL_FLOAT_MAT3x4, GL_DOUBLE_MAT3x4 };
MNCL_MATRIX_ACCEPT_TYPES(glm::tmat4x2, float) = { GL_FLOAT_MAT4x2, GL_DOUBLE_MAT4x2 };
MNCL_MATRIX_ACCEPT_TYPES(glm::tmat4x3, float) = { GL_FLOAT_MAT4x3, GL_DOUBLE_MAT4x3 };
MNCL_MATRIX_ACCEPT_TYPES(glm::tmat4x4, float) = { GL_FLOAT_MAT4  , GL_DOUBLE_MAT4 };


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


ShaderModule::ShaderModule(GLuint shaderID) : shaderID(shaderID) {}


ShaderModule::~ShaderModule() 
{
	glDeleteShader(shaderID);
}


bool ShaderModule::Create(ShaderModule **out, GLenum shaderType, std::istream *inputStream)
{
	//Get source for shader
	std::string fileString = std::string(std::istreambuf_iterator<char>(*inputStream), std::istreambuf_iterator<char>());
	const char* src = fileString.c_str();
	GLuint shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &src, nullptr);
	glCompileShader(shaderID);

	//Check for compile error
	GLint compSuccess = GL_FALSE;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compSuccess);
	if (!compSuccess)
	{
		glDeleteShader(shaderID);

		//Log error
		GLint logSize = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> errMsg((uint64_t)logSize + 1);
		glGetShaderInfoLog(shaderID, logSize, nullptr, errMsg.data());
		printf("Shader error:\n%s\n", errMsg.data());
		
		return false;
	}
	*out = new ShaderModule(shaderID);
	return true;
};


ShaderProgram::ShaderProgram(GLuint programID) : programID(programID) {};


ShaderProgram::~ShaderProgram() {
	glDeleteProgram(programID);
}

bool ShaderProgram::Create(ShaderProgram **out, uint32_t moduleCount, ShaderModule **modules)
{

	GLint programID = glCreateProgram();

	for (uint32_t moduleIndex = 0; moduleIndex < moduleCount; moduleIndex++)
	{
		glAttachShader(programID, modules[moduleIndex]->shaderID);
	}

	glLinkProgram(programID);

	//Check for link error
	GLint linkSuccess = GL_FALSE;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess) 
	{
		glDeleteProgram(programID);

		//Log error
		GLint logSize = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> errMsg((uint64_t)logSize + 1);
		glGetProgramInfoLog(programID, logSize, nullptr, errMsg.data());
		printf("Program link error:\n%s\n", errMsg.data());
		return 1;
	}
	for (uint32_t moduleIndex = 0; moduleIndex < moduleCount; moduleIndex++)
	{
		glDetachShader(programID, modules[moduleIndex]->shaderID);
	}
	*out = new ShaderProgram(programID); 
	return true;
};




void ShaderProgram::use() {
	glUseProgram(programID);
}


