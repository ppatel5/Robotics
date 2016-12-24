/* sonartest.c */ 
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "stdlib.h"

/*
 * LEJOS NXJ development team fixed I2C issue on port S4.
 * This sample also verifies the updated I2C comm. functionality
 */
#define PORT_IN_USE_CD NXT_PORT_S3
#define PORT_IN_USE_TL NXT_PORT_S1
#define PORT_IN_USE_TR NXT_PORT_S4
#define PORT_IN_USE_CF NXT_PORT_S2

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
int tsl; //touch sensor left
int tsr; //touch sensor right
U8 goalReached = 2;  //set to 0 when cube is found
U8 cubefound = 0;   //set to 1 when cube is lifted
U8 cubereleased = 0;  //set to 1 when cube is released
U8 cubeRed = 0;   //to identify color of cube
U8 cubeBlue = 0;
U8 finaltile = 0;   //set to one when goal (where cube is to be dropped) is reached

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
	ecrobot_init_i2c(PORT_IN_USE_CD, LOWSPEED);
	ecrobot_init_i2c(PORT_IN_USE_CF, LOWSPEED);
}
void ecrobot_device_terminate()
{
	ecrobot_term_i2c(PORT_IN_USE_CD);
	ecrobot_term_i2c(PORT_IN_USE_CF);
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
void stop()
	{
		nxt_motor_set_speed(NXT_PORT_C,0,1);
		nxt_motor_set_speed(NXT_PORT_B,0,1);
		systick_wait_ms(500);
	}
void rotate180()
	{	
	int count=0;
		while(count<8)
		{
			nxt_motor_set_speed(NXT_PORT_C,-35,1);
			nxt_motor_set_speed(NXT_PORT_B,35,1);
			count++;
			systick_wait_ms(300);
		}
	stop();
	
	}
void back()
	{
		goalReached--;
		U8 count = 0;
		while(count<5)
		{
			nxt_motor_set_speed(NXT_PORT_C,50,1);
			nxt_motor_set_speed(NXT_PORT_B,50,1); count++;
		}
		stop();
	}
int backleft()
	{
		int count = 0;
		while(count<2)
		{
			nxt_motor_set_speed(NXT_PORT_C,40,1);
			nxt_motor_set_speed(NXT_PORT_B,40,1); count++; disp(5, "count",count); systick_wait_ms(300); 
		}

		stop();
		count = 0; 
		while(count<(rand()%5+2))
		{
			nxt_motor_set_speed(NXT_PORT_C,35,1);
			nxt_motor_set_speed(NXT_PORT_B,-35,1); count++; disp(5, "count",count); systick_wait_ms(300);
		}
		
	return 1;
}
int backright()
	{
		int count = 0;
		while(count<2)
		{
			nxt_motor_set_speed(NXT_PORT_C,40,1);
			nxt_motor_set_speed(NXT_PORT_B,40,1); count++; disp(5, "count",count); systick_wait_ms(300); 
		}

		stop();
		count = 0; 
		while(count<(rand()%5+2))
		{
			nxt_motor_set_speed(NXT_PORT_C,-35,1);
			nxt_motor_set_speed(NXT_PORT_B,35,1); count++; disp(5, "count",count); systick_wait_ms(300);
		}
		
	return 1;
}
void run()
	{
		nxt_motor_set_speed(NXT_PORT_C,-35,1);
		nxt_motor_set_speed(NXT_PORT_B,-35,1);
		
	}
void lilforward(){
	int count =0;
	while(count<2){
	nxt_motor_set_speed(NXT_PORT_C,-50,1);
	nxt_motor_set_speed(NXT_PORT_B,-50,1);
	//disp(5, "forward_count",count); 
	systick_wait_ms(300);count++;}
	stop();
}	
void lilback(){
	int count =0;
	while(count<2){
	nxt_motor_set_speed(NXT_PORT_C,50,1);
	nxt_motor_set_speed(NXT_PORT_B,50,1);
	//disp(5, "back_count",count);
	systick_wait_ms(300);count++;}
	stop();
}
void rotate10(){
	int count =0;
	while(count<5){
	nxt_motor_set_speed(NXT_PORT_C,-30,1);
	nxt_motor_set_speed(NXT_PORT_B,30,1);
	//disp(5, "rotate_count",count);
	 systick_wait_ms(300);count++;}
	stop();
}
int getRandom(int min, int max)
	{
		return min + (int)((double)rand()*(max-min+1.0)/(1.0+RAND_MAX));
	}
	
void clawclock()
{
	//nxt_motor_set_speed(NXT_PORT_B,0,1);nxt_motor_set_speed(NXT_PORT_C,0,1);
	int count = 0;
	while(count<25)
	{
		nxt_motor_set_speed(NXT_PORT_A,-45,1);
		disp(5, "c_claw ", count);
		systick_wait_ms(300);
		nxt_motor_set_speed(NXT_PORT_A,0,1);
		//display_clear(0);
		count++;
	}

}
void clawanticlock()
{
	//nxt_motor_set_speed(NXT_PORT_B,0,1);nxt_motor_set_speed(NXT_PORT_C,0,1);
	int count = 0;
	while(count<27)
	{
		nxt_motor_set_speed(NXT_PORT_A,45,1);
		disp(6, "ac_claw ", count);
		systick_wait_ms(300);
		nxt_motor_set_speed(NXT_PORT_A,0,1);
		//display_clear(0);
		count++;
	}
}
/* Task1 executed every 50msec */
TASK(Task1)
	{
	S16 data[3]; //color data for floor; needs to be local; problem occurs if global
	S16 datacube[3]; //color sensor to detect cube color
	U8 count = 0, flag = 1, falseFlag=0;
	tsl=ecrobot_get_touch_sensor(PORT_IN_USE_TL);
	tsr=ecrobot_get_touch_sensor(PORT_IN_USE_TR);
	data[0] = 10, data[1] = 10, data[2] = 10;  

	while(goalReached && !cubereleased){
		
		if(data[0] <= 1 && data[1] <= 1 && data[2] <= 1)
		{
		//disp(1,"if1",finaltile);	
			goalReached--;
			break;
		}
		tsl=ecrobot_get_touch_sensor(PORT_IN_USE_TL);
		tsr=ecrobot_get_touch_sensor(PORT_IN_USE_TR);
		ecrobot_get_color_sensor(PORT_IN_USE_CD, data);
		finaltile = 0;
		display_clear(0);
		disp(1,"R",data[0]); 
		disp(2,"G",data[1]);
		disp(3,"B",data[2]);

		systick_wait_ms(300);
		
		if((tsl==1) && flag==1) 
		{		flag = 0;
				flag = backright();
				stop();		
		}
		else if(tsr==1 && flag==1)
		{		systick_wait_ms(300);
				flag = 0;
				flag = backleft();
				stop();		
		}		
		run();
		
	}
	stop();
	if(goalReached==0 && cubefound ==0)
	{
	//disp(2,"if2",finaltile);	
	// when the goal is reached and there is no cube:
		// go back to get off the black tile, rotate so that the claw faces the block
		//bring claw down and open it
		// while the color detected by the sensor on the claw is not red/ blue
			// move forward, get the color of the from the sensor, if red or blue-> grab it, else move back, 
			// and rotate a little bit
		//disp(5, "goal ",goalReached);
		systick_wait_ms(300);
		//lilback();
		rotate180();//rotate 180
		clawclock();//bring claw down //open claw
		ecrobot_get_color_sensor(PORT_IN_USE_CF, datacube);

		while(!cubefound)
		{
			lilforward();
			systick_wait_ms(2000);
			ecrobot_get_color_sensor(PORT_IN_USE_CF, datacube);

			if((datacube[0]>datacube[1] && datacube[0]>datacube[2]) || (datacube[2]>datacube[1] && datacube[2]>datacube[0]))
			{
				clawanticlock();
				cubefound=1;
				if(datacube[0] > datacube[2]){
					cubeRed = 1;
				}
				else{
					cubeBlue = 1;
				}
				break;
			}
			rotate10(); 
			systick_wait_ms(2000);
			ecrobot_get_color_sensor(PORT_IN_USE_CF, datacube);
			
			if((datacube[0]>datacube[1] && datacube[0]>datacube[2]) || (datacube[2]>datacube[1]&&datacube[2]>datacube[0]))
			{
				clawanticlock();
				cubefound=1;
				if(datacube[0] > datacube[2]){
					cubeRed = 1;
				}
				else{
					cubeBlue = 1;
				}
				break;
			}			
			else
			{
			lilback();
			systick_wait_ms(2000);
				ecrobot_get_color_sensor(PORT_IN_USE_CF, datacube);

				if((datacube[0]>datacube[1] && datacube[0]>datacube[2]) || (datacube[2]>datacube[1] && datacube[2]>datacube[0]))
				{
					clawanticlock();
					cubefound=1;
					if(datacube[0] > datacube[2]){
						cubeRed = 1;
					}
					else{
						cubeBlue = 1;
					}
					break;
				}	
			rotate10(); 
			systick_wait_ms(2000);
			ecrobot_get_color_sensor(PORT_IN_USE_CF, datacube);

			if((datacube[0]>datacube[1] && datacube[0]>datacube[2]) || (datacube[2]>datacube[1] && datacube[2]>datacube[0]))
			{
				clawanticlock();
				cubefound=1;
				if(datacube[0] > datacube[2]){
					cubeRed = 1;
				}
				else{
					cubeBlue = 1;
				}
				break;
			}
			}
			if(falseFlag>25){
				clawanticlock();
				rotate180();
				lilforward();
				lilforward();
				lilforward();
				break;
			}
			
		}
			stop();
	}
	
	while(cubefound){
	
	if(goalReached == 0  && cubefound == 1 && finaltile == 0)
	{
	//disp(3,"if3",flag);
			
		
		if(cubeRed==1){   // for red cube green tile
			ecrobot_get_color_sensor(PORT_IN_USE_CD, data);

			if(data[1] > data[0] && data[1] > data[2] && finaltile==0)
			{	
				//disp(1,"if",finaltile);		
				lilback();
				rotate180();
				clawclock();
				lilforward();
				clawanticlock();
				finaltile = 1;
				goalReached = 2;
				cubefound = 0;
				cubereleased = 0;
				cubeRed = 0;
				cubeBlue = 0;	
					
			}
			tsl=ecrobot_get_touch_sensor(PORT_IN_USE_TL);
			tsr=ecrobot_get_touch_sensor(PORT_IN_USE_TR);
			

			systick_wait_ms(300);
		
			if((tsl==1) && flag==1) 
			{		flag = 0;
					flag = backright();
					stop();		
			}
			else if(tsr==1 && flag==1)
			{		systick_wait_ms(300);
					flag = 0;
					flag = backleft();
					stop();		
			}		
			run();
		}
		if(cubeBlue==1){  // for blue cube blue tile
			ecrobot_get_color_sensor(PORT_IN_USE_CD, data);
			display_clear(0);
			disp(1,"Rt",data[0]); 
			disp(2,"Gt",data[1]);
			disp(3,"Bt",data[2]);
			if(data[2] > data[0] && data[2] > data[1] && finaltile==0)
			{				
				lilback();
				rotate180();
				clawclock();
				lilforward();
				clawanticlock();
				finaltile = 1;
				goalReached = 2;
				cubefound = 0;
				cubereleased = 0;
				cubeRed = 0;
				cubeBlue = 0;	
				break;	
			}
			tsl=ecrobot_get_touch_sensor(PORT_IN_USE_TL);
			tsr=ecrobot_get_touch_sensor(PORT_IN_USE_TR);

			systick_wait_ms(300);
		
			if((tsl==1) && flag==1) 
			{		flag = 0;
					flag = backright();
					stop();		
			}
			else if(tsr==1 && flag==1)
			{		systick_wait_ms(300);
					flag = 0;
					flag = backleft();
					stop();		
			}		
			run();
		}
	}
	}
TerminateTask();
	
}

TASK(Task2)
{


  TerminateTask();
}
