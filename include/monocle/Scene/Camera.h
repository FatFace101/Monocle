#ifndef MONOCLE_CAMERA_H
#define MONOCLE_CAMERA_H

#include "monocle/Scene/SceneObject.h"

namespace mncl {
	class Camera : public SceneObject {
	public:
		float fov;
		float nearClip;
		float farClip;

		glm::mat4 getProjection(float aspectRatio);

		Camera(glm::vec3 position, glm::vec3 scale, glm::quat &rotation, float fov, float nearClip, float farClip) : SceneObject(position, scale, rotation), 
			fov(fov), nearClip(nearClip), farClip(farClip) {};
	};
};



#endif