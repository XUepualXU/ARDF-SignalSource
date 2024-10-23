/*
    ARDF-SignalSource: A code used to control a signal transmitter 
    for Amateur radio direction finding motion

    Copyright (C) 2024, Xu Qianyi

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.H"
#include "MyI2C.H"
#include "KEY.H"
#include "LED.H"

#define time  100
/* 
address[]: CDCE913中的寄存器地址
data[]: 寄存器中应该写入的值
CDCE_Addr: CDCE913在I2C总线上的地址（向左平移1位后的）
*/
uint8_t address[] = {0, 1, 2, 3, 4, 5, 6, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
uint8_t data[] = {0x81, 0x01, 0xb4, 0x41, 0x02, 0x50, 0x41, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xff, 0xfc, 0xf2, 0x47, 0xff, 0xfc, 0xf2, //3.50  0
               0x81, 0x01, 0xb4, 0x40, 0x02, 0x50, 0x41, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xf8, 0xa9, 0x92, 0x47, 0xf8, 0xa9, 0x92, //3.51  1
               0x81, 0x01, 0xb4, 0x40, 0x02, 0x50, 0x41, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xdc, 0x09, 0x12, 0x47, 0xdc, 0x09, 0x12, //3.52  2
               0x81, 0x01, 0xb4, 0x40, 0x02, 0x50, 0x41, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xdc, 0xa9, 0xb2, 0x47, 0xdc, 0xa9, 0xb2, //3.53  3
               0x81, 0x01, 0xb4, 0x40, 0x02, 0x50, 0x41, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xfa, 0xcb, 0xb2, 0x47, 0xfa, 0xcb, 0xb2, //3.54  4
               0x81, 0x01, 0xb4, 0x3e, 0x02, 0x50, 0x41, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0x89, 0x92, 0x92, 0x47, 0x89, 0x92, 0x92, //3.55  5
               0x81, 0x01, 0xb4, 0x3f, 0x02, 0x50, 0x41, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xe9, 0xa8, 0xa2, 0x47, 0xe9, 0xa8, 0xa2, //3.56  6
               0x81, 0x01, 0xb4, 0x3e, 0x02, 0x50, 0x41, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xe6, 0x95, 0x92, 0x47, 0xe6, 0x95, 0x92, //3.57  7
               0x81, 0x01, 0xb4, 0x3f, 0x02, 0x50, 0x41, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xea, 0xf9, 0xf2, 0x47, 0xea, 0xf9, 0xf2, //3.58  8
               0x81, 0x01, 0xb4, 0x38, 0x02, 0x50, 0x41, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0x9d, 0x17, 0x12, 0x07, 0x9d, 0x17, 0x12, //3.59  9
               0x81, 0x01, 0xb4, 0x3f, 0x02, 0x50, 0x41, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xf8, 0x1b, 0xd2, 0x47, 0xf8, 0x1b, 0xd2, //3.60  MO
               0x81, 0x01, 0xb4, 0x01, 0x02, 0x50, 0x41, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xbf, 0xa0, 0x03, 0x09, 0xbf, 0xa0, 0x03
              };//144
#define CDCE_Addr 0xCA

/*
  * @brief  根据选择的台号，向寄存器写入相应频率对应的值.
  * @param  Choose: 选择的台号. [MO台认为是10号台]
  * @retval None
*/
void Send_Choose(uint8_t Choose){
	for(int i = 2; i < 22; i ++){
		CDCE_I2C_Start();
		CDCE_SendByte(CDCE_Addr);
		CDCE_ReceiveAck();
		CDCE_SendByte(address[i] | 0x80);
		CDCE_ReceiveAck();
		CDCE_SendByte(data[Choose * 22 + i]);
		CDCE_ReceiveAck();
		CDCE_I2C_Stop();
	}
}

/*
  * @brief  摩斯码中长信号.
  * @param  None
  * @retval None
*/
void DASH(){
	High();
	Delay_ms(4 * time);
	Low();
	Delay_ms(2 * time);
}
/*
  * @brief  摩斯码中短信号.
  * @param  None
  * @retval None
*/
void DOT(){
	High();
	Delay_ms(1.2 * time);
	Low();
	Delay_ms(1.5 * time);
}
/*
  * @brief  摩斯码中间的停顿，台号和台号中间停顿两次.
  * @param  None
  * @retval None
*/
void Longrest(){
	Low();
	Delay_ms(3 * time);
}

/*
  * @brief  实现对应台号的摩斯码.
  * @param  Choose: 选择的台号
  			0号台: _ _ _ _ _
  			1号台: _ _  _ _ _ .
     			2号台: _ _  _ _ _ . .
	     		3号台: _ _  _ _ _ . . .
	     		4号台: _ _  _ _ _ . . . .
	     		5号台: _ _  _ _ _ . . . . .
	     		6号台: _ . . . .
	     		7号台: _ _ . . .
	     		8号台: _ _ _ . .
	     		9号台: _ _ _ _ .
	     		MO 台: _ _  _ _ _ 
  * @retval None
*/
void speak(uint8_t Choose){
	if (Choose == 0){
		DASH();
		DASH();
		DASH();
		DASH();
		DASH();
		Longrest();
		Longrest();
	}

	if (Choose == 1){
		DASH();
		DASH();
		Longrest();
		DASH();
		DASH();
		DASH();
		Longrest();
		DOT();
		Longrest();
		Longrest();
	}
	if (Choose == 2){
		DASH();
		DASH();
		Longrest();
		DASH();
		DASH();
		DASH();
		Longrest();
		DOT();
		DOT();
		Longrest();
		Longrest();
	 }
	if (Choose == 3){
		DASH();
		DASH();
		Longrest();
		DASH();
		DASH();
		DASH();
		Longrest();
		DOT();
		DOT();
		DOT();
		Longrest();
		Longrest();
	 }
	if (Choose == 4){
		DASH();
		DASH();
		Longrest();
		DASH();
		DASH();
		DASH();
		Longrest();
		DOT();
		DOT();
		DOT();
		DOT();
		Longrest();
		Longrest();
	}
	if (Choose == 5){
		DASH();
		DASH();
		Longrest();
		DASH();
		DASH();
		DASH();
		Longrest();
		DOT();
		DOT();
		DOT();
		DOT();
		DOT();
		Longrest();
		Longrest();
	}
	if (Choose == 6){
		DASH();
		DOT();
		DOT();
		DOT();
		DOT();
		Longrest();
		Longrest();
	}
	if (Choose == 7){
		DASH();
		DASH();
		DOT();
		DOT();
		DOT();
		Longrest();
		Longrest();
	}
	if (Choose == 8){
		DASH();
		DASH();
		DASH();
		DOT();
		DOT();
		Longrest();
		Longrest();
	}
	if (Choose == 9){
		DASH();
		DASH();
		DASH();
		DASH();
		DOT();
		Longrest();
		Longrest();
	}
	if (Choose == 10){
		DASH();
		DASH();
		Longrest();
		DASH();
		DASH();
		DASH();
		Longrest();
		Longrest();
	}
}

/*
  * @brief  主函数
  * @param  None
  * @retval None
*/
int main(){
	uint8_t Key_Num, Key_tmp, data;
	CDCE_I2C_Init();  // 初始化与CDCE913芯片进行I2C通信的引脚
	LED_Init();  // 初始化控制LAMP和CTRL的引脚。CTRL用来控制CDCE913间断发送信号，LAMP使用led灯观察信号变化
	Key_Init();  // 初始化与8421拨码开关相连的引脚

	/*
	如果8421拨码开关上的数字大于10，全按照10处理
	*/
	Key_Num = Key_GetNum();
	if (Key_Num > 10){
		Key_Num = 10;
	}

	Send_Choose(Key_Num);

	while(1){
		/* 
  		循环读入拨码开关对应的值，如果被改变，则发送新的台号对应的信号
    		注： 上一个信号完整发送完毕之后才会换成新的信号
    		*/
		Key_tmp = Key_GetNum();
		if (Key_tmp > 10){
			Key_tmp = 10;
		}
		if(Key_Num != Key_tmp){
			Send_Choose(Key_tmp);
			Key_Num = Key_tmp;
		}
		speak(Key_Num);
	}
}
