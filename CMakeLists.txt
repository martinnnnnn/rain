cmake_minimum_required(VERSION 3.10.1)
project(rain)
message(STATUS "project name : ${PROJECT_NAME}")

set(RAIN_ENGINE rain_engine)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

add_subdirectory(server)

#### VULKAN ####
find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    message(STATUS "Headers path : ${Vulkan_INCLUDE_DIRS}")
    message(STATUS "Headers path : ${Vulkan_LIBRARIES}")
else()
    message(STATUS "Couldn't find vulkan ;(")
endif()

#### OPENCL ####
find_package(OpenCL REQUIRED)

#### CXX FLAGS ####
message(STATUS "> setting cxx flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /MP /GR-")
# /W4 /Za


#### GLEW ####
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/glew-2.1.0")
set(CMAKE_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/glew-2.1.0/lib/Release/Win32/")
find_package(GLEW REQUIRED)

#### WINDOWS SDK PATHS ####
file(TO_CMAKE_PATH "$ENV{PROGRAMFILES\(X86\)}" _progFilePath)
message("${_progFilePath}/Windows\ Kits/10/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um")
set(WINDOWS_SDK_INCLUDES "${_progFilePath}/Windows Kits/10/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/")
set(WINDOWS_SDK_LIBS "${_progFilePath}/Windows Kits/10/Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/x86")

#### INCLUDES ####
message(STATUS "> including directories")
include_directories(src/
                    ${WINDOWS_SDK_INCLUDES}
                    external/glm/
                    external/entt/
                    external/json/
                    ${GLEW_INCLUDE_DIRS})

#### SOURCE DIRECTORY ####
add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

#### EXECUTABLE CREATION ####

message(STATUS "> globing source files")
file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.h" "src/*.hpp" "src/*.cpp" "src/*.inl")

message(STATUS "> adding files to exetutable")
add_executable(${RAIN_ENGINE} WIN32 ${PROJECT_SOURCE_FILES})
# 

#### GROUP CREATION ####
macro(GroupSources curdir)
    file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir}
    ${PROJECT_SOURCE_DIR}/${curdir}/*)

    foreach(child ${children})

        if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
            GroupSources(${curdir}/${child})
        else()
            string(REPLACE "/" "\\" groupname ${curdir})
            string(REPLACE "src" "src" groupname ${groupname})
            source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})

        endif()
    endforeach()
endmacro()

message(STATUS "> grouping sources")
GroupSources(src)

#### LIBRARIES TO TARGET LINKING ####
set(LIBS_PATH ${PROJECT_SOURCE_DIR}/libs)

message(STATUS "> Linking libraries to target")

find_library(d3dcompilerlib
    NAMES d3dcompiler
    HINTS "${WINDOWS_SDK_LIBS}"
)
find_library(dinput8lib
    NAMES dinput8
    HINTS "${WINDOWS_SDK_LIBS}"
)
find_library(d3d12lib
    NAMES d3d12
    HINTS "${WINDOWS_SDK_LIBS}"
)
find_library(dxgilib
    NAMES dxgi
    HINTS "${WINDOWS_SDK_LIBS}"
)
find_library(dxguidlib
    NAMES dxguid
    HINTS "${WINDOWS_SDK_LIBS}"
)
find_library(opengllib
    NAMES OpenGL32
    HINTS "${WINDOWS_SDK_LIBS}"
)
find_library(freeglutdlib
    NAMES freeglutd
    HINTS "${WINDOWS_SDK_LIBS}"
)
find_library(glulib
    NAMES GlU32
    HINTS "${WINDOWS_SDK_LIBS}"
)

target_link_libraries(${RAIN_ENGINE}
                        ${dinput8lib}
                        ${d3d12lib}
                        ${dxgilib}
                        ${dxguidlib}
                        ${d3dcompilerlib}
                        ${opengllib}
                        ${glulib}
                        Shlwapi.lib
                        ${GLEW_LIBRARIES}
                        OpenCL::OpenCL)




set_target_properties(${RAIN_ENGINE} PROPERTIES FOLDER "rain")

#### OUTPUT DIRECTORY ####
message(STATUS "> setting target property: RUNTIME_OUTPUT_DIRECTORY")
set_target_properties(${RAIN_ENGINE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/engine")

#### SETTING THE OUPUT CONSOLE TO 
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")

#project(rain)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${RAIN_ENGINE})

message(STATUS "> end of generation")