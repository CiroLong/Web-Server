#add_subdirectory(memory)
#add_subdirectory(type)
add_subdirectory(util)
add_subdirectory(net)
add_subdirectory(http)
add_subdirectory(common)


# add_libray must have files
add_library(webserver STATIC ${ALL_OBJECT_FILES})

set(WEBSERVER_LIBS
        webserver_util
        webserver_net
        webserver_http
        webserver_common
        )

# target_link_libraries
target_link_libraries(webserver ${WEBSERVER_LIBS})

target_include_directories(webserver PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

add_executable(server server.cpp)

target_link_libraries(server webserver)