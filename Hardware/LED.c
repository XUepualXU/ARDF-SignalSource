#include "stm32f10x.h"                  // Device header

/*
  * @brief  初始化LAMP与CTRL对应的引脚
  * @param  None
  * @retval None
*/
void LED_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}
/*
  * @brief  将LAMP与CTRL置高电平
  * @param  None
  * @retval None
*/
void High(void){
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	GPIO_SetBits(GPIOB, GPIO_Pin_11);
}
/*
  * @brief  将LAMP与CTRL置低电平
  * @param  None
  * @retval None
*/
void Low(void){
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	GPIO_ResetBits(GPIOB, GPIO_Pin_11);
}

	
