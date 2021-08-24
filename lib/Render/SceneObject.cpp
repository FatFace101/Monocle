#include "monocle/Scene/SceneObject.h"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/quaternion.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/vec3.hpp"

using namespace mncl;

glm::mat4 SceneObject::getObjectToWorldMatrix() {
	return glm::translate(glm::scale(glm::toMat4(rotation), scale), position);
};

glm::mat4 SceneObject::getWorldToObjectMatrix() {
	return glm::toMat4(glm::conjugate<float>(rotation)) * glm::scale(glm::translate<float>(-position),glm::vec3(1.0f/scale.x, 1.0f/scale.y, 1.0f/scale.z));
};