# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(button-buzzer C CXX ASM)

pico_sdk_init()

add_executable(button-buzzer
    button-buzzer.c
)

pico_set_program_name(button-buzzer "button-buzzer")
pico_set_program_version(button-buzzer "0.1")

pico_enable_stdio_uart(button-buzzer 0)
pico_enable_stdio_usb(button-buzzer 1)

target_link_libraries(button-buzzer
    pico_stdlib
    hardware_pwm
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip
    pico_lwip_mqtt
)

target_include_directories(button-buzzer PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(button-buzzer)