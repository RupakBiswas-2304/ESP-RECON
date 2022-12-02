# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/rupak/esp/esp-idf/components/bootloader/subproject"
  "/home/rupak/Desktop/esp32/mprojects/ESP-RECON/build/bootloader"
  "/home/rupak/Desktop/esp32/mprojects/ESP-RECON/build/bootloader-prefix"
  "/home/rupak/Desktop/esp32/mprojects/ESP-RECON/build/bootloader-prefix/tmp"
  "/home/rupak/Desktop/esp32/mprojects/ESP-RECON/build/bootloader-prefix/src/bootloader-stamp"
  "/home/rupak/Desktop/esp32/mprojects/ESP-RECON/build/bootloader-prefix/src"
  "/home/rupak/Desktop/esp32/mprojects/ESP-RECON/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/rupak/Desktop/esp32/mprojects/ESP-RECON/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/rupak/Desktop/esp32/mprojects/ESP-RECON/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
