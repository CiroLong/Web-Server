add_library(
        webserver_http
        OBJECT
        http.cc
)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:webserver_http>
        PARENT_SCOPE)
