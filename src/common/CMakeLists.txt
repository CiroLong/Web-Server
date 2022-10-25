add_library(
        webserver_common
        OBJECT
        common.cc
)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:webserver_common>
        PARENT_SCOPE)
