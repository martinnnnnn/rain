

file(GLOB_RECURSE CORE_SOURCE_FILES "*.h" "*.hpp" "*.cpp" "*.inl")

add_library(${RAIN_CORE} STATIC ${CORE_SOURCE_FILES})

set_target_properties(${RAIN_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CORE_LIBRARY_PATH})
target_include_directories(${RAIN_CORE} PUBLIC
    ${CORE_INCLUDE_PATH}
    C:/boost_1_69_0/
    ${CMAKE_SOURCE_DIR}/external/glm)

message(STATUS "hello ${CMAKE_SOURCE_DIR}/external/glm")

GroupSources(${CMAKE_CURRENT_SOURCE_DIR} core)