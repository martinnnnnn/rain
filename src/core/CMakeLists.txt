

file(GLOB_RECURSE CORE_SOURCE_FILES "*.h" "*.hpp" "*.cpp" "*.inl")

add_library(${RAIN_CORE} STATIC ${CORE_SOURCE_FILES})

set_target_properties(${RAIN_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CORE_LIBRARY_PATH})
target_include_directories(${RAIN_CORE} PUBLIC
    ${CORE_INCLUDE_PATH}
    C:/boost/
    ${CMAKE_SOURCE_DIR}/external/glm/glm/)

target_compile_definitions(${RAIN_CORE} PUBLIC RAIN_DATA_ROOT="${CMAKE_SOURCE_DIR}/data")
target_compile_definitions(${RAIN_CORE} PUBLIC RAIN_RUNTIME_DATA_ROOT="${CMAKE_SOURCE_DIR}/runtime_data")

GroupSources(${CMAKE_CURRENT_SOURCE_DIR} core)