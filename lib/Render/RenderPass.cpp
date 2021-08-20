#include "monocle/Render/RenderPass.h"



using namespace mncl;

template<class TriggerType>
void RenderPass<TriggerType>::render() {
	for (int i = 0; i < triggers.size; i++) {
		subpassTrigger = triggers[i];
		
	}
}

