cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

target_sources(app PRIVATE src/main.c)

# Add GPIO tracing hooks when tracing is enabled
if(CONFIG_TRACING_GPIO AND CONFIG_TRACING_USER)
    target_sources(app PRIVATE src/trace_hooks.c)
endif()

