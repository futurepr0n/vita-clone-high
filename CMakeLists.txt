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
set(SHORT_NAME vita_clone_high)
project(${SHORT_NAME})
include("${VITASDK}/share/vita.cmake" REQUIRED)
set(VITA_APP_NAME "vita-clone-high")
set(VITA_TITLEID  "CLONEHIGH")
set(VITA_VERSION  "01.51")

# Flags and includes
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
 audio/
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(${SHORT_NAME}
  main.c
  audio/vita_audio.c
  smb_fireball.wav
  abe.png
  image.png
  lockers.png
  peter.png
)

target_link_libraries(${SHORT_NAME}
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
vita_create_self(${SHORT_NAME}.self ${SHORT_NAME} eboot.bin)
vita_create_vpk(${SHORT_NAME}.vpk ${VITA_TITLEID} eboot.bin abe.png image.png lockers.png peter.png smb_fireball.wav
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}

)


