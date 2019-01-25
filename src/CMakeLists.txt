


link_directories(
    ${CMAKE_SOURCE_DIR}/bin
    ${WINDOWS_SDK_LIBS}
    ${CMAKE_SOURCE_DIR}/libs)

message(STATUS "Rain Core library generation")
add_subdirectory(core)
message(STATUS "Rain Math library generation")
add_subdirectory(math)
message(STATUS "Rain Server executable generation")
add_subdirectory(server)
message(STATUS "Rain Engine executable generation")
add_subdirectory(engine)

#### SERVER DEPENDENCIES ####
add_dependencies(${RAIN_SERVER} ${RAIN_CORE})
#### ENGINE DEPENDENCIES ####
add_dependencies(${RAIN_ENGINE} ${RAIN_CORE})

