#include "mcu_dma.h"      // NOLINT
#include "mcu_chip_map.h" // NOLINT
#include "mcu_rcc.h"      // NOLINT
#include <stdint.h>       // NOLINT

#define DMA_TIMEOUT_MAX 100000

DMA_Status_t DMA_Clock_Control(DMA_Handle_t *pDMAHandle, uint8_t EnOrDis) {
  if ((pDMAHandle == NULL) || (pDMAHandle->pDMAx == NULL)) {
    return DMA_ERROR;
  } else {
    if (EnOrDis == ENABLE) {
      RCC->AHB1ENR |= (1 << pDMAHandle->DMA_BitPos);
    } else {
      RCC->AHB1ENR &= ~(1 << pDMAHandle->DMA_BitPos);
    }
    return DMA_OK;
  }
}

static DMA_Stream_RegDef_t *DMA_GetStream(DMA_Handle_t *pDMAHandle) {
  return &(pDMAHandle->pDMAx->Stream[pDMAHandle->Config.Stream_idx]);
}

static DMA_Status_t DMA_WaitForFlag(DMA_Stream_RegDef_t *pStream,
                                    uint32_t FlagBit, uint8_t Status) {
  uint32_t timeout = DMA_TIMEOUT_MAX;
  while (((pStream->CR >> FlagBit) & 0x1) != Status) {
    if (--timeout == 0) {
      return DMA_TIME_OUT;
    }
  }
  return DMA_OK;
}

DMA_Status_t DMA_Init(DMA_Handle_t *pDMAHandle) {
  if ((pDMAHandle == NULL) || (pDMAHandle->pDMAx == NULL)) {
    return DMA_ERROR;
  } else {
    DMA_Stream_RegDef_t *pStream = DMA_GetStream(pDMAHandle);
    // Check bit EN
    if ((pStream->CR & (1 << 0)) == ENABLE) {
      pStream->CR &= ~(1 << 0);
      if (DMA_WaitForFlag(pStream, 0, RESET) == DMA_TIME_OUT) {
        return DMA_ERROR;
      }
      pDMAHandle->pDMAx->HIFCR = 0x3F3F3F3F;
      pDMAHandle->pDMAx->LIFCR = 0x3F3F3F3F;
    }

    // Set địa chỉ Peripheral
    pStream->PAR = (pDMAHandle->Config.Periph_BaseAddr << 0);

    // Set địa chỉ Mem
    pStream->M0AR = (pDMAHandle->Config.Mem_BaseAddr << 0);

    // Config độ dài data
    pStream->NDTR = (pDMAHandle->Config.Data_len << 0);

    // Cấu hình FIFO mode ở dạng direct
    pStream->FCR |= (1 << 2);

    // Cấu hình các thông số cơ bản
    uint32_t temp_cr = 0;
    temp_cr |= (pDMAHandle->Config.Direction << 6);
    temp_cr |= (pDMAHandle->Config.Mode << 8);
    temp_cr |= (pDMAHandle->Config.PInc << 9);
    temp_cr |= (pDMAHandle->Config.MInc << 10);
    temp_cr |= (pDMAHandle->Config.PSize << 11);
    temp_cr |= (pDMAHandle->Config.MSize << 13);
    temp_cr |= (pDMAHandle->Config.Priority << 16);
    temp_cr |= (pDMAHandle->Config.Channel_idx << 25);

    pStream->CR = temp_cr;

    return DMA_OK;
  }
}

DMA_Status_t DMA_Cmd(DMA_Handle_t *pDMAHandle, uint8_t EnOrDis) {
  if ((pDMAHandle == NULL) || (pDMAHandle->pDMAx == NULL)) {
    return DMA_ERROR;
  } else {
    DMA_Stream_RegDef_t *pStream = DMA_GetStream(pDMAHandle);

    if (EnOrDis == ENABLE) {
      pStream->CR |= (1 << 0);
      if (DMA_WaitForFlag(pStream, 0, SET) == DMA_TIME_OUT) {
        return DMA_ERROR;
      }
    } else {
      pStream->CR &= ~(1 << 0);
      if (DMA_WaitForFlag(pStream, 0, RESET) == DMA_TIME_OUT) {
        return DMA_ERROR;
      }
    }
    return DMA_OK;
  }
}
