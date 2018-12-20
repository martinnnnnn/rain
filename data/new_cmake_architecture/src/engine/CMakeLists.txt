

file(GLOB_RECURSE ENGINE_SOURCE_FILES "*.h" "*.hpp" "*.cpp" "*.inl")

add_executable(${RAIN_ENGINE} WIN32 ${ENGINE_SOURCE_FILES})

target_include_directories(${RAIN_ENGINE} PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${RAIN_ENGINE} ${RAIN_THREAD})
target_link_libraries(${RAIN_ENGINE} ${RAIN_NETWORK})
target_include_directories(${RAIN_ENGINE} PUBLIC ${THREAD_INCLUDE_PATH})
target_include_directories(${RAIN_ENGINE} PUBLIC ${NETWORK_INCLUDE_PATH})

set(ENGINE_EXE_PATH "${CMAKE_SOURCE_DIR}/bin/rain_engine")
set_target_properties(${RAIN_ENGINE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ENGINE_EXE_PATH})

GroupSources(src)