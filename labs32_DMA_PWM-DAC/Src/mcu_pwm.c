#include "mcu_pwm.h"      // NOLINT
#include "mcu_chip_map.h" // NOLINT
#include <stdint.h>

PWM_Status_t PWM_Init(PWM_Handle_t *pPWMHandle) {
  if ((pPWMHandle == NULL) || (pPWMHandle->PWM_Config.pTIMHandle == NULL)) {
    return PWM_ERROR;
  } else {
    TIM_RegDef_t *pTIMx = pPWMHandle->PWM_Config.pTIMHandle->pTIMx;
    uint8_t bit_offset = 0;
    volatile uint32_t *pCCMR_reg = NULL;
    switch (pPWMHandle->PWM_Config.Channel) {
    case PWM_CHANNEL_1:
      bit_offset = 0;
      pCCMR_reg = &(pTIMx->CCMR1);
      break;

    case PWM_CHANNEL_2:
      bit_offset = 8;
      pCCMR_reg = &(pTIMx->CCMR1);
      break;

    case PWM_CHANNEL_3:
      bit_offset = 0;
      pCCMR_reg = &(pTIMx->CCMR2);
      break;

    case PWM_CHANNEL_4:
      bit_offset = 8;
      pCCMR_reg = &(pTIMx->CCMR2);
      break;

    default:
      return PWM_ERROR;
    }
    // Cấu hình Mode
    *pCCMR_reg &= ~(7 << (bit_offset + 4));
    *pCCMR_reg |= (pPWMHandle->PWM_Config.PWM_Mode << (bit_offset + 4));

    // Cấu hình Preload
    *pCCMR_reg &= ~(1 << (bit_offset + 3));
    *pCCMR_reg |= (1 << (bit_offset + 3));

    // Cấu hình Polarity
    uint8_t polarity_bit_pos =
        ((uint8_t)(pPWMHandle->PWM_Config.Channel - 1)) * 4 + 1;

    pTIMx->CCER &= ~(1 << polarity_bit_pos);
    pTIMx->CCER |= (pPWMHandle->PWM_Config.Polarity << polarity_bit_pos);

    // Nạp Duty Cycle vào thanh ghi CCR tương ứng
    switch (pPWMHandle->PWM_Config.Channel) {
    case PWM_CHANNEL_1:
      pTIMx->CCR1 = pPWMHandle->PWM_Config.Duty_Cycle;
      break;
    case PWM_CHANNEL_2:
      pTIMx->CCR2 = pPWMHandle->PWM_Config.Duty_Cycle;
      break;
    case PWM_CHANNEL_3:
      pTIMx->CCR3 = pPWMHandle->PWM_Config.Duty_Cycle;
      break;
    case PWM_CHANNEL_4:
      pTIMx->CCR4 = pPWMHandle->PWM_Config.Duty_Cycle;
      break;
    }

    return PWM_OK;
  }
}

PWM_Status_t PWM_Start(PWM_Handle_t *pPWMHandle) {
  if ((pPWMHandle == NULL) || (pPWMHandle->PWM_Config.pTIMHandle == NULL)) {
    return PWM_ERROR;
  } else {
    TIM_RegDef_t *pTIMx = pPWMHandle->PWM_Config.pTIMHandle->pTIMx;
    uint8_t output_bit_pos =
        ((uint8_t)(pPWMHandle->PWM_Config.Channel - 1)) * 4;
    pTIMx->CCER |= (1 << output_bit_pos);
    return PWM_READY;
  }
}

PWM_Status_t PWM_Set_Duty_Cycle(PWM_Handle_t *pPWMHandle, uint32_t duty_cycle) {
  if ((pPWMHandle == NULL) || (pPWMHandle->PWM_Config.pTIMHandle == NULL)) {
    return PWM_ERROR;
  } else {
    TIM_RegDef_t *pTIMx = pPWMHandle->PWM_Config.pTIMHandle->pTIMx;
    pPWMHandle->PWM_Config.Duty_Cycle = duty_cycle;
    switch (pPWMHandle->PWM_Config.Channel) {
    case PWM_CHANNEL_1:
      pTIMx->CCR1 = pPWMHandle->PWM_Config.Duty_Cycle;
      break;
    case PWM_CHANNEL_2:
      pTIMx->CCR2 = pPWMHandle->PWM_Config.Duty_Cycle;
      break;
    case PWM_CHANNEL_3:
      pTIMx->CCR3 = pPWMHandle->PWM_Config.Duty_Cycle;
      break;
    case PWM_CHANNEL_4:
      pTIMx->CCR4 = pPWMHandle->PWM_Config.Duty_Cycle;
      break;
    }

    return PWM_OK;
  }
}
