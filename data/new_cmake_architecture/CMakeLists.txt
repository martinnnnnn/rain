cmake_minimum_required(VERSION 3.10.1)

project(rain)
message(STATUS "project name : ${PROJECT_NAME}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ARCH_32 ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

#### CXX FLAGS ####
message(STATUS "> setting cxx flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /MP /GR-")

#### GROUP CREATION ####
macro(GroupSources curdir)
    file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir}
    ${PROJECT_SOURCE_DIR}/${curdir}/*)

    foreach(child ${children})

        if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
            GroupSources(${curdir}/${child})
        else()
            string(REPLACE "/" "\\" groupname ${curdir})
            string(REPLACE "src" "" groupname ${groupname})
            source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})

        endif()
    endforeach()
endmacro()

add_subdirectory(src)