/**
 * @file gtask.cpp
 * 
 * @brief ������������������������loopѭ����
 * ��Ҫһ��1000hz��ʱ����ÿ��ֵ+1
 * ��Բ�ͬƽ̨��ʱ���׼��Ҫ�궨��millis��������Arduinoƽ̨��Ҫ����Arduinoͷ�ļ�
 * stm32ƽ̨������HAL_GetTick����
 * ����stm32����������ֲƽ̨��ע�������ʱ���׼����
 * 
 * @note ʹ�÷���-��ArduinoΪ��
 * 
 * #define SchRun_GetSysTimMs() millis()//����ʱ���׼����
 * 
 * void setup()
 * {
 *  Scheduler_Setup();
 * }
 * 
 * void loop()
 * {
 *  Scheduler_Run();
 * }
 * //����ͻ�ִ��Loop_xxxHz������
 * 
 */

#include "gtask.h"
#include "stdint.h"
#include "Arduino.h"
#include "gDebug.h"
#include "gScope.h"
#include "gmain.h"
#include "gsource.h"
//****************************************************
//����ִ��
//****************************************************
static void Loop_1000Hz(void)	//1msִ��һ��
{
	
}

static void Loop_500Hz(void)	//2msִ��һ��
{

}

static void Loop_200Hz(void)	//5msִ��һ��
{

}

static void Loop_100Hz(void)	//10msִ��һ��
{
	
}

static void Loop_50Hz(void)	//20msִ��һ��
{	
	
}

static void Loop_20Hz(void)	//50msִ��һ��
{
	
}

static void Loop_2Hz(void)	//500msִ��һ��
{
	
}

static void Loop_1Hz(void)	//500msִ��һ��
{
	
}



//****************************************************
//�������д��벻��Ҫ�޸�
//****************************************************
typedef struct
{
void(*task_func)(void);
uint16_t rate_hz;
uint16_t interval_ticks;
uint32_t last_run;
}sched_task_t;

//ϵͳ�������ã�������ִͬ��Ƶ�ʵġ��̡߳�
static sched_task_t sched_tasks[] = 
{
	{Loop_1000Hz,1000,  0, 0},
	{Loop_500Hz , 500,  0, 0},
	{Loop_200Hz , 200,  0, 0},
	{Loop_100Hz , 100,  0, 0},
	{Loop_50Hz  ,  50,  0, 0},
	{Loop_20Hz  ,  20,  0, 0},
	{Loop_2Hz   ,   2,  0, 0},
  	{Loop_1Hz   ,   1,  0, 0},
};

//�������鳤�ȣ��ж��߳�����
#define TASK_NUM (sizeof(sched_tasks)/sizeof(sched_task_t))

//����ÿ���ʱ��
#define TICK_PER_SECOND	1000

//��ʼ��
void Scheduler_Setup(void)
{
	uint8_t index = 0;
	//��ʼ�������
	for(index=0;index < TASK_NUM;index++)
	{
		//����ÿ���������ʱ������
		sched_tasks[index].interval_ticks = TICK_PER_SECOND/sched_tasks[index].rate_hz;
		//�������Ϊ1��Ҳ����1ms
		if(sched_tasks[index].interval_ticks < 1)
		{
			sched_tasks[index].interval_ticks = 1;
		}
	}
}


/**
 * @brief ��������ŵ�main������while(1)�У���ͣ�ж��Ƿ����߳�Ӧ��ִ��
 * 
 */
void Scheduler_Run(void)
{
	uint8_t index = 0;
	//ѭ���ж������̣߳��Ƿ�Ӧ��ִ��
	for(index=0;index < TASK_NUM;index++)
	{
		//��ȡϵͳ��ǰʱ�䣬��λMS
		uint32_t tnow = millis();
		//�����жϣ������ǰʱ���ȥ��һ��ִ�е�ʱ�䣬���ڵ��ڸ��̵߳�ִ�����ڣ���ִ���߳�
		if(tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
		{
			//�����̵߳�ִ��ʱ�䣬������һ���ж�
			sched_tasks[index].last_run = tnow;
			//ִ���̺߳�����ʹ�õ��Ǻ���ָ��
			sched_tasks[index].task_func();
		}	 
	}
}