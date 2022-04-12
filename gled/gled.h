#ifndef GXT_KT_GPIO_H__
#define GXT_KT_GPIO_H__

#include "Arduino.h"

//��ɫled
class LED_SINGLE
{
public:
  //��ʼ��led��GPIO:gpiox��GPIO_Pin��gpioPin��i��0������1����
  LED_SINGLE(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin,bool i){
    GPIOx=GPIO;
    GPIO_Pin_x=GPIO_Pin;
    Blinkstandard=i;
  }
  void setSta(bool i);
  void open();
  void close();
  void toggle();
  void setBlinkStandard(bool i);
  bool getCurrentState();
  void setPin(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin);
private:
  GPIO_TypeDef *GPIOx;
  uint16_t GPIO_Pin_x;
  bool Blinkstandard;
};

////led��ɫ�ռ�
#include "vector"
namespace LEDRGB {
    extern std::vector<uint8_t> black;
    extern std::vector<uint8_t> white;
    extern std::vector<uint8_t> red;
    extern std::vector<uint8_t> green;
    extern std::vector<uint8_t> blue;
    extern std::vector<uint8_t> yellow;
    extern std::vector<uint8_t> cyan;
    extern std::vector<uint8_t> magenta;
};
//led��ɫ�ռ�
// class LEDRGB{
// public:
//     static bool black[];
//     static bool white[];
//     static bool red[];
//     static bool green[];
//     static bool blue[];
//     static bool yellow[];
//     static bool cyan[];
//     static bool magenta[];
// };

//3ɫRGBled
class LED_RGB
{
public:
  //R-G-B-����������0������1����
  LED_RGB(GPIO_TypeDef *GPIO_R,uint16_t GPIO_Pin_R,GPIO_TypeDef *GPIO_G,\
  uint16_t GPIO_Pin_G,GPIO_TypeDef *GPIO_B,uint16_t GPIO_Pin_B,bool i)
  {
    ledR=new LED_SINGLE(GPIO_R,GPIO_Pin_R,i);
    ledG=new LED_SINGLE(GPIO_G,GPIO_Pin_G,i);
    ledB=new LED_SINGLE(GPIO_B,GPIO_Pin_B,i);
  }
  //~LED_RGB(){};
  //������ɫ ����setColor(1,0,1);
  void setColor(bool colorR,bool colorG,bool colorB);
  //������ɫ ����setColor(LEDRGB::cyan);
  void setColor(std::vector<uint8_t> color);
  //void setColor(bool* color);

  //��ɫ��ɫ��ת
  void toggle(void);

  //������ʾ
  void turn(void);
private:
  LED_SINGLE *ledR;
  LED_SINGLE *ledG;
  LED_SINGLE *ledB;
};

#endif // !GXT_KT_GPIO_H__