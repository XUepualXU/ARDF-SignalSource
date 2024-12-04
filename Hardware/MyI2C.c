#include "stm32f10x.h"
#include "DELAY.H"

#define Addr      0xCA         // 向左平移一位之后的I2C总线上CDCE913地址
#define RCC_APB2Periph_GPIOX RCC_APB2Periph_GPIOA
#define GPIOX     GPIOA        // 使用的GPIO
#define SCL_Pin   GPIO_Pin_4   // I2C通信中的时钟线
#define SDA_Pin   GPIO_Pin_3   // I2C通信中的数据线
#define Daley  10              // I2C通信时，让数据在总线上的停留时间，防止读取数据发生差错

/*
  * @brief  STM32控制时钟线，将时钟线置0或1。STM32将一直控制时钟线。
  * @param  往时钟线上放置的1位  [0 0r 1]
  * @retval None
*/
void CDCE_W_SCL(uint8_t x){
	GPIO_WriteBit(GPIOX, SCL_Pin, (BitAction)(x));
	Delay_us(Daley);
}
/*
  * @brief  STM32往数据线上放置1位。在需要STM32读数据时，放置1代表放开对数据线的控制权
  * @param  往时钟线上放置的1位  [0 0r 1]
  * @retval None
*/
void CDCE_W_SDA(uint8_t x){
	GPIO_WriteBit(GPIOX, SDA_Pin, (BitAction)(x));
	Delay_us(Daley);
}
/*
  * @brief  STM32读取数据线上的1位。
  * @param  None
  * @retval 在数据线上读取的1位
*/
uint8_t CDCE_R_SDA(void){
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOX, SDA_Pin);
	Delay_us(Daley);
	return BitValue;
}

/*
  * @brief  初始化STM32与CDCE913进行I2C通信时使用的引脚，要设置为开漏输出
  * @param  None
  * @retval None
*/
void CDCE_I2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOX, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SDA_Pin | SCL_Pin;
 	GPIO_Init(GPIOX, &GPIO_InitStructure);
	
	CDCE_W_SCL(1);
	CDCE_W_SDA(1);
}

/**
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
void CDCE_I2C_Start(void)
{
	CDCE_W_SDA(1);
	CDCE_W_SCL(1);
	CDCE_W_SDA(0);
	CDCE_W_SCL(0);
}

/**
  * @brief  I2C停止
  * @param  无
  * @retval 无
  */
void CDCE_I2C_Stop(void)
{
	CDCE_W_SDA(0);
	CDCE_W_SCL(1);
	CDCE_W_SDA(1);
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void CDCE_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		CDCE_W_SDA(Byte & (0x80 >> i));
		CDCE_W_SCL(1);
		CDCE_W_SCL(0);
	}
	//CDCE_W_SCL(1);	//额外的一个时钟，应答位
	//CDCE_W_SCL(0);
}


//void CDCE_WriteData(uint8_t Reg, uint8_t Data)
//{
//	CDCE_I2C_Start();
//	CDCE_SendByte(Addr);	//从机地址
//	CDCE_SendByte(Reg);		//传送寄存器地址
//	CDCE_SendByte(Data);	//传送数据
//	CDCE_I2C_Stop();
//}

/**
  * @brief  I2C接收一个应答位
  * @param  None
  * @retval 接收的应答位
  */
uint8_t CDCE_ReceiveAck(void){
	uint8_t AckByte = 0;
	//uint8_t i;
	CDCE_W_SDA(1);

	CDCE_W_SCL(1);
	AckByte = CDCE_R_SDA();
	CDCE_W_SCL(0);
	return AckByte ;
}
/**
  * @brief  I2C接收一个字节
  * @param  None
  * @retval 接收的字节
  */
uint8_t CDCE_ReceiveByte(void){
	uint8_t Byte = 0;
	uint8_t i;
	CDCE_W_SDA(1);
	for(i = 0; i < 8; i++){
		CDCE_W_SCL(1);
		if(CDCE_R_SDA() == 1){
			Byte |= (0x80 >> i);
		}
		CDCE_W_SCL(0);
	}
	return Byte ;
}
/**
  * @brief  I2C发送一个应答位
  * @param  AckByte 要发送的应答位
  * @retval 无
  */
void CDCE_SendAck(uint8_t AckByte){
	//uint8_t i;

	CDCE_W_SDA(AckByte);
	CDCE_W_SCL(1);
	CDCE_W_SCL(0);
}
