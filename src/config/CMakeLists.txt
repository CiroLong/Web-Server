add_library(
        webserver_config
        OBJECT
        config.cc
)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:webserver_config>
        PARENT_SCOPE)
