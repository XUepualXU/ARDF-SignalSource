#include "stm32f10x.h"                  // Device header
#include "DELAY.H"

/*
  * @brief  初始化读取8421拨码开关对应的引脚
  * @param  None
  * @retval None
*/
void Key_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*
  * @brief  按位读取8421拨码开关的状态，组成二进制台号
  * @param  None
  * @retval 得到8421拨码开关组成的台号
*/
uint8_t Key_GetNum(void){
	uint8_t KeyNum = 0;
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 1){
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0);
		Delay_ms(20);
		KeyNum |= 0x08;
	}
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 1){
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0);
		Delay_ms(20);
		KeyNum |= 0x04;
	}
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 1){
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0);
		Delay_ms(20);
		KeyNum |= 0x02;
	}
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 1){
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0);
		Delay_ms(20);
		KeyNum |= 0x01;
	}
	return KeyNum;
}
