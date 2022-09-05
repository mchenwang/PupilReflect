set(TEST_NAME test-refl-auto)

message(STATUS "===================================== ${TEST_NAME}")

set(REFLECT_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/test.h
    ${CMAKE_CURRENT_SOURCE_DIR}/test2.h
)

add_custom_target(PRE ALL
COMMAND ${CMAKE_COMMAND} -E echo "=============== [Precompile] BEGIN "
COMMAND ${PROJECT_ROOT}/tool/PupilReflTool.exe ${REFLECT_FILE}
COMMAND ${CMAKE_COMMAND} -E echo "=============== [Precompile] FINISHED"
)

add_executable(${TEST_NAME} test.h test.cpp)