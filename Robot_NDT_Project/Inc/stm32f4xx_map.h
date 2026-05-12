#include <stdint.h> // NOLINT

//
#define HSE_VALUE 8000000
#define HSI_VALUE 16000000

// Địa chỉ gốc các vùng nhớ chính
#define PERIPHERIAL_BASEADDR 0x40000000U
#define ABP1_PERIPH_BASEADDR 0x40000000U
#define APB2_PERIPH_BASEADDR 0x40010000U
#define AHB1_PERIPH_BASEADDR 0x40020000U
#define AHB2_PERIPH_BASEADDR 0x50000000U
#define FLASH_R_BASEADDR 0x40023C00U

// Địa chỉ các thành phần RCC
#define RCC_BASEADDR (AHB1_PERIPH_BASEADDR + 0x3800U)

// Cấu trúc các thanh ghi RCC
typedef struct __attribute__((packed)) {
  volatile uint32_t CR;
  volatile uint32_t PLLCFGR;
  volatile uint32_t CFGR;
  volatile uint32_t CIR;
  uint32_t reserved0[8];
  volatile uint32_t AHB1ENR;
  volatile uint32_t AHB2ENR;
  uint32_t reserved1[2];
  volatile uint32_t APB1ENR;
  volatile uint32_t APB2ENR;
} RCC_RegDef_t;

// Địa chỉ các thành phần I2C
#define I2C1_BASEADDR (ABP1_PERIPH_BASEADDR + 0X5400U)
#define I2C2_BASEADDR (ABP1_PERIPH_BASEADDR + 0X5800U)
#define I2C3_BASEADDR (ABP1_PERIPH_BASEADDR + 0X5C00U)

// Cấu trúc các thanh ghi I2C
typedef struct __attribute__((packed)) {
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t OAR1;
  volatile uint32_t OAR2;
  volatile uint32_t DR;
  volatile uint32_t SR1;
  volatile uint32_t SR2;
  volatile uint32_t CCR;
  volatile uint32_t TRISE;
} I2C_RegDef_t;

// Macro trong I2C
#define ENABLE 1
#define DISABLE 0

// Ép kiểu địa chỉ thành đối tượng
#define RCC ((RCC_RegDef_t *)RCC_BASEADDR)
#define I2C1 ((I2C_RegDef_t *)I2C1_BASEADDR)
#define I2C2 ((I2C_RegDef_t *)I2C2_BASEADDR)
#define I2C3 ((I2C_RegDef_t *)I2C3_BASEADDR)

// Cấu trúc thanh ghi Flash
typedef struct {
  volatile uint32_t ACR;
  volatile uint32_t KEYR;
  volatile uint32_t OPTKEYR;
  volatile uint32_t SR;
  volatile uint32_t CR;
  volatile uint32_t OPTCR;
} FLASH_RegDef_t;

// Ép kiểu địa chỉ thành đối tượng
#define FLASH ((FLASH_RegDef_t *)FLASH_R_BASEADDR)
