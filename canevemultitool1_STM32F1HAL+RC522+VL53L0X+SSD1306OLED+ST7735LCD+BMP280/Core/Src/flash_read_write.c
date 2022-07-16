#include "flash_read_write.h"
#define PAGE_START_ADDRESS 0x8000000
uint8_t flash_data_read[10];

int flashWriteData(int paramSizeKb, uint64_t data[], size_t len) {

HAL_FLASH_Unlock();//����flash

//׼��������ҳ,д?ǰ��Ҫ����

FLASH_EraseInitTypeDef f;

f.TypeErase = FLASH_TYPEERASE_PAGES;//ҳ����

f.PageAddress = PAGE_START_ADDRESS + (1024 * paramSizeKb);//����ҳ��ַ

f.NbPages = 1;//����1ҳ

uint32_t PageError = 0;

HAL_FLASHEx_Erase(&f, &PageError);//����PageError == 0xFFFFFFFF��?�ɹ�

if(PageError != 0xFFFFFFFF) {

return HAL_ERROR;

}
//��ʼд����

uint32_t startAddressIndex = PAGE_START_ADDRESS + (1024 * paramSizeKb);

//startAddressIndex += 8,д?λ�ã�ÿ��+8����Ϊ�洢��������uint64_t,ռ?64bit,8�ֽ�


for(int i = 0 ; i < len;i ++ , startAddressIndex += 8) {

//д?����

HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startAddressIndex, data[i]);

}
//��������

HAL_FLASH_Lock();

return HAL_OK;

}

//��ȡ����

//paramSizeKb ����??

//data ��?����

//len ��?���ݳ���

void flashReadData(int paramSizeKb, uint8_t *data, size_t len) {

uint32_t startAddressIndex = PAGE_START_ADDRESS + (1024 * paramSizeKb);

for(int i = 0; i < len; i++, startAddressIndex += 8){

data[i] = *(__IO uint32_t*)(startAddressIndex);

}

}