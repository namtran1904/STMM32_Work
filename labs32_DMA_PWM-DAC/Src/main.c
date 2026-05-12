#include "mcu_chip_map.h"
#include "mcu_dma.h"
#include "mcu_pwm.h"
#include "mcu_rcc.h"
#include "mcu_timer.h"

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
} GPIO_RegDef_t;

#define GPIOA ((GPIO_RegDef_t *)0x40020000U)

// 16 mẫu sóng Sin (Giá trị 0 đến 1000)
uint32_t sine_lut[] = {500, 695, 853, 961, 1000, 961, 853, 695,
                       500, 304, 146, 38,  0,    38,  146, 304};

void GPIO_Init_PA0_AF(void) {
  RCC->AHB1ENR |= (1 << 0);
  GPIOA->MODER &= ~(0x3 << 0);
  GPIOA->MODER |= (0x2 << 0);
  GPIOA->AFR[0] &= ~(0xF << 0);
  GPIOA->AFR[0] |= (0x1 << 0);
}

int main(void) {
  GPIO_Init_PA0_AF();

  TIM_Handle_t tim2_h = {.pTIMx = TIM2,
                         .TIM_Config = {.Period = 1000,
                                        .Prescaler = 0,
                                        .Counter_Mode = TIM_COUNTER_MODE_UP,
                                        .ARPRE = ENABLE}};
  TIM_Init(&tim2_h);

  PWM_Handle_t pwm_h = {.PWM_Config = {.pTIMHandle = &tim2_h,
                                       .Channel = PWM_CHANNEL_1,
                                       .PWM_Mode = PWM_MODE_1,
                                       .Duty_Cycle = 0,
                                       .Polarity = PWM_POLARITY_HIGH}};
  PWM_Init(&pwm_h);
  PWM_Start(&pwm_h);

  DMA_Handle_t dma_h;
  dma_h.pDMAx = DMA1;
  dma_h.DMA_BitPos = 21; // DMA1 nằm ở bit 21 AHB1ENR
  dma_h.Config = (DMA_Config_t){.Stream_idx = 0,
                                .Channel_idx = 3,
                                .Direction = DMA_DIR_MEM_TO_PERIPH,
                                .Mode = DMA_CIRCULAR_ENABLE,
                                .MSize = DMA_MSIZE_32BITS,
                                .PSize = DMA_PSIZE_32_BITS,
                                .MInc = DMA_MEM_INCREASE,
                                .PInc = 0,
                                .Priority = PL_HIGH,
                                .Mem_BaseAddr = (uint32_t)sine_lut,
                                .Periph_BaseAddr = (uint32_t) & (TIM2->CCR1),
                                .Data_len = 16};

  DMA_Clock_Control(&dma_h, ENABLE);
  DMA_Init(&dma_h);

  DMA_Cmd(&dma_h, ENABLE);
  TIM_DMA_Enable_Request(&tim2_h, ENABLE);
  TIM_Cmd(&tim2_h, ENABLE);

  while (1) {
  }
}
