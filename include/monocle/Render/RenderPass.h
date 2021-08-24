#ifndef MNCL_RENDER_PASS_H
#define MNCL_RENDER_PASS_H

#include <vector>

#include "monocle/Scene/SceneObject.h"
#include "monocle/Asset/ShaderProgram.h"

namespace mncl {

template<class TriggerType> 
class RenderPass {

	void render();

public:
	TriggerType *subpassTrigger;
	// Designates a new subpass with an associated trigger.
	void addSubpass(TriggerType *trigger);

	// Determines whether SceneObject obj should be rendered for this subpass.
	virtual bool cullObject(SceneObject obj) = 0;
	
	// Loads uniforms to a ShaderProgram given a SceneObject and the current subpass trigger.
	virtual void loadUniforms(ShaderProgram program, SceneObject obj) = 0;
	

};


};

#endif
