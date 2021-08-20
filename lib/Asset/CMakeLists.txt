add_library(MonocleAsset STATIC ShaderProgram.cpp Texture.cpp)
target_link_libraries(MonocleAsset glfw3 glad)
target_include_directories(MonocleAsset PRIVATE ${ALL_INCLUDES})
set_target_properties(MonocleAsset PROPERTIES FOLDER "Monocle/Libraries")
