cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(ttx-rp2040 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# use custom board
set(PICO_BOARD_HEADER_DIRS ${CMAKE_SOURCE_DIR}) 
set(PICO_BOARD board)

# Initialize the SDK
pico_sdk_init()

add_executable(${PROJECT_NAME} 
main.c
usb_descriptors.c
debug.c
ttx4_usb.c
ws2812.c
)
target_include_directories(
    ${PROJECT_NAME} 
    PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROJECT_NAME} 
PRIVATE 
pico_stdlib
tinyusb_device
tinyusb_board
hardware_pio
)

pico_add_extra_outputs(${PROJECT_NAME})

pico_enable_stdio_uart(${PROJECT_NAME} 1)

# ws2812
pico_generate_pio_header(${PROJECT_NAME}  ${CMAKE_SOURCE_DIR}/ws2812.pio)

# add flash target (make flash)
add_custom_target(flash DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(TARGET flash
    USES_TERMINAL
    COMMAND openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program ${PROJECT_NAME}.elf verify reset exit")
add_custom_target(pflash DEPENDS ${PROJECT_NAME}.uf2)
    add_custom_command(TARGET pflash
        USES_TERMINAL
        COMMAND picotool load ${PROJECT_NAME}.uf2 -f && picotool reboot)