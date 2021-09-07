/*
 * main.h
 *
 *  Created on: Aug 14, 2021
 *      Author: aleksandruzienko
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f1xx.h"
#include <stdbool.h>
#include <stdio.h>
#include "rcc.h"
#include "uart.h"
#include "gpio.h"
#include "tm1637.h"
#include "exti.h"
#include "adc.h"
#include "rtc.h"
#include "spi.h"
#include "rc522.h"
#include "i2c.h"

//TM1637 Digits pins
#define DIGIT_Port    GPIOB
#define DIGIT_CLK_Pin (1UL << 10)
#define DIGIT_DIO_Pin (1UL << 11)

#endif /* INC_MAIN_H_ */
