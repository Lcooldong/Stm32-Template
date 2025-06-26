cmake_minimum_required(VERSION 3.22)

#
# User is free to modify the file as much as necessary
#

#list(APPEND CMAKE_MODULE_PATH "C:/Users/user1/Downloads/CmakeTestProject/cmake")
list(APPEND CMAKE_MODULE_PATH "{{sr:cmake_path}}")
message("Build CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})
include("cmake/gcc-arm-none-eabi.cmake")
message("Build CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})

# Core project settings
project(STM32H750VBT6-sample-fw)
enable_language(C CXX ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Setup compiler settings
set(CMAKE_C_STANDARD            11)
set(CMAKE_C_STANDARD_REQUIRED   ON)
set(CMAKE_C_EXTENSIONS          ON)
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        ON)

# Core MCU flags, CPU type, instruction set and FPU setup
set(cpu_PARAMS 
    -mthumb
    -mcpu=cortex-m7
    -mfpu=fpv5-d16
    -mfloat-abi=hard
)

set(linker_script_SRC ${linker_script_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/ldscript/stm32h750vbtx_FLASH.ld
)

file(GLOB SRC_FILES CONFIGURE_DEPENDS
    src/*.cpp
    src/*.c
    src/ap/*.cpp
    src/ap/*.c
    src/bsp/*.c
    src/bsp/device/*.c
    
)

file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS
    src/ap/*.cpp
    src/ap/*.c
    src/bsp/*.s
    src/common/*.c
    src/hw/*.c
    src/lib/STM32H7xx_HAL_Driver/Src/*.c
    src/lib/STM32_USB_Device_Library/Core/Src/*.c

)

file(GLOB SRC_FILES_EXCLUDE CONFIGURE_DEPENDS
    src/lib/STM32H7xx_HAL_Driver/Src/stm32H7xx_hal_timebase_tim_template.c
    src/lib/STM32H7xx_HAL_Driver/Src/stm32H7xx_hal_msp_template.c
    src/lib/STM32H7xx_HAL_Driver/Src/stm32H7xx_hal_timebase_rtc_wakeup_template.c
    src/lib/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_timebase_rtc_alarm_template.c
  
)

set(EXCLUDE_PATHS 
    ${SRC_FILES_EXCLUDE}
)

foreach(TMP_PATH ${SRC_FILES_RECURSE}) 
  foreach(EXCLUDE_PATH ${EXCLUDE_PATHS}) 
    string(FIND ${TMP_PATH} ${EXCLUDE_PATH} RESULT) 
    if(NOT ${RESULT} EQUAL -1) 
      list(REMOVE_ITEM SRC_FILES_RECURSE ${TMP_PATH}) 
    endif() 
  endforeach(EXCLUDE_PATH) 
endforeach(TMP_PATH)

set(include_path_DIRS

    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ap
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/hw/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/device
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hw
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hw/driver
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hw/driver/usb
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hw/driver/usb/usb_cdc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/CMSIS/Device/ST/STM32H7xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/STM32_USB_Device_Library/Core/Inc
    
)

set(symbols_SYMB
    -DSTM32H750xx
)

add_executable(${CMAKE_PROJECT_NAME} 
  ${SRC_FILES}
  ${SRC_FILES_RECURSE}
)



set(compiler_OPTS)

# Linker options
set(linker_OPTS)



# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${include_path_DIRS})

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE ${symbols_SYMB})

# Add linked libraries
# target_link_libraries(${CMAKE_PROJECT_NAME} ${link_LIBS})

# Compiler options
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    ${cpu_PARAMS}
    ${compiler_OPTS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:C>: >
    $<$<COMPILE_LANGUAGE:CXX>:

    # -Wno-volatile
    # -Wold-style-cast
    # -Wuseless-cast
    # -Wsuggest-override
    >
    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp -MMD -MP>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>
)

# Linker options
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -T${linker_script_SRC}
    ${cpu_PARAMS}
    ${linker_OPTS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -u _printf_float # STDIO float formatting support (remove if not used)
    --specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,-z,max-page-size=8 # Allow good software remapping across address space (with proper GCC section making)
    -Wl,--print-memory-usage
)

# Execute post-build to print size, generate hex and bin
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
)
