# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

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
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(EmbarcaTech_U7_Projeto C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

file(GLOB LIBSOURCES "src/*.c")

# Add executable. Default name is the project name, version 0.1

add_executable(EmbarcaTech_U7_Projeto EmbarcaTech_U7_Projeto.c ${LIBSOURCES})

pico_set_program_name(EmbarcaTech_U7_Projeto "EmbarcaTech_U7_Projeto")
pico_set_program_version(EmbarcaTech_U7_Projeto "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(EmbarcaTech_U7_Projeto 0)
pico_enable_stdio_usb(EmbarcaTech_U7_Projeto 1)

pico_generate_pio_header(EmbarcaTech_U7_Projeto ${CMAKE_CURRENT_LIST_DIR}/include/ws2812.pio)

# Add the standard library to the build
target_link_libraries(EmbarcaTech_U7_Projeto
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(EmbarcaTech_U7_Projeto PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(EmbarcaTech_U7_Projeto
            pico_time
            hardware_i2c
            hardware_adc
            hardware_pwm
            hardware_timer
            hardware_gpio
            hardware_pio
            hardware_clocks
            hardware_uart
        )

pico_add_extra_outputs(EmbarcaTech_U7_Projeto)

