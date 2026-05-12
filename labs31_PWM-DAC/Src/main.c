#include "mcu_chip_map.h"
#include "mcu_dac_pwm.h"
#include "mcu_pwm.h"
#include "mcu_rcc.h"
#include "mcu_timer.h"

// DỮ LIỆU SÓNG SIN
static uint32_t Sine_LUT_32[32] = {50, 60, 69, 78, 86, 93, 97, 99, 100, 99, 97,
                                   93, 86, 78, 69, 60, 50, 40, 31, 22,  14, 7,
                                   3,  1,  0,  1,  3,  7,  14, 22, 31,  40};

RCC_Handle_t hRcc;
TIM_Handle_t hTimerPWM;  // TIM2 cho PWM 100kHz
TIM_Handle_t hTimerBase; // TIM3 cho Ngắt 3.2kHz
PWM_Handle_t hPwm1;
DAC_PWM_Handle_t hDac;

// TRÌNH PHỤC VỤ NGẮT (ISR) CHO TIM3
void TIM3_IRQHandler(void) {
  if (TIM3->SR & (1 << 0)) {
    TIM3->SR &= ~(1 << 0);

    // Thực hiện 1 bước nhảy của sóng Sin
    DAC_PWM_Step(&hDac);
  }
}

// Bật ngắt trên NVIC
void NVIC_Enable_TIM3(void) {
  volatile uint32_t *pNVIC_ISER0 = (uint32_t *)0xE000E100;
  *pNVIC_ISER0 |= (1 << 29);
}

// Cấu hình PA0
void GPIO_Init_PA0_TIM2(void) {
  RCC->AHB1ENR |= (1 << 0);
  volatile uint32_t *pGPIOA_MODER = (uint32_t *)0x40020000;
  *pGPIOA_MODER &= ~(3 << 0);
  *pGPIOA_MODER |= (2 << 0);
  volatile uint32_t *pGPIOA_AFRL = (uint32_t *)0x40020020;
  *pGPIOA_AFRL &= ~(0xF << 0);
  *pGPIOA_AFRL |= (1 << 0); // AF1
}

int main(void) {
  // CẤU HÌNH RCC LÊN 100MHz
  hRcc.pRCC = RCC;
  hRcc.RCC_Config.PLL_Source = PLL_SOURCE_HSE;
  hRcc.RCC_Config.PLL_M = 4;
  hRcc.RCC_Config.PLL_N = 100;
  hRcc.RCC_Config.PLL_P = 2;
  hRcc.RCC_Config.AHB_Divider = 0;
  hRcc.RCC_Config.APB1_Divider = 4;
  hRcc.RCC_Config.APB2_Divider = 0;

  if (RCC_InitSystemClock(&hRcc) != RCC_OK)
    while (1)
      ;

  // CẤU HÌNH NGOẠI VI
  GPIO_Init_PA0_TIM2();

  // CẤU HÌNH TIM2
  hTimerPWM.pTIMx = TIM2;
  hTimerPWM.TIM_Config.Prescaler = 9;
  hTimerPWM.TIM_Config.Period = 99;
  hTimerPWM.TIM_Config.Counter_Mode = TIM_COUNTER_MODE_UP;
  hTimerPWM.TIM_Config.ARPRE = ENABLE;
  TIM_Init(&hTimerPWM);

  hPwm1.PWM_Config.pTIMHandle = &hTimerPWM;
  hPwm1.PWM_Config.Channel = PWM_CHANNEL_1;
  hPwm1.PWM_Config.PWM_Mode = PWM_MODE_1;
  hPwm1.PWM_Config.Polarity = PWM_POLARITY_HIGH;
  hPwm1.PWM_Config.Duty_Cycle = 50;
  PWM_Init(&hPwm1);
  PWM_Start(&hPwm1);

  // CẤU HÌNH TIM3
  // Mục tiêu: 100Hz * 32 điểm = 3.2kHz ngắt/giây
  hTimerBase.pTIMx = TIM3;
  hTimerBase.TIM_Config.Prescaler = 99;
  hTimerBase.TIM_Config.Period = 311;
  hTimerBase.TIM_Config.Counter_Mode = TIM_COUNTER_MODE_UP;
  TIM_Init(&hTimerBase);

  // Bật ngắt cho TIM3
  TIM_Interrupt_Control(&hTimerBase, ENABLE);
  NVIC_Enable_TIM3();

  // KHỞI TẠO DAC
  hDac.DAC_PWM_Config.pPWMHandle = &hPwm1;
  hDac.DAC_PWM_Config.pLUTS = Sine_LUT_32;
  hDac.DAC_PWM_Config.Size = 32;
  hDac.DAC_PWM_Config.Idx = 0;

  // Kích hoạt hệ thống
  TIM_Cmd(&hTimerPWM, ENABLE);
  TIM_Cmd(&hTimerBase, ENABLE);

  while (1) {
  }
}
