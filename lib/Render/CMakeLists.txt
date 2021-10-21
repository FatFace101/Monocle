add_library(MonocleRender STATIC RenderPass.cpp Camera.cpp SceneObject.cpp Scene.cpp RenderLoop.cpp SceneOrdering.cpp)
target_link_libraries(MonocleRender glfw3 glad)
target_include_directories(MonocleRender PRIVATE ${ALL_INCLUDES})
set_target_properties(MonocleRender PROPERTIES FOLDER "Monocle/Libraries")
