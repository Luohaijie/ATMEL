
#include <asf.h>
#include "flash.h"
#include "function.h"


unsigned char Flash_Read(uint32_t addr)
{	        
	uint8_t aTemp;	
	nvm_read_char(INT_FLASH, addr, &aTemp);
	return aTemp;		//Read data form code dat
}

/****************************************************************
函数名：Flash读
输入：要读的地址
输出：无
函数功能：从Flash指定的地址读出相应的数据，并放到相应变量中
*****************************************************************/

unsigned int Flash_Read_Int(uint32_t addr)
{		
    uint8_t aTemp_H,aTemp_L; 
	uint16_t aTemp;	
	aTemp_H = Flash_Read(addr);
	aTemp_L = Flash_Read(addr+1);
	aTemp =  aTemp_H*256+aTemp_L;
	return aTemp;		//Read data form code dat
}
/****************************************************************
函数名：Flash写
输入：要写入的地址和数据
输出：无
函数功能：给Flash指定的地址写入相应的数据
 ****************************************************************/

void Flash_Write(uint32_t addr,uint8_t dat)
{	     
    nvm_write_char(INT_FLASH, addr, dat);
}

/****************************************************************
函数名：Flash擦出
输入：要擦除的地址
输出：无
函数功能：擦除Flash制定位置
****************************************************************/	
void Flash_Erase(uint32_t addr)
{
	nvm_page_erase(INT_FLASH,(addr/FLASH_PAGE_SIZE));
}	