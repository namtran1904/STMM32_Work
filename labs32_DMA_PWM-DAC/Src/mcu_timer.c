#include "mcu_timer.h" // NOLINT
#include "mcu_rcc.h"   // NOLINT
#include <stdint.h>

TIM_Status_t TIM_Peripheral_Clock_Control(TIM_Handle_t *pTIMHandle,
                                          uint8_t EnOrDis) {
  if ((pTIMHandle == NULL) || (pTIMHandle->pTIMx == NULL)) {
    return TIM_ERROR;
  } else {
    if (pTIMHandle->pTIMx == TIM2 || pTIMHandle->pTIMx == TIM3 ||
        pTIMHandle->pTIMx == TIM4 || pTIMHandle->pTIMx == TIM5) {
      uint8_t bit_pos = ((uint32_t)pTIMHandle->pTIMx - TIM2_BASEADDR) / 0x400;
      if (EnOrDis == ENABLE) {
        RCC->APB1ENR |= (1 << bit_pos);
      } else {
        RCC->APB1ENR &= ~(1 << bit_pos);
      }
    }
    return TIM_OK;
  }
}

TIM_Status_t TIM_Init(TIM_Handle_t *pTIMHandle) {
  if ((pTIMHandle == NULL) || pTIMHandle->pTIMx == NULL) {
    return TIM_ERROR;
  } else {
    // Bật clock cho timer
    TIM_Peripheral_Clock_Control(pTIMHandle, ENABLE);

    // Ghi PSC
    pTIMHandle->pTIMx->PSC = pTIMHandle->TIM_Config.Prescaler << 0;

    // Ghi ARR
    pTIMHandle->pTIMx->ARR = pTIMHandle->TIM_Config.Period << 0;

    // Chọn cách đếm
    pTIMHandle->pTIMx->CR1 &= ~(7 << 4);
    pTIMHandle->pTIMx->CR1 |= (pTIMHandle->TIM_Config.Counter_Mode << 4);

    // Xử lý Preload
    if (pTIMHandle->TIM_Config.ARPRE == ENABLE) {
      pTIMHandle->pTIMx->CR1 |= (1 << 7);
    } else {
      pTIMHandle->pTIMx->CR1 &= ~(1 << 7);
    }

    // Ép update event
    pTIMHandle->pTIMx->EGR |= (1 << 0);
    pTIMHandle->status = TIM_READY;
    return TIM_OK;
  }
}

TIM_Status_t TIM_Cmd(TIM_Handle_t *pTIMHandle, uint8_t EnOrDis) {
  if ((pTIMHandle == NULL) || (pTIMHandle->pTIMx == NULL)) {
    return TIM_ERROR;
  } else {
    if (EnOrDis == ENABLE) {
      pTIMHandle->pTIMx->CR1 |= (1 << 0);
      pTIMHandle->status = TIM_BUSY;
    } else {
      pTIMHandle->pTIMx->CR1 &= ~(1 << 0);
      pTIMHandle->status = TIM_READY;
    }
    return TIM_OK;
  }
}

TIM_Status_t TIM_Interrupt_Control(TIM_Handle_t *pTIMHandle, uint8_t EnOrDis) {
  if ((pTIMHandle == NULL) || (pTIMHandle->pTIMx == NULL)) {
    return TIM_ERROR;
  } else {
    if (EnOrDis == ENABLE) {
      pTIMHandle->pTIMx->DIER |= (1 << 0);
    } else {
      pTIMHandle->pTIMx->DIER &= ~(1 << 0);
    }
    return TIM_OK;
  }
}

TIM_Status_t TIM_DMA_Enable_Request(TIM_Handle_t *pTIMHandle, uint8_t EnOrDis) {
  if ((pTIMHandle->pTIMx == NULL) || (pTIMHandle->pTIMx == NULL)) {
    return TIM_ERROR;
  }
  // Bật tắt bit UDE (update DMA Request Enable)
  if (EnOrDis == ENABLE) {
    pTIMHandle->pTIMx->DIER |= (1 << 8);
  } else {
    pTIMHandle->pTIMx->DIER &= ~(1 << 8);
  }
  return TIM_OK;
}
