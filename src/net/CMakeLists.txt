add_library(
        webserver_net
        OBJECT
        connection.cc
)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:webserver_net>
        PARENT_SCOPE)
