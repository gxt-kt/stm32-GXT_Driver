/**
 * @file gtask.cpp
 * 
 * @brief 软件任务调度器，放在主函数loop循环中
 * 需要一个1000hz定时器，每次值+1
 * 针对不同平台，时间基准需要宏定义millis函数，在Arduino平台需要包含Arduino头文件
 * stm32平台可以用HAL_GetTick函数
 * 不绑定stm32，可任意移植平台，注意上面的时间基准就行
 * 
 * @note 使用方法-以Arduino为例
 * 
 * #define SchRun_GetSysTimMs() millis()//定义时间基准函数
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
 * //后面就会执行Loop_xxxHz函数了
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
//函数执行
//****************************************************
static void Loop_1000Hz(void)	//1ms执行一次
{
	
}

static void Loop_500Hz(void)	//2ms执行一次
{

}

static void Loop_200Hz(void)	//5ms执行一次
{

}

static void Loop_100Hz(void)	//10ms执行一次
{
	
}

static void Loop_50Hz(void)	//20ms执行一次
{	
	
}

static void Loop_20Hz(void)	//50ms执行一次
{
	
}

static void Loop_2Hz(void)	//500ms执行一次
{
	
}

static void Loop_1Hz(void)	//500ms执行一次
{
	
}



//****************************************************
//以下所有代码不需要修改
//****************************************************
typedef struct
{
void(*task_func)(void);
uint16_t rate_hz;
uint16_t interval_ticks;
uint32_t last_run;
}sched_task_t;

//系统任务配置，创建不同执行频率的“线程”
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

//根据数组长度，判断线程数量
#define TASK_NUM (sizeof(sched_tasks)/sizeof(sched_task_t))

//定义每秒计时数
#define TICK_PER_SECOND	1000

//初始化
void Scheduler_Setup(void)
{
	uint8_t index = 0;
	//初始化任务表
	for(index=0;index < TASK_NUM;index++)
	{
		//计算每个任务的延时周期数
		sched_tasks[index].interval_ticks = TICK_PER_SECOND/sched_tasks[index].rate_hz;
		//最短周期为1，也就是1ms
		if(sched_tasks[index].interval_ticks < 1)
		{
			sched_tasks[index].interval_ticks = 1;
		}
	}
}


/**
 * @brief 这个函数放到main函数的while(1)中，不停判断是否有线程应该执行
 * 
 */
void Scheduler_Run(void)
{
	uint8_t index = 0;
	//循环判断所有线程，是否应该执行
	for(index=0;index < TASK_NUM;index++)
	{
		//获取系统当前时间，单位MS
		uint32_t tnow = millis();
		//进行判断，如果当前时间减去上一次执行的时间，大于等于该线程的执行周期，则执行线程
		if(tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
		{
			//更新线程的执行时间，用于下一次判断
			sched_tasks[index].last_run = tnow;
			//执行线程函数，使用的是函数指针
			sched_tasks[index].task_func();
		}	 
	}
}