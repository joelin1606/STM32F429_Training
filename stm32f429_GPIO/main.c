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

    /*-------------------------- GPIO Configuration for Push Button ----------------------------*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP ;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

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


void LED3_On(void){

  GPIO_SetBits(GPIOG,GPIO_Pin_13);

}

void LED3_Off(void){

  GPIO_ResetBits(GPIOG,GPIO_Pin_13);

}

void LED4_On(void){

  GPIO_SetBits(GPIOG,GPIO_Pin_14);

}

void LED4_Off(void){

  GPIO_ResetBits(GPIOG,GPIO_Pin_14);

}

void LED3_Toggle(void){


  GPIO_ToggleBits(GPIOG,GPIO_Pin_13);

}

void LED4_Toggle(void){


  GPIO_ToggleBits(GPIOG,GPIO_Pin_14);

}

uint8_t PushButton_Read(void){

    return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);

}
/**************************************************************************************/
int main(void)
{
    
    RCC_Configuration();
    GPIO_Configuration();
    LED_Initialization();


    uint32_t bbb=0;
    uint32_t b=20000;
    uint32_t X=1500;
    uint32_t Y=1500;
    uint32_t Z=1500;
    
    while(1)
    {

    if(PushButton_Read())
    {      
      bbb=bbb+1;

      if(bbb==4)
        bbb=1;
      Delay_1us(200000);
      
    }
     
    if(bbb==1)
    {
      X=1000; 
      Y=1500;
      Z=2000;    
    }

    if(bbb==2)
    {
      X=1500;
      Y=2000;
      Z=1000;
    }

    if(bbb==3)
    {
      X=2000;       
      Y=1000;
      Z=1500;
    }
    
    
    GPIO_SetBits(GPIOD, GPIO_Pin_8);
    Delay_1us(X);
    GPIO_ResetBits(GPIOD, GPIO_Pin_8);
    Delay_1us(b-X);

    GPIO_SetBits(GPIOD, GPIO_Pin_10);
    Delay_1us(Y);
    GPIO_ResetBits(GPIOD, GPIO_Pin_10);
    Delay_1us(b-Y);

    GPIO_SetBits(GPIOD, GPIO_Pin_12);
    Delay_1us(Z);
    GPIO_ResetBits(GPIOD, GPIO_Pin_12);
    Delay_1us(b-Z);

    }

    while(1); // Don't want to exit
}
