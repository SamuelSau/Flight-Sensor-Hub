add_library(cmsis INTERFACE)

target_include_directories(cmsis INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Device/ST/STM32F4xx/Include
)

target_compile_definitions(cmsis INTERFACE
    STM32F446xx
)
