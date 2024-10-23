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
//#define Choose  10
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
#define RegAddress  0x06
			  
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

void DASH(){
	High();
	Delay_ms(4 * time);
	Low();
	Delay_ms(2 * time);
}
void DOT(){
	High();
	Delay_ms(1.2 * time);
	Low();
	Delay_ms(1.5 * time);
}
void Longrest(){
	Low();
	Delay_ms(3 * time);
}

void speak(uint8_t way){
	if (way == 0){
		DASH();
		DASH();
		DASH();
		DASH();
		DASH();
		Longrest();
		Longrest();
	}

	if (way == 1){
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
	if (way == 2){
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
	if (way == 3){
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
	if (way == 4){
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
	if (way == 5){
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
	if (way == 6){
		DASH();
		DOT();
		DOT();
		DOT();
		DOT();
		Longrest();
		Longrest();
	}
	if (way == 7){
		DASH();
		DASH();
		DOT();
		DOT();
		DOT();
		Longrest();
		Longrest();
	}
	if (way == 8){
		DASH();
		DASH();
		DASH();
		DOT();
		DOT();
		Longrest();
		Longrest();
	}
	if (way == 9){
		DASH();
		DASH();
		DASH();
		DASH();
		DOT();
		Longrest();
		Longrest();
	}
	if (way == 10){
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

int main(){
	uint8_t Key_Num, Key_tmp, data;
	OLED_Init();
	CDCE_I2C_Init();
	LED_Init();
	Key_Init();
	
	Key_Num = Key_GetNum();
	if (Key_Num > 10){
		Key_Num = 10;
	}
	Send_Choose(Key_Num);
	OLED_ShowString (3,1,"data:");
	OLED_ShowHexNum (3,6,data, 2);


	OLED_ShowString (1,1,"Num:");
	OLED_ShowString (2,1,"tmp:");
	


	while(1){
		Key_tmp = Key_GetNum();
		if (Key_tmp > 10){
			Key_tmp = 10;
		}
		OLED_ShowNum (1,5,Key_tmp,3);
		OLED_ShowNum (2,5,Key_Num,3);
		if(Key_Num != Key_tmp){
			Send_Choose(Key_tmp);
			Key_Num = Key_tmp;
		}
		speak(Key_Num);
		
	}
}
