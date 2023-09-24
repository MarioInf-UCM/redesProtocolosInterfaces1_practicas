# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/mario/esp/esp-idf/components/bootloader/subproject"
  "/home/mario/Documentos/universidad/ArquitecturaDelNodoIOT/practicas/practica1/blink/build/bootloader"
  "/home/mario/Documentos/universidad/ArquitecturaDelNodoIOT/practicas/practica1/blink/build/bootloader-prefix"
  "/home/mario/Documentos/universidad/ArquitecturaDelNodoIOT/practicas/practica1/blink/build/bootloader-prefix/tmp"
  "/home/mario/Documentos/universidad/ArquitecturaDelNodoIOT/practicas/practica1/blink/build/bootloader-prefix/src/bootloader-stamp"
  "/home/mario/Documentos/universidad/ArquitecturaDelNodoIOT/practicas/practica1/blink/build/bootloader-prefix/src"
  "/home/mario/Documentos/universidad/ArquitecturaDelNodoIOT/practicas/practica1/blink/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/mario/Documentos/universidad/ArquitecturaDelNodoIOT/practicas/practica1/blink/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/mario/Documentos/universidad/ArquitecturaDelNodoIOT/practicas/practica1/blink/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()