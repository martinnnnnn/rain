

file(GLOB_RECURSE ENGINE_SOURCE_FILES "*.h" "*.hpp" "*.cpp" "*.inl")

add_executable(${RAIN_ENGINE} WIN32 ${ENGINE_SOURCE_FILES})

target_include_directories(${RAIN_ENGINE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${RAIN_ENGINE} ${RAIN_CORE})
target_include_directories(${RAIN_ENGINE} PUBLIC
    ${CORE_INCLUDE_PATH}
    ${ENGINE_INCLUDE_PATH}
    ${CMAKE_SOURCE_DIR}/external/glm)

set_target_properties(${RAIN_ENGINE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ENGINE_EXE_PATH})

GroupSources(${CMAKE_CURRENT_SOURCE_DIR} engine)

add_custom_command(TARGET ${RAIN_ENGINE} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data/exe_root"
         ${ENGINE_EXE_PATH}/$<CONFIGURATION>)