#ifndef __MYI2C_H__
#define __MYI2C_H__

void CDCE_I2C_Init(void);
void CDCE_W_SCL(uint8_t x);
void CDCE_W_SDA(uint8_t x);
void CDCE_I2C_Start(void);
void CDCE_I2C_Stop(void);

void CDCE_SendByte(uint8_t Byte);
void CDCE_WriteData(uint8_t Reg, uint8_t Data);

uint8_t CDCE_ReceiveAck(void);

uint8_t CDCE_ReceiveByte(void);
void CDCE_SendAck(uint8_t AckByte);

#endif

