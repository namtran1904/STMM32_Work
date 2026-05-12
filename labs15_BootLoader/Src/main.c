#include <stdint.h> // NOLINT

#define APP_START_ADDR 0x08008000

#define RCC_AHB1ENR (*(volatile uint32_t *)(0x40023830))
#define GPIOA_MODER (*(volatile uint32_t *)(0x40020000))
#define GPIOA_BSRR (*(volatile uint32_t *)(0x40020018))
// kiểu con trỏ hàm để trỏ vào Reset Handler của App
typedef void (*pFunction)(void);

void Delay(uint32_t count) {
  for (volatile uint32_t i = 0; i < count; i++) {
    __asm("nop");
  }
}

void Jump_to_Application(void) {
  uint32_t msp_value = *(volatile uint32_t *)(APP_START_ADDR);

  if ((msp_value & 0x2FF00000) != 0x20000000) {
    return;
  }

  uint32_t jump_addr = *(volatile uint32_t *)(APP_START_ADDR + 4);
  pFunction jump_to_app = (pFunction)jump_addr;

  __asm volatile("cpsid i");

  __asm volatile("MSR msp, %0" : : "r"(msp_value) :);

  jump_to_app();
}

int main(void) {
  RCC_AHB1ENR |= (1 << 0);
  GPIOA_MODER &= ~(3 << 10);
  GPIOA_MODER |= (1 << 10);

  for (int i = 0; i < 3; i++) {
    GPIOA_BSRR = (1 << 5);
    Delay(2000000);
    GPIOA_BSRR = (1 << 21);
    Delay(2000000);
  }

  Jump_to_Application();

  while (1) {
  }
}
