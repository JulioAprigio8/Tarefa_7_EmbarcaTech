# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(pico_sdk_import.cmake)

project(Tarefa_73 C CXX ASM)

pico_sdk_init()

add_executable(Tarefa_73
    Tarefa_73.c
    inc/ssd1306_i2c.c
    inc/ssd1306_font.h
    inc/ssd1306.h
    inc/ssd1306_i2c.h
)

pico_set_program_name(Tarefa_73 "Tarefa_73")
pico_set_program_version(Tarefa_73 "0.1")

pico_enable_stdio_uart(Tarefa_73 0)
pico_enable_stdio_usb(Tarefa_73 1)

target_link_libraries(Tarefa_73
    pico_stdlib
    hardware_adc
    hardware_pwm
    hardware_i2c
    pico_cyw43_arch_lwip_threadsafe_background
)

target_include_directories(Tarefa_73 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

pico_add_extra_outputs(Tarefa_73)
