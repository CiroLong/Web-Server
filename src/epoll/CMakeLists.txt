add_library(
        webserver_epoll
        OBJECT
        thread_pool.cc
)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:webserver_epoll>
        PARENT_SCOPE)
