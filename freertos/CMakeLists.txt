add_library(freertos STATIC
    kernel/tasks.c
    kernel/queue.c
    kernel/list.c
    kernel/timers.c
    kernel/portable/GCC/ARM_CM4F/port.c
    kernel/portable/MemMang/heap_4.c
)

target_include_directories(freertos PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/include
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/portable/GCC/ARM_CM4F
)

target_link_libraries(freertos PUBLIC cmsis)
