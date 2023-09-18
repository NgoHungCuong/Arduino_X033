/*
 Copyright (c) 2011 Arduino.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Arduino.h"
#include "PinConfigured.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(DAC_MODULE_ENABLED) && !defined(DAC_MODULE_ONLY)) ||\
    (defined(TIM_MODULE_ENABLED) && !defined(TIM_MODULE_ONLY))
//This is the list of the IOs configured
uint32_t g_anOutputPinConfigured[MAX_NB_PORT] = {0};
#endif

#define MAX_ADC_RESOLUTION 12
#define MAX_PWM_RESOLUTION 16

static int _readResolution = ADC_RESOLUTION;

static int _internalReadResolution =
#if ADC_RESOLUTION > MAX_ADC_RESOLUTION
  MAX_ADC_RESOLUTION
#else
  ADC_RESOLUTION
#endif /* ADC_RESOLUTION > MAX_ADC_RESOLUTION */
  ;

static int _writeResolution = PWM_RESOLUTION;
static int _internalWriteResolution =
#if PWM_RESOLUTION > MAX_PWM_RESOLUTION
  MAX_PWM_RESOLUTION
#else
  PWM_RESOLUTION
#endif
  ;

static uint32_t _writeFreq = PWM_FREQUENCY;

void analogReadResolution(int res)
{
  if ((res > 0) && (res <= 32)) 
  {
      _readResolution = res;
      _internalReadResolution = _readResolution;
      if (_readResolution > MAX_ADC_RESOLUTION) 
      {
        _internalReadResolution = MAX_ADC_RESOLUTION;
      } 
      else 
      {
        _internalReadResolution = 12;
      }
  } 
  else 
  {
    Error_Handler();
  }
}

void analogWriteResolution(int res)
{
  if ((res > 0) && (res <= 32)) 
  {
      _writeResolution = res;
      if (_writeResolution > MAX_PWM_RESOLUTION) 
      {
        _internalWriteResolution = MAX_PWM_RESOLUTION;
      } 
      else 
      {
        _internalWriteResolution = _writeResolution;
      }
  } 
  else 
  {
    Error_Handler();
  }
}

void analogWriteFrequency(uint32_t freq)
{
  _writeFreq = freq;
}

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to)
{
  if (from != to) {
    if (from > to) {
      value = (value < (uint32_t)(1 << (from - to))) ? 0 : ((value + 1) >> (from - to)) - 1;
    } else {
      if (value != 0) {
        value = ((value + 1) << (to - from)) - 1;
      }
    }
  }
  return value;
}

void analogReference(eAnalogReference ulMode)
{
  (void) ulMode;
}

// Perform the read operation on the selected analog pin.
// the initialization of the analog PIN is done through this function
uint32_t analogRead(uint32_t ulPin)
{
  uint32_t value = 0;
#if defined(ADC_MODULE_ENABLED) && !defined(ADC_MODULE_ONLY)
  PinName p = analogInputToPinName(ulPin);
  if (p != NC) {
    value = adc_read_value(p, _internalReadResolution);
    value = mapResolution(value, _internalReadResolution, _readResolution);
  }
#else
  (void) ulPin;
#endif
  return value;
}


void analogOutputInit(void)
{
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// variant.cpp file.  For the rest of the pins, we default
// to digital output.
void analogWrite(uint32_t ulPin, uint32_t ulValue)
{
#if defined(DAC_MODULE_ENABLED) && !defined(DAC_MODULE_ONLY)
  uint8_t do_init = 0;
#endif
  PinName p = digitalPinToPinName(ulPin);
  if (p != NC) {
#if defined(DAC_MODULE_ENABLED) && !defined(DAC_MODULE_ONLY)
    if (pin_in_pinmap(p, PinMap_DAC)) {
      if (is_pin_configured(p, g_anOutputPinConfigured) == false) {
        do_init = 1;
        set_pin_configured(p, g_anOutputPinConfigured);
      }
      ulValue = mapResolution(ulValue, _writeResolution, DACC_RESOLUTION);
      dac_write_value(p, ulValue, do_init);
    } else
#endif //DAC_MODULE_ENABLED && !DAC_MODULE_ONLY
#if defined(TIM_MODULE_ENABLED) && !defined(TIM_MODULE_ONLY)
      if (pin_in_pinmap(p, PinMap_TIM)) {
        if (is_pin_configured(p, g_anOutputPinConfigured) == false) {
          set_pin_configured(p, g_anOutputPinConfigured);
        }
        ulValue = mapResolution(ulValue, _writeResolution, _internalWriteResolution);
        pwm_start(p, _writeFreq, ulValue, _internalWriteResolution);
      } else
#endif /* TIM_MODULE_ENABLED && !TIM_MODULE_ONLY */
      {
        //DIGITAL PIN ONLY
        // Defaults to digital write
        pinMode(ulPin, OUTPUT);
        ulValue = mapResolution(ulValue, _writeResolution, 8);
        if (ulValue < 128) {
          digitalWrite(ulPin, LOW);
        } else {
          digitalWrite(ulPin, HIGH);
        }
      }
  }
}

#ifdef __cplusplus
}
#endif
