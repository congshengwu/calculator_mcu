/**************************************************************************************
*		              LCD1602液晶显示实验												  *
实现现象：	具体接线操作请参考视频教程
			下载程序后插上LCD1602液晶在开发板上，即可显示
注意事项：																				  
***************************************************************************************/

#include <reg52.h>		//此文件中定义了单片机的一些特殊功能寄存器
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "calculate.h"

#define GPIO_KEY P1		//矩阵按键控制口

/*
sbit btn_left= P3^0;
sbit btn_right=P3^1;		 	
sbit btn_dot = P3^2;	//定义P3.2口是k3，"小数点"按钮
sbit btn_eq  = P3^3;	//定义P3.3口是k4，"等于"按钮
sbit btn_del = P3^4;	//定义P3.6口是k7，"删除"按钮
sbit btn_cls = P3^5 ;	//定义P3.7口是k8，"清空"按钮
*/
sbit btn_eq = P3^0;
sbit btn_dot = P3^1;
sbit btn_cls = P3^2;	//定义P3.2口是k3，"小数点"按钮
sbit btn_del = P3^3;	//定义P3.3口是k4，"等于"按钮
sbit btn_left = P3^4;	//定义P3.6口是k7，"删除"按钮
sbit btn_right = P3^5;	//定义P3.7口是k8，"清空"按钮

typedef unsigned int u16;	//对数据类型进行声明定义
typedef unsigned char u8;

u8 KeyValue;				//存矩阵按键的位置 0~15
u8 calc_arr[30] = { 0 };	//计算式字符串
u8 calc_len = 0;			//计算式长度,下标，为其赋值
u8 result_arr[16] = { 0 };	//结果字符串，长度用strlen函数得出

u8 code char_red[16] = {'7', '8', '9', '/', 
	 				  	'4', '5', '6', '*', 
						'1', '2', '3', '-', 
						'(', '0', ')', '+'  }; 

void delay(u16 i)
{
	while(i--);
}

void delay2s(void)   //误差 0us
{
    unsigned char a,b,c,n;
    for(c=167;c>0;c--)
        for(b=39;b>0;b--)
            for(a=152;a>0;a--);
    for(n=1;n>0;n--);
}

void moveTo_left()
{
	LcdWriteCom(0x18);//向左移
}
void moveTo_right()
{
	LcdWriteCom(0x1c);//向右移
}
	
void display_up(u8 calc_arr[])
{
	u8 i;
	LcdWriteCom(0x80);
	for(i = 0; i < strlen(calc_arr); i++)
	{
		LcdWriteData(calc_arr[i]);
	}			
}

void display_down(u8 result_arr[])
{
	u8 i;
	LcdWriteCom(0x80 + 0x40);
	for(i = 0; i < strlen(result_arr); i++)
	{
		LcdWriteData(result_arr[i]);
	}			
}

void OnRedKeyDown()
{
	char a = 0;
	u8 i = 0;
	GPIO_KEY = 0x0f;//高四位低电平，底四位高电平
	if(GPIO_KEY != 0x0f)
	{
	 	delay(1000);//延时10ms消抖
		if(GPIO_KEY != 0x0f)
		{
			//扫描列
		 	switch(GPIO_KEY)
			{
				case 0x07: 
					KeyValue = 0; 
					break;
				case 0x0b:
					KeyValue = 1;
					break;
				case 0x0d:
					KeyValue = 2;
					break;
				case 0x0e:
					KeyValue = 3;
					break;	 
			}
			//线转发，扫描行
			GPIO_KEY = 0xf0;
			switch(GPIO_KEY)
			{
				case 0x70: 
					KeyValue = KeyValue; 
					break;
				case 0xb0:
					KeyValue = KeyValue + 4;
					break;
				case 0xd0:
					KeyValue = KeyValue + 8;
					break;
				case 0xe0:
					KeyValue = KeyValue + 12;
					break;	 
			}
			calc_arr[calc_len] = char_red[KeyValue];
			calc_len++;
			
			if(strlen(calc_arr) > 16)//超出显示屏长度，公式左移
				moveTo_left();	

			display_up(calc_arr);

			//长时间按,强制退出，等于0xf0说明没有按键按下
			while((a < 50) && (GPIO_KEY != 0xf0))//a<50并且按键按下
			{
			 	delay(1000);
				a++;
			}
		}			
	}
}

void OnBlackKeyDown()
{
	double result;
	u8 i = 0;
	if(btn_eq == 0)		  //检测按键K1是否按下
	{	
		delay(1000);   //消除抖动 一般大约10ms
		if(btn_eq == 0)	 //再次判断按键是否按下
		{
			//执行代码
			result = calculate(calc_arr);
			sprintf(result_arr, result == (int)result ? "%.0f" : "%.6f", result);				
			display_down(result_arr);
		}
		while(!btn_eq);	 //检测按键是否松开
	}
	else if(btn_dot == 0)
	{
		delay(1000);   //消除抖动 一般大约10ms
		if(btn_dot == 0)	 //再次判断按键是否按下
		{
			//执行代码
			calc_len++;
			calc_arr[calc_len - 1] = '.';
			display_up(calc_arr);
		}
		while(!btn_dot);	 //检测按键是否松开
	}
	else if(btn_del == 0)
	{
	 	delay(1000);   //消除抖动 一般大约10ms
		if(btn_del == 0)	 //再次判断按键是否按下
		{
			//执行代码
			calc_arr[calc_len - 1] = ' ';
			display_up(calc_arr);
			calc_arr[calc_len - 1] = '\0';
			calc_len--;
		}
		while(!btn_del);	 //检测按键是否松开
	}
	else if(btn_cls == 0)
	{
		delay(1000);   //消除抖动 一般大约10ms
		if(btn_cls == 0)	 //再次判断按键是否按下
		{
			
			//清空显示
			LcdWriteCom(0x01);//清屏,同时好像也归位了
			//清空式子和结果数组
			memset(calc_arr, 0, sizeof(calc_arr));
			memset(result_arr, 0, sizeof(result_arr));
			calc_len = 0;
		}
		while(!btn_cls);	 //检测按键是否松开
	}
	else if(btn_left == 0)
	{
		delay(1000);   //消除抖动 一般大约10ms		
		if(btn_left == 0)//长度超过16方可移动
		{
			moveTo_right();
		}
		while(!btn_left);
	}
	else if(btn_right == 0)
	{
		delay(1000);   //消除抖动 一般大约10ms		
		if(btn_right == 0)
		{
			moveTo_left();
		}
		while(!btn_right);
	}
}


void main(void)
{
	char cc[16] = " CC Calculator  ";
	char ccc[16]= "    Welcome     ";
	LcdInit();

	display_up(cc);
	display_down(ccc);
	delay2s();	
	LcdWriteCom(0x01);//清空显示
	
	while(1)
	{		
		OnRedKeyDown();
		OnBlackKeyDown();
	}				
}