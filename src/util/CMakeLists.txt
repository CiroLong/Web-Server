add_library(
        webserver_util
        OBJECT
        util.cc
)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:webserver_util>
        PARENT_SCOPE)
