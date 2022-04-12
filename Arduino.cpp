/**
 * @file Arduino.cpp
 * @brief 虽然名字叫Arduino，实际是为了适配Arduino工程
 * 真到了Arduino平台的时候，需要用Arduino自带的
 * 
 */
#include "Arduino.h"

//需要cubemx配置时SYS时基源选择SysTick,否则无效
__weak void HAL_Delay_us(uint32_t us)//延时us函数
{
    __IO uint32_t currentTicks = SysTick->VAL;
  /* Number of ticks per millisecond */
  const uint32_t tickPerMs = SysTick->LOAD + 1;
  /* Number of ticks to count */
  const uint32_t nbTicks = ((us - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000;
  /* Number of elapsed ticks */
  uint32_t elapsedTicks = 0;
  __IO uint32_t oldTicks = currentTicks;
  do {
    currentTicks = SysTick->VAL;
    elapsedTicks += (oldTicks < currentTicks) ? tickPerMs + oldTicks - currentTicks :
                    oldTicks - currentTicks;
    oldTicks = currentTicks;
  } while (nbTicks > elapsedTicks);
}


#if SYS_Delay_Tim_En//使用Tim来延时ms或us
	void HAL_TimDelay_us(uint32_t us)//延时us函数
	{
		__HAL_TIM_SetCounter(&US_TIM,0);//清零
		HAL_TIM_Base_Start(&US_TIM);//开启定时器
		while(__HAL_TIM_GetCounter(&US_TIM)<us){};
		HAL_TIM_Base_Stop(&US_TIM);//关闭定时器
	}
	void HAL_TimDelay_ms(uint32_t ms)//延时ms函数
	{
		for(uint32_t Delay_Cnt=0;Delay_Cnt<ms;Delay_Cnt++)
		HAL_TimDelay_us(998);//考虑实际代码运行损耗，不写1000，差一点点补上
	}
#endif

//获取系统时间，单位ms
__weak uint32_t millis(void)
{
	return HAL_GetTick();
}


__STATIC_INLINE uint32_t GXT_SYSTICK_IsActiveCounterFlag(void)
{
  return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk));
}
static uint32_t getCurrentMicros(void)
{
  /* Ensure COUNTFLAG is reset by reading SysTick control and status register */
  GXT_SYSTICK_IsActiveCounterFlag();
  uint32_t m = HAL_GetTick();
  const uint32_t tms = SysTick->LOAD + 1;
  __IO uint32_t u = tms - SysTick->VAL;
  if (GXT_SYSTICK_IsActiveCounterFlag()) {
    m = HAL_GetTick();
    u = tms - SysTick->VAL;
  }
  return (m * 1000 + (u * 1000) / tms);
}
//获取系统时间，单位us
__weak uint32_t micros(void)
{
  return getCurrentMicros();
}




void digitalWrite(uint8_t dwNum,uint8_t i)
{
  HAL_GPIO_WritePin(GetGPIOx(dwNum),GetGPIO_PIN_x(dwNum),(GPIO_PinState)i);
}

uint8_t digitalRead(uint8_t dwNum)
{
  return HAL_GPIO_ReadPin(GetGPIOx(dwNum),GetGPIO_PIN_x(dwNum));
}

void digitalToggle(uint8_t dwNum)
{
  HAL_GPIO_TogglePin(GetGPIOx(dwNum),GetGPIO_PIN_x(dwNum));
}

/**
 * @brief 获取GPIOx
 * 
 * @param dwNum 例PC13
 * @return GPIOC
 */
GPIO_TypeDef* GetGPIOx(uint8_t dwNum)
{
  GPIO_TypeDef * GPIOX;
  
  switch (dwNum>>4)
  {
  #ifdef GPIOA
  case 0:GPIOX=GPIOA;  break;
  #endif
  #ifdef GPIOB
  case 1:GPIOX=GPIOB;  break;
  #endif
  #ifdef GPIOC
  case 2:GPIOX=GPIOC;  break;
  #endif
  #ifdef GPIOD
  case 3:GPIOX=GPIOD;  break;
  #endif
  #ifdef GPIOE
  case 4:GPIOX=GPIOE;  break;
  #endif
  #ifdef GPIOF
  case 5:GPIOX=GPIOF;  break;
  #endif
  #ifdef GPIOG
  case 6:GPIOX=GPIOG;  break;
  #endif
  #ifdef GPIOH
  case 7:GPIOX=GPIOH;  break;
  #endif
  #ifdef GPIOI
  case 8:GPIOX=GPIOI;  break;
  #endif
  #ifdef GPIOJ
  case 9:GPIOX=GPIOJ;  break;
  #endif
  #ifdef GPIOK
  case 10:GPIOX=GPIOK;  break;
  #endif
  default:return 0;break;
  }
  return GPIOX;
}

/**
 * @brief 获取GPIO_PIN_X
 * 
 * @param dwNum 比如PC13
 * @return GPIO_PIN_13
 */
uint16_t GetGPIO_PIN_x(uint8_t dwNum)
{
  uint16_t  GPIO_PIN_X;
  // switch (dwNum&0x0F)
  // {
  // case 0:GPIO_PIN_X=GPIO_PIN_0;   break;
  // case 1:GPIO_PIN_X=GPIO_PIN_1;   break;
  // case 2:GPIO_PIN_X=GPIO_PIN_2;   break;
  // case 3:GPIO_PIN_X=GPIO_PIN_3;   break;
  // case 4:GPIO_PIN_X=GPIO_PIN_4;   break;
  // case 5:GPIO_PIN_X=GPIO_PIN_5;   break;
  // case 6:GPIO_PIN_X=GPIO_PIN_6;   break;
  // case 7:GPIO_PIN_X=GPIO_PIN_7;   break;
  // case 8:GPIO_PIN_X=GPIO_PIN_8;   break;
  // case 9:GPIO_PIN_X=GPIO_PIN_9;   break;
  // case 10:GPIO_PIN_X=GPIO_PIN_10;  break;
  // case 11:GPIO_PIN_X=GPIO_PIN_11;  break;
  // case 12:GPIO_PIN_X=GPIO_PIN_12;  break;
  // case 13:GPIO_PIN_X=GPIO_PIN_13;  break;
  // case 14:GPIO_PIN_X=GPIO_PIN_14;  break;
  // case 15:GPIO_PIN_X=GPIO_PIN_15;  break;
  // default:return;break;
  // }
  //以下三句话效果等效于上面switch
  uint8_t pinnum=dwNum&0x0F;
  if(pinnum>15 || pinnum<0)  return 0;
  GPIO_PIN_X= 0x01<<(pinnum&0x0F);

  return GPIO_PIN_X;
}

void randomSeed(unsigned long seed)
{
  if (seed != 0) {
    srand(seed);
  }
}

long random(long howbig)
{
  randomSeed(micros());
  if (howbig == 0) {
    return 0;
  }
  return rand() % howbig;
}

long random(long howsmall, long howbig)
{
  if (howsmall >= howbig) {
    return howsmall;
  }
  long diff = howbig - howsmall;
  return random(diff) + howsmall;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
