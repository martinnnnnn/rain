cmake_minimum_required(VERSION 3.0)
project(rain)
message(STATUS "project name : ${PROJECT_NAME}")
#### MODULES ####
message(STATUS "> setting modules path")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

#### GLFW ####
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(external/glfw)

#### ASSIMP BUILDING ####
message(STATUS "> building assimp")
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(external/assimp)

#### BULLET BUILDING ####
message(STATUS "> building bullet")
option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(external/bullet)

#### CXX FLAGS ####
message(STATUS "> setting cxx flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
# /W4 /Za

#### INCLUDES ####
message(STATUS "> including directories")
include_directories(src/
                    external/assimp/include/
                    external/bullet/src/
                    external/glad/include/
                    external/glm/
                    external/glfw/include/
                    external/stb/
                    external/json/
					external/imgui/)

#### SOURCE DIRECTORY ####
add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

#### EXECUTABLE CREATION ####
message(STATUS "> globing source files")
file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.h" "src/*.hpp" "src/*.cpp")
file(GLOB GLAD_SOURCES external/glad/src/glad.c)
file(GLOB IMGUI_SOURCES external/imgui/*.cpp)

message(STATUS "> adding files to exetutable")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES} ${GLAD_SOURCES} ${IMGUI_SOURCES})

#### GROUP CREATION ####
macro(GroupSources curdir)
    message(STATUS ">> globing children")
    file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir}
    ${PROJECT_SOURCE_DIR}/${curdir}/*)

    foreach(child ${children})

        if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
            GroupSources(${curdir}/${child})
        else()
            string(REPLACE "/" "\\" groupname ${curdir})
            string(REPLACE "src" "src" groupname ${groupname})
            message(STATUS ">>> creating source groupe : ${groupname}")
            source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})

        endif()
    endforeach()
endmacro()

message(STATUS "> grouping sources")
GroupSources(src)
source_group("libraries" FILES ${LIBS_SOURCES})

#### LIBRARIES TO TARGET LINKING ####
message(STATUS "> Linking libraries to target")
target_link_libraries(${PROJECT_NAME} assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)

#### RUNTIME OUTPUT DIRECTORY ####
message(STATUS "> setting target property: RUNTIME_OUTPUT_DIRECTORY")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

message(STATUS "> end of generation")
