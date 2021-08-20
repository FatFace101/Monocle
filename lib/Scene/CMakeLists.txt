add_library(MonocleScene STATIC Camera.cpp SceneObject.cpp)
target_link_libraries(MonocleScene glad glm_static)
target_include_directories(MonocleScene PRIVATE ${ALL_INCLUDES})
set_target_properties(MonocleScene PROPERTIES FOLDER "Monocle/Libraries")
