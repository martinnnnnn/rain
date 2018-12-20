

file(GLOB_RECURSE NETWORK_SOURCE_FILES "*.h" "*.hpp" "*.cpp" "*.inl")


add_library(${RAIN_NETWORK} STATIC ${NETWORK_SOURCE_FILES})
target_link_libraries(${RAIN_NETWORK} ${RAIN_THREAD})

target_include_directories(${RAIN_NETWORK} PUBLIC ${THREAD_INCLUDE_PATH})

set(NETWORK_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/bin/rain_network")
set_target_properties(${RAIN_NETWORK} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${NETWORK_LIBRARY_PATH})

GroupSources(src)