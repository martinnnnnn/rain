
set(RAIN_CORE rain_core)
set(RAIN_THREAD rain_thread)
set(RAIN_NETWORK rain_network)
set(RAIN_SERVER rain_server)
set(RAIN_ENGINE rain_engine)

set(CORE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/core/src")
set(THREAD_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/thread/src")
set(NETWORK_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/network/src")
set(SERVER_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/server/src")
set(ENGINE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/src/engine/src")

link_directories(${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(core)
add_subdirectory(thread)
add_subdirectory(network)
add_subdirectory(engine)
add_subdirectory(server)

#### THREAD DEPENDENCIES ####
add_dependencies(${RAIN_THREAD} ${RAIN_CORE})
#### NETWORK DEPENDENCIES ####
add_dependencies(${RAIN_NETWORK} ${RAIN_CORE})
add_dependencies(${RAIN_NETWORK} ${RAIN_THREAD})
#### SERVER DEPENDENCIES ####
add_dependencies(${RAIN_SERVER} ${RAIN_CORE})
add_dependencies(${RAIN_SERVER} ${RAIN_THREAD})
add_dependencies(${RAIN_SERVER} ${RAIN_NETWORK})
#### ENGINE DEPENDENCIES ####
add_dependencies(${RAIN_ENGINE} ${RAIN_CORE})
add_dependencies(${RAIN_ENGINE} ${RAIN_THREAD})
add_dependencies(${RAIN_ENGINE} ${RAIN_NETWORK})