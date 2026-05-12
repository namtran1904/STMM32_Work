#include "mpu6050.h" // NOLINT
#include "mcu_i2c.h" // NOLINT
#include <stdint.h>  // NOLINT

I2C_Status_t MPU6050_WriteReg(I2C_Handle_t *pI2CHandle, uint8_t reg_addr,
                              uint8_t data) {
  uint8_t temp_buffer[2] = {reg_addr, data};
  return I2C_MasterSendData(pI2CHandle, temp_buffer, 2, MPU6050_BASEADDR);
}

I2C_Status_t MPU6050_ReadReg(I2C_Handle_t *pI2CHandle, uint8_t reg_addr,
                             uint8_t *pdata, uint32_t Len) {
  I2C_Status_t status;
  status = I2C_MasterSendData(pI2CHandle, &reg_addr, 1, MPU6050_BASEADDR);
  if (status != I2C_OK) {
    return status;
  }
  return I2C_MasterReceiveData(pI2CHandle, pdata, Len, MPU6050_BASEADDR);
}

I2C_Status_t MPU6050_Read_All(I2C_Handle_t *pI2CHandle, MPU6050_Data_t *pdata) {
  uint8_t temp_buffer[14];
  int16_t temperature = 0;
  I2C_Status_t status;

  status = MPU6050_ReadReg(pI2CHandle, ACCEL_XOUT_H, temp_buffer, 14);
  if (status != I2C_OK) {
    return status;
  }

  pdata->Accel_X_Raw = (int16_t)(temp_buffer[0] << 8 | temp_buffer[1] << 0);
  pdata->Accel_Y_Raw = (int16_t)(temp_buffer[2] << 8 | temp_buffer[3] << 0);
  pdata->Accel_Z_Raw = (int16_t)(temp_buffer[4] << 8 | temp_buffer[5] << 0);

  temperature = (int16_t)(temp_buffer[6] << 8 | temp_buffer[7] << 0);
  pdata->Temperature = (temperature / 340.0f) + 36.53f;

  pdata->Gyro_X_Raw = (int16_t)(temp_buffer[8] << 8 | temp_buffer[9] << 0);
  pdata->Gyro_Y_Raw = (int16_t)(temp_buffer[10] << 8 | temp_buffer[11] << 0);
  pdata->Gyro_Z_Raw = (int16_t)(temp_buffer[12] << 8 | temp_buffer[13] << 0);

  return I2C_OK;
}

I2C_Status_t MPU6050_Init(I2C_Handle_t *pI2CHandle) {
  I2C_Status_t status;
  uint8_t who_am_i = 0;
  status = MPU6050_ReadReg(pI2CHandle, MPU6050_NAME, &who_am_i, 1);
  if (status != I2C_OK) {
    return status;
  }

  if (who_am_i != 0x68) {
    return I2C_ERROR;
  }
  status = MPU6050_WriteReg(pI2CHandle, PWR_MGMT_1, 0x00);
  return I2C_OK;
}
