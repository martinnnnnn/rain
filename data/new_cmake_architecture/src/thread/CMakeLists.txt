


file(GLOB_RECURSE THREAD_SOURCE_FILES "*.h" "*.hpp" "*.cpp" "*.inl")

add_library(${RAIN_THREAD} STATIC ${THREAD_SOURCE_FILES})

set(THREAD_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/bin/rain_thread")
set_target_properties(${RAIN_THREAD} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${THREAD_LIBRARY_PATH})

GroupSources(src)