#ifndef MCU_TIMER_H // NOLINT
#define MCU_TIMER_H
#include "mcu_chip_map.h" // NOLINT

#define TIM_COUNTER_MODE_UP ((uint8_t)0x000)
#define TIM_COUNTER_MODE_DOWN ((uint8_t)0x001)
#define TIM_COUNTER_MODE_CENTER ((uint8_t)0x010)

#define ENABLE 1
#define DISABLE 0

typedef enum {
  TIM_OK = 0,
  TIM_ERROR,
  TIM_TIME_OUT,
  TIM_RESET, // Chưa Init  // NOLINT
  TIM_READY, // Init nhưng chưa chạy  // NOLINT
  TIM_BUSY   // Đang chạy
} TIM_Status_t;

typedef struct {
  uint32_t Period;
  uint32_t Prescaler;
  uint8_t Counter_Mode;
  uint32_t ARPRE; // Auto Reload Preload  // NOLINT
} TIM_Config_t;

typedef struct {
  TIM_RegDef_t *pTIMx;
  TIM_Config_t TIM_Config;
  TIM_Status_t status;
} TIM_Handle_t;

TIM_Status_t TIM_Peripheral_Clock_Control(TIM_Handle_t *pTIMHandle,
                                          uint8_t EnOrDis);

TIM_Status_t TIM_Init(TIM_Handle_t *pTIMHandle);

TIM_Status_t TIM_Cmd(TIM_Handle_t *pTIMHandle, uint8_t EnOrDis);

TIM_Status_t TIM_Interrupt_Control(TIM_Handle_t *pTIMx, uint8_t EnOrDis);

TIM_Status_t TIM_DMA_Enable_Request(TIM_Handle_t *pTIMHandle, uint8_t EnOrDis);

#endif // NOLINT
