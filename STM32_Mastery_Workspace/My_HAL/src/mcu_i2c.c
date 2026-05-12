#include "mcu_i2c.h"      // NOLINT
#include "mcu_chip_map.h" // NOLINT  // NOLINT
#include "mcu_rcc.h"      // NOLINT
#include <stddef.h>

#define I2C_TIMEOUT_MAX 1000000

void I2C_Peripheral_Clock_Control(I2C_Handle_t *pI2CHandle, uint8_t EnorDis) {
  if (EnorDis == ENABLE) {
    RCC->APB1ENR |= (1 << pI2CHandle->I2C_BitPos);
  } else {
    RCC->APB1ENR &= ~(1 << pI2CHandle->I2C_BitPos);
  }
}
I2C_Status_t I2C_Init(I2C_Handle_t *pI2CHandle) {
  if (pI2CHandle == NULL) {
    return I2C_ERROR;
  }

  // Bật clock
  I2C_Peripheral_Clock_Control(pI2CHandle, ENABLE);

  // Tắt I2C
  pI2CHandle->pI2Cx->CR1 &= ~(1 << 0);

  // Cấu hình APB1
  uint32_t pclk_freq = RCC_GetPCLK1Freq();
  uint8_t freq_val = (uint8_t)(pclk_freq / 1000000U);
  pI2CHandle->pI2Cx->CR2 = (freq_val & 0x3F);

  // Cấu hình tốc độ và rise time
  uint16_t ccr_temp = 0;
  uint8_t trise_val = 0;
  if (pI2CHandle->I2C_Config.I2C_SCLspeed == I2C_SCL_STANDARD_MODE) {
    ccr_temp = pclk_freq / (2 * pI2CHandle->I2C_Config.I2C_SCLspeed);
    trise_val = freq_val + 1;
  } else if (pI2CHandle->I2C_Config.I2C_SCLspeed == I2C_SCL_FAST_MODE) {
    if (pI2CHandle->I2C_Config.I2C_DutyCycle == I2C_FM_DUTY_2) {
      ccr_temp = pclk_freq / (3 * pI2CHandle->I2C_Config.I2C_SCLspeed);
    } else if (pI2CHandle->I2C_Config.I2C_DutyCycle == I2C_FM_DUTY_16_9) {
      ccr_temp = pclk_freq / (25 * pI2CHandle->I2C_Config.I2C_SCLspeed);
    }
    ccr_temp |= (1 << 15);

    if (pI2CHandle->I2C_Config.I2C_DutyCycle == I2C_FM_DUTY_16_9) {
      ccr_temp |= (1 << 14);
    }

    trise_val = ((freq_val * 3) / 10) + 1;
  }

  // Ghi vào CCR
  pI2CHandle->pI2Cx->CCR = (ccr_temp & 0xFFFF);

  // Ghi vào TRISE
  pI2CHandle->pI2Cx->TRISE = (trise_val & 0x3F);

  // Ghi vào OAR1
  pI2CHandle->pI2Cx->OAR1 |= (1 << 14);

  // Bật PE
  pI2CHandle->pI2Cx->CR1 |= (1 << 0);

  // Bật ACK
  if (pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE) {
    pI2CHandle->pI2Cx->CR1 |= (1 << 10);
  }

  return I2C_OK;
}

static I2C_Status_t I2C_WaitForFlag(I2C_RegDef_t *pI2Cx, uint32_t FlagBit,
                                    uint8_t Status) {
  uint32_t timeout = I2C_TIMEOUT_MAX;
  while (((pI2Cx->SR1 >> FlagBit) & 0x1) != Status) {
    if (--timeout == 0) {
      return I2C_TIME_OUT;
    }
  }
  return I2C_OK;
}

I2C_Status_t I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer,
                                uint32_t Len, uint8_t SlaveAddr) {
  if ((pI2CHandle == NULL) || (pTxBuffer == NULL)) {
    return I2C_ERROR;
  } else {
    pI2CHandle->Status = I2C_BUSY_IN_TX;
    pI2CHandle->pI2Cx->CR1 |= (1 << 8);
    if (I2C_WaitForFlag(pI2CHandle->pI2Cx, 0, SET) == I2C_TIME_OUT) {
      pI2CHandle->pI2Cx->CR1 |= (1 << 9);
      pI2CHandle->Status = I2C_OK;
      return I2C_TIME_OUT;
    }

    SlaveAddr = SlaveAddr << 1;
    SlaveAddr &= ~(1 << 0);
    pI2CHandle->pI2Cx->DR = SlaveAddr;

    if (I2C_WaitForFlag(pI2CHandle->pI2Cx, 1, SET) == I2C_TIME_OUT) {
      pI2CHandle->pI2Cx->CR1 |= (1 << 9);
      pI2CHandle->Status = I2C_OK;
      return I2C_TIME_OUT;
    }

    uint32_t dummy_read = pI2CHandle->pI2Cx->SR1;
    dummy_read = pI2CHandle->pI2Cx->SR2;
    (void)dummy_read;

    while (Len > 0) {
      if (I2C_WaitForFlag(pI2CHandle->pI2Cx, 7, SET) == I2C_TIME_OUT) {
        pI2CHandle->pI2Cx->CR1 |= (1 << 9);
        pI2CHandle->Status = I2C_OK;
        return I2C_TIME_OUT;
      }
      pI2CHandle->pI2Cx->DR = *pTxBuffer;
      Len -= 1;
      pTxBuffer += 1;
    }

    if (I2C_WaitForFlag(pI2CHandle->pI2Cx, 2, SET) == I2C_TIME_OUT) {
      pI2CHandle->pI2Cx->CR1 |= (1 << 9);
      pI2CHandle->Status = I2C_OK;
      return I2C_TIME_OUT;
    }
    pI2CHandle->pI2Cx->CR1 |= (1 << 9);

    pI2CHandle->Status = I2C_OK;

    return I2C_OK;
  }
}

I2C_Status_t I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer,
                                   uint32_t Len, uint8_t SlaveAddr) {
  if (pI2CHandle == NULL || pRxBuffer == NULL) {
    return I2C_ERROR;
  }
  // Check busy
  pI2CHandle->Status = I2C_BUSY_IN_RX;

  // Bật ACK
  if (pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE) {
    pI2CHandle->pI2Cx->CR1 |= (1 << 10);
  } else {
    pI2CHandle->pI2Cx->CR1 &= ~(1 << 10);
  }

  // Tạo start
  pI2CHandle->pI2Cx->CR1 |= (1 << 8);
  if (I2C_WaitForFlag(pI2CHandle->pI2Cx, 0, SET) != I2C_OK) {
    pI2CHandle->pI2Cx->CR1 |= (1 << 9);
    return I2C_TIME_OUT;
  }

  // Gửi địa chỉ slave
  pI2CHandle->pI2Cx->DR = ((SlaveAddr << 1) | 0x01);
  if (I2C_WaitForFlag(pI2CHandle->pI2Cx, 1, SET) != I2C_OK) {
    pI2CHandle->pI2Cx->CR1 |= (1 << 9);
    return I2C_TIME_OUT;
  }

  // Nhận 1 byte
  if (Len == 1) {
    pI2CHandle->pI2Cx->CR1 &= ~(1 << 10);
    (void)pI2CHandle->pI2Cx->SR1;
    (void)pI2CHandle->pI2Cx->SR2;
    pI2CHandle->pI2Cx->CR1 |= (1 << 9);
    if (I2C_WaitForFlag(pI2CHandle->pI2Cx, 6, SET) != I2C_OK) {
      return I2C_TIME_OUT;
    }
    *pRxBuffer = pI2CHandle->pI2Cx->DR;
  }
  // Nhận hơn 1 byte
  else { // NOLINT
    (void)pI2CHandle->pI2Cx->SR1;
    (void)pI2CHandle->pI2Cx->SR2;
    for (uint32_t i = Len; i > 0; i--) {
      if (I2C_WaitForFlag(pI2CHandle->pI2Cx, 6, SET) != I2C_OK) {
        pI2CHandle->pI2Cx->CR1 |= (1 << 9);
        return I2C_TIME_OUT;
      }
      if (i == 2) {
        pI2CHandle->pI2Cx->CR1 &= ~(1 << 10);
        pI2CHandle->pI2Cx->CR1 |= (1 << 9);
      }
      *pRxBuffer = pI2CHandle->pI2Cx->DR;
      pRxBuffer++;
    }
  }

  // Bật lại ACK
  if (pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE) {
    pI2CHandle->pI2Cx->CR1 |= (1 << 10);
  }

  // Đặt lại state
  pI2CHandle->Status = I2C_OK;
  return I2C_OK;
}
