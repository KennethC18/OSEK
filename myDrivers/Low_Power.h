/*
 * Low_Power.h
 *
 *  Created on: 7 mar. 2025
 *      Author: qcezs
 */

#ifndef LOW_POWER_H_
#define LOW_POWER_H_

#include "MK66F18.h"
#include "cmsis_gcc.h"
#include "fsl_common_arm.h"

/*!
 * @brief enter into sleep mode using CMSIS (wait)
 */
void sleep (void);

/*!
 * @brief enter into Deep Sleep mode using CMSIS (STOP)
 */
void deepsleep(void);

#endif /* LOW_POWER_H_ */
