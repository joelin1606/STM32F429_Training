
#include "main.h"
#include <stdio.h>

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


void Timer5_Initialization(void)
{

 /* -- Timer Configuration --------------------------------------------------- */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  
  TIM_DeInit(TIM5);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_TimeBaseStruct.TIM_Period = 10000 - 1 ;  //250ms  --> 4Hz
  TIM_TimeBaseStruct.TIM_Prescaler = 900 - 1; // Prescaled by 900 -> = 0.1M(10us)
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1; // Div by one -> 90 MHz (Now RCC_DCKCFGR_TIMPRE is configured to divide clock by two)
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStruct);
  // TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);    // Set interrupt when timer reloads (overflow)
  // TIM_ARRPreloadConfig(TIM5, DISABLE);       //Put ARR value into register
  TIM_Cmd(TIM5, ENABLE);
}


void DrawThickCircle(uint32_t x,uint32_t y,uint32_t radius, uint32_t thickness)
{

    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_DrawFullCircle(x, y, radius);
    LCD_SetColors(LCD_COLOR_WHITE-1,LCD_COLOR_WHITE);
    LCD_DrawFullCircle(x, y, radius-thickness);


}

void RCC_Configuration(void)
{
      /* --------------------------- System Clocks Configuration -----------------*/
      /* USART1 clock enable */
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
      /* GPIOA clock enable */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
}
 
/**************************************************************************************/
 
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*-------------------------- GPIO Configuration ----------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Connect USART pins to AF */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);   // USART1_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  // USART1_RX
}
 
/**************************************************************************************/




void ADC_Initialization(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADC3, DMA2 and GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* Configure ADC3 Channel7 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* ADC Common Init **********************************************************/

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* ADC Common Init **********************************************************/

  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     // No external trigger is connected
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;                  // ADC clock prescaler
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;      // No DMA (polling mode)
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                       // Resolution 12 bits
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;                                // Use single channel 
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                           // Continue conversion
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                       // Data bits shifted to right hand side (Low)
  ADC_InitStructure.ADC_NbrOfConversion = 1;                                   // Convert only once
  ADC_Init(ADC3, &ADC_InitStructure);

/* ADC2 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                       // Resolution 12 bits
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;                                // Use single channel 
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                           // Continue conversion
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                       // Data bits shifted to right hand side (Low)
  ADC_InitStructure.ADC_NbrOfConversion = 1;                                   // Convert only once
  ADC_Init(ADC2, &ADC_InitStructure);

/* ADC1 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                       // Resolution 12 bits
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;                                // Use single channel 
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                           // Continue conversion
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                       // Data bits shifted to right hand side (Low)
  ADC_InitStructure.ADC_NbrOfConversion = 1;                                   // Convert only once
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC3.ADC2.ADC1 regular channel configuration *************************************/
  ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 1, ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_7, 1, ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
  /* Enable ADC3 */
  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);
  ADC_Cmd(ADC3, ENABLE);
}

/**************************************************************************************/
 
void LED_Initialization(void){

  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE); //LED3/4 GPIO Port

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;  // LED is connected to PG13/PG14
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

}

void LED4_Toggle(void){


  GPIOG->ODR ^= GPIO_Pin_14;

}


///////////////////////////////////////////////////////////////////////////////////////
/*PWM*/
void PWM_Initialization(void)
{

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE); 

  /* -- GPIO Configuration ---------------------------------------------------- */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9);

  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_5 ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* -- Timer Configuration --------------------------------------------------- */
  TIM_DeInit(TIM9);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_TimeBaseStruct.TIM_Period = (uint32_t)(20000 - 1);  //2.5ms , 400Hz
  TIM_TimeBaseStruct.TIM_Prescaler = (uint16_t)(180 - 1); //84 = 1M(1us)
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;    // No division, so 180MHz
  TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;           // Not used
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStruct);


  TIM_OCInitTypeDef TIM_OCInitStruct;
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;               //PWM Edge mode
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_Pulse = 1000-1;
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;        // Output polarity High
  TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;      // Complementary output polarity :Not used
  TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;     // No output polarity : reset (low)
  TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;    // Complementary idle output : reset (not used)

  TIM_OC1Init(TIM9, &TIM_OCInitStruct);
  TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM9, ENABLE);       //Put ARR value into register
  TIM_Cmd(TIM9, ENABLE);                    // Enable Timer 1
  TIM_CtrlPWMOutputs(TIM9, ENABLE);         // Enable output (To GPIO)


/////////////////////////////////////////////////////////////////////////////////////////////

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF , ENABLE); 

  /* -- GPIO Configuration ---------------------------------------------------- */
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_TIM10);

  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_6 ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* -- Timer Configuration --------------------------------------------------- */
  TIM_DeInit(TIM10);

  
  TIM_TimeBaseStruct.TIM_Period = (uint32_t)(20000 - 1);  //2.5ms , 400Hz
  TIM_TimeBaseStruct.TIM_Prescaler = (uint16_t)(180 - 1); //84 = 1M(1us)
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;    // No division, so 180MHz
  TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;           // Not used
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStruct);


  
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;               //PWM Edge mode
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_Pulse = 1000-1;
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;        // Output polarity High
  TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;      // Complementary output polarity :Not used
  TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;     // No output polarity : reset (low)
  TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;    // Complementary idle output : reset (not used)

  TIM_OC1Init(TIM10, &TIM_OCInitStruct);
  TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM10, ENABLE);       //Put ARR value into register
  TIM_Cmd(TIM10, ENABLE);                    // Enable Timer 1
  TIM_CtrlPWMOutputs(TIM10, ENABLE);         // Enable output (To GPIO)


}
/*PWM   */
////////////////////////////////////////////////////////////////////////////////////




void USART1_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;

    /* USARTx configuration ------------------------------------------------------*/
    /* USARTx configured as follow:
     *  - BaudRate = 57600 baud
     *  - Word Length = 8 Bits
     *  - One Stop Bit
     *  - No parity
     *  - Hardware flow control disabled (RTS and CTS signals)
     *  - Receive and transmit enabled
     */
    USART_InitStructure.USART_BaudRate = 57600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

void USART1_puts(char* s)
{
    while(*s) {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, *s);
        s++;
    }
}

/**************************************************************************************/
uint32_t PWMoutput=1000;
uint8_t buff_transmit[100];
uint8_t k=0;

int main(void)
{
  /**************************************************************************************/
  /**************************************************************************************/
  /**************************************************************************************/
  uint8_t colorR =0 ,colorG =0 ,colorB =0 ;
  uint8_t colorR_dir =0 ,colorG_dir =0 ,colorB_dir =0 ;
  

  /* LCD initialization */
  LCD_Init();
  
  /* LCD Layer initialization */
  LCD_LayerInit();
    
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  LCD_SetColorKeying(0xFFFFFF);

  /* Need to reload */
  LTDC_ReloadConfig(LTDC_IMReload);

  /* Enable the LTDC */
  LTDC_Cmd(ENABLE);
  
  /* Set LCD foreground layer */

  /* Clear the LCD */ 
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_SetFont(&Font16x24);

  LCD_SetLayer(LCD_BACKGROUND_LAYER);
  LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);

  LCD_DisplayStringLine(LINE(1), (uint8_t*)" LCD text print example ");
  LCD_DisplayStringLine(LINE(2), (uint8_t*)" Ming6842 @ github");

    LCD_SetLayer(LCD_FOREGROUND_LAYER);

    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_WHITE);
    LCD_DrawFullRect(0,0,240,320);

    LCD_Clear(LCD_COLOR_WHITE);

#define X_MIDDLE 120
#define Y_MIDDLE 180

    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_DrawUniLine(X_MIDDLE+75, Y_MIDDLE-75  , X_MIDDLE-75, Y_MIDDLE+75);
    LCD_DrawUniLine(X_MIDDLE+75, Y_MIDDLE-75-1, X_MIDDLE-75, Y_MIDDLE+75-1);
    LCD_DrawUniLine(X_MIDDLE+75, Y_MIDDLE-75+1, X_MIDDLE-75, Y_MIDDLE+75+1);

    LCD_DrawUniLine(X_MIDDLE-75, Y_MIDDLE-75  , X_MIDDLE+75, Y_MIDDLE+75);
    LCD_DrawUniLine(X_MIDDLE-75, Y_MIDDLE-75-1, X_MIDDLE+75, Y_MIDDLE+75-1);
    LCD_DrawUniLine(X_MIDDLE-75, Y_MIDDLE-75+1, X_MIDDLE+75, Y_MIDDLE+75+1);


    LCD_DrawFullRect(X_MIDDLE-60,Y_MIDDLE-5,120,10);
    LCD_DrawFullRect(X_MIDDLE-5,Y_MIDDLE-60,10,120);


    DrawThickCircle(X_MIDDLE,Y_MIDDLE,30,7);

    DrawThickCircle(X_MIDDLE+60,Y_MIDDLE,22,   5);
    DrawThickCircle(X_MIDDLE-60,Y_MIDDLE,22,   5);
    DrawThickCircle(X_MIDDLE   ,Y_MIDDLE+60,22,5);
    DrawThickCircle(X_MIDDLE   ,Y_MIDDLE-60,22,5);

    DrawThickCircle(X_MIDDLE+75,Y_MIDDLE+75,32,5);
    DrawThickCircle(X_MIDDLE-75,Y_MIDDLE-75,32,5);
    DrawThickCircle(X_MIDDLE+75,Y_MIDDLE-75,32,5);
    DrawThickCircle(X_MIDDLE-75,Y_MIDDLE+75,32,5);

    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE-1);


    LCD_DisplayStringLine(LINE(1), (uint8_t*)" Balance Plate ");
    


  LCD_SetLayer(LCD_BACKGROUND_LAYER);

  uint16_t b=0;
  uint8_t buff_transmit[100];
  /**************************************************************************************/
  /**************************************************************************************/
  /**************************************************************************************/
    RCC_Configuration();
    GPIO_Configuration();
    USART1_Configuration();
    LED_Initialization();
    PWM_Initialization();
    ADC_Initialization();
    SPI_Initialization();
    Timer5_Initialization();

    int16_t adc_data1=0,adc_data2=0, adc_data3 =0;
    uint ADC1sum=0,ADC2sum=0,ADC3sum=0;
    uint16_t ADC1set=0,ADC2set=0,ADC3set=0;
    int i=0;
    float voltage1 =0.0f,voltage2 =0.0f,voltage3 =0.0f;
    double dt=1.0f;
    int32_t timevalue=0;
    int32_t foretimevalue=0;


    // uint16_t Xpositive=1000,Xnegative=2560;
    // uint16_t Ypositive=3260,Ynegative=3660;
    // uint16_t Zpositive=2040,Znegative=3000;

    // double Xaverage=0.5*(Xpositive+Xnegative);
    // double Yaverage=0.5*(Ypositive+Ynegative);
    // double Zaverage=0.5*(Zpositive+Znegative);

    uint16_t Xscale=375;
    uint16_t Yscale=375;
    uint16_t Zscale=375;
    // double Yscale=0.5*(Ypositive-Ynegative);
    // double Zscale=0.5*(Zpositive-Znegative);
   

    ADC_SoftwareStartConv(ADC3);
    ADC_SoftwareStartConv(ADC2);
    ADC_SoftwareStartConv(ADC1);

    
    float Xg=0.0f,Yg=0.0f, Zg=0.0f,R=0.0f;
    float X1=0.0f,Y1=0.0f, Z1=0.0f,angle=0.0f,angle0=0.0f;
    float kp=0.4f,ki=0.7f,kd=0.1f;
    float smallangle=0.0f;
    float output=0.0f;
    float integral=0.0f;
    float error=0.0f;
    float preerror=0.0f;
    float derivative=0.0f;
    float fixmotor=28.15f;
/////////////////////////////////////////////////////////////////////////////////////
     

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
    int Xsum=0;
    int Ysum=0;
    int Zsum=0;
    float gyrox=0;
    float y_acc=0;
    float z_acc=0;
/////////////////////////////////////////////////////////////////////////////////////


    GPIO_ResetBits(GPIOC, GPIO_Pin_1);

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

    uint8_t j=0;


Delay_1us(1000000);
GPIO_ToggleBits(GPIOG,GPIO_Pin_14);

/////////////////////////////////////////////SET UP//////////////////////////////////////////////
/////////////////////////////////////////////SET UP//////////////////////////////////////////////
/////////////////////////////////////////////SET UP//////////////////////////////////////////////


uint8_t N=30;

   while(j<N)
{

        adc_data1 = ADC_GetConversionValue(ADC3);
        adc_data2 = ADC_GetConversionValue(ADC2);
        adc_data3 = ADC_GetConversionValue(ADC1);


GPIO_ResetBits(GPIOC, GPIO_Pin_1);

      SPI_I2S_SendData(SPI5,0xa8);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedData2=SPI_I2S_ReceiveData(SPI5);
     

      SPI_I2S_SendData(SPI5,0xff);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedDataLX=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC, GPIO_Pin_1);
     
GPIO_SetBits(GPIOC, GPIO_Pin_1);

GPIO_ResetBits(GPIOC, GPIO_Pin_1);

      SPI_I2S_SendData(SPI5,0xa9);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedData2=SPI_I2S_ReceiveData(SPI5);

      SPI_I2S_SendData(SPI5,0xff);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedDataHX=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC, GPIO_Pin_1);
GPIO_SetBits(GPIOC, GPIO_Pin_1);

//////////////////////////YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

GPIO_ResetBits(GPIOC, GPIO_Pin_1);
      SPI_I2S_SendData(SPI5,0xaa);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedData2=SPI_I2S_ReceiveData(SPI5);

      SPI_I2S_SendData(SPI5,0xff);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedDataLY=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC, GPIO_Pin_1);
GPIO_SetBits(GPIOC, GPIO_Pin_1);


GPIO_ResetBits(GPIOC, GPIO_Pin_1);
      SPI_I2S_SendData(SPI5,0xab);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedData2=SPI_I2S_ReceiveData(SPI5);


      SPI_I2S_SendData(SPI5,0xff);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedDataHY=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC, GPIO_Pin_1);
GPIO_SetBits(GPIOC, GPIO_Pin_1);

//////////////////////////ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZz


GPIO_ResetBits(GPIOC, GPIO_Pin_1);
      SPI_I2S_SendData(SPI5,0xaC);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedData2=SPI_I2S_ReceiveData(SPI5);

      SPI_I2S_SendData(SPI5,0xff);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedDataLZ=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC, GPIO_Pin_1);
GPIO_SetBits(GPIOC, GPIO_Pin_1);


GPIO_ResetBits(GPIOC, GPIO_Pin_1);
      SPI_I2S_SendData(SPI5,0xaD);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedData2=SPI_I2S_ReceiveData(SPI5);

      SPI_I2S_SendData(SPI5,0xff);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);
      while (SPI_I2S_GetFlagStatus(SPI5, SPI_FLAG_RXNE) == RESET);
      receivedDataHZ=SPI_I2S_ReceiveData(SPI5);
      GPIO_SetBits(GPIOC, GPIO_Pin_1);
GPIO_SetBits(GPIOC, GPIO_Pin_1);


      X=receivedDataLX | receivedDataHX<<8;
      Y=receivedDataLY | receivedDataHY<<8;
      Z=receivedDataLZ | receivedDataHZ<<8;

      Xsum+=X;
      Ysum+=Y;
      Zsum+=Z;

      ADC1sum+= ADC_GetConversionValue(ADC3);
      ADC2sum+= ADC_GetConversionValue(ADC2);
      ADC3sum+= ADC_GetConversionValue(ADC1);


  j++;
  TIM9->CCR1 =PWMoutput ;
  TIM10->CCR1 =PWMoutput;
   Delay_1us(100000);
}

int16_t Xset;
int16_t Yset;
int16_t Zset;

Xset=Xsum/N;
Yset=Ysum/N;
Zset=Zsum/N;

ADC1set= ADC1sum/N;
ADC2set= ADC2sum/N;
ADC3set= ADC3sum/N;


GPIO_ToggleBits(GPIOG,GPIO_Pin_14);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    while(1)
    {
        
    



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
      X-=Xset;
      Y-=Yset;
      Z-=Zset;

      gyrox=250.0f*(float) X/32768.0f;
      y_acc=250.0f*(float) Y/32768.0f;
      z_acc=250.0f*(float) Z/32768.0f;



        // sprintf((char *)buff_transmit, "x_av = %.3f\r\n",gyrox);
        // USART1_puts((char *)buff_transmit);

        // sprintf((char *)buff_transmit, "y_av = %.3f\r\n",y_acc);
        // USART1_puts((char *)buff_transmit);

        // sprintf((char *)buff_transmit, "z_av = %.3f\r\n",z_acc);
        // USART1_puts((char *)buff_transmit);

        for (i=0;i<100;i++){

            buff_transmit[i]=0;
          }

//////////////////////////////////////////////////////////////////////
        // LED3_Toggle();
        //ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 1, ADC_SampleTime_3Cycles);
        //ADC_SoftwareStartConv(ADC3);
        
        adc_data1 = ADC_GetConversionValue(ADC3);
        adc_data2 = ADC_GetConversionValue(ADC2);
        adc_data3 = ADC_GetConversionValue(ADC1);

        adc_data1 = adc_data1-ADC1set;
        adc_data2 = adc_data2-ADC2set;
        adc_data3 = adc_data3-ADC3set;


        Z1=(float)adc_data1/Zscale;
        Y1=(float)adc_data2/Yscale;
        X1=(float)adc_data3/Xscale;

        if(X1>1)
          {X1=1;}
        if(X1<-1)
          {X1=-1;}

        /////////    dt        ////////////////////////////////////
        foretimevalue=timevalue;
        timevalue=TIM_GetCounter(TIM5);
        LED4_Toggle();
        


        if(timevalue>foretimevalue)
        {
          dt=(float)(timevalue-foretimevalue)/100000.0f;
        }

        if(timevalue<=foretimevalue)
        {
          dt=(float)(10000+timevalue-foretimevalue)/100000.0f;
        }

        
        /////////    dt        ////////////////////////////////////

        PWMoutput=1400;
        smallangle=180.0f*X1/M_PI;

//////////////////////////////////////

        angle0=180.0f*asin(X1)/M_PI;
        if (angle0<5)
        {angle0=smallangle;}

/////////////////////////////////////////

        angle =0.9f*(angle+gyrox*dt)+0.1f*angle0;
        error=angle*8;

///////////////////////////////////////
       
        integral=integral+error*dt;

        derivative=(error-preerror)/dt;
        output=kp*error+ki*integral+kd*derivative;
        preerror=error;



////////////////////////////////////////////////////

      TIM9->CCR1 =  PWMoutput +  fixmotor  -  output*1.5  ;
      TIM10->CCR1 =  PWMoutput -  fixmotor  +  output*0.5 ;

        






        sprintf((char *)buff_transmit, "angle0 = %f\r\n",angle0);
        USART1_puts((char *)buff_transmit);
        // R=pow(pow(X1,2)+pow(Y1,2)+pow(Z1,2),0.5);

        // Xg=X1/R;
        // Yg=Y1/R;
        // Zg=Z1/R;

        // ATT_angle=180*asin(-Xg)/M_PI;
        // Roll_angle=180*asin(Yg )/M_PI;
        
        
        // ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_3Cycles);
        // ADC_SoftwareStartConv(ADC3);
        // Delay_1us(10);
        // adc_data2 = ADC_GetConversionValue(ADC3);

        // voltage1 = (float)adc_data1*3.3f/4095.0f;
        // voltage2 = (float)adc_data2*3.3f/4095.0f;
        // voltage3 = (float)adc_data3*3.3f/4095.0f;

        sprintf((char *)buff_transmit, "ADC Data1 = %d\r\n",adc_data1);
        USART1_puts((char *)buff_transmit);
        for (i=0;i<100;i++){

            buff_transmit[i]=0;
          }


        sprintf((char *)buff_transmit, "ADC Data2 = %d\r\n",adc_data2);
        USART1_puts((char *)buff_transmit);
        for (i=0;i<100;i++){

            buff_transmit[i]=0;
          }


        sprintf((char *)buff_transmit, "ADC Data3 = %d\r\n",adc_data3);
        USART1_puts((char *)buff_transmit);

        for (i=0;i<100;i++){

            buff_transmit[i]=0;
          }


          /////////////////////////////////////////////////////////////////////////////////
          /////////////////////////////////////////////////////////////////////////////////
          b++;
    if(b==999)
      {
        b=0;
      }
    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE-1);
    LCD_SetLayer(LCD_FOREGROUND_LAYER);
    sprintf( (char *)buff_transmit,"err=%f             ",error);
    LCD_DisplayStringLine(LINE(2), buff_transmit);


    LCD_SetLayer(LCD_FOREGROUND_LAYER);
    sprintf( (char *)buff_transmit,"i=%f           ",integral);
    LCD_DisplayStringLine(LINE(3), buff_transmit);


    LCD_SetLayer(LCD_FOREGROUND_LAYER);
    sprintf( (char *)buff_transmit,"d=%f           ",derivative);
    LCD_DisplayStringLine(LINE(4), buff_transmit);


    LCD_SetLayer(LCD_FOREGROUND_LAYER);
    sprintf( (char *)buff_transmit,"gyrox=%f             ",gyrox);
    LCD_DisplayStringLine(LINE(5), buff_transmit);


    LCD_SetLayer(LCD_FOREGROUND_LAYER);
    sprintf( (char *)buff_transmit,"output=%f           ",output);
    LCD_DisplayStringLine(LINE(6), buff_transmit);


    LCD_SetLayer(LCD_FOREGROUND_LAYER);
    sprintf( (char *)buff_transmit,"Angle=%f             ",angle);
    LCD_DisplayStringLine(LINE(7), buff_transmit);


    LCD_SetLayer(LCD_FOREGROUND_LAYER);  
    sprintf( (char *)buff_transmit,"Angle0=%f             ",angle0);
    LCD_DisplayStringLine(LINE(8), buff_transmit);


    LCD_SetLayer(LCD_FOREGROUND_LAYER);  
    sprintf( (char *)buff_transmit,"dt=%f             ",dt);
    LCD_DisplayStringLine(LINE(9), buff_transmit);

    float frequency=1/dt;

    LCD_SetLayer(LCD_FOREGROUND_LAYER);
    sprintf( (char *)buff_transmit,"Hz=%f             ",frequency);
    LCD_DisplayStringLine(LINE(10), buff_transmit);


    LCD_SetLayer(LCD_FOREGROUND_LAYER);
    sprintf( (char *)buff_transmit,"Angle0=%f     ",angle0);
    LCD_DisplayStringLine(LINE(11), buff_transmit);


    LCD_SetLayer(LCD_FOREGROUND_LAYER);
    sprintf( (char *)buff_transmit,"Angle=%f     ",angle);
    LCD_DisplayStringLine(LINE(12), buff_transmit);



    if(colorR_dir){

          colorR += 1;

      if(colorR > 250) colorR_dir=0;
      
    }else{

      colorR -= 1;

      if(colorR<20) colorR_dir=1;
    }

    if(colorG_dir){

          colorG += 2;

      if(colorG > 250) colorG_dir=0;
      
    }else{

      colorG -= 2;

      if(colorG<25) colorG_dir=1;
    }

    if(colorB_dir){

          colorB += 3;

      if(colorB > 250) colorB_dir=0;
      
    }else{

      colorB -= 3;

      if(colorB<25) colorB_dir=1;
    }


    LCD_SetLayer(LCD_BACKGROUND_LAYER);
    LCD_SetColors(ASSEMBLE_RGB(colorR, colorG, colorB),LCD_COLOR_BLACK);
    LCD_DrawFullRect(0,0,240,320);
    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////

          
    }


}

