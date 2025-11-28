cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(bitdoglab_alerta C CXX ASM)

pico_sdk_init()

add_executable(bitdoglab_alerta
    main.c
)

pico_set_program_name(bitdoglab_alerta "bitdoglab_alerta")
pico_set_program_version(bitdoglab_alerta "1.0")

# USB + UART ATIVADOS
pico_enable_stdio_usb(bitdoglab_alerta 1)
pico_enable_stdio_uart(bitdoglab_alerta 1)

target_link_libraries(bitdoglab_alerta
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
)

target_include_directories(bitdoglab_alerta PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(bitdoglab_alerta)
