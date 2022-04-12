/**
 * @brief GXT-����������
 * ��д��printf����--gDebug()
 * ��д��cout<<endl--gDebug()
 * ������ֲ������stm32�󶨣��ṩ��д����
 * 
 * @note ʹ��ǰ�������ģ�
 * ����ʹ��gDebug()���ն������gDebugSendString(������)���ͣ�����������Ҫ
 * ��д���� void gDebugSendString(const char *str, int num)
 * ����
 * void gDebugSendString(const char *str, int num)
 * {
 *      //stm32
 *      CDC_Transmit_FS((uint8_t*)str,num); // ���� USB CDC������������
 *      HAL_UART_Transmit(&huart1,(uint8_t *)str,num,0xFFFF);//hal�ⷢ������
 * 
 *      //arduino
 *      Serial.write(str,num);
 * }
 * 
 * @note ʹ��ʾ��
 * void loop()
 * {
 *      gDebug("hello world\n");
 *      gDebug()<< "hello" << "world" <<endl;
 *      delay(500);
 * }
 * 
 * @note ����ģ�
 * ��������Ҫ����debugʱ�����Զ����G_NO_DEBUG_OUTPUT
 */




#include "gDebug.h"
#include "stdarg.h"
#include "string.h"
#include "stdio.h"

static char _gDebugBuf[256] = {0};//���ͻ�����

__attribute__((weak)) void gDebugSendString(const char *str, int num) 
{
  ;
}

// extern gRingFifo cdcFifo;
// //�û�����ʵ�ַ�ʽ��0����ֱ�Ӷ������ͣ�1usbcdcֱ�ӷ��ͣ�2���뷢�ͻ�����
// #define SendMethod 2
// __weak void _gDebugSendString(const char *str, int num)
// {
// #if (SendMethod==0)//���ڶ�������
//   HAL_UART_Transmit(&SEND_USART,(uint8_t *)_gDebugBuf,i,0xFFFF);//hal�ⷢ������

// #elif (SendMethod==1)//USBCDC����
//   //FIXME:ʹ��usbcdc��Ҫ��CDC_Transmit_FS������ͷ�������´���
// /************************************************************
//   USBD_CDC_HandleTypeDef *ghcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;// ��ȡCDC״̬
// 	long long	int Tickstart = HAL_GetTick();		// ��ȡ��ǰʱ��
//   while(ghcdc->TxState !=0)		// �ȴ��������
//   {
//     if((HAL_GetTick() - Tickstart) > 10){break;} 	// �����������ʱ��10ms
//   }
// *************************************************************/
//   CDC_Transmit_FS((uint8_t*)str,num); // ���� USB CDC������������

// #elif (SendMethod==2)//���뷢�ͻ�����
//   //Ring_Fifo_Write_String(cdcFifo.getHandle(),(uint8_t*)str,num);
//   cdcFifo.push((uint8_t *)str,num);
// #endif
// }

void gDebug(const char* fmt, ...)
{
#ifdef G_NO_DEBUG_OUTPUT //�Ƿ���õ��Թ���
    return;
#endif // G_NO_DEBUG_OUTPUT
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)_gDebugBuf, fmt, ap);
    va_end(ap);
    uint16_t i = strlen((const char *)_gDebugBuf); //�˴η������ݵĳ���
    gDebugSendString(_gDebugBuf, i);
}

/**********C++�汾������/<<�汾**************************************************************************************/
#if G_DEBUG_CPP_EN
static gxtCout gcout={};//ʵ���Լ���cout
struct gxtEndl {}endl;//����ṹ�壬ʵ�ֶ���endl����ʶ��
gxtCout& gDebug(void)//��������
{
    return gcout;
}
//***********����*************
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
