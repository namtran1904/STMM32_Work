#ifndef MCU_PWM_H // NOLINT
#define MCU_PWM_H

#include "mcu_chip_map.h" // NOLINT
#include "mcu_timer.h"    // NOLINT

#define PWM_MODE_1 6
#define PWM_MODE_2 7

#define PWM_CHANNEL_1 1
#define PWM_CHANNEL_2 2
#define PWM_CHANNEL_3 3
#define PWM_CHANNEL_4 4

#define PWM_POLARITY_HIGH 0
#define PWM_POLARITY_LOW 1

typedef enum { PWM_READY, PWM_OK, PWM_ERROR } PWM_Status_t;

typedef struct {
  TIM_Handle_t *pTIMHandle;
  uint8_t Channel;
  uint8_t PWM_Mode;
  uint32_t Duty_Cycle;
  uint8_t Polarity;
} PWM_Config_t;

typedef struct {
  PWM_Config_t PWM_Config;
} PWM_Handle_t;

PWM_Status_t PWM_Init(PWM_Handle_t *pPWMHandle);

PWM_Status_t PWM_Start(PWM_Handle_t *pPWMHandle);

PWM_Status_t PWM_Set_Duty_Cycle(PWM_Handle_t *pPWMHandle, uint32_t duty_cycle);
#endif // NOLINT
