
#include "uart.h"

#define Buf_Max 50
uint8 xdata Rec_Buf[Buf_Max];     //接收串口2缓存数组
uint8 i = 0;   

/**************************************
功能描述：串口初始化(串口1和串口4)
入口参数：无
返回值：无
备注：晶振选择18.432MHz   串口1波特率9600bps   串口4波特率115200bps   
***************************************/
void UartInit(void)		
{
	PCON &= 0x3f;		//波特率不倍速，串行口工作方式由SM0、SM1决定	
	SCON = 0x50;		//8位数据，可变波特率
	AUXR |= 0x40;		//定时器1时钟1为FOSC,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0x20;		  //设定定时初值
	TH1 = 0xFE;		  //设定定时初值
	ET1 = 0;		    //禁止定时器1中断
	TR1 = 1;		    //启动定时器1

	S4CON = 0x10;		//启动串行接收器	
	S4CON |= 0x40;		
	T4T3M |= 0x20;
	T4L = 0xD8;		  //设定初始值
	T4H = 0xFF;		  //设定初始值
	T4T3M |= 0x80;		//启动定时器4
}

/**************************************
功能描述：串口1发送数据
入口参数：uint8 ch
返回值：无
***************************************/
void U1SendData(uint8 ch)
{
    SBUF = ch;                  //写数据到UART数据寄存器
		while(TI == 0);             //在停止位没有发送时，TI为0即一直等待
		TI = 0;                     //清除TI位（该位必须软件清零）
}

/**************************************
功能描述：串口1发送字符串
入口参数：uint8 *s
返回值：无
***************************************/
void U1SendString(uint8 *s)
{
    while (*s)                    //检测字符串结束标志
    {
        U1SendData(*s++);         //发送当前字符
    }
}

/**************************************
功能描述：串口4发送数据
入口参数：uint8 ch
返回值：无
***************************************/
void U4SendData(uint8 ch)
{
    S4BUF = ch;                 //写数据到UART数据寄存器
		while(!(S4CON&S4TI));       //在停止位没有发送时，S4TI为0即一直等待
    S4CON&=~S4TI;               //清除S4CON寄存器对应S4TI位（该位必须软件清零）
}

/**************************************
功能描述：串口4发送字符串
入口参数：uint8 *s
返回值：无
***************************************/
void U4SendString(uint8 *s)
{
     IE2 &= 0xEF; 			         // 串口4中断关闭
	   while (*s)                  //检测字符串结束标志
     {
        U4SendData(*s++);        //发送当前字符
     }
     IE2 |= 0x10;                // 串口4中断打开		
}

/**************************************
功能描述：握手成功与否函数
入口参数：uint8 *a
返回值：位
***************************************/
bit Hand(uint8 *a)
{ 
    if(strstr(Rec_Buf,a)!=NULL)     //判断字符串a是否是字符串Rec_Buf的子串
	    return 1;                     //如果字符串a是字符串Rec_Buf的子串
	else
		  return 0;                     //如果字符串a不是字符串Rec_Buf的子串
}

/**************************************
功能描述：清除缓存内容函数
入口参数：无
返回值：无
***************************************/
void CLR_Buf(void)
{
	uint8 k;
    for(k=0;k<Buf_Max;k++)        //将串口2缓存数组的值都清为零  
	    {
			   Rec_Buf[k] = 0;
		  }
    i = 0;                    
}

/***************************************************************************
 * 描  述 : 串口1中断服务函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************/
void Uart1() interrupt 4 using 1
{
	ES = 0;                       // 串口1中断关闭
	if (RI)                       //串行接收到停止位的中间时刻时，该位置1
  {
      RI = 0;                   //清除RI位 （该位必须软件清零）          
   }
   if (TI)                      //在停止位开始发送时，该位置1
   {
      TI = 0;                   //清除TI位（该位必须软件清零）
   }
	 ES =  1;                     // 串口1中断打开
}

/***************************************************************************
 * 描  述 : 串口4中断服务函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************/
void Uart4() interrupt 18 using 1
{
	IE2 &= 0xEF; 			                         // 串口4中断关闭
	if (S4CON & S4RI)                          //串行接收到停止位的中间时刻时，该位置1
  {
      S4CON &= ~S4RI;                        //清除S4CON寄存器对应S4RI位（该位必须软件清零）
			Rec_Buf[i] = S4BUF;                    //把串口4缓存SBUF寄存器数据依次存放到数组Rec_Buf中
			i++;                                   
	    if(i>Buf_Max)                          //接收数大于定义接收数组最大个数时，覆盖接收数组之前值
			{
					i = 0;
			}           
   }
      if (S4CON & S4TI)                       //在停止位开始发送时，该位置1
      {
        S4CON &= ~S4TI;   			              //清除S4CON寄存器对应S4TI位（该位必须软件清零）
      }
	 IE2 |= 0x10;                               // 串口4中断打开	
}


