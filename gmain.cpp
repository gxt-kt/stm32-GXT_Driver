#include "gmain.h"
#include "gtask.h"
#include "gsource.h"
#include "gDebug.h"

//

//
void setup(void){
	//�������
	Scheduler_Setup();
}



void loop(void){
	Scheduler_Run();
}

