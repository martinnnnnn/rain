

file(GLOB_RECURSE CORE_SOURCE_FILES "*.h" "*.hpp" "*.cpp" "*.inl")

add_library(${RAIN_CORE} STATIC ${CORE_SOURCE_FILES})

set_target_properties(${RAIN_CORE} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CORE_LIBRARY_PATH})
target_include_directories(${RAIN_CORE} PUBLIC ${CMAKE_SOURCE_DIR}/src/core/src)

# GroupSources(src)
message(STATUS "core ${CMAKE_CURRENT_SOURCE_DIR}")
GroupSources2(${CMAKE_CURRENT_SOURCE_DIR} core)