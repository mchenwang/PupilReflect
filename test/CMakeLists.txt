set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/target/)

include_directories(${PROJECT_SOURCE_DIR}/src)

# add_subdirectory(basic)
# add_subdirectory(refl-simple)
# add_subdirectory(refl-template)
# add_subdirectory(refl-nest)
# add_subdirectory(refl-nest2)
# add_subdirectory(refl-inherit)
add_subdirectory(refl-auto)