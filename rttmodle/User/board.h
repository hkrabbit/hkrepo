#ifndef __BOARD__H__

#define __BOARD__H__


#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"

#include "bsp_key.h"  

void rt_hw_board_init(void);
void SysTick_Handler(void);

#endif