/**
 *******************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * This software component is licensed by WCH under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#include "backup.h"
#include "clock.h"
#include "core_riscv_ch32yyxx.h"
#include "ch32yyxx_rcc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TICK_FREQ_1KHz    1L
// #define TICK_FREQ_100Hz   10L
// #define TICK_FREQ_10Hz    100L 


__IO uint64_t msTick=0;
WEAK uint64_t GetTick(void)
{
  return msTick;
}


void osSystickHandler() __attribute__((weak, alias("noOsSystickHandler")));
void noOsSystickHandler()
{

}

/**
  * @brief  Function called wto read the current millisecond
  * @param  None
  * @retval None
  */
uint32_t getCurrentMillis(void)
{
  return GetTick();
}



#if defined(CH32V20x) || defined(CH32V30x) || defined(CH32V30x_C) || defined(CH32V00x) || defined(CH32X035)

uint32_t getCurrentMicros(void)
{
  
  uint64_t m0 = GetTick();
  __IO uint64_t u0 = SysTick->CNT;
  uint64_t m1 = GetTick();
  __IO uint32_t u1 = SysTick->CNT;   //may be a interruption
   uint64_t tms = SysTick->CMP + 1;

  if (m1 != m0) {
    return (m1 * 1000 + ((tms - u1) * 1000) / tms);
  } else {
    return (m0 * 1000 + ((tms - u0) * 1000) / tms);
  }
}


/*********************************************************************
 * @fn      SysTick_Handler
 *
 * @brief   This function handles systick interrupt.
 *
 * @return  none
 */
void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void)
{
  msTick+=TICK_FREQ_1KHz;
  osSystickHandler();
  SysTick->SR = 0;
}

#endif





// for 10x serils (Qingke V3A) 
#if defined (CH32V10x)

#define SYSTICK_CNTL    (0xE000F004)   
#define SYSTICK_CNTH    (0xE000F008)
#define SYSTICK_CMPL    (0xE000F00C)
#define SYSTICK_CMPH    (0xE000F010)

uint32_t getCurrentMicros(void)
{
  
  uint64_t m0 = GetTick();
  uint64_t u0 = *((__IO uint32_t *)SYSTICK_CNTH);  
           u0 = (u0 << 32) + *((__IO uint32_t *)SYSTICK_CNTL);
  
  uint64_t m1 = GetTick();
  uint64_t u1 = *((__IO uint32_t *)SYSTICK_CNTH); //may be a interruption
           u1 = (u1 << 32) + *((__IO uint32_t *)SYSTICK_CNTL);

  uint64_t tms = *((__IO uint32_t *)SYSTICK_CMPH);
           tms = (tms << 32) + *((__IO uint32_t *)SYSTICK_CMPL) + 1;     

  if (m1 != m0) {
    return (m1 * 1000 + ((tms - u1) * 1000) / tms);
  } else {
    return (m0 * 1000 + ((tms - u0) * 1000) / tms);
  }
}



/*********************************************************************
 * @fn      SysTick_Handler
 *
 * @brief   This function handles systick interrupt.
 *
 * @return  none
 */
void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void)
{
  SysTick->CTLR=0;
  msTick+=TICK_FREQ_1KHz;
  SysTick->CNTL0=0;SysTick->CNTL1=0;SysTick->CNTL2=0;SysTick->CNTL3=0;
  SysTick->CNTH0=0;SysTick->CNTH1=0;SysTick->CNTH2=0;SysTick->CNTH3=0;
  SysTick->CTLR=0x1;
  osSystickHandler();
}

#endif




#ifdef __cplusplus
}
#endif


