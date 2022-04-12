/**
 * @brief GXT-发送驱动包
 * 重写了printf函数--gDebug()
 * 重写了cout<<endl--gDebug()
 * 方便移植，不和stm32绑定，提供重写函数
 * 
 * @note 使用前必须做的：
 * 由于使用gDebug()最终都会调用gDebugSendString(弱定义)发送，所以我们需要
 * 重写函数 void gDebugSendString(const char *str, int num)
 * 例：
 * void gDebugSendString(const char *str, int num)
 * {
 *      //stm32
 *      CDC_Transmit_FS((uint8_t*)str,num); // 调用 USB CDC函数发送数据
 *      HAL_UART_Transmit(&huart1,(uint8_t *)str,num,0xFFFF);//hal库发送数据
 * 
 *      //arduino
 *      Serial.write(str,num);
 * }
 * 
 * @note 使用示例
 * void loop()
 * {
 *      gDebug("hello world\n");
 *      gDebug()<< "hello" << "world" <<endl;
 *      delay(500);
 * }
 * 
 * @note 额外的：
 * 当我们想要禁用debug时，可以定义宏G_NO_DEBUG_OUTPUT
 */




#include "gDebug.h"
#include "stdarg.h"
#include "string.h"
#include "stdio.h"

static char _gDebugBuf[256] = {0};//发送缓冲区

__attribute__((weak)) void gDebugSendString(const char *str, int num) 
{
  ;
}

// extern gRingFifo cdcFifo;
// //用户更改实现方式，0串口直接堵塞发送，1usbcdc直接发送，2存入发送缓冲区
// #define SendMethod 2
// __weak void _gDebugSendString(const char *str, int num)
// {
// #if (SendMethod==0)//串口堵塞发送
//   HAL_UART_Transmit(&SEND_USART,(uint8_t *)_gDebugBuf,i,0xFFFF);//hal库发送数据

// #elif (SendMethod==1)//USBCDC发送
//   //FIXME:使用usbcdc需要在CDC_Transmit_FS函数开头加入以下代码
// /************************************************************
//   USBD_CDC_HandleTypeDef *ghcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;// 获取CDC状态
// 	long long	int Tickstart = HAL_GetTick();		// 获取当前时间
//   while(ghcdc->TxState !=0)		// 等待发送完成
//   {
//     if((HAL_GetTick() - Tickstart) > 10){break;} 	// 设置阻塞最大时长10ms
//   }
// *************************************************************/
//   CDC_Transmit_FS((uint8_t*)str,num); // 调用 USB CDC函数发送数据

// #elif (SendMethod==2)//存入发送缓冲区
//   //Ring_Fifo_Write_String(cdcFifo.getHandle(),(uint8_t*)str,num);
//   cdcFifo.push((uint8_t *)str,num);
// #endif
// }

void gDebug(const char* fmt, ...)
{
#ifdef G_NO_DEBUG_OUTPUT //是否禁用调试功能
    return;
#endif // G_NO_DEBUG_OUTPUT
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)_gDebugBuf, fmt, ap);
    va_end(ap);
    uint16_t i = strlen((const char *)_gDebugBuf); //此次发送数据的长度
    gDebugSendString(_gDebugBuf, i);
}

/**********C++版本，重载/<<版本**************************************************************************************/
#if G_DEBUG_CPP_EN
static gxtCout gcout={};//实现自己的cout
struct gxtEndl {}endl;//定义结构体，实现对于endl特殊识别
gxtCout& gDebug(void)//函数重载
{
    return gcout;
}
//***********重载*************
// void gDebug(std::string &str){
//     gDebug("%s", str.c_str());
// }
void gDebug(char c){
    gDebug("%c", c);
}
// void gDebug(const char *str){
//     gDebug("%s", str);
// }
void gDebug(short number){
    gDebug("%d", number);
}
void gDebug(int number){
    gDebug("%d", number);
}
void gDebug(uint32_t number){
    gDebug("%d", number);
}
void gDebug(uint16_t number){
    gDebug("%d", number);
}
void gDebug(uint8_t number){
    gDebug("%d", number);
}
// void gDebug(uint8_t *string){
//     gDebug("%s", string);
// }
// void gDebug(char *string){
//     gDebug("%s", string);
// }
void gDebug(long number){
    gDebug("%d", number); 
}
void gDebug(float number){
    gDebug("%f", number);
}
void gDebug(double number){
    gDebug("%f", number);
}
void gDebug(const char str[]){
    gDebug("%s", str);
}

#endif
