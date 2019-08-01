

file(GLOB_RECURSE ENGINE_SOURCE_FILES "*.h" "*.hpp" "*.cpp" "*.inl")


# FOREACH(FILE_NAME ${ENGINE_SOURCE_FILES})
#     string(FIND ${FILE_NAME} "win32" retval)
#     string(COMPARE NOTEQUAL ${retval}  "-1" is_not_equal)
#     if (${is_not_equal})
#         list(REMOVE_ITEM ENGINE_SOURCE_FILES ${FILE_NAME})
#     endif()
# ENDFOREACH()

# FOREACH(FILE_NAME ${ENGINE_SOURCE_FILES})
#     message(STATUS "<<${FILE_NAME}>>")
# ENDFOREACH()

add_executable(${RAIN_ENGINE} WIN32 ${ENGINE_SOURCE_FILES})

set(LIBS
    ${GLEW_LIBRARIES}
    OpenGL32
    GlU32
    freetype
    dinput8
    dxguid)
# set (LIBS_RELEASE ${LIBS} ${FBXSDK_LIBRARY})
# set (LIBS_DEBUG ${LIBS} ${FBXSDK_LIBRARY_DEBUG})
target_link_libraries(${RAIN_ENGINE} ${LIBS} optimized ${FBXSDK_LIBRARY} debug ${FBXSDK_LIBRARY_DEBUG})


target_include_directories(${RAIN_ENGINE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${RAIN_ENGINE}
    ${RAIN_CORE}
    ${RAIN_NEWORK}
    )
target_include_directories(${RAIN_ENGINE} PUBLIC
    ${CORE_INCLUDE_PATH}
    ${NETWORK_INCLUDE_PATH}
    ${ENGINE_INCLUDE_PATH}
    ${CMAKE_SOURCE_DIR}/external/json/
    ${FBXSDK_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/external/entt/
    ${CMAKE_SOURCE_DIR}/external/freetype/
    ${CMAKE_SOURCE_DIR}/external/glm/glm/
    ${GLEW_INCLUDE_DIRS}
    )

set_target_properties(${RAIN_ENGINE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ENGINE_EXE_PATH})

GroupSources(${CMAKE_CURRENT_SOURCE_DIR} engine)

add_custom_command(TARGET ${RAIN_ENGINE} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data/exe_root"
         ${ENGINE_EXE_PATH}/$<CONFIGURATION>)



add_library("${RAIN_ENGINE_EXPORT}" SHARED ${ENGINE_SOURCE_FILES})
target_link_libraries("${RAIN_ENGINE_EXPORT}" ${LIBS} optimized ${FBXSDK_LIBRARY} debug ${FBXSDK_LIBRARY_DEBUG})


target_include_directories("${RAIN_ENGINE_EXPORT}" PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries("${RAIN_ENGINE_EXPORT}"
    ${RAIN_CORE}
    ${RAIN_NETWORK}
    )
target_include_directories("${RAIN_ENGINE_EXPORT}" PUBLIC
    ${CORE_INCLUDE_PATH}
    ${NETWORK_INCLUDE_PATH}
    ${ENGINE_INCLUDE_PATH}
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/json
    ${FBXSDK_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/external/entt/
    ${CMAKE_SOURCE_DIR}/external/freetype/
    ${GLEW_INCLUDE_DIRS})

set_target_properties("${RAIN_ENGINE_EXPORT}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ENGINE_EXPORT_PATH})

add_custom_command(TARGET ${RAIN_ENGINE} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data/exe_root"
         ${ENGINE_EXPORT_PATH}/$<CONFIGURATION>)
