/**
 * \file
 * \brief CMSIS proxy header for STM32F1
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_
#define SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_

#include "distortos/distortosConfiguration.h"

#if defined(DISTORTOS_CHIP_STM32F100C4) || defined(DISTORTOS_CHIP_STM32F100R4) || \
		defined(DISTORTOS_CHIP_STM32F100C6) || defined(DISTORTOS_CHIP_STM32F100R6) || \
		defined(DISTORTOS_CHIP_STM32F100C8) || defined(DISTORTOS_CHIP_STM32F100R8) || \
		defined(DISTORTOS_CHIP_STM32F100V8) || defined(DISTORTOS_CHIP_STM32F100CB) || \
		defined(DISTORTOS_CHIP_STM32F100RB) || defined(DISTORTOS_CHIP_STM32F100VB)
	#define STM32F100xB
#elif defined(DISTORTOS_CHIP_STM32F100RC) || defined(DISTORTOS_CHIP_STM32F100VC) || \
		defined(DISTORTOS_CHIP_STM32F100ZC) || defined(DISTORTOS_CHIP_STM32F100RD) || \
		defined(DISTORTOS_CHIP_STM32F100VD) || defined(DISTORTOS_CHIP_STM32F100ZD) || \
		defined(DISTORTOS_CHIP_STM32F100RE) || defined(DISTORTOS_CHIP_STM32F100VE) || \
		defined(DISTORTOS_CHIP_STM32F100ZE)
	#define STM32F100xE
#elif defined(DISTORTOS_CHIP_STM32F101C4) || defined(DISTORTOS_CHIP_STM32F101R4) || \
		defined(DISTORTOS_CHIP_STM32F101T4) || defined(DISTORTOS_CHIP_STM32F101C6) || \
		defined(DISTORTOS_CHIP_STM32F101R6) || defined(DISTORTOS_CHIP_STM32F101T6)
	#define STM32F101x6
#elif defined(DISTORTOS_CHIP_STM32F101C8) || defined(DISTORTOS_CHIP_STM32F101R8) || \
		defined(DISTORTOS_CHIP_STM32F101T8) || defined(DISTORTOS_CHIP_STM32F101V8) || \
		defined(DISTORTOS_CHIP_STM32F101CB) || defined(DISTORTOS_CHIP_STM32F101RB) || \
		defined(DISTORTOS_CHIP_STM32F101TB) || defined(DISTORTOS_CHIP_STM32F101VB)
	#define STM32F101xB
#elif defined(DISTORTOS_CHIP_STM32F101RC) || defined(DISTORTOS_CHIP_STM32F101VC) || \
		defined(DISTORTOS_CHIP_STM32F101ZC) || defined(DISTORTOS_CHIP_STM32F101RD) || \
		defined(DISTORTOS_CHIP_STM32F101VD) || defined(DISTORTOS_CHIP_STM32F101ZD) || \
		defined(DISTORTOS_CHIP_STM32F101RE) || defined(DISTORTOS_CHIP_STM32F101VE) || \
		defined(DISTORTOS_CHIP_STM32F101ZE)
	#define STM32F101xE
#elif defined(DISTORTOS_CHIP_STM32F101RF) || defined(DISTORTOS_CHIP_STM32F101VF) || \
		defined(DISTORTOS_CHIP_STM32F101ZF) || defined(DISTORTOS_CHIP_STM32F101RG) || \
		defined(DISTORTOS_CHIP_STM32F101VG) || defined(DISTORTOS_CHIP_STM32F101ZG)
	#define STM32F101xG
#elif defined(DISTORTOS_CHIP_STM32F102C4) || defined(DISTORTOS_CHIP_STM32F102R4) || \
		defined(DISTORTOS_CHIP_STM32F102C6) || defined(DISTORTOS_CHIP_STM32F102R6)
	#define STM32F102x6
#elif defined(DISTORTOS_CHIP_STM32F102C8) || defined(DISTORTOS_CHIP_STM32F102R8) || \
		defined(DISTORTOS_CHIP_STM32F102CB) || defined(DISTORTOS_CHIP_STM32F102RB)
	#define STM32F102xB
#elif defined(DISTORTOS_CHIP_STM32F103C4) || defined(DISTORTOS_CHIP_STM32F103R4) || \
		defined(DISTORTOS_CHIP_STM32F103T4) || defined(DISTORTOS_CHIP_STM32F103C6) || \
		defined(DISTORTOS_CHIP_STM32F103R6) || defined(DISTORTOS_CHIP_STM32F103T6)
	#define STM32F103x6
#elif defined(DISTORTOS_CHIP_STM32F103C8) || defined(DISTORTOS_CHIP_STM32F103R8) || \
		defined(DISTORTOS_CHIP_STM32F103T8) || defined(DISTORTOS_CHIP_STM32F103V8) || \
		defined(DISTORTOS_CHIP_STM32F103CB) || defined(DISTORTOS_CHIP_STM32F103RB) || \
		defined(DISTORTOS_CHIP_STM32F103TB) || defined(DISTORTOS_CHIP_STM32F103VB)
	#define STM32F103xB
#elif defined(DISTORTOS_CHIP_STM32F103RC) || defined(DISTORTOS_CHIP_STM32F103VC) || \
		defined(DISTORTOS_CHIP_STM32F103ZC) || defined(DISTORTOS_CHIP_STM32F103RD) || \
		defined(DISTORTOS_CHIP_STM32F103VD) || defined(DISTORTOS_CHIP_STM32F103ZD) || \
		defined(DISTORTOS_CHIP_STM32F103RE) || defined(DISTORTOS_CHIP_STM32F103VE) || \
		defined(DISTORTOS_CHIP_STM32F103ZE)
	#define STM32F103xE
#elif defined(DISTORTOS_CHIP_STM32F103RF) || defined(DISTORTOS_CHIP_STM32F103VF) || \
		defined(DISTORTOS_CHIP_STM32F103ZF) || defined(DISTORTOS_CHIP_STM32F103RG) || \
		defined(DISTORTOS_CHIP_STM32F103VG) || defined(DISTORTOS_CHIP_STM32F103ZG)
	#define STM32F103xG
#elif defined(DISTORTOS_CHIP_STM32F105R8) || defined(DISTORTOS_CHIP_STM32F105V8) || \
		defined(DISTORTOS_CHIP_STM32F105RB) || defined(DISTORTOS_CHIP_STM32F105VB) || \
		defined(DISTORTOS_CHIP_STM32F105RC) || defined(DISTORTOS_CHIP_STM32F105VC)
	#define STM32F105xC
#elif defined(DISTORTOS_CHIP_STM32F107RB) || defined(DISTORTOS_CHIP_STM32F107VB) || \
		defined(DISTORTOS_CHIP_STM32F107RC) || defined(DISTORTOS_CHIP_STM32F107VC)
	#define STM32F107xC
#else
	#error "Unsupported STM32F1 chip!"
#endif

#include "stm32f1xx.h"

#endif	/* SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_CMSIS_PROXY_H_ */
