cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME STM32H7B0RBT6-sample-fw)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

file(GLOB SRC_FILES CMAKE_CONFIGURE_DEPENDS
    App/ap/*.c
    App/common/src/*.c
    App/hw/*.c
    App/hw/src/*.c

    # App/hw/src/usb/*.c
    # App/hw/src/usb/usb_cdc/*.c
    # App/hw/src/usb/usb_hid/*.c
    # App/hw/src/usb/usb_cmp/*.c
    # App/lib/ST/STM32_USB_Device_Library/Core/Src/*.c
    
)

file(GLOB SRC_FILES_EXCLUDE CMAKE_CONFIGURE_DEPENDS
    # App/lib/ST/STM32_USB_Device_Library/Core/Inc/usbd_conf_template.h
    # App/lib/ST/STM32_USB_Device_Library/Core/Src/usbd_conf_template.c
    # App/lib/ST/STM32_USB_Device_Library/Core/Inc/usbd_desc_template.h
    # App/lib/ST/STM32_USB_Device_Library/Core/Src/usbd_desc_template.c
)

set(EXCLUDE_PATHS
    ${SRC_FILES_EXCLUDE}
)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${SRC_FILES}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    App/ap
    App/common
    App/common/include
    App/hw
    App/hw/include

    # App/hw/src/usb
    # App/hw/src/usb/usb_cdc
    # App/hw/src/usb/usb_hid
    # App/hw/src/usb/usb_cmp
    # App/lib/ST/STM32_USB_Device_Library/Core/Inc
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
