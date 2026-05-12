#ifndef MCU_DMA_H // NOLINT
#define MCU_DMA_H

#include "mcu_chip_map.h" // NOLINT

#define ENABLE 1
#define DISABLE 0

#define DMA_DIR_PERIPH_TO_MEM 0
#define DMA_DIR_MEM_TO_PERIPH 1
#define DMA_DIR_MEM_TO_MEM 2

#define DMA_CIRCULAR_ENABLE 1
#define DMA_CIRCULAR_DISABLE 0

#define DMA_MEM_INCREASE 1
#define DMA_PERIPH_INCREASE 1

#define DMA_MSIZE_8BITS 0
#define DMA_MSIZE_16BITS 1
#define DMA_MSIZE_32BITS 2

#define DMA_PSIZE_8BITS 0
#define DMA_PSIZE_16BITS 1
#define DMA_PSIZE_32_BITS 2

#define PL_LOW 0
#define PL_MED 1
#define PL_HIGH 2
#define PL_VH 3

typedef enum {
  DMA_OK = 0,
  DMA_ERROR,
  DMA_BUSY,
  DMA_READY,
  DMA_DISABLE = 0,
  DMA_TIME_OUT
} DMA_Status_t;

typedef struct {
  uint8_t Stream_idx;
  uint8_t Channel_idx;
  uint8_t Direction;
  uint8_t Mode;
  uint8_t MSize;
  uint8_t PSize;
  uint32_t Mem_BaseAddr;
  uint32_t Periph_BaseAddr;
  uint8_t Priority;
  uint8_t MInc;
  uint8_t PInc;
  uint32_t Data_len;
} DMA_Config_t;

typedef struct {
  DMA_Config_t Config;
  DMA_Status_t Status;
  uint8_t DMA_BitPos;
  DMA_RegDef_t *pDMAx;
} DMA_Handle_t;

DMA_Status_t DMA_Clock_Control(DMA_Handle_t *pDMAHandle, uint8_t EnOrDis);
DMA_Status_t DMA_Init(DMA_Handle_t *pDMAHandle);
DMA_Status_t DMA_Cmd(DMA_Handle_t *pDMAHandle, uint8_t EnOrDis);
#endif // NOLINT
