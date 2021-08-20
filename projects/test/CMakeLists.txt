add_executable(test test.cpp)
target_link_libraries(test glfw3 glad MonocleAsset MonocleScene MonocleSupport glm_static)
target_include_directories(test PRIVATE ${ALL_INCLUDES})
set_target_properties(test PROPERTIES FOLDER "Monocle/Projects")