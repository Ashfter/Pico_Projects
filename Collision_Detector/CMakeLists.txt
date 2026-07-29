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
set(sdkVersion 2.3.0)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Collision_Detector C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(../pico-ssd1306 pico-ssd1306_build)

# Add executable. Default name is the project name, version 0.1

add_executable(Collision_Detector Collision_Detector.cpp ssd1306.c )

pico_set_program_name(Collision_Detector "Collision_Detector")
pico_set_program_version(Collision_Detector "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Collision_Detector 0)
pico_enable_stdio_usb(Collision_Detector 1)

# Add the standard library to the build
target_link_libraries(Collision_Detector
        pico_stdlib
        hardware_i2c
        ssd1306)

# Add the standard include files to the build
target_include_directories(Collision_Detector PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(Collision_Detector)

