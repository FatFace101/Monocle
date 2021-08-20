#ifndef MONOCLE_ASSET_MODEL_H
#define MONOCLE_ASSET_MODEL_H

#include "glad/glad.h"
#include "glm/glm/vec3.hpp"

namespace mncl {
	class VertexDataBuffer {
		uint32_t flags;
		GLuint buffer;
	};

	class Model {
	private:
		GLuint vertexBuffer;
		GLuint vertexIndexBuffer;
	public:
		Model(glm::vec3* verticies, uint32_t vertexCount);

	};




}

#endif