#include "monocle/Render/Camera.h"
#include "glm/glm/gtc/matrix_transform.hpp"

glm::mat4 mncl::Camera::getProjection(float aspectRatio) {
	return glm::perspective(fov, aspectRatio, nearClip, farClip) * getWorldToObjectMatrix();

};