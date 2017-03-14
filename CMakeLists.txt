cmake_minimum_required(VERSION 2.8)

# VitaSDK defines
if( NOT DEFINED CMAKE_TOOLCHAIN_FILE )
  if( DEFINED ENV{VITASDK} )
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

# Project start
project(vita_clone_high)
include("${VITASDK}/share/vita.cmake" REQUIRED)
set(VITA_APP_NAME "vita-clone-high")
set(VITA_TITLEID  "CLONEHIGH")
set(VITA_VERSION  "01.51")

# Flags and includes
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(

)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

# Builds
FUNCTION(ADD_RESOURCES out_var)
  SET(result)
  FOREACH(in_f ${ARGN})
    SET(out_f "${CMAKE_CURRENT_BINARY_DIR}/${in_f}.o")
    GET_FILENAME_COMPONENT(out_dir ${out_f} DIRECTORY)
    ADD_CUSTOM_COMMAND(OUTPUT ${out_f}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${out_dir}
      COMMAND ${CMAKE_LINKER} -r -b binary -o ${out_f} ${in_f}
      DEPENDS ${in_f}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Building resource ${out_f}"
      VERBATIM
      )
    LIST(APPEND result ${out_f})
  ENDFOREACH()
  SET(${out_var} "${result}" PARENT_SCOPE)
ENDFUNCTION()

file(GLOB res_files RELATIVE ${CMAKE_SOURCE_DIR} resources/*.png)
add_resources(vita_clone_high_res ${res_files})



add_executable(vita_clone_high
  ${vita_clone_high_res}
  src/main.c
  src/audio/vita_audio.c
)

target_link_libraries(vita_clone_high
  vita2d
  png
  jpeg
  z
  m
  c
  freetype
  stdc++
  SceAudio_stub
  SceAudiodec_stub
  SceCommonDialog_stub
  SceCtrl_stub
  SceDisplay_stub
  SceGxm_stub
  SceMtpIfDriver_stub
  SceMusicExport_stub
  SceSysmodule_stub
  ScePgf_stub
  ScePower_stub
  SceTouch_stub
  ScePgf_stub
  ScePvf_stub  
)

# Create Vita artifacts
vita_create_self(eboot.bin vita_clone_high SAFE)
vita_create_vpk(vita_clone_high.vpk ${VITA_TITLEID} eboot.bin 
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE ${CMAKE_SOURCE_DIR}/resources/smb_fireball.wav resources/smb_fireball.wav
)


