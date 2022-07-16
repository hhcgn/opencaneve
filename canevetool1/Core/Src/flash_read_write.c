#include "flash_read_write.h"
#define PAGE_START_ADDRESS 0x8000000
uint8_t flash_data_read[10];

int flashWriteData(int paramSizeKb, uint64_t data[], size_t len) {

HAL_FLASH_Unlock();//解锁flash

//准备擦除整页,写?前需要擦除

FLASH_EraseInitTypeDef f;

f.TypeErase = FLASH_TYPEERASE_PAGES;//页擦除

f.PageAddress = PAGE_START_ADDRESS + (1024 * paramSizeKb);//擦除页地址

f.NbPages = 1;//擦除1页

uint32_t PageError = 0;

HAL_FLASHEx_Erase(&f, &PageError);//擦除PageError == 0xFFFFFFFF表?成功

if(PageError != 0xFFFFFFFF) {

return HAL_ERROR;

}
//开始写数据

uint32_t startAddressIndex = PAGE_START_ADDRESS + (1024 * paramSizeKb);

//startAddressIndex += 8,写?位置，每次+8是因为存储的类型是uint64_t,占?64bit,8字节


for(int i = 0 ; i < len;i ++ , startAddressIndex += 8) {

//写?数据

HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startAddressIndex, data[i]);

}
//重新上锁

HAL_FLASH_Lock();

return HAL_OK;

}

//读取数据

//paramSizeKb 程序??

//data 读?数据

//len 读?数据长度

void flashReadData(int paramSizeKb, uint8_t *data, size_t len) {

uint32_t startAddressIndex = PAGE_START_ADDRESS + (1024 * paramSizeKb);

for(int i = 0; i < len; i++, startAddressIndex += 8){

data[i] = *(__IO uint32_t*)(startAddressIndex);

}

}