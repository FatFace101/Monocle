#ifndef MONOCLE_SCENE_OBJECT_H
#define MONOCLE_SCENE_OBJECT_H

#include "glm/glm/gtx/quaternion.hpp"
#include "glm/glm/mat4x4.hpp"

namespace mncl {
	class SceneObject {
	public:
		glm::vec3 position;
		glm::vec3 scale;
		glm::quat rotation;

		glm::mat4 getObjectToWorldMatrix();
		glm::mat4 getWorldToObjectMatrix();

		SceneObject(glm::vec3 position, glm::vec3 scale, glm::quat rotation) : position(position), scale(scale), rotation(rotation) {};
	};
};



#endif