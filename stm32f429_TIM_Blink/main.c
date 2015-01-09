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
      /* GPIOA clock enable */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);

}
 
/**************************************************************************************/
 
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*-------------------------- GPIO Configuration for Push Button ----------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD ;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

/////////////////////////////////////////////////////////////////////////////////
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP ;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

}

void LED_Initialization(void)
{

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


void LED3_Toggle(void){


  GPIOG->ODR ^= GPIO_Pin_13;

}

void LED4_Toggle(void){


  GPIOG->ODR ^= GPIO_Pin_14;

}

/**************************************************************************************/
int main(void)
{
    RCC_Configuration();
    LED_Initialization();
    Timer5_Initialization();
    GPIO_Configuration();
    int timevalue=0;
    uint8_t i=0;

    while (1)
    {
      timevalue=TIM_GetCounter(TIM5);
      if(timevalue == 1000)
      {
        LED4_Toggle();
        
        // if(i==0)
        //   {
        //     GPIO_SetBits(GPIOD, GPIO_Pin_8);
        //     i=5;
        //   }
        // else
        //   {
        //     GPIO_ResetBits(GPIOD, GPIO_Pin_8);
        //     i=0;
        //   }

      }
      
      
        // this loop is doing nothing but smiling at you :)
      
    }
}



