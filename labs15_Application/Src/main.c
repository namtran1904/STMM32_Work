#include <stdint.h> // NOLINT

#define SCB_VTOR (*(volatile uint32_t *)(0xE000ED08))

#define RCC_CR (*(volatile uint32_t *)(0x40023800))
#define RCC_PLLCFGR (*(volatile uint32_t *)(0x40023804))
#define RCC_CFGR (*(volatile uint32_t *)(0x40023808))
#define FLASH_ACR (*(volatile uint32_t *)(0x40023C00))
#define RCC_AHB1ENR (*(volatile uint32_t *)(0x40023830))

#define GPIOA_MODER (*(volatile uint32_t *)(0x40020000))
#define GPIOA_BSRR (*(volatile uint32_t *)(0x40020018))
void Clock_Config(void) {
  // Bật HSE
  RCC_CR |= (1 << 16);
  while (!(RCC_CR & (1 << 17))) {
  }
  // Bật Flash Latency
  FLASH_ACR &= ~(7 << 0);
  FLASH_ACR |= (3 << 0);

  // Cấu hình AHB, APB1, APB2
  RCC_CFGR &= ~((15 << 4) | (7 << 10) | (7 << 13));
  RCC_CFGR |= ((0X00 << 4) | (4 << 10) | (0X00 << 13));

  // Thiết lập tham số M, N, P
  RCC_PLLCFGR &= ~((0x3F << 0) | (0x1FF << 6) | (0x3 << 16) | (1 << 22));
  RCC_PLLCFGR |= ((4 << 0) | (200 << 6) | (1 << 16) | (1 << 22));

  // Bật PLL VÀ check bit
  RCC_CR |= (1 << 24);
  while (!(RCC_CR & (1 << 25))) {
  }

  // Chuyển thành clock hệ thống
  RCC_CFGR |= (2 << 0);
  while ((RCC_CFGR & (3 << 2)) != (2 << 2)) {
  }
}

void Delay(uint32_t count) {
  for (volatile uint32_t i = 0; i < count - 1; i++) {
    __asm("nop");
  }
}

int main(void) {
  SCB_VTOR = 0x08008000;

  // Clock_Config();

  RCC_AHB1ENR |= (1 << 0);

  GPIOA_MODER &= ~(3 << 10);
  GPIOA_MODER |= (1 << 10);

  while (1) {
    GPIOA_BSRR = (1 << 5);
    Delay(500000);
    GPIOA_BSRR = (1 << 21);
    Delay(500000);
  }
}
