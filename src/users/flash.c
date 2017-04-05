
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
��������Flash��
���룺Ҫ���ĵ�ַ
�������
�������ܣ���Flashָ���ĵ�ַ������Ӧ�����ݣ����ŵ���Ӧ������
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
��������Flashд
���룺Ҫд��ĵ�ַ������
�������
�������ܣ���Flashָ���ĵ�ַд����Ӧ������
 ****************************************************************/

void Flash_Write(uint32_t addr,uint8_t dat)
{	     
    nvm_write_char(INT_FLASH, addr, dat);
}

/****************************************************************
��������Flash����
���룺Ҫ�����ĵ�ַ
�������
�������ܣ�����Flash�ƶ�λ��
****************************************************************/	
void Flash_Erase(uint32_t addr)
{
	nvm_page_erase(INT_FLASH,(addr/FLASH_PAGE_SIZE));
}	