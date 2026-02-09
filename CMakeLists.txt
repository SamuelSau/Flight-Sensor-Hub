cmake_minimum_required(VERSION 3.20)

project(FlightSensorHub
    LANGUAGES C ASM
    VERSION 1.0.0
)

# Startup file
add_library(startup OBJECT
    startup/startup_stm32f446xx.c
)
target_link_libraries(startup PRIVATE cmsis)

# Subdirectories
add_subdirectory(cmsis)
add_subdirectory(freertos)
add_subdirectory(src)

# Link startup object into the main executable
target_sources(FlightSensorHub PRIVATE $<TARGET_OBJECTS:startup>)
