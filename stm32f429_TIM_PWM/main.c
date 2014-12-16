#include "main.h"


static inline void Delay_1us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 13; nCnt != 0; nCnt--);
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


void PWM_Initialization(void)
{

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE); 

  /* -- GPIO Configuration ---------------------------------------------------- */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);

  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_11 ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* -- Timer Configuration --------------------------------------------------- */
  TIM_DeInit(TIM1);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_TimeBaseStruct.TIM_Period = (uint32_t)(20000 - 1);  //2.5ms , 400Hz
  TIM_TimeBaseStruct.TIM_Prescaler = (uint16_t)(180 - 1); //84 = 1M(1us)
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;    // No division, so 180MHz
  TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;           // Not used
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct);


  TIM_OCInitTypeDef TIM_OCInitStruct;
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;               //PWM Edge mode
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_Pulse = 1000-1;
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;        // Output polarity High
  TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;      // Complementary output polarity :Not used
  TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;     // No output polarity : reset (low)
  TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;    // Complementary idle output : reset (not used)

  TIM_OC2Init(TIM1, &TIM_OCInitStruct);
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM1, ENABLE);       //Put ARR value into register
  TIM_Cmd(TIM1, ENABLE);                    // Enable Timer 1
  TIM_CtrlPWMOutputs(TIM1, ENABLE);         // Enable output (To GPIO)
}




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

    USART_ClearFlag(USART1, USART_FLAG_TC);

    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /* NVIC Initialization */
    NVIC_InitTypeDef NVIC_InitStruct = {
      .NVIC_IRQChannel = USART1_IRQn,
      .NVIC_IRQChannelPreemptionPriority = 0,
      .NVIC_IRQChannelSubPriority = 0,
      .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&NVIC_InitStruct);

}

void USART1_puts(char* s)
{
    while(*s) {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, *s);
        s++;
    }
}

void LED3_On(void){

  GPIO_SetBits(GPIOG,GPIO_Pin_13);

}

void LED3_Toggle(void){


  GPIOG->ODR ^= GPIO_Pin_13;

}
/**************************************************************************************/
uint8_t t=0;// USART input and output
uint8_t q[5];//input array
uint8_t checksum=0x00;
uint8_t PWMoutput=0;
uint8_t c10=0,c10output;;

/**************************************************************************************/
int main(void)
{
    RCC_Configuration();
    GPIO_Configuration();
    LED_Initialization();
    PWM_Initialization();
    USART1_Configuration();

    TIM1->CCR2 = 0;
    G
    while(1)
    {
      checksum=q[0]+q[1]+q[2]+q[3];

        if(q[0]==0x91)
        {
          if(q[1]==0x71)
          {
            if(q[2]==0x01)
            {
              if(q[4]==checksum)
              {
                PWMoutput=q[3];
                if(q[3]<0x03)
                {
                  PWMoutput=0x03;
                }
                if(q[3]>0xfc)
                {
                  PWMoutput=0xfc;
                }

                TIM1->CCR2 =988+PWMoutput*4;
              }
            }
          }
        }
    }
}

void USART1_IRQHandler(void)//USART interrupt function
{
  
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
  {
    t = USART_ReceiveData(USART1);
    USART_SendData(USART1, t);
    q[0]=q[1];
    q[1]=q[2];
    q[2]=q[3];
    q[3]=q[4];
    q[4]=t;
  }

}
