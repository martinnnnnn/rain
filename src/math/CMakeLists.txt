

file(GLOB_RECURSE MATH_SOURCE_FILES "*.h" "*.hpp" "*.cpp" "*.inl")

add_library(${RAIN_MATH} STATIC ${MATH_SOURCE_FILES})

target_include_directories(${RAIN_MATH} PUBLIC
    ${CMAKE_SOURCE_DIR}/external/json
    ${MATH_INCLUDE_PATH})

set_target_properties(${RAIN_MATH} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${MATH_LIBRARY_PATH})

GroupSources(${CMAKE_CURRENT_SOURCE_DIR} math)