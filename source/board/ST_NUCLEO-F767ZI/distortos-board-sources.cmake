#
# file: distortos-board-sources.cmake
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_BOARD_ST_NUCLEO_F767ZI
		ON)

distortosSetFixedConfiguration(STRING
		DISTORTOS_BOARD
		"ST,NUCLEO-F767ZI")

target_include_directories(distortos PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/include)

target_sources(distortos PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/ST_NUCLEO-F767ZI-buttons.cpp
		${CMAKE_CURRENT_LIST_DIR}/ST_NUCLEO-F767ZI-dmas.cpp
		${CMAKE_CURRENT_LIST_DIR}/ST_NUCLEO-F767ZI-leds.cpp
		${CMAKE_CURRENT_LIST_DIR}/ST_NUCLEO-F767ZI-sdmmcs.cpp
		${CMAKE_CURRENT_LIST_DIR}/ST_NUCLEO-F767ZI-spis.cpp
		${CMAKE_CURRENT_LIST_DIR}/ST_NUCLEO-F767ZI-uarts.cpp
		${CMAKE_CURRENT_LIST_DIR}/ST_NUCLEO-F767ZI-vectorTable.cpp)

doxygen(INPUT ${CMAKE_CURRENT_LIST_DIR} INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/10-buttons.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/10-leds.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/50-STM32-SDMMCv1.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/50-STM32-SPIv2.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/50-STM32-USARTv2.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/60-STM32-GPIOv2.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/60-STM32-device-electronic-signature.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/70-STM32-DMAv2.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/80-STM32.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/90-ARMv6-M-ARMv7-M.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/distortos-board-sources.extension.cmake OPTIONAL)

set(DISTORTOS_BOARD_VERSION 13)
