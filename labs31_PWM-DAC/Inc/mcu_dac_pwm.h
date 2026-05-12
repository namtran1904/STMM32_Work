#ifndef MCU_DAC_PWM_H // NOLINT
#define MCU_DAC_PWM_H
#include "mcu_pwm.h"   // NOLINT
#include "mcu_timer.h" // NOLINT

typedef enum { DAC_OK, DAC_ERROR } DAC_PWM_Status_t;
typedef struct {
  PWM_Handle_t *pPWMHandle;
  uint32_t *pLUTS;
  uint32_t Size;
  uint32_t Idx;
} DAC_PWM_Config_t;

typedef struct {
  DAC_PWM_Config_t DAC_PWM_Config;
} DAC_PWM_Handle_t;

DAC_PWM_Status_t DAC_PWM_Step(DAC_PWM_Handle_t *pDAC);
#endif // NOLINT
