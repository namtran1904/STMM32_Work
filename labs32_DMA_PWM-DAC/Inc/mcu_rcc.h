#ifndef MCU_RCC_H // NOLINT
#define MCU_RCC_H

#include "mcu_chip_map.h" // NOLINT
#include <stdint.h>

#define SET 1
#define RESET 0

#define CLK_SOURCE_HSI 0
#define CLK_SOURCE_HSE 1
#define CLK_SOURCE_PLL 2

#define PLL_SOURCE_HSI 0
#define PLL_SOURCE_HSE 1

#define HSE_FREQ 8000000U
#define HSI_FREQ 16000000U

#define FREQ_1 30000000U
#define FREQ_2 64000000U
#define FREQ_3 90000000U

typedef enum { RCC_OK, RCC_ERROR } RCC_Status_t;

typedef struct {
  uint8_t SYSCLK_Source; // HSE,HSI,PLL  // NOLINT
  uint8_t PLL_Source;
  uint32_t PLL_M;
  uint32_t PLL_N;
  uint32_t PLL_P;
  uint32_t AHB_Divider;
  uint32_t APB1_Divider;
  uint32_t APB2_Divider;
} RCC_Config_t;

typedef struct {
  RCC_RegDef_t *pRCC;
  RCC_Config_t RCC_Config;
  RCC_Status_t Status;
} RCC_Handle_t;

uint8_t RCC_InitSystemClock(RCC_Handle_t *pRCCHandle);

uint32_t RCC_GetSYSCLKFreq(void);

uint32_t RCC_GetHCLKFreq(void);

uint32_t RCC_GetPCLK1Freq(void);

#endif // NOLINT
