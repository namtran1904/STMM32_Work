#include "mcu_rcc.h"      // NOLINT
#include "mcu_chip_map.h" // NOLINT
#include <stddef.h>

uint16_t AHB_Prescaler_Table[] = {1, 1, 1, 1,  1,  1,   1,   1,
                                  2, 4, 8, 16, 64, 128, 256, 512};
uint8_t APB_Prescaler_Table[] = {1, 1, 1, 1, 2, 4, 8, 16};

uint8_t RCC_InitSystemClock(RCC_Handle_t *pRCCHandle) {
  if (pRCCHandle == NULL) {
    return RCC_ERROR;
  } else {
    uint8_t pll_source_bit = 0;
    if (pRCCHandle->RCC_Config.PLL_Source == PLL_SOURCE_HSE) {
      pRCCHandle->pRCC->CR |= (1 << 16);
      while (!(pRCCHandle->pRCC->CR & (1 << 17))) {
      }
      pll_source_bit = PLL_SOURCE_HSE;
    }
    if (pRCCHandle->RCC_Config.PLL_Source == PLL_SOURCE_HSI) {
      pRCCHandle->pRCC->CR |= (1 << 0);
      while (!(pRCCHandle->pRCC->CR & (1 << 1))) {
      }
      pll_source_bit = PLL_SOURCE_HSI;
    }

    uint32_t freq_input = (pRCCHandle->RCC_Config.PLL_Source == PLL_SOURCE_HSE)
                              ? HSE_FREQ
                              : HSI_FREQ;
    uint32_t target_freq = pRCCHandle->RCC_Config.PLL_N *
                           (freq_input / pRCCHandle->RCC_Config.PLL_M);
    target_freq = target_freq / pRCCHandle->RCC_Config.PLL_P;

    if (target_freq <= FREQ_1) {
      FLASH->ACR &= ~(0x0F << 0);
    } else if (target_freq <= FREQ_2) {
      FLASH->ACR |= (1 << 0);
    } else if (target_freq <= FREQ_3) {
      FLASH->ACR |= (2 << 0);
    } else {
      FLASH->ACR |= (3 << 0);
    }
    FLASH->ACR |= ((1 << 8) | (1 << 9) | (1 << 10));

    uint32_t temp_config = 0;
    temp_config |= (pRCCHandle->RCC_Config.AHB_Divider << 4);
    temp_config |= (pRCCHandle->RCC_Config.APB1_Divider << 10);
    temp_config |= (pRCCHandle->RCC_Config.APB2_Divider << 13);
    pRCCHandle->pRCC->CFGR = temp_config;

    uint32_t temp_pll = 0;
    temp_pll |= (pRCCHandle->RCC_Config.PLL_M << 0);
    temp_pll |= (pRCCHandle->RCC_Config.PLL_N << 6);
    temp_pll |= (((pRCCHandle->RCC_Config.PLL_P / 2) - 1) << 16);
    temp_pll |= (pll_source_bit << 22);
    pRCCHandle->pRCC->PLLCFGR = temp_pll;

    pRCCHandle->pRCC->CR |= (1 << 24);
    while (!(pRCCHandle->pRCC->CR & (1 << 25))) {
    }

    pRCCHandle->pRCC->CFGR &= ~(3 << 0);
    pRCCHandle->pRCC->CFGR |= (2 << 0);
    while (((pRCCHandle->pRCC->CFGR >> 2) & 3) != 2) {
    }

    return RCC_OK;
  }
}

uint32_t RCC_GetSYSCLKFreq(void) {
  uint32_t sys_clk = 0;
  uint32_t clk_source = (RCC->CFGR >> 2) & 0x03;

  if (clk_source == CLK_SOURCE_HSI) {
    sys_clk = HSI_FREQ;
  } else if (clk_source == CLK_SOURCE_HSE) {
    sys_clk = HSE_FREQ;
  } else {
    uint32_t pll_source = 0;
    uint32_t pll_input = 0;
    uint32_t pll_n = 0;
    uint32_t pll_m = 0;
    uint32_t pll_p = 0;

    pll_source = ((RCC->PLLCFGR >> 22) & 1);
    pll_input = (pll_source != PLL_SOURCE_HSI) ? HSE_FREQ : HSI_FREQ;

    pll_m = (RCC->PLLCFGR & 0x3F);
    pll_n = (RCC->PLLCFGR >> 6) & 0x1FF;

    uint8_t temp_p = (RCC->PLLCFGR >> 16) & 3;
    pll_p = (temp_p + 1) * 2;

    sys_clk = ((pll_input / pll_m) * pll_n) / pll_p;
  }
  return sys_clk;
}

uint32_t RCC_GetHCLKFreq(void) {
  uint32_t sys_clk = RCC_GetSYSCLKFreq();
  uint16_t hpre = (RCC->CFGR >> 4) & 0X0F;
  return sys_clk / (AHB_Prescaler_Table[hpre]);
}

uint32_t RCC_GetPCLK1Freq(void) {
  uint32_t hclk = RCC_GetHCLKFreq();
  uint8_t ppre1 = (RCC->CFGR >> 10) & 0x07;
  return hclk / (APB_Prescaler_Table[ppre1]);
}
