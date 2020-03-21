cmake_minimum_required(VERSION 3.10.1)


#### GENERAL CMAKE SETTINGS ####
project(rain)
message(STATUS "project name : ${PROJECT_NAME}")

set(ARCH_32 ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

message(STATUS "> setting cxx flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /MP /GR-")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${RAIN_ENGINE})


#### SETTING PROJECT MAIN PATHS AND NAMES ####
set(RAIN_CORE rain_core)
set(RAIN_SERVER rain_server)
set(RAIN_ENGINE rain_engine)
set(RAIN_ENGINE_EXPORT rain_engine_export)

set(CORE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/core")
set(SERVER_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/server")
set(ENGINE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/engine")

set(CORE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/bin/rain_core")
set(SERVER_EXE_PATH "${CMAKE_SOURCE_DIR}/bin/rain_server")
set(ENGINE_EXE_PATH "${CMAKE_SOURCE_DIR}/bin/rain_engine")
set(ENGINE_EXPORT_PATH "${CMAKE_SOURCE_DIR}/bin/rain_engine_export")


### GROUP CREATION ####
macro(GroupSources root_dir curdir)
    file(GLOB children RELATIVE ${root_dir}/${curdir}
            ${root_dir}/${curdir}/*)

    foreach(child ${children})
        if(IS_DIRECTORY ${root_dir}/${curdir}/${child})
            GroupSources(${root_dir} ${curdir}/${child})
        else()
            string(REPLACE "/" "\\" groupname ${curdir})
            source_group(${groupname} FILES ${curdir}/${child})

        endif()
    endforeach()
endmacro()


#### SETTING WINDOWS SDK PATHS ####
file(TO_CMAKE_PATH "$ENV{PROGRAMFILES\(X86\)}" _progFilePath)
set(WINDOWS_SDK_INCLUDES "${_progFilePath}/Windows Kits/10/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/")
set(WINDOWS_SDK_LIBS "${_progFilePath}/Windows Kits/10/Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/x86")

#### FBX SDK PACKAGE ####
set(FBXSDK_VERSION "2019.0")
set(FBXSDK_VS_VERSION "vs2015")
find_package(FBX REQUIRED)

#### GLEW PACKAGE ####
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/glew-2.1.0")
set(CMAKE_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/glew-2.1.0/lib/Release/Win32/")
find_package(GLEW REQUIRED)


add_subdirectory(src)


# #### VULKAN ####
# find_package(Vulkan REQUIRED)
# if (Vulkan_FOUND)
#     message(STATUS "Headers path : ${Vulkan_INCLUDE_DIRS}")
#     message(STATUS "Headers path : ${Vulkan_LIBRARIES}")
# else()
#     message(STATUS "Couldn't find vulkan ;(")
# endif()

# #### OPENCL ####
# find_package(OpenCL REQUIRED)

# #### FBX SDK ####
# set(FBXSDK_VERSION "2019.0")
# set(FBXSDK_VS_VERSION "vs2015")
# find_package(FBX REQUIRED)

# #### GLEW ####
# set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/glew-2.1.0")
# set(CMAKE_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/glew-2.1.0/lib/Release/Win32/")
# find_package(GLEW REQUIRED)

# #### WINDOWS SDK PATHS ####
# file(TO_CMAKE_PATH "$ENV{PROGRAMFILES\(X86\)}" _progFilePath)
# set(WINDOWS_SDK_INCLUDES "${_progFilePath}/Windows Kits/10/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/")
# set(WINDOWS_SDK_LIBS "${_progFilePath}/Windows Kits/10/Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/x86")