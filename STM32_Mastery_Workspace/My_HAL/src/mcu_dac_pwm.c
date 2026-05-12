#include "mcu_dac_pwm.h" // NOLINT
#include "mcu_pwm.h"     // NOLINT
#include <stdint.h>

DAC_PWM_Status_t DAC_PWM_Step(DAC_PWM_Handle_t *pDAC) {
  if ((pDAC == NULL) || (pDAC->DAC_PWM_Config.pPWMHandle == NULL)) {
    return DAC_ERROR;
  } else {
    // Tính toán duty cycle
    uint32_t duty_cycle = pDAC->DAC_PWM_Config.pLUTS[pDAC->DAC_PWM_Config.Idx];

    // Cập nhật duty cycle
    PWM_Set_Duty_Cycle(pDAC->DAC_PWM_Config.pPWMHandle, duty_cycle);

    // Tăng index
    pDAC->DAC_PWM_Config.Idx++;

    // Reset index
    if (pDAC->DAC_PWM_Config.Idx >= pDAC->DAC_PWM_Config.Size) {
      pDAC->DAC_PWM_Config.Idx = 0;
    }

    return DAC_OK;
  }
}
