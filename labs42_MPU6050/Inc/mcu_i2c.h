#ifndef MCU_I2C_H // NOLINT
#define MCU_I2C_H

#include "mcu_chip_map.h" // NOLINT
#include <stdint.h>

typedef enum {
  I2C_OK = 0,
  I2C_ERROR,
  I2C_BUSY_IN_TX,
  I2C_BUSY_IN_RX,
  I2C_ACK_FAILURE,
  I2C_TIME_OUT
} I2C_Status_t;

#define I2C_SCL_STANDARD_MODE 100000
#define I2C_SCL_FAST_MODE 400000
#define I2C_ACK_ENABLE 1
#define I2C_ACK_DISABLE 0
#define I2C_FM_DUTY_2 0
#define I2C_FM_DUTY_16_9 1

typedef struct {
  uint32_t I2C_SCLspeed;
  uint8_t I2C_DeviceAddress;
  uint8_t I2C_AckControl;
  uint8_t I2C_DutyCycle;
} I2C_Config_t;

typedef struct {
  I2C_RegDef_t *pI2Cx;
  I2C_Config_t I2C_Config;
  uint8_t I2C_BitPos;
  I2C_Status_t Status;
} I2C_Handle_t;

void I2C_Peripherial_Clock_Control(I2C_Handle_t *pI2CHandle, uint8_t EnorDi);

I2C_Status_t I2C_Init(I2C_Handle_t *pI2CHandle);

I2C_Status_t I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer,
                                uint32_t Len, uint8_t SlaveAddr);

I2C_Status_t I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer,
                                   uint32_t Len, uint8_t SlaveAddr);

#endif // NOLINT
// NOLINT
