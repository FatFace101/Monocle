add_library("glad" STATIC src/glad.c)
set_target_properties("glad" PROPERTIES FOLDER "Glad")
target_include_directories("glad" PRIVATE "include")