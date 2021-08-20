add_library(MonocleShader STATIC ShaderLoader.cpp)
target_link_libraries(MonocleShader glfw3 glad)
target_include_directories(MonocleShader PRIVATE ${ALL_INCLUDES})
set_target_properties(MonocleShader PROPERTIES FOLDER "Monocle/Libraries")
