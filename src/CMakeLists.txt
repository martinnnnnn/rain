
set(RAIN_CORE rain_core)
set(RAIN_SERVER rain_server)
set(RAIN_ENGINE rain_engine)

set(CORE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/core/src")
set(SERVER_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/server/src")
set(ENGINE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/engine/src")

link_directories(${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(core)
add_subdirectory(engine)
add_subdirectory(server)

#### SERVER DEPENDENCIES ####
add_dependencies(${RAIN_SERVER} ${RAIN_CORE})
#### ENGINE DEPENDENCIES ####
add_dependencies(${RAIN_ENGINE} ${RAIN_CORE})