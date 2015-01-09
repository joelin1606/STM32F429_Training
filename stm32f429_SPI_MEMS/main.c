#include "main.h"

static inline void Delay_1us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 13; nCnt != 0; nCnt--);
}

void SPI_Initialization(void){

  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);


  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPI5);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  /* SPI baudrate is set to 5.6 MHz (PCLK2/SPI_BaudRatePrescaler = 90/16 = 5.625 MHz)  */

  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI5, &SPI_InitStructure);

  /* Enable SPI5  */
  SPI_Cmd(SPI5, ENABLE);
  
  /* Configure GPIO PIN for Chip select */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_SPI5);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_SPI5);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_SPI5);

  /* Configure GPIO PIN for SPI4 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOF, &GPIO_InitStructure);


}

int main(void)
{

  char lcd_text_main[100];
 
    

    lcd_init();
    lcd_drawBackground(20,60,250);
    lcd_drawBGPersimmon(20, 60, 250);
    LCD_SetColors(LCD_COLOR_WHITE-1,LCD_COLOR_WHITE);

    LCD_SetFont(&Font8x12); 
    terminalBufferInitilization();


    SPI_Initialization();

    uint8_t receivedData1=0;
    uint8_t receivedData2=0;
    uint8_t receivedDataLX=0;
    uint8_t receivedDataHX=0;
    uint8_t receivedDataLY=0;
    uint8_t receivedDataHY=0;
    uint8_t receivedDataLZ=0;
    uint8_t receivedDataHZ=0;
    int16_t X=0;
    int16_t Y=0;
    int16_t Z=0;
    float x_acc=0;
    float y_acc=0;
    float z_acc=0;



GPIO_ResetBits(GPIOC, GPIO_Pin_1);
      /* SPI config */
      SPI_I2S_SendData(SPI5,0x20);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedData1=SPI_I2S_ReceiveData(SPI5);
      // sprintf(lcd_text_main,"receivedData1: %x    \n", receivedData1); terminalWrite(lcd_text_main);

      
      SPI_I2S_SendData(SPI5,0x0f);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedData2=SPI_I2S_ReceiveData(SPI5);
      // sprintf(lcd_text_main,"receivedData2: %x    \n", receivedData2); terminalWrite(lcd_text_main); 


GPIO_SetBits(GPIOC, GPIO_Pin_1);


    while(1){

GPIO_ResetBits(GPIOC, GPIO_Pin_1);

      SPI_I2S_SendData(SPI5,0xa8);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedData2=SPI_I2S_ReceiveData(SPI5);
      // sprintf(lcd_text_main,"receivedData3: %d    \n", receivedData3); terminalWrite(lcd_text_main); 

      SPI_I2S_SendData(SPI5,0xff);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedDataLX=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC, GPIO_Pin_1);
      // sprintf(lcd_text_main,"\n\n\n\nreceivedDataLX: %d    \n\n", receivedDataLX); terminalWrite(lcd_text_main); 
GPIO_SetBits(GPIOC, GPIO_Pin_1);


GPIO_ResetBits(GPIOC, GPIO_Pin_1);

      SPI_I2S_SendData(SPI5,0xa9);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedData2=SPI_I2S_ReceiveData(SPI5);
      // sprintf(lcd_text_main,"receivedData3: %d    \n", receivedData3); terminalWrite(lcd_text_main); 

      SPI_I2S_SendData(SPI5,0xff);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedDataHX=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC, GPIO_Pin_1);
      // sprintf(lcd_text_main,"receivedDataHX: %d    \n\n", receivedDataHX); terminalWrite(lcd_text_main); 
GPIO_SetBits(GPIOC, GPIO_Pin_1);

//////////////////////////YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

GPIO_ResetBits(GPIOC, GPIO_Pin_1);
      SPI_I2S_SendData(SPI5,0xaa);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedData2=SPI_I2S_ReceiveData(SPI5);
      // sprintf(lcd_text_main,"receivedData3: %d    \n", receivedData3); terminalWrite(lcd_text_main); 

      SPI_I2S_SendData(SPI5,0xff);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedDataLY=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC, GPIO_Pin_1);
      // sprintf(lcd_text_main,"receivedDataLY: %d    \n\n", receivedDataLY); terminalWrite(lcd_text_main); 
GPIO_SetBits(GPIOC, GPIO_Pin_1);


GPIO_ResetBits(GPIOC, GPIO_Pin_1);
      SPI_I2S_SendData(SPI5,0xab);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedData2=SPI_I2S_ReceiveData(SPI5);
      // sprintf(lcd_text_main,"receivedData3: %d    \n", receivedData3); terminalWrite(lcd_text_main); 

      SPI_I2S_SendData(SPI5,0xff);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedDataHY=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC, GPIO_Pin_1);
      // sprintf(lcd_text_main,"receivedDataHY: %d    \n\n", receivedDataHY); terminalWrite(lcd_text_main); 
GPIO_SetBits(GPIOC, GPIO_Pin_1);

//////////////////////////ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZz


GPIO_ResetBits(GPIOC, GPIO_Pin_1);
      SPI_I2S_SendData(SPI5,0xaC);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedData2=SPI_I2S_ReceiveData(SPI5);
      // sprintf(lcd_text_main,"receivedData3: %d    \n", receivedData3); terminalWrite(lcd_text_main); 

      SPI_I2S_SendData(SPI5,0xff);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedDataLZ=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC, GPIO_Pin_1);
       // sprintf(lcd_text_main,"LZ: %d ", receivedDataLZ); terminalWrite(lcd_text_main); 
GPIO_SetBits(GPIOC, GPIO_Pin_1);


GPIO_ResetBits(GPIOC, GPIO_Pin_1);
      SPI_I2S_SendData(SPI5,0xaD);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedData2=SPI_I2S_ReceiveData(SPI5);
      // sprintf(lcd_text_main,"receivedData3: %d    \n", receivedData3); terminalWrite(lcd_text_main); 

      SPI_I2S_SendData(SPI5,0xff);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedDataHZ=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC, GPIO_Pin_1);
      // sprintf(lcd_text_main,"HZ: %d  ", receivedDataHZ); terminalWrite(lcd_text_main); 
GPIO_SetBits(GPIOC, GPIO_Pin_1);


      X=receivedDataLX | receivedDataHX<<8;
      Y=receivedDataLY | receivedDataHY<<8;
      Z=receivedDataLZ | receivedDataHZ<<8;

      x_acc=250.0f*(float) X/32768.0f;
      y_acc=250.0f*(float) Y/32768.0f;
      z_acc=250.0f*(float) Z/32768.0f;

      sprintf(lcd_text_main,"X=%.3f Y=%.3f Z=%.3f\n",x_acc,y_acc,z_acc); terminalWrite(lcd_text_main);
      
      Delay_1us(15000);
    }
  // ,x_acc,y_acc,z_acc
}
