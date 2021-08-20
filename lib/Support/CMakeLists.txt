add_library(MonocleSupport STATIC StreamReader.cpp)
target_link_libraries(MonocleSupport glm_static)
target_include_directories(MonocleSupport PRIVATE ${ALL_INCLUDES})
set_target_properties(MonocleSupport PROPERTIES FOLDER "Monocle/Libraries")
