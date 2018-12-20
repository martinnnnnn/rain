

file(GLOB_RECURSE SERVER_SOURCE_FILES "*.h" "*.hpp" "*.cpp" "*.inl")


add_executable(${RAIN_SERVER} ${SERVER_SOURCE_FILES})

target_link_libraries(${RAIN_SERVER} ${RAIN_THREAD})
target_link_libraries(${RAIN_SERVER} ${RAIN_NETWORK})

target_include_directories(${RAIN_SERVER} PUBLIC ${THREAD_INCLUDE_PATH})
target_include_directories(${RAIN_SERVER} PUBLIC ${NETWORK_INCLUDE_PATH})

set(SERVER_EXE_PATH "${CMAKE_SOURCE_DIR}/bin/rain_server")
set_target_properties(${RAIN_SERVER} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SERVER_EXE_PATH})

GroupSources(src)