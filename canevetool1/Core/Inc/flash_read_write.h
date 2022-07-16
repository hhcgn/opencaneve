
#ifndef  _FLASH_READ_WRITE_H
#define  _FLASH_READ_WRITE_H
#include "stm32f1xx_hal.h"

extern uint8_t flash_data_read[10];
int flashWriteData(int paramSizeKb, uint64_t data[], size_t len);
void flashReadData(int paramSizeKb, uint8_t *data, size_t len);
#endif