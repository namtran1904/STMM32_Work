#ifndef MPU_6050_H // NOLINT
#define MPU_6050_H

#include "mcu_i2c.h" // NOLINT
#include <stdint.h>  // NOLINT

#define MPU6050_BASEADDR 0x68 // địa chỉ nguồn  // NOLINT
#define MPU6050_NAME 0X75     // thanh ghi định danh
#define PWR_MGMT_1 0X6B       // thanh ghi quản lý nguồn
#define ACCEL_XOUT_H 0X3B     // bắt đầu dữ liệu

typedef struct {
  int16_t Accel_X_Raw;
  int16_t Accel_Y_Raw;
  int16_t Accel_Z_Raw;
  int16_t Gyro_X_Raw;
  int16_t Gyro_Y_Raw;
  int16_t Gyro_Z_Raw;
  float Temperature;
} MPU6050_Data_t;

I2C_Status_t MPU6050_Init(I2C_Handle_t *pI2CHandle);

I2C_Status_t MPU6050_Read_All(I2C_Handle_t *pI2CHandle, MPU6050_Data_t *pData);

// Copy dòng này vào file mpu6050.h
I2C_Status_t MPU6050_ReadReg(I2C_Handle_t *pI2CHandle, uint8_t reg_addr,
                             uint8_t *pdata, uint32_t Len);

#endif // NOLINT
