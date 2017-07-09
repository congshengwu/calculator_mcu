/**************************************************************************************
*		              LCD1602Һ����ʾʵ��												  *
ʵ������	������߲�����ο���Ƶ�̳�
			���س�������LCD1602Һ���ڿ������ϣ�������ʾ
ע�����																				  
***************************************************************************************/

#include <reg52.h>		//���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "calculate.h"

#define GPIO_KEY P1		//���󰴼����ƿ�

/*
sbit btn_left= P3^0;
sbit btn_right=P3^1;		 	
sbit btn_dot = P3^2;	//����P3.2����k3��"С����"��ť
sbit btn_eq  = P3^3;	//����P3.3����k4��"����"��ť
sbit btn_del = P3^4;	//����P3.6����k7��"ɾ��"��ť
sbit btn_cls = P3^5 ;	//����P3.7����k8��"���"��ť
*/
sbit btn_eq = P3^0;
sbit btn_dot = P3^1;
sbit btn_cls = P3^2;	//����P3.2����k3��"С����"��ť
sbit btn_del = P3^3;	//����P3.3����k4��"����"��ť
sbit btn_left = P3^4;	//����P3.6����k7��"ɾ��"��ť
sbit btn_right = P3^5;	//����P3.7����k8��"���"��ť

typedef unsigned int u16;	//���������ͽ�����������
typedef unsigned char u8;

u8 KeyValue;				//����󰴼���λ�� 0~15
u8 calc_arr[30] = { 0 };	//����ʽ�ַ���
u8 calc_len = 0;			//����ʽ����,�±꣬Ϊ�丳ֵ
u8 result_arr[16] = { 0 };	//����ַ�����������strlen�����ó�

u8 code char_red[16] = {'7', '8', '9', '/', 
	 				  	'4', '5', '6', '*', 
						'1', '2', '3', '-', 
						'(', '0', ')', '+'  }; 

void delay(u16 i)
{
	while(i--);
}

void delay2s(void)   //��� 0us
{
    unsigned char a,b,c,n;
    for(c=167;c>0;c--)
        for(b=39;b>0;b--)
            for(a=152;a>0;a--);
    for(n=1;n>0;n--);
}

void moveTo_left()
{
	LcdWriteCom(0x18);//������
}
void moveTo_right()
{
	LcdWriteCom(0x1c);//������
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
	GPIO_KEY = 0x0f;//����λ�͵�ƽ������λ�ߵ�ƽ
	if(GPIO_KEY != 0x0f)
	{
	 	delay(1000);//��ʱ10ms����
		if(GPIO_KEY != 0x0f)
		{
			//ɨ����
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
			//��ת����ɨ����
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
			
			if(strlen(calc_arr) > 16)//������ʾ�����ȣ���ʽ����
				moveTo_left();	

			display_up(calc_arr);

			//��ʱ�䰴,ǿ���˳�������0xf0˵��û�а�������
			while((a < 50) && (GPIO_KEY != 0xf0))//a<50���Ұ�������
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
	if(btn_eq == 0)		  //��ⰴ��K1�Ƿ���
	{	
		delay(1000);   //�������� һ���Լ10ms
		if(btn_eq == 0)	 //�ٴ��жϰ����Ƿ���
		{
			//ִ�д���
			result = calculate(calc_arr);
			sprintf(result_arr, result == (int)result ? "%.0f" : "%.6f", result);				
			display_down(result_arr);
		}
		while(!btn_eq);	 //��ⰴ���Ƿ��ɿ�
	}
	else if(btn_dot == 0)
	{
		delay(1000);   //�������� һ���Լ10ms
		if(btn_dot == 0)	 //�ٴ��жϰ����Ƿ���
		{
			//ִ�д���
			calc_len++;
			calc_arr[calc_len - 1] = '.';
			display_up(calc_arr);
		}
		while(!btn_dot);	 //��ⰴ���Ƿ��ɿ�
	}
	else if(btn_del == 0)
	{
	 	delay(1000);   //�������� һ���Լ10ms
		if(btn_del == 0)	 //�ٴ��жϰ����Ƿ���
		{
			//ִ�д���
			calc_arr[calc_len - 1] = ' ';
			display_up(calc_arr);
			calc_arr[calc_len - 1] = '\0';
			calc_len--;
		}
		while(!btn_del);	 //��ⰴ���Ƿ��ɿ�
	}
	else if(btn_cls == 0)
	{
		delay(1000);   //�������� һ���Լ10ms
		if(btn_cls == 0)	 //�ٴ��жϰ����Ƿ���
		{
			
			//�����ʾ
			LcdWriteCom(0x01);//����,ͬʱ����Ҳ��λ��
			//���ʽ�Ӻͽ������
			memset(calc_arr, 0, sizeof(calc_arr));
			memset(result_arr, 0, sizeof(result_arr));
			calc_len = 0;
		}
		while(!btn_cls);	 //��ⰴ���Ƿ��ɿ�
	}
	else if(btn_left == 0)
	{
		delay(1000);   //�������� һ���Լ10ms		
		if(btn_left == 0)//���ȳ���16�����ƶ�
		{
			moveTo_right();
		}
		while(!btn_left);
	}
	else if(btn_right == 0)
	{
		delay(1000);   //�������� һ���Լ10ms		
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
	LcdWriteCom(0x01);//�����ʾ
	
	while(1)
	{		
		OnRedKeyDown();
		OnBlackKeyDown();
	}				
}