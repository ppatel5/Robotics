/* sonartest.c */ 
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "stdlib.h"

/*
 * LEJOS NXJ development team fixed I2C issue on port S4.
 * This sample also verifies the updated I2C comm. functionality
 */
#define PORT_IN_USE_C NXT_PORT_S3
#define PORT_IN_USE_TL NXT_PORT_S2
#define PORT_IN_USE_TR NXT_PORT_S1
#define PORT_IN_USE_S NXT_PORT_S4

void disp(U8 row, char *str, int val)
{
#define DISPLAY_ON
#ifdef DISPLAY_ON 
	//display_clear(0);
	display_goto_xy(0, row);
	display_string(str);
	display_int(val, 0);
	display_update();
#endif
}
int ls; //light sensor
int tsl; //touch sensor left
int tsr; //touch sensor right
int ss; //sonar sensor
U8 goalReached = 2;
/* Task1 executed every 50msec */

/* OSEK declaratio0
ns */
DeclareCounter(SysTimerCnt);
DeclareTask(Task1);
DeclareTask(Task2);

/* LEJOS OSEK hooks */
void ecrobot_device_initialize()
{
	nxt_motor_set_speed(NXT_PORT_C,0,1);
	nxt_motor_set_speed(NXT_PORT_B,0,1);
	nxt_motor_set_speed(NXT_PORT_A,0,1);
	//ecrobot_set_light_sensor_active(PORT_IN_USE_C);
	ecrobot_init_i2c(PORT_IN_USE_C, LOWSPEED);
	ecrobot_init_sonar_sensor(PORT_IN_USE_S); 
}
void ecrobot_device_terminate()
{
	//ecrobot_set_light_sensor_inactive(PORT_IN_USE_L);
	ecrobot_term_i2c(PORT_IN_USE_C);
	ecrobot_term_sonar_sensor(PORT_IN_USE_S); 
}

/* LEJOS OSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void)
{
  StatusType ercd;

  ercd = SignalCounter(SysTimerCnt); /* Increment OSEK Alarm Counter */
  if(ercd != E_OK)
  {
    ShutdownOS(ercd);
  }
}



void rotate()
	{
		
			nxt_motor_set_speed(NXT_PORT_C,-15,1);
			nxt_motor_set_speed(NXT_PORT_B,15,1);
	}


void stop()
	{
		nxt_motor_set_speed(NXT_PORT_C,0,1);
		nxt_motor_set_speed(NXT_PORT_B,0,1);
		systick_wait_ms(500);
	}

void back()
	{
		goalReached--;
		U8 count = 0;
		while(count<5)
		{
			nxt_motor_set_speed(NXT_PORT_C,-20,1);
			nxt_motor_set_speed(NXT_PORT_B,-20,1); count++;
		}
		stop();
	}


int backleft()
	{
		int count = 0;
		while(count<2)
		{
			nxt_motor_set_speed(NXT_PORT_C,-40,1);
			nxt_motor_set_speed(NXT_PORT_B,-40,1); count++; disp(5, "count",count); systick_wait_ms(300); 
		}

		stop();
		count = 0; 
		while(count<(rand()%3+2))
		{
			nxt_motor_set_speed(NXT_PORT_C,-35,1);
			nxt_motor_set_speed(NXT_PORT_B,35,1); count++; disp(5, "count",count); systick_wait_ms(300);
		}
		
	return 1;
}

int backright()
	{
		int count = 0;
		while(count<2)
		{
			nxt_motor_set_speed(NXT_PORT_C,-40,1);
			nxt_motor_set_speed(NXT_PORT_B,-40,1); count++; disp(5, "count",count); systick_wait_ms(300); 
		}

		stop();
		count = 0; 
		while(count<(rand()%3+2))
		{
			nxt_motor_set_speed(NXT_PORT_C,35,1);
			nxt_motor_set_speed(NXT_PORT_B,-35,1); count++; disp(5, "count",count); systick_wait_ms(300);
		}
		
	return 1;
}

void run()
	{
		nxt_motor_set_speed(NXT_PORT_C,35,1);
		nxt_motor_set_speed(NXT_PORT_B,35,1);
		//systick_wait_ms(500);
	}

TASK(Task1)
	{
	S16 data[3]; //color data; needs to be local; problem occurs if global
	U8 count = 0, flag = 1;
	//U8 revA=nxt_motor_get_count(NXT_PORT_A);
	tsl=ecrobot_get_touch_sensor(PORT_IN_USE_TL);
	tsr=ecrobot_get_touch_sensor(PORT_IN_USE_TR);

	data[0] = 10, data[1] = 10, data[2] = 10;  

	while(goalReached){
		
		if(data[0] <= 1 && data[1] <= 1 && data[2] <= 1)
		{
			goalReached--;
			//systick_wait_ms(1000);
			while(count<5)
			{
				nxt_motor_set_speed(NXT_PORT_C,-20,1);
				nxt_motor_set_speed(NXT_PORT_B,-20,1); 
				count++;
				disp(5, "count",count); systick_wait_ms(300);
			} 
			break;
		}
		tsl=ecrobot_get_touch_sensor(PORT_IN_USE_TL);
		tsr=ecrobot_get_touch_sensor(PORT_IN_USE_TR);
		ecrobot_get_color_sensor(PORT_IN_USE_C, data);
		display_clear(0);
		disp(1,"R",data[0]); 
		disp(2,"G",data[1]);
		disp(3,"B",data[2]);
		disp(4, "tsl",tsl);
		disp(5, "tsr",tsr);
		systick_wait_ms(300);
		
		if((tsl==1) && flag==1) 
		{		flag = 0;
				flag = backleft();
				stop();		
		}
		else if(tsr==1 && flag==1)
		{		flag = 0;
				flag = backright();
				stop();		
		}		
		run();
		
	}
	stop();
	if(goalReached==0)
	{
		ecrobot_sound_tone(500, 50000, 50);
		count = 0;
//while(1){
		//flipping
		while(nxt_motor_get_count(NXT_PORT_A)<1){
			nxt_motor_set_speed(NXT_PORT_A,75,1);
			disp(6, "count",nxt_motor_get_count(NXT_PORT_A));
		}
		systick_wait_ms(300);
		nxt_motor_set_count(NXT_PORT_A,0);
		nxt_motor_set_speed(NXT_PORT_A,0,1);
		
		while(nxt_motor_get_count(NXT_PORT_A)>-1){
			nxt_motor_set_speed(NXT_PORT_A,-75,1);
			disp(7, "count",nxt_motor_get_count(NXT_PORT_A));
		}
		systick_wait_ms(300);
		nxt_motor_set_speed(NXT_PORT_A,0,1);
		nxt_motor_set_count(NXT_PORT_A,0);
		//rotate left
		count = 0; 
		while(count<10)
		{
			nxt_motor_set_speed(NXT_PORT_C,-30,1);
			nxt_motor_set_speed(NXT_PORT_B,30,1); count++; disp(5, "count",count);systick_wait_ms(300);
		}
		count = 0; 
		//flipping
		while(nxt_motor_get_count(NXT_PORT_A)<1){
			nxt_motor_set_speed(NXT_PORT_A,75,1);
			disp(6, "count",nxt_motor_get_count(NXT_PORT_A));
		}
		systick_wait_ms(300);
		nxt_motor_set_count(NXT_PORT_A,0);
		nxt_motor_set_speed(NXT_PORT_A,0,1);
		
		while(nxt_motor_get_count(NXT_PORT_A)>-1){
			nxt_motor_set_speed(NXT_PORT_A,-75,1);
			disp(7, "count",nxt_motor_get_count(NXT_PORT_A));
		}
		systick_wait_ms(300);
		nxt_motor_set_speed(NXT_PORT_A,0,1);
		nxt_motor_set_count(NXT_PORT_A,0);
		count = 0; 
		//rotate right
		while(count<10)
		{
			nxt_motor_set_speed(NXT_PORT_C,30,1);
			nxt_motor_set_speed(NXT_PORT_B,-30,1); count++; disp(5, "count",count); systick_wait_ms(300);
		}
		//rotate right
		while(count<10)
		{
			nxt_motor_set_speed(NXT_PORT_C,30,1);
			nxt_motor_set_speed(NXT_PORT_B,-30,1); count++; disp(5, "count",count); systick_wait_ms(300);
		}
		//flipping
		while(nxt_motor_get_count(NXT_PORT_A)<1){
			nxt_motor_set_speed(NXT_PORT_A,75,1);
			disp(6, "count",nxt_motor_get_count(NXT_PORT_A));
		}
		systick_wait_ms(300);
		nxt_motor_set_count(NXT_PORT_A,0);
		nxt_motor_set_speed(NXT_PORT_A,0,1);
		
		while(nxt_motor_get_count(NXT_PORT_A)>-1){
			nxt_motor_set_speed(NXT_PORT_A,-75,1);
			disp(7, "count",nxt_motor_get_count(NXT_PORT_A));
		}
		systick_wait_ms(300);
		nxt_motor_set_speed(NXT_PORT_A,0,1);
		nxt_motor_set_count(NXT_PORT_A,0);
		//rotate left
		count = 0; 
		while(count<10)
		{
			nxt_motor_set_speed(NXT_PORT_C,-30,1);
			nxt_motor_set_speed(NXT_PORT_B,30,1); count++; disp(5, "count",count);systick_wait_ms(300);
		}
	//}
}
TerminateTask();
	
}

TASK(Task2)
{
	//disp(1,"R",data[0]);
	//disp(4,"TSL",tsl);
	//disp(5,"TSR",tsr); //systick_wait_ms(1600);
	//disp(4,"SS",ss);
	//disp(2,"G",data[1]);
	//disp(3,"B",data[2]); systick_wait_ms(300);
	//display_clear(0);
  //ecrobot_status_monitor("Sonar Test");

  TerminateTask();
}
