
set(RAIN_SERVER rain_server)
message(STATUS "> Beginning server project generation")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

#### CXX FLAGS ####
message(STATUS "> setting cxx flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /MP /GR-")
# /W4 /Za

#### INCLUDES ####
message(STATUS "> including directories")
include_directories(src/
                    external/glm/
                    external/json/)

#### SOURCE DIRECTORY ####
add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

#### EXECUTABLE CREATION ####
message(STATUS "> globing source files")
file(GLOB_RECURSE SERVER_SOURCE_FILES "*.h" "*.hpp" "*.cpp" "*.inl")

message(STATUS "> adding files to exetutable")
add_executable(${RAIN_SERVER} ${SERVER_SOURCE_FILES})

#### GROUP CREATION ####
macro(GroupSources curdir)
    file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir}
    ${PROJECT_SOURCE_DIR}/${curdir}/*)

    foreach(child ${children})

        if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
            GroupSources(${curdir}/${child})
        else()
            string(REPLACE "/" "\\" groupname ${curdir})
            string(REPLACE "server" "server" groupname ${groupname})
            source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})

        endif()
    endforeach()
endmacro()

message(STATUS "> grouping sources")
GroupSources(server)

#### LIBRARIES TO TARGET LINKING ####
set_target_properties(${RAIN_SERVER} PROPERTIES FOLDER "rain")

#### OUTPUT DIRECTORY ####
message(STATUS "> setting target property: RUNTIME_OUTPUT_DIRECTORY")
set_target_properties(${RAIN_SERVER} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/server/")

#project(rain)

message(STATUS "> end of generation")