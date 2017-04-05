#ifndef __FLASH_H_
#define __FLASH_H_

unsigned char Flash_Read(uint32_t addr);
unsigned int Flash_Read_Int(uint32_t addr);
void Flash_Erase(uint32_t page_number);
void Flash_Write(uint32_t addr,unsigned char dat);
 
#endif  
                                                                                                                                                                                                                                       