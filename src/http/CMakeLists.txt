add_library(
        webserver_http
        OBJECT
        http.cc
        handler.cc
        connection.cc
)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:webserver_http>
        PARENT_SCOPE)
