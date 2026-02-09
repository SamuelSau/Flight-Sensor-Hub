add_executable(FlightSensorHub
    main.c
    bsp/clock.c
    bsp/gpio.c
    bsp/uart.c
    bsp/adc.c
    bsp/dma.c
    util/ring_buffer.c
    app/sensor_task.c
    app/processing_task.c
    app/telemetry_task.c
)

target_include_directories(FlightSensorHub PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp
    ${CMAKE_CURRENT_SOURCE_DIR}/app
    ${CMAKE_CURRENT_SOURCE_DIR}/util
)

target_link_libraries(FlightSensorHub PRIVATE
    cmsis
    freertos
    m
)

target_link_options(FlightSensorHub PRIVATE
    -T${CMAKE_SOURCE_DIR}/linker/STM32F446RE.ld
    -Wl,-Map=FlightSensorHub.map,--cref
)

# Generate .bin from .elf
add_custom_command(TARGET FlightSensorHub POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:FlightSensorHub> FlightSensorHub.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:FlightSensorHub>
    COMMENT "Generating FlightSensorHub.bin"
)
