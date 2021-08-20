#ifndef MONOCLE_SHADER_PROGRAM_H
#define MONOCLE_SHADER_PROGRAM_H

#include <istream>
#include <map>

#include "glad/glad.h"

#include "monocle/Scene/SceneObject.h"



namespace mncl 
{

	class ShaderProgram;
	
	template<typename T> class ShaderUniform;
	template<typename T, typename S> class ShaderUniformVector;
	template<typename T, typename S> class ShaderUniformMatrix;

	class ShaderUniformBase {
		friend class ShaderProgram;
		template<typename T> friend class ShaderUniform;
		template<typename T, typename S> friend class ShaderUniformVector;
		template<typename T, typename S> friend class ShaderUniformMatrix;

		GLint uniformLocation;

		ShaderUniformBase();
	public:
		static void SetupGLFunctions();


		bool isValid() const;
		operator bool() const;
	};

	template<typename T, typename S> class ShaderUniformVector : public ShaderUniformBase {
		template<typename T> friend class ShaderUniform;
		friend class ShaderUniformBase;
		static void (APIENTRYP GLFunc)(GLint location, GLsizei count, const S* value);
		static const GLenum AcceptTypes[];
		ShaderUniformVector() = default;
	public:
		void setValue(const T& value) {
			GLFunc(uniformLocation, 1, glm::value_ptr<S>(value));
		};
	};

	template<typename T, typename S> class ShaderUniformMatrix : public ShaderUniformBase {
		template<typename T> friend class ShaderUniform;
		friend class ShaderUniformBase;
		static void (APIENTRYP GLFunc)(GLint location, GLsizei count, GLboolean transpose, const S* value);
		static const GLenum AcceptTypes[];
		ShaderUniformMatrix() = default;
	public:
		
		void setValue(const T& value) {
			GLFunc(uniformLocation, 1, false, glm::value_ptr<S>(value));
		};
	};

	template<typename T> class ShaderUniform final : public ShaderUniformBase {
		friend class ShaderUniformBase;
		static void (APIENTRYP GLFunc)(GLint location, T value);
		static const GLenum AcceptTypes[];
	public:
		void setValue(const T* value) {
			GLFunc(uniformLocation, *value);
		};
	};

	// --- Vector uniforms ---
	template<typename T, glm::precision P> class ShaderUniform<glm::tvec1<T, P>> final : public ShaderUniformVector<glm::tvec1<T, P>, T> {};
	template<typename T, glm::precision P> class ShaderUniform<glm::tvec2<T, P>> final : public ShaderUniformVector<glm::tvec2<T, P>, T> {};
	template<typename T, glm::precision P> class ShaderUniform<glm::tvec3<T, P>> final : public ShaderUniformVector<glm::tvec3<T, P>, T> {};
	template<typename T, glm::precision P> class ShaderUniform<glm::tvec4<T, P>> final : public ShaderUniformVector<glm::tvec4<T, P>, T> {};

	// --- Matrix uniforms ---
	template<typename T, glm::precision P> class ShaderUniform<glm::tmat2x2<T, P>> final : public ShaderUniformMatrix<glm::tmat2x2<T, P>, T> {};
	template<typename T, glm::precision P> class ShaderUniform<glm::tmat2x3<T, P>> final : public ShaderUniformMatrix<glm::tmat2x3<T, P>, T> {};
	template<typename T, glm::precision P> class ShaderUniform<glm::tmat2x4<T, P>> final : public ShaderUniformMatrix<glm::tmat2x4<T, P>, T> {};

	template<typename T, glm::precision P> class ShaderUniform<glm::tmat3x2<T, P>> final : public ShaderUniformMatrix<glm::tmat3x2<T, P>, T> {};
	template<typename T, glm::precision P> class ShaderUniform<glm::tmat3x3<T, P>> final : public ShaderUniformMatrix<glm::tmat3x3<T, P>, T> {};
	template<typename T, glm::precision P> class ShaderUniform<glm::tmat3x4<T, P>> final : public ShaderUniformMatrix<glm::tmat3x4<T, P>, T> {};

	template<typename T, glm::precision P> class ShaderUniform<glm::tmat4x2<T, P>> final : public ShaderUniformMatrix<glm::tmat4x2<T, P>, T> {};
	template<typename T, glm::precision P> class ShaderUniform<glm::tmat4x3<T, P>> final : public ShaderUniformMatrix<glm::tmat4x3<T, P>, T> {};
	template<typename T, glm::precision P> class ShaderUniform<glm::tmat4x4<T, P>> final : public ShaderUniformMatrix<glm::tmat4x4<T, P>, T> {};

	
	class ShaderModule
	{
	private:
		friend class ShaderProgram;

		GLuint shaderID;
		
		ShaderModule(GLuint shaderID);
		~ShaderModule();
		GLuint *getUniformLocation();
	public:
		static bool Create(ShaderModule** out, GLenum shaderType, std::istream* inputStream);
	};


	class ShaderProgram
	{
	private:
		ShaderProgram(GLuint programID);
		~ShaderProgram();
	public:
		GLint programID;
		template <typename T> bool getUniform(ShaderUniform<T>* uniform, const char* uniformName) {
			use();
			GLint ul = glGetUniformLocation(programID, uniformName);
			if (ul < 0) {
				return false;
			}
			int uniformSize;
			GLenum uniformType;
			glGetActiveUniform(programID, ul, 0, nullptr, &uniformSize, &uniformType, nullptr);

			for (int )

			uniform->uniformLocation = ul;

			return true;
		};
		void use();
		static bool Create(ShaderProgram** out, uint32_t moduleCount, ShaderModule** modules);
	};

	

	#define MNCL_SCALAR_FUNCTION(T) void (APIENTRYP ShaderUniform<T>::GLFunc)(GLint, T) 
	#define MNCL_VECTOR_FUNCTION(T, S) void (APIENTRYP ShaderUniformVector<T<S, glm::highp>, S>::GLFunc)(GLint, GLsizei, const S*)
	#define MNCL_MATRIX_FUNCTION(T, S) void (APIENTRYP ShaderUniformMatrix<T<S, glm::highp>, S>::GLFunc)(GLint, GLsizei, GLboolean, const S*)

	MNCL_SCALAR_FUNCTION(float);
	MNCL_SCALAR_FUNCTION(int);
	MNCL_SCALAR_FUNCTION(unsigned int);

	MNCL_VECTOR_FUNCTION(glm::tvec1, float);
	MNCL_VECTOR_FUNCTION(glm::tvec2, float);
	MNCL_VECTOR_FUNCTION(glm::tvec3, float);
	MNCL_VECTOR_FUNCTION(glm::tvec4, float);

	MNCL_VECTOR_FUNCTION(glm::tvec1, int);
	MNCL_VECTOR_FUNCTION(glm::tvec2, int);
	MNCL_VECTOR_FUNCTION(glm::tvec3, int);
	MNCL_VECTOR_FUNCTION(glm::tvec4, int);

	MNCL_VECTOR_FUNCTION(glm::tvec1, unsigned int);
	MNCL_VECTOR_FUNCTION(glm::tvec2, unsigned int);
	MNCL_VECTOR_FUNCTION(glm::tvec3, unsigned int);
	MNCL_VECTOR_FUNCTION(glm::tvec4, unsigned int);

	MNCL_MATRIX_FUNCTION(glm::tmat2x2, float);
	MNCL_MATRIX_FUNCTION(glm::tmat2x3, float);
	MNCL_MATRIX_FUNCTION(glm::tmat2x4, float);
	MNCL_MATRIX_FUNCTION(glm::tmat3x2, float);
	MNCL_MATRIX_FUNCTION(glm::tmat3x3, float);
	MNCL_MATRIX_FUNCTION(glm::tmat3x4, float);
	MNCL_MATRIX_FUNCTION(glm::tmat4x2, float);
	MNCL_MATRIX_FUNCTION(glm::tmat4x3, float);
	MNCL_MATRIX_FUNCTION(glm::tmat4x4, float);


};



#endif