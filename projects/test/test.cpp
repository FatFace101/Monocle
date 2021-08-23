


#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#include "monocle/Asset/ShaderProgram.h"
#include "monocle/Scene/SceneObject.h"
#include "monocle/Scene/Camera.h"
#include "monocle/Asset/Texture.h"
#include "monocle/Asset/ShaderUniformValue.h"



#include <map>
#include <fstream>
#include <vector>

//Angle is in radians
static glm::quat AxisAngle(glm::vec3 axis, float angle) {
	glm::quat ret;
	float sv = glm::sin(angle * 0.5f);
	ret.w = glm::cos(angle * 0.5f);
	ret.x = axis.x * sv;
	ret.y = axis.y * sv;
	ret.z = axis.z * sv;
	return ret;
};

static void ErrCallback(int err, const char* msg) {
	printf("Error: %s\n", msg);
};

static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
	glViewport(0,0,width,height);
};

int main(int argc, char** argv) {
	if (!glfwInit()) {
		return 1;
	}
	GLushort vertexIndecies[] = {
		0,2,1,
		1,2,3,
		0,4,2,
		2,4,6,
		4,0,1,
		4,1,5,
		5,7,6,
		6,4,5,
		7,5,3,
		5,1,3,
		7,3,6,
		3,2,6
	};
	float vertexUVs[] = {
		0.0f,0.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f
	};
	float verts[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	glfwSetErrorCallback(ErrCallback);
	
	GLFWwindow *window = glfwCreateWindow(800,450,"Monocle", nullptr, nullptr);
	if (!window) {
		return 1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	glfwMakeContextCurrent(window);
	gladLoadGL();

	mncl::ShaderUniformBase::SetupGLFunctions();

	glfwSwapInterval(1);
	GLuint vertexBuffer;
	glGenBuffers(1,&vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	GLuint polyBuffer;
	glGenBuffers(1, &polyBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polyBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIndecies), vertexIndecies, GL_STATIC_DRAW);

	GLuint uvBuffer;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexUVs), vertexUVs, GL_STATIC_DRAW);
	
	mncl::ShaderModule* vShader; 
	if (!mncl::ShaderModule::Create(&vShader, GL_VERTEX_SHADER, new std::ifstream("vertex.gl"))) {
		return 1;
	}

	mncl::ShaderModule* fShader; 
	if (!mncl::ShaderModule::Create(&fShader, GL_FRAGMENT_SHADER, new std::ifstream("fragment.gl"))) {
		return 1;
	}

	mncl::ShaderProgram* shaderProgram;
	if (!mncl::ShaderProgram::Create(&shaderProgram, 2, new mncl::ShaderModule*[2] {vShader, fShader})) {
		return 1;
	}

	
	

	glm::vec3 col(0.0f,0.0f,1.0f);

	
	mncl::Texture* tex = new mncl::Texture("boi.png");

	

	mncl::ShaderUniform<glm::mat4x4> modelToScreenMatrixShaderUL;
	shaderProgram->getUniform(&modelToScreenMatrixShaderUL, "ModelToScreen");
	mncl::ShaderUniform<glm::vec3> backColorUL;
	shaderProgram->getUniform(&backColorUL, "BackGColor");
	mncl::ShaderUniform<int> textureLoc;
	shaderProgram->getUniform(&textureLoc, "TexSampler");

	
	glClearColor(1.0f,1.0f,0.5f,0.0f);
	
	glm::quat camrot;
		camrot.x = 0.0f;
		camrot.y = 0.0f;
		camrot.z = 0.0f;
		camrot.w = 1.0f;
	glm::quat objrot;
		objrot.x = 0.0f;
		objrot.y = 0.38268343236f;
		objrot.z = 0.0f;
		objrot.w = 0.92387953251f;
	mncl::Camera camera = mncl::Camera(glm::vec3(0,0,10),glm::vec3(1,1,1),camrot,glm::radians<float>(70.0f),0.1f,100.0f); 
	mncl::SceneObject triangle = mncl::SceneObject(glm::vec3(0,0,0),glm::vec3(1,1,1),objrot);
	
	
	double lastTime = glfwGetTime();
	double lastSecondTime = lastTime;
	int nbFrames = 0;

	//glm::quat animRot = glm::rotation<float>(glm::vec);
	float cameraXRot = 0;
	float cameraYRot = 0;

	double lastCursorX;
	double lastCursorY;
	glfwGetCursorPos(window, &lastCursorX, &lastCursorY);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	double angle = 0.0f;
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	while (!glfwWindowShouldClose(window))
	{
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int windowWidth;
		int windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		float aspectRatio = (float)windowWidth / (float)windowHeight;

		triangle.rotation = AxisAngle(glm::vec3(0.0f, 1.0f, 0.0f), angle);

		glm::mat4 modelToScreenMatrix = camera.getProjection(aspectRatio) * triangle.getObjectToWorldMatrix();


		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glVertexAttribPointer(
			1,                                
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);


		shaderProgram->use();

		modelToScreenMatrixShaderUL.setValue(modelToScreenMatrix);
		backColorUL.setValue(glm::vec3(1.0,0.0,0.0));

		int texSlot = 0;
		textureLoc.setValue(&texSlot);
		tex->bind(texSlot);
		glDrawElements(GL_TRIANGLES, sizeof(vertexIndecies)/sizeof(GLushort), GL_UNSIGNED_SHORT, (void*)0); // Starting from vertex 0; 3 vertices total -> 1 triangle
		

		glfwSwapBuffers(window);
		glfwPollEvents();

		
		double currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastSecondTime >= 1.0 ){ 
			// printf and reset timer
			printf("%f ms/frame\n", 1000.0/double(nbFrames));
			nbFrames = 0;
			lastSecondTime += 1.0;
		}

		double deltaTime = currentTime - lastTime;

		double cursorX;
		double cursorY;
		glfwGetCursorPos(window, &cursorX, &cursorY);
		double dCursorX = cursorX - lastCursorX;
		double dCursorY = cursorY - lastCursorY;
		angle += deltaTime * 0.1f;

		cameraXRot = glm::clamp((float)(dCursorY * -0.005 + cameraXRot), -1.5f, 1.5f);
		cameraYRot = glm::mod((float)(dCursorX * -0.005 + cameraYRot), glm::two_pi<float>());
		camera.rotation = AxisAngle(glm::vec3(0.0f,1.0f,0.0f), cameraYRot) * AxisAngle(glm::vec3(1.0f,0.0f,0.0f), cameraXRot);


		lastCursorX = cursorX;
		lastCursorY = cursorY;

		glm::vec4 camTranslate = glm::vec4(0.0f,0.0f,0.0f,0.0f);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camTranslate.z -= 1;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			camTranslate.z += 1;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camTranslate.x -= 1;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camTranslate.x += 1;
		}
		camTranslate *= deltaTime * 1;
		
		camTranslate = camera.getObjectToWorldMatrix() * camTranslate;
		camera.position = glm::vec3(camTranslate.x,camTranslate.y,camTranslate.z) + camera.position;


		lastTime = currentTime;
	}
	glfwDestroyWindow(window);
	glfwTerminate();
};