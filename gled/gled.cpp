
/**
 * @brief GXT-LED驱动库，使用C++,高度绑定stm32hal库，不易移植到其它平台
 * 
 */

//**********************************************************************
//单色led
//**********************************************************************
/*
LED_SINGLE ledsingle(GPIOA,GPIO_PIN_0,0);//绑定led,PA0,共阴

void test(){
    ledsingle.toggle();
    HAL_Delay(500);
}
*/

//**********************************************************************
//rgbled
//**********************************************************************
/*
LED_RGB ledrgb(GPIOA,GPIO_PIN_0,GPIOA,GPIO_PIN_1,GPIOA,GPIO_PIN_2,0);//绑定led PA0-R,PA1-G,PA2-B,共阴
void test(){
    ledrgb.setColor(LEDRGB::cyan);
    HAL_Delay(500);
    ledrgb.setColor(LEDRGB::black);
    HAL_Delay(500);
}
*/


#include "gled.h"


//设置led，1打开，0关闭
void LED_SINGLE::setSta(bool i)
{
    if(i==0) close();
    else open();
}

//打开led
void LED_SINGLE::open()
{
    if (Blinkstandard == 0)
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin_x, (GPIO_PinState)1);
    else
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin_x, (GPIO_PinState)0);
}

//关闭led
void LED_SINGLE::close()
{
    if (Blinkstandard == 0)
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin_x, (GPIO_PinState)0);
    else
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin_x, (GPIO_PinState)1);
}

//翻转led
void LED_SINGLE::toggle()
{
    HAL_GPIO_TogglePin(GPIOx,GPIO_Pin_x);
}

//获取当前是亮还是灭,1亮0灭
bool LED_SINGLE::getCurrentState()
{
    if(Blinkstandard==0) return ((bool)HAL_GPIO_ReadPin(GPIOx,GPIO_Pin_x));
    else return (!(bool)HAL_GPIO_ReadPin(GPIOx,GPIO_Pin_x));
}

//设置共阴共阳，0共阴，1共阳
void LED_SINGLE::setBlinkStandard(bool i)
{
    Blinkstandard = i;
}

//绑定引脚
void LED_SINGLE::setPin(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin)
{
    GPIOx=GPIO;
    GPIO_Pin_x=GPIO_Pin;
}

namespace LEDRGB {
    std::vector<uint8_t> black=  {0,0,0};
    std::vector<uint8_t> white=  {1,1,1};
    std::vector<uint8_t> red=    {1,0,0};
    std::vector<uint8_t> green=  {0,1,0};
    std::vector<uint8_t> blue=   {0,0,1};
    std::vector<uint8_t> yellow= {1,1,0};
    std::vector<uint8_t> cyan=   {0,1,1};
    std::vector<uint8_t> magenta={1,0,1};
};

// bool LEDRGB::black[3]=  {0,0,0};
// bool LEDRGB::white[3]=  {1,1,1};
// bool LEDRGB::red[3]=    {1,0,0};
// bool LEDRGB::green[3]=  {0,1,0};
// bool LEDRGB::blue[3]=   {0,0,1};
// bool LEDRGB::yellow[3]= {1,1,0};
// bool LEDRGB::cyan[3]=   {0,1,1};
// bool LEDRGB::magenta[3]={1,0,1};



void LED_RGB::setColor(bool colorR,bool colorG,bool colorB){
    this->ledR->setSta(colorR);
    this->ledG->setSta(colorG);
    this->ledB->setSta(colorB);
}
void LED_RGB::setColor(std::vector<uint8_t> color){
    this->ledR->setSta(color[0]);
    this->ledG->setSta(color[1]);
    this->ledB->setSta(color[2]);
}
// void LED_RGB::setColor(bool* color){
//     this->ledR->setSta(color[0]);
//     this->ledG->setSta(color[1]);
//     this->ledB->setSta(color[2]);
// }

void LED_RGB::toggle(void){
    static bool togSta;
    if(togSta) setColor(LEDRGB::white);
    else setColor(LEDRGB::black);
    togSta=!togSta;
}

void LED_RGB::turn(void){
    static uint8_t turnNum;
    turnNum++;
    turnNum=turnNum%8;
    setColor(turnNum&0B001,turnNum&0B010,turnNum&0B100);
}