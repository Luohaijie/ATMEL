#ifndef __FUNCTION_H
#define __FUNCTION_H

					  
#define FLAG_ADDR		0x4000      //储存工作模式地址
#define WORK_STAR_BASE	0x5000      //压控曲线首地址
#define	TMP_AD_H	    1280			//高温（100度）对应的AD采样值
#define	TMP_AD_L	    3328		//底温（-60度）对应的AD采样值
#define TMP_AD_40L      3300        //低温（-40度）对应的AD采样值
#define TMP_AD_85H      1500         //高温（85度）对应的AD采样值
#define	VCP_AD_H	    4096
#define	VCP_AD_L	    0
#define TMP_ADD_LEN	    4096
#define SN_ADDR			0x4400
#define PR_ADDR			0x4800
#define VR_ADDR			0x4C00
#define PWM_AD_L        0x1800
#define Correct_Flag    0x7000
#define Correct_Addr    0x7000

/********************数组长度******************/
#define RX_BUFF_LEN	    262
#define TX_BUFF_LEN	    262

/********************命令码********************/
#define READ_PR         6
#define READ_SN         7
#define READ_VR         8
#define WRITE_SN        16
#define ERASE_FLASH     17
#define WRITE_MODE      18
#define READ_CP		    19
#define WRITE_FLASH     20
#define READ_FLASH      21
#define READ_MODE   	25


typedef struct package{
    unsigned char Head;
    unsigned char Commond;
    unsigned char Error;
    unsigned char Frame;
    unsigned char Null;
    unsigned char Data[256];
    unsigned char Tail;
}Package;

extern Package ReceiveData;


/*
signed int vcp_sum;
unsigned int tmp_sum;
int vpp_f;
unsigned int flag;
signed int diff,diff_last;
signed int correct_x,correct_y;
unsigned int correct_f1,correct_f2,correct_f3;
unsigned int tmp_last=0;
unsigned int  tmp_sumChange=0;

unsigned int  tmp_sum_last[4]={0xffff,0xffff,0xffff,0xffff};
char ntab[17] = "0123456789ABCDEF";
unsigned int  vpull_sum;
unsigned int  vpull_sum1[2];
unsigned int  vcp[10];
unsigned int  tmp[10];
//unsigned int  vpull1[66];
signed int  vpp;
signed int 	  vpp_PWM;
signed int  vpp_Last=0;
char TEM_Change=1;
char TEM_CNT=0;
signed int  vpp1;
signed int  VPP2_Last;
signed int  vpp2;
signed int  vpp4;

signed int  vpull;
signed int  vpp5;
unsigned int  vcp_flash;
//unsigned int  tmp_flash;

//unsigned char Rx_buff[RX_BUFF_LEN];

unsigned int PWM_UP,PWM_LOW;
unsigned int PWM_UP_H,PWM_UP_L,PWM_LOW_H,PWM_LOW_L;
unsigned int PWM_flag ;		//高低电平标志位
//unsigned int CALC_PWM;
//unsigned char byte;

*/
extern uint8_t lock_b;
extern float  K;
extern int16_t Badd;
extern uint8_t first_time;//是否第一次运行
extern uint8_t first_correct;//是否第一次运行
extern uint8_t flag_uart;
extern uint16_t flag_mode;
extern uint16_t rx_buff_cnt;
extern uint16_t VPP2_Last;

extern void work_mode(void);
extern void vpull_mode(void);
extern void vpull_mode2(void);
extern void uart_op(void);
extern void write_info(void);
extern void erase_flash(void);



#endif

