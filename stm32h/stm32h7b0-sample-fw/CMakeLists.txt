cmake_minimum_required(VERSION 3.28)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})  
message("Build type: "              ${CMAKE_BUILD_TYPE})

include("cmake/gcc-arm-none-eabi.cmake")
# project(stm32g431-usb2can-fw VERSION 0.1.0)
# enable_language(C CXX ASM)
# set(CMAKE_PROJECT_NAME stm32h7b0-sample-fw)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")


# enable_language(ASM C CXX)
set(PRJ_NAME stm32h7b0-sample-fw)
project(${PRJ_NAME}
  LANGUAGES ASM C CXX
)

# project(${CMAKE_PROJECT_NAME})


set(EXECUTABLE ${PRJ_NAME})



# 지정한 폴더에 있는 파일만 포함한다.
#
file(GLOB SRC_FILES CONFIGURE_DEPENDS
  src/*.cpp
  src/*.c
  src/bsp/*.c
  src/bsp/device/*.c

  # FatFs
#   src/lib/FatFs/src/*.c
#   src/lib/FatFs/src/option/syscall.c
#   src/lib/FatFs/src/option/unicode.c  

  # LittleFS
#   src/lib/littlefs/*.c

  # USB
#   src/lib/ST/STM32_USB_Device_Library/Core/Src/*.c
)

# 하위폴더에 있는 파일까지 포함한다.
#
file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS
  src/ap/*.cpp
  src/ap/*.c
  src/bsp/*.s
  src/common/*.c
  src/hw/*.c
  src/lib/ST/STM32H7xx_HAL_Driver/Src/*.c
)

# 특정 폴더를 빌드에서 제외한다.
#
set(EXCLUDE_PATHS 
)

foreach(TMP_PATH ${SRC_FILES_RECURSE}) 
  foreach(EXCLUDE_PATH ${EXCLUDE_PATHS}) 
    string(FIND ${TMP_PATH} ${EXCLUDE_PATH} RESULT) 
    if(NOT ${RESULT} EQUAL -1) 
      list(REMOVE_ITEM SRC_FILES_RECURSE ${TMP_PATH}) 
    endif() 
  endforeach(EXCLUDE_PATH) 
endforeach(TMP_PATH)


add_executable(${EXECUTABLE} 
  ${SRC_FILES}
  ${SRC_FILES_RECURSE}
)



target_include_directories(${EXECUTABLE} PRIVATE 
  src  
  src/ap
  src/bsp
  src/bsp/device

  src/common 
  src/common/core
  src/common/hw/include

  src/hw
  src/hw/driver

  src/lib
  src/lib/ST/CMSIS/Include
  src/lib/ST/CMSIS/Device/ST/STM32H7xx/Include
  # src/lib/ST/CMSIS/DSP/Include
  src/lib/ST/STM32H7xx_HAL_Driver/Inc 

  # FatFs
#   src/hw/driver/fatfs
#   src/lib/FatFs/src  

  # USB
#   src/hw/driver/usb
#   src/hw/driver/usb/usb_cdc
#   src/hw/driver/usb/usb_msc
  # src/lib/ST/STM32_USB_Device_Library/Core/Inc
)

target_compile_definitions(${EXECUTABLE} PRIVATE
  -DSTM32H7B0xx
  # -DDEBUG
  -DUSE_HAL_DRIVER  
  # -DUSE_FULL_ASSERT
  # -DLFS_THREADSAFE
  -DUSE_PWR_LDO_SUPPLY
  )

target_compile_options(${EXECUTABLE} PRIVATE
  -mcpu=cortex-m7
  -mthumb
  -mfpu=fpv5-d16 
  -mfloat-abi=hard

  -fdata-sections
  -ffunction-sections
  -fstack-usage
  -fcyclomatic-complexity

  -Wall
  -g3
  -O0
  # -Os
  )

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/ldscript/stm32h7b0rbtx_flash.ld)

target_link_options(${EXECUTABLE} PRIVATE
  # -T../src/bsp/ldscript/stm32h7b0rbtx_flash.ld
  -T${LINKER_SCRIPT}

  -mcpu=cortex-m7
  -mthumb
  -mfpu=fpv5-d16 
  -mfloat-abi=hard
  # -specs=nosys.specs
  # --specs=nano.specs
  -lc
  -lm
  -lstdc++
  -lsupc++
  -fno-rtti
  
  -Wl,-Map=${PRJ_NAME}.map,--cref
  -Wl,--gc-sections
  # -Wl,-Map=${PRJ_NAME}.map
  # -Wl,--gc-sections -static
  # -Wl,--start-group
  # -lc -lm -Wl,--end-group
  -Xlinker -print-memory-usage -Xlinker
  )

# set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP")
# set (CMAKE_CXX_FLAGS "-fno-rtti")


add_custom_command(TARGET ${EXECUTABLE} 
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${EXECUTABLE}.elf ${PROJECT_NAME}.bin
  COMMENT "Invoking: Make Binary"
  )  