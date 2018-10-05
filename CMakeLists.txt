cmake_minimum_required(VERSION 3.10.1)
project(rain)
message(STATUS "project name : ${PROJECT_NAME}")

set(RAIN_EXE rain_exe)


#### CXX FLAGS ####
message(STATUS "> setting cxx flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /MP /GR-")
# /W4 /Za

#### ASSIMP BUILDING ####
# message(STATUS "> building assimp")
# option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
# option(ASSIMP_BUILD_SAMPLES OFF)
# option(ASSIMP_BUILD_TESTS OFF)
# add_subdirectory(external/assimp)


set(WINKIT "C:/Program Files (x86)/Windows Kits/10") 
#file(TO_CMAKE_PATH "$ENV{ProgramFiles}" _progFilePath)

#### GLEW ####
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/glew-2.1.0")
set(CMAKE_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/glew-2.1.0/lib/Release/Win32/")
find_package(GLEW REQUIRED)

#### SETTING RIGHT DX12 PATHS ####
set(DX12_INCLUDEPATH
    ${WINKIT}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um
    ${WINKIT}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/shared)


set(DX12_LIBPATH ${PROJECT_SOURCE_DIR}/libs)

#### INCLUDES ####
message(STATUS "> including directories")
include_directories(src/
                    # external/assimp/include/
                    external/glm/
                    ${DX12_INCLUDEPATH}
                    ${GLEW_INCLUDE_DIRS})

#### SOURCE DIRECTORY ####
add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
#### EXECUTABLE CREATION ####

message(STATUS "> globing source files")
file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.h" "src/*.hpp" "src/*.cpp" "src/*.inl")
file(GLOB_RECURSE HLSL_VS_SHADER_SOURCES "resources/engine/dx12/shaders/vertex/*.hlsl")
file(GLOB_RECURSE HLSL_PS_SHADER_SOURCES "resources/engine/dx12/shaders/fragment/*.hlsl")
set_source_files_properties( ${HLSL_VS_SHADER_SOURCES} PROPERTIES VS_SHADER_TYPE Vertex VS_SHADER_MODEL 5.0 VS_SHADER_ENTRYPOINT main )
set_source_files_properties( ${HLSL_PS_SHADER_SOURCES} PROPERTIES VS_SHADER_TYPE Pixel VS_SHADER_MODEL 5.0 VS_SHADER_ENTRYPOINT main )


source_group("shader\\hlsl" FILES ${HLSL_VS_SHADER_SOURCES})
source_group("shader\\hlsl" FILES ${HLSL_PS_SHADER_SOURCES})

message(STATUS "> adding files to exetutable")
add_executable(${RAIN_EXE} WIN32 ${PROJECT_SOURCE_FILES} ${HLSL_VS_SHADER_SOURCES} ${HLSL_PS_SHADER_SOURCES})
# WIN32

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
message(STATUS "> Linking libraries to target")
find_library(d3d12lib
    NAMES d3d12
    HINTS "${DX12_LIBPATH}"
)
find_library(dxgilib
    NAMES dxgi
    HINTS "${DX12_LIBPATH}"
)
find_library(dxguidlib
    NAMES dxguid
    HINTS "${DX12_LIBPATH}"
)
find_library(d3dcompilerlib
    NAMES d3dcompiler
    HINTS "${DX12_LIBPATH}"
)
find_library(opengllib
    NAMES OpenGL32
    HINTS "${DX12_LIBPATH}"
)
find_library(freeglutdlib
    NAMES freeglutd
    HINTS "${DX12_LIBPATH}"
)
find_library(glulib
    NAMES GlU32
    HINTS "${DX12_LIBPATH}"
)
    

target_link_libraries(${RAIN_EXE} 
                        # assimp
                        ${d3d12lib} ${dxgilib} ${dxguidlib} ${d3dcompilerlib} ${opengllib} ${glulib} Shlwapi.lib ${GLEW_LIBRARIES})




#set_target_properties(assimp PROPERTIES FOLDER "assimp")      

set_target_properties(${RAIN_EXE} PROPERTIES FOLDER "rain")

# set_target_properties(zlib PROPERTIES FOLDER "assimp")
# set_target_properties(IrrXML PROPERTIES FOLDER "assimp")
# set_target_properties(zlibstatic PROPERTIES FOLDER "assimp")
# set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER "assimp")

#### OUTPUT DIRECTORY ####
message(STATUS "> setting target property: RUNTIME_OUTPUT_DIRECTORY")
set_target_properties(${RAIN_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/exe")

#project(rain)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${RAIN_EXE})

message(STATUS "> end of generation")