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

#include "pins_arduino.h"




// Digital PinName array
const PinName digitalPin[] = {
  PA_0, //D0
  PA_1, //D1
  PA_2, //D2
  PA_3, //D3
  PA_4, //D4
  PA_5, //D5
  PA_6, //D6
  PA_7, //D7
  PA_9, //D8
  PA_10, //D9
  PA_11, //D10
  PB_1, //D11
  PC_3, //D12
  PC_16, //D13
  PC_17, //D14
  PC_18, //D15
  PC_19 //D16
};

// Analog (Ax) pin number array
const uint32_t analogInputPin[] = {
  0,  // A0,  PA0
  1,  // A1,  PA1
  2,  // A2,  PA2
  3,  // A3,  PA3
  4,  // A4,  PA4
  5,  // A5,  PA5
  6,  // A6,  PA6
  7,  // A7,  PA7
  8,  // A8,  PB0
  9,  // A9,  PB1
  19, // A10, PC0
  20, // A13  PC3
};



