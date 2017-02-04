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
project(vita-clone-high)
include("${VITASDK}/share/vita.cmake" REQUIRED)
set(VITA_APP_NAME "CloneHigh")
set(VITA_TITLEID  "CLONEHIGH")
set(VITA_VERSION  "01.51")

# Flags and includes
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -Wno-unused-variable -Wno-unused-but-set-variable -fno-lto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")
set(VITA_MAKE_FSELF_FLAGS "${VITA_MAKE_FSELF_FLAGS} -a 0x2800000000000001")

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

file(GLOB res_files RELATIVE ${CMAKE_SOURCE_DIR} *.png *.bin *.sprx)
add_resources(vitashell_res ${res_files})

add_executable(CloneHigh
  ${vitashell_res}
  main.c
  audio/vita_audio.c
)

target_link_libraries(
  vita2d
  vorbisfile
  vorbis
  ogg
  png
  jpeg
  z
  m
  c
  onigmo
  unrar
  stdc++
  taihen_stub
  HENkaku_stub
  SceAppMgr_stub
  SceAppUtil_stub
  SceAudio_stub
  SceAudiodec_stub
  SceCommonDialog_stub
  SceCtrl_stub
  SceDisplay_stub
  SceGxm_stub
  SceIme_stub
  SceHttp_stub
  SceMtpIfDriver_stub
  SceMusicExport_stub
  SceNet_stub
  SceNetCtl_stub
  SceShellSvc_stub
  SceSsl_stub
  SceSysmodule_stub
  ScePgf_stub
  ScePhotoExport_stub
  ScePower_stub
  ScePromoterUtil_stub
  SceTouch_stub
  SceUdcd_stub
  SceUsbstorVStorDriver_stub
  SceVshBridge_stub
)

# Create Vita artifacts
vita_create_self(eboot.bin CloneHigh UNSAFE)
vita_create_vpk(CloneHigh.vpk ${VITA_TITLEID} eboot.bin
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE smb_fireball.wav
)

add_custom_target(send
  COMMAND curl -T eboot.bin ftp://$(PSVITAIP):1337/ux0:/app/${VITA_TITLEID}/
  DEPENDS eboot.bin
)

add_custom_target(copy
  COMMAND cp eboot.bin F:/app/${VITA_TITLEID}/eboot.bin
  DEPENDS eboot.bin
)
