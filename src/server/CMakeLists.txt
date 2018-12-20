

file(GLOB_RECURSE SERVER_SOURCE_FILES "*.h" "*.hpp" "*.cpp" "*.inl")


add_executable(${RAIN_SERVER} ${SERVER_SOURCE_FILES})

target_link_libraries(${RAIN_SERVER} ${RAIN_CORE})

target_include_directories(${RAIN_SERVER} PUBLIC ${CORE_INCLUDE_PATH})

set_target_properties(${RAIN_SERVER} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SERVER_EXE_PATH})

# GroupSources(src)
message(STATUS "server ${CMAKE_CURRENT_SOURCE_DIR}")
GroupSources2(${CMAKE_CURRENT_SOURCE_DIR} server)