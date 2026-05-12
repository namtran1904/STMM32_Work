#include "mcu_chip_map.h"
#include "mcu_i2c.h"
#include "mcu_rcc.h"
#include "mpu6050.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  volatile uint32_t MODER;
  volatile uint32_t OTYPER;
  volatile uint32_t OSPEEDR;
  volatile uint32_t PUPDR;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t LCKR;
  volatile uint32_t AFR[2];
} GPIO_Reg_t;

typedef struct {
  volatile uint32_t SR;
  volatile uint32_t DR;
  volatile uint32_t BRR;
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t CR3;
} USART_Reg_t;

#define GPIOA_PTR ((GPIO_Reg_t *)0x40020000)
#define GPIOB_PTR ((GPIO_Reg_t *)0x40020400)
#define USART2_PTR ((USART_Reg_t *)0x40004400)

void UART_SendString(char *s) {
  while (*s) {
    while (!(USART2_PTR->SR & (1 << 7)))
      USART2_PTR->DR = *s++;
  }
}

void Delay_ms(uint32_t ms) {
  for (volatile uint32_t i = 0; i < ms * 10000; i++)
    ;
}

int main(void) {
  *(uint32_t *)(0xE000ED88) |= ((3UL << 20) | (3UL << 22));

  RCC->CR |= (1 << 16);
  while (!(RCC->CR & (1 << 17)))
    ;

  RCC->PLLCFGR = (8 << 0) | (200 << 6) | (0 << 16) | (1 << 22);

  FLASH->ACR |= (3 << 0);

  RCC->CR |= (1 << 24);
  while (!(RCC->CR & (1 << 25)))
    ;

  RCC->CFGR |= (4 << 10) | (2 << 0);
  ;

  RCC->AHB1ENR |= (1 << 0);
  RCC->APB1ENR |= (1 << 17);
  GPIOA_PTR->MODER |= (2 << 4);
  GPIOA_PTR->AFR[0] |= (7 << 8);
  USART2_PTR->BRR = (27 << 4) | 2;
  USART2_PTR->CR1 |= (1 << 13) | (1 << 3);

  UART_SendString("\r\n--- MPU6050 READ ALL START ---\r\n");

  RCC->AHB1ENR |= (1 << 1);
  GPIOB_PTR->MODER &= ~((3 << 16) | (3 << 18));
  GPIOB_PTR->MODER |= ((2 << 16) | (2 << 18));
  GPIOB_PTR->OTYPER |= ((1 << 8) | (1 << 9));
  GPIOB_PTR->PUPDR |= ((1 << 16) | (1 << 18));
  GPIOB_PTR->AFR[1] |= ((4 << 0) | (4 << 4));

  I2C_Handle_t hmpu;
  hmpu.pI2Cx = I2C1;
  hmpu.I2C_BitPos = 21;
  hmpu.I2C_Config.I2C_SCLspeed = I2C_SCL_STANDARD_MODE;
  hmpu.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
  hmpu.I2C_Config.I2C_DutyCycle = I2C_FM_DUTY_2;

  if (I2C_Init(&hmpu) != I2C_OK) {
    UART_SendString("[ERROR] I2C Init Failed!\r\n");
    while (1)
      ;
  }

  Delay_ms(100); // Đợi nguồn ổn định
  if (MPU6050_Init(&hmpu) != I2C_OK) {
    UART_SendString("[ERROR] MPU6050 Init Failed! Check connections.\r\n");
  } else {
    UART_SendString("[SUCCESS] MPU6050 is Online!\r\n");
  }

  /*Vòng lặp đọc dữ liệu */
  MPU6050_Data_t sensor_data;
  char uart_buf[128];

  while (1) {
    if (MPU6050_Read_All(&hmpu, &sensor_data) == I2C_OK) {
      // CHỈ IN SỐ NGUYÊN ĐỂ KIỂM TRA
      sprintf(uart_buf, "AX: %d | AY: %d | AZ: %d\r\n", sensor_data.Accel_X_Raw,
              sensor_data.Accel_Y_Raw, sensor_data.Accel_Z_Raw);
      UART_SendString(uart_buf);
    } else {
      UART_SendString("[ERR] Read failed!\r\n");
    }
    Delay_ms(200);
  }
}
