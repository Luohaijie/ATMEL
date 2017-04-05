#include <asf.h>
#include "function.h" 
#include "user_adc.h"
#include "cacul.h"
#include "flash.h"
#include "user_uart.h"
#include "config.h"
#include "string.h"

uint8_t lock_b;
float  K;
int16_t Badd;
uint8_t first_time;//是否第一次运行
uint8_t first_correct;//是否第一次运行
uint8_t flag_uart;
uint16_t flag_mode;
uint16_t rx_buff_cnt;
uint16_t VPP2_Last;
Package ReceiveData;

static void ReadCp(void);
static void WriteFlash(void);
static void ReadFlash(void);
static void ReadMode(void);
static void ReadPr(void);
static void ReadSn(void);
static void WriteSn(void);
static void ReadVr(void);
static void write_flag(void);


void work_mode(void)
{
#if CORRECT_MODE
	signed int vpp2;
#endif 	
	uint8_t i;
	uint16_t tmp[10];
//	static unsigned int tmp_sum_last[5]={0xffff,0xffff,0xffff,0xffff,0xffff};
	uint16_t tmp_sum;
	int16_t vpp1=0,vpp_PWM=0;

//	uint8_t table[5];	 
//	char ntab[17] = "0123456789ABCDEF";

	for(i=0;i<10;i++)
	{
		if(flag_uart == 1)
		{
				return;
		}

		tmp[i]=Temp_ADC();

	}

//	Insert_Sort (tmp,10);
	tmp_sum=(unsigned int)((tmp[4]+tmp[5]+tmp[6]+tmp[7])>>2);
/*
	if(tmp_sum_last[0]!=0xffff)
	{
		tmp_sum=(tmp_sum+tmp_sum_last[0]+tmp_sum_last[1]+tmp_sum_last[2]+tmp_sum_last[3])/5;
	}
	if((tmp_sum<TMP_AD_40L )&&(tmp_sum>TMP_AD_85H))
	{
		tmp_sum_last[0]=tmp_sum_last[1];
		tmp_sum_last[1]=tmp_sum_last[2];
		tmp_sum_last[2]=tmp_sum_last[3];
		tmp_sum_last[3]=tmp_sum;
	}
	else
	{
		tmp_sum_last[0]=0xffff;
	}	
*/
	if((tmp_sum<TMP_AD_L)&&(tmp_sum>TMP_AD_H ))
	{
		vpp1=Flash_Read_Int(WORK_STAR_BASE+(tmp_sum-TMP_AD_H)*2);	
	
#if CORRECT_MODE
		vpp2=Flash_Read_Int(Correct_Addr+(tmp_sum-TMP_AD_H)*2);
		if(((vpp2-VPP2_Last)>5)||((vpp2-VPP2_Last)<-5))
		{
			vpp2=VPP2_Last;
		}
#if CORRECT_DEBUG		
	UART_printf("  Vpp2: ");
	printn(vpp2,10);
#endif
		vpp1=vpp1+vpp2-1024;
		VPP2_Last=vpp2;
#endif			
	 vpp_PWM=(int)(K*vpp1)+Badd;	
	 
	}

/*  
	UART_printf("  Temprature: ");
	printn(tmp_sum,10);

	UART_printf("  Vpp: ");
	printn(vpp1,10);

	UART_printf("  vpp_PWM: ");
	printn(vpp_PWM,10);
 	UART_printf("  "); 
*/
	if(vpp_PWM<2)vpp_PWM=2;
	else if(vpp_PWM>4095)vpp_PWM=4095;
	DAC_Output(vpp_PWM);	
//	delay_ms(200);
 /*	  
	vpp1 = Vcp_Collect();
	UART_printf("  vcp: ");
	printn(vpp1,10);
 	UART_printf("  \n");
*/	
	

	
/*
	 	table[0]=ntab[tmp_sum/1000];
		table[1]=ntab[tmp_sum%1000/100];
		table[2]=ntab[tmp_sum%100/10];
		table[3]=ntab[tmp_sum%10];
		UART_printf("temperature= ");
		Uart_Send(table,4);

		table[0]=ntab[vpp/1000];
		table[1]=ntab[vpp%1000/100];
		table[2]=ntab[vpp%100/10];
		table[3]=ntab[vpp%10];
		UART_printf(" ,  Flash= ");
		Uart_Send(table,4);
		
	    for(i=0;i<10;i++)
		{
			vcp[i]=Vcp_Collect();
			delay_ms(10);
		}
		Insert_Sort(vcp,10);
#if TEN_BIT
		vcp_sum=(vcp[3]+vcp[4]+vcp[5]+vcp[6])/8;
#else
        vcp_sum=(vcp[3]+vcp[4]+vcp[5]+vcp[6])/2;
#endif
		table[0]=ntab[vcp_sum/1000];
		table[1]=ntab[vcp_sum%1000/100];
		table[2]=ntab[vcp_sum%100/10];
		table[3]=ntab[vcp_sum%10];
		UART_printf(" ,  VCP= ");
		Uart_Send(table,4);
		UART_printf("\n");
*/
}

/***************************************
输入：无
输出：采集后的10位温度
功能：采集来自晶体的温度对应的电压值,得到数字量
****************************************/
void vpull_mode2(void) 
{
#if CORRECT_MODE
	signed int  vpp2;
#endif
	uint8_t i;
    unsigned int vpull1[66];
	uint16_t tmp[10];
	static unsigned int tmp_sum_last[5]={0xffff,0xffff,0xffff,0xffff,0xffff};
	int16_t tmp_sum;
	long vpull_sum;
	signed int  vpp1=0,vpp_PWM,vpull;

//	uint8_t table[5];	 
//	char ntab[17] = "0123456789ABCDEF";

	for(i=0;i<8;i++)
	{
		if(flag_uart == 1)
		{
			return;
		}
		tmp[i]=Temp_ADC();
	}

	Insert_Sort (tmp,8);
	tmp_sum=((tmp[2]+tmp[3]+tmp[4]+tmp[5])>>2);
	if(tmp_sum>4095)
	{
		tmp_sum=4095;
	}
	if(tmp_sum<=0)
	{
		tmp_sum=0;
	}


  
	for(i=0;i<32;i++)
	{
		if(flag_uart == 1)
		{
			return;
		}

		vpull1[i]=Vpull_Collect();

	}
	Insert_Sort (vpull1,32); 
	vpull_sum=0;	
	for(i=16;i<20;i++)//首尾两个数据不舍弃，否则拉偏反应慢
	{ 
	    vpull_sum=vpull_sum+vpull1[i];
	}
	vpull_sum = vpull_sum/4; 	
	if(vpull_sum>4095)
	{
		vpull_sum=4095;
	}

	vpull=(signed int)vpull_sum-2048;

	if(tmp_sum_last[0]!=0xffff)
	{
		tmp_sum=(tmp_sum+tmp_sum_last[0]+tmp_sum_last[1]+tmp_sum_last[2]+tmp_sum_last[3])/5;
	}
	// 平滑温度补偿值
	// 极度温度下不平滑温度
	if((tmp_sum<TMP_AD_40L )&&(tmp_sum>TMP_AD_85H))
	{
		tmp_sum_last[0]=tmp_sum_last[1];
		tmp_sum_last[1]=tmp_sum_last[2];
		tmp_sum_last[2]=tmp_sum_last[3];
		tmp_sum_last[3]=tmp_sum;
	}
	else
	{
		tmp_sum_last[0]=0xffff;
	}

	
    if((tmp_sum<TMP_AD_L)&&(tmp_sum>TMP_AD_H ))
    {
	    vpp1 = Flash_Read_Int(WORK_STAR_BASE+(tmp_sum-TMP_AD_H)*2);
#if CORRECT_MODE
		vpp2=Flash_Read_Int(Correct_Addr+(tmp_sum-TMP_AD_H)*2);
		if(((vpp2-VPP2_Last)>5)||((vpp2-VPP2_Last)<-5))
		{
			vpp2=VPP2_Last;
		}
		
		vpp1 = vpp1+vpp2-1024;
		VPP2_Last=vpp2;
#endif
	}  
	vpp_PWM=vpp1+vpull;
	
	vpp_PWM=(int)(K*vpp_PWM)+Badd;

	if(vpp_PWM<2)vpp_PWM=2;
	else if(vpp_PWM>4095)vpp_PWM=4095;
	DAC_Output(vpp_PWM);
	delay_ms(500); 

}
/*****************读CP电压*************************/
static void ReadCp(void)
{
	int16_t tmp_sum,vcp_sum;
    lock_b = 1;   

    tmp_sum=Temp_Collect();	
	vcp_sum=Vcp_Collect();     
    ReceiveData.Data[0] = tmp_sum>>8;
    ReceiveData.Data[1] = tmp_sum;
    ReceiveData.Data[2] = vcp_sum>>8;
    ReceiveData.Data[3] = vcp_sum;    
}
/*****************写FLASH压控曲线*******************/
static void WriteFlash(void)
{   	   
#if ATUO_FALLFLASH
	 for(uint16_t i = 0 ; i < 128 ; i++)
    {		        
        ReceiveData.Data[i*2]=0x08;
		ReceiveData.Data[i*2+1]=0x98;	
    }
	for(uint16_t i = 0 ; i < 16 ; i++)
	{
		nvm_write(INT_FLASH, WORK_STAR_BASE +(i*256), ReceiveData.Data, 256);
	}
	
	Flash_Erase(FLAG_ADDR);
	delay_ms(10);  		
	Flash_Write(FLAG_ADDR,0x00);
	flag_mode=0;
	dac_enable(&DACA);
#else
	nvm_write(INT_FLASH, WORK_STAR_BASE +((ReceiveData.Frame)*256), ReceiveData.Data, 256);
#endif
}
/*****************读压控曲线*******************/
static void ReadFlash(void)
{      
	nvm_read(INT_FLASH, WORK_STAR_BASE+((ReceiveData.Frame)*256), ReceiveData.Data, 256);
       
    ReceiveData.Error = 0;
    Uart_Send((unsigned char *)(&ReceiveData),RX_BUFF_LEN);	         
}		
/********读模式--0工作模式，非0锁相模式********/
static void ReadMode(void)
{    
    ReceiveData.Data[1] = 0xff;
	ReceiveData.Data[2] = Flash_Read(FLAG_ADDR);      	
}
/*****************读产品     *******************/
static void ReadPr(void)
{   
	uint8_t i; 
    for(i = 0 ; i < 7; i++ )
    {
        ReceiveData.Data[i] = 	Flash_Read(PR_ADDR+i);
    }
}
/*****************读SN     *******************/
static void ReadSn(void)
{    
	uint8_t i;
    for(i=0;i<12;i++)
    {
        ReceiveData.Data[i]= Flash_Read(SN_ADDR+i);         
    }
}		
/*****************写SN*******************/
static void WriteSn(void)
{
	uint8_t i;
    Flash_Erase(SN_ADDR);
    for(i = 0 ; i < 12 ; i++)
    {         
        Flash_Write(SN_ADDR + i,ReceiveData.Data[i]);        
    }
}
/*****************读软件版本     *******************/
static void ReadVr(void)
{
	uint8_t i;
    for(i = 0 ; i < 7; i++ )
    {
        ReceiveData.Data[i] = Flash_Read(VR_ADDR+i);
    }
}
/***************************************
输入：无
输出：无
功能：解析串口读出的数据并返回数据
****************************************/
void uart_op(void)
{
	uint8_t tx_flag;	
	if(flag_uart==1)
	{
        switch (ReceiveData.Commond){
            case READ_CP:
                ReadCp();
                tx_flag=1;
                break;
            case WRITE_FLASH:
                WriteFlash();
                tx_flag=1;
                break;
            case READ_FLASH:
                ReadFlash();
				tx_flag=0;
                break;
            case ERASE_FLASH:
                erase_flash();
                tx_flag=1;
                break;
            case WRITE_MODE:
                write_flag();
                tx_flag=1;
                break;
            case READ_MODE:
                ReadMode();
                tx_flag=1;
                break;
            case READ_PR:
                ReadPr();
                tx_flag=1;
                break;
            case READ_VR:
                ReadVr();
                tx_flag=1;
                break;
            case READ_SN:
                ReadSn();
                tx_flag=1;                
                break;
            case WRITE_SN:
                WriteSn();
                tx_flag=1;
                break;
            default :
                tx_flag=2;
                break;   
        }
/*****************发送数据*******************/
        if(tx_flag==1)
        {
            tx_flag=0;
			ReceiveData.Error = 0;
        	Uart_Send((unsigned char *)(&ReceiveData),RX_BUFF_LEN);

        }
        else if(tx_flag==2)
        {
            tx_flag=0;
            ReceiveData.Head = 0x7E;
            ReceiveData.Tail = 0xAA;
            ReceiveData.Error = 1;
            Uart_Send((unsigned char *)(&ReceiveData),RX_BUFF_LEN);
        }
         
			
	}
	flag_uart=0;
	memset((unsigned char *)(&ReceiveData),0x00,RX_BUFF_LEN);
}


/***************************************
输入：无
输出：无
功能：擦除拟合数据区间的数据
擦除16行的数据4*1024=4096个地址。
****************************************/
 void  erase_flash(void)
{
		uint8_t i;		
		for(i=0;i<64;i++)
		{			
			Flash_Erase(WORK_STAR_BASE+i*FLASH_PAGE_SIZE);
			delay_ms(1);				
		}

		Flash_Erase(FLAG_ADDR);
		delay_ms(1);		
		ReceiveData.Error = 0;    //成功标志
		
}
/***************************************
输入：无
输出：无
功能：通过擦写FLASH设置工作模式
****************************************/
static void write_flag(void)
{
	if(ReceiveData.Data[2]==0x00)  //工作模式
	{
		Flash_Erase(FLAG_ADDR);
		delay_ms(10);  		
		Flash_Write(FLAG_ADDR,0x00);
		ReceiveData.Error=0;
		flag_mode=0;
		dac_enable(&DACA);
	}
	else if(ReceiveData.Data[2]==0xFF)  //锁相模式
	{
		Flash_Erase(FLAG_ADDR);
		delay_ms(10);
		Flash_Write(FLAG_ADDR,0xFF);		
		ReceiveData.Error=0;
		flag_mode=0xff;
	}
	else if (ReceiveData.Data[2]==0x01)  //拉偏模式
	{
		Flash_Erase(FLAG_ADDR);
		delay_ms(10);
		Flash_Write(FLAG_ADDR,0x01); 				
		ReceiveData.Error=0;
		flag_mode=1;
		dac_enable(&DACA);
	}
	else
	{
		ReceiveData.Error=1;
	}
}
void write_info(void)
{	
	uint8_t i; 
	uint8_t PR_SN[7] = "CS1521X";
	uint8_t VR_SN[7] = "Z.00.05";	
	

    Flash_Erase(PR_ADDR);
	Flash_Erase(VR_ADDR);
	for(i=0;i<7;i++)
	{
		Flash_Write(PR_ADDR+i,PR_SN[i]);
		Flash_Write(VR_ADDR+i,VR_SN[i]);			
	}

}

