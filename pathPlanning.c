#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
//#include "course_info.h"

//#include "math.h"



void disp(U8 row, char *str, U8 val)
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


#define MAX_OBSTACLES 25 
U8 num_obstacles = 13; 
U8 a[9][20];

int speed = 20;
int revB =0, revC =0 ;


char min(U8 u,U8 d,U8 l,U8 r);
U8 max(U8 x, U8 y){ return x>y ? x*2 : y*2; }

U8 ceil1(float num) {
    U8 inum = (U8)num;
    if (num == (float)inum) {
        return inum;
    }
    return inum + 1;
}

char min(U8 u,U8 d,U8 l,U8 r){

    U8 min=u;
    char c='u';

    if(d<min){
        min=d;
        c='d';
    }
    if(l<min){
        min=l;
        c='l';
    }
    if(r<min){
        min=r;
        c='r';
    }

  return c;

}
void setZero(U8 temp){
disp(4, " :: ", temp);


	nxt_motor_set_count(NXT_PORT_B,0);
	nxt_motor_set_count(NXT_PORT_C,0);


}
void stop(int revB,int revC){
	nxt_motor_set_speed(NXT_PORT_C,0,1);
	nxt_motor_set_speed(NXT_PORT_B,0,1);

	nxt_motor_set_count(NXT_PORT_C,0);
	nxt_motor_set_count(NXT_PORT_B,0);

	//disp(2, " REVB: ", revB);	
	//disp(3, " REVC: ", revC);
	systick_wait_ms(1200);
	display_clear(0);
}
void move(int revB, int revC,int x_rot){
/*go in x direction*/

		while(revB < x_rot || revC < x_rot){
disp(1, " x_rot: ", x_rot);
		revB = nxt_motor_get_count(NXT_PORT_B);
		revC = nxt_motor_get_count(NXT_PORT_C);

		if(revB<revC){
			speed -=.02;
			nxt_motor_set_speed(NXT_PORT_C,speed,1);
			nxt_motor_set_speed(NXT_PORT_B,20,1);
		//disp(4, " if: ", revB);
		} 
		else if(revB>revC){
			speed -=.02;
			nxt_motor_set_speed(NXT_PORT_B,speed,1);
			nxt_motor_set_speed(NXT_PORT_C,20,1);
		//disp(4, " eif: ", revB);
		}
		else{
			speed = 20;
			nxt_motor_set_speed(NXT_PORT_B,20,1);
			nxt_motor_set_speed(NXT_PORT_C,20,1);
		//disp(4, " e: ", revB);
		}

		//disp(2, " REVB: ", revB);	
		//disp(3, " REVC: ", revC);

		}
stop(revB,revC);
}
void rotate180(int revB, int revC, U8 direction){
	
	
		while(revB<225)
		{
			revB = nxt_motor_get_count(NXT_PORT_B);
			revC = nxt_motor_get_count(NXT_PORT_C);

			nxt_motor_set_speed(NXT_PORT_C,-20,1);
			nxt_motor_set_speed(NXT_PORT_B,20,1);
			//disp(2, " REVW1B: ", revB);	
			//disp(3, " REVW1C: ", revC);
		}
	
		stop(revB,revC);

}
void moveBack(int revB, int revC,int x_rot){
/*go in x direction*/
	rotate180(revB,revC,1);
	move(revB,revC,420);

stop(revB,revC);
}




void rotate(int revB, int revC, U8 direction){
	
	if(direction == 1){
		revB=0;
		while(revB<116)
		{
			revB = nxt_motor_get_count(NXT_PORT_B);
			revC = nxt_motor_get_count(NXT_PORT_C);

			nxt_motor_set_speed(NXT_PORT_C,-20,1);
			nxt_motor_set_speed(NXT_PORT_B,20,1);
			//disp(2, " REVW1B: ", revB);	
			//disp(3, " REVW1C: ", revC);
		}
	}
	else if(direction == 2){
		revC=0;
		while(revC<116)
		{
			revB = nxt_motor_get_count(NXT_PORT_B);
			revC = nxt_motor_get_count(NXT_PORT_C);

			nxt_motor_set_speed(NXT_PORT_C,20,1);
			nxt_motor_set_speed(NXT_PORT_B,-20,1);

			//disp(2, " REVW2B: ", revB);	
			//disp(3, " REVW2C: ", revC);
		}
	}
stop(revB,revC);
//systick_wait_ms(1200);

move(revB,revC,420);
}

void create_matrix()
{


}

void user_1ms_isr_type2(void){}
void ecrobot_device_initialize()
{
	nxt_motor_set_speed(NXT_PORT_C,0,1);
	nxt_motor_set_speed(NXT_PORT_B,0,1);
}
void ecrobot_device_terminate()
{
	nxt_motor_set_speed(NXT_PORT_C,0,1);
	nxt_motor_set_speed(NXT_PORT_B,0,1);	
}

TASK(Task1)
{
char prevDir;

float obstacle[MAX_OBSTACLES][2] = 
{{0.61, 2.743},{0.915, 2.743},{1.219, 2.743},{1.829, 1.219},
{1.829, 1.524},{ 1.829, 1.829}, {1.829, 2.134},{2.743, 0.305},
{2.743, 0.61},{2.743, 0.915},{2.743, 2.135},{3.048, 2.135},
{3.353, 2.135},
{2.44, 1.829},{2.44, 2.135},{ 2.44, 2.44}, {2.44, 2.743},{3.353,0.61},{3.353,0.915},{3.353,1.219},{3.353,1.524},{3.353,1.829},
{4.573, 0.61},{4.573, 0.915},{4.573, 1.219},{4.573, 1.829}};
//{3.353,1.829},{3.353,2.134},{3.353,1.524}
float start[2] = {0.305, 1.219}; 
float goal[2] = {3.658, 1.829}; 

U8 i, j, k, m, n, sqr_len=2, value=1, y, x ,xs,ys,u,d,l,r,z=0; //Taking a 9*20 array which means every intersection is a index in array
for(i=0; i<9; i++){
for(j=0; j<20; j++) a[i][j]=77; //Default value
}
//y = goal[0]/.305+1; //11
//x = 8-goal[1]/.305-1; //
y = ceil1(goal[0]/.305)-1;
x = 9-ceil1(goal[1]/.305);
a[x][y] = 0; //Setting the goal positon in array

//disp(3, " value: ", x);
//disp(4, " value: ", y);
//	systick_wait_ms(300);

for(k=0; k<MAX_OBSTACLES; k++){
    if(obstacle[k][0]<0 || obstacle[k][1]<0) break;
    j = ceil1(obstacle[k][0]/.305)-1;
    i = 9-ceil1(obstacle[k][1]/.305);
    a[i][j] = 99;   //Setting the obstacle value
}

//disp(2, " Startj: ", a[x][y]);
U8 maxNumber = max(x,y); //Getting max number of square possible

while(sqr_len<=maxNumber){
    j = y-value < 0 ? 0: y-value;
    i = x-value< 0 ? 0: x-value;

    for(m = i; m<=i+sqr_len; m++)
        for(n = j; n<=j+sqr_len; n++){

            if(m>=9 || n>=20){continue;}
            if(a[m][n]>value && a[m][n]!=99) a[m][n]=value; //Assigning Manhattan Distance in a square window
        }
    sqr_len +=2;
    value++;
}


ys = ceil1(start[0]/.305)-1;
xs = 9-ceil1(start[1]/.305);
a[xs][ys] = 100;
/*
for(i=0; i<9; i++)
for(j=0; j<20; j++){
display_clear(0);
disp(1, " i: ", i);
disp(2, " j: ", j);
disp(3, " a: ", a[i][j]);
systick_wait_ms(1200);
}
*/

revB=nxt_motor_get_count(NXT_PORT_B);
revC=nxt_motor_get_count(NXT_PORT_C);

    u=a[xs-1][ys];
    d=a[xs+1][ys];
    r=a[xs][ys+1];
/*    disp(1, " u: ", u);
//systick_wait_ms(1200);
    //disp(2, " d: ", d); 
//systick_wait_ms(1200);
    disp(3, " r: ", r);
//systick_wait_ms(1200);
*/
    if(min(u,d,99,r)=='u'){
        //printf("u\n");
	rotate(revB,revC, 2);
        xs = xs-1;
    }
     if(min(u,d,99,r)=='d'){
        //printf("d\n");
	rotate(revB,revC, 1);
        xs = xs-1;
    }
     if(min(u,d,99,r)=='r'){
	//disp(4, " in if : ", r);
	move(revB,revC,420);
	//stop(revB,revC);
	//systick_wait_ms(1200);
        ys=ys+1;
    }
prevDir = min(u,d,99,r);
//1=left
//2=right
while(a[xs][ys]!=0)
{
   
    u=a[xs-1][ys];
    d=a[xs+1][ys];
    l=a[xs][ys-1];
    r=a[xs][ys+1];
    if(ys==0){l=99;}
    if(xs==0){u=99;}
    if(ys==14){r=99;}
    if(xs==8){d=99;}

    //Boxed condition
    //if( (u==99&&d==99&&l==99&&r!=99) || (u==99&&d==99&&l!=99&&r==99) || (u==99&&d!=99&&l==99&&r==99)|| (u!=99&&d==99&&l==99&&r==99) )
	//{
	 //moveBack(revB,revC,420); //moving it back
	 //prevDir=min(u,d,l,r);		
		

	//}

    if(min(u,d,l,r)=='u'){
	if(prevDir=='r')
	{
	rotate(revB,revC, 2);
	}
	else if(prevDir=='l')
	{
	rotate(revB,revC, 1);
	}
	else if(prevDir=='d')// for up-down
	{		     //boxed condition
		if(d==99 && l==99 && r==99){
			moveBack(revB,revC,420);
			a[xs][ys] = 99;
		}else{
		if(min(99,d,l,r)=='d'){move(revB,revC,420);}
		else if(min(99,d,l,r)=='l'){rotate(revB,revC, 1);}
		else {rotate(revB,revC, 2);}}
	}
	else
	{
	move(revB,revC,420);
	}
	if(prevDir=='d'){
		if(d==99 && l==99 && r==99){
			prevDir='u';
       		  	xs = xs-1;	
		}else{
		a[xs-1][ys] = 99;
		if(min(99,d,l,r)=='d'){
			prevDir='d';
        		xs = xs+1;}
		else if(min(99,d,l,r)=='l'){
			prevDir='l';
        		ys = ys-1;}
		else {	prevDir='r';
        		ys = ys+1;}}
	}
	else {
		prevDir='u';
       		 xs = xs-1;
	}
	disp(1, " dir: ", min(u,d,l,r));
	disp(1, " point: ", a[xs][ys]);
    }


     if(min(u,d,l,r)=='d'){
 	if(prevDir=='r')
	{
	rotate(revB,revC, 1);
	}
	else if(prevDir=='l')
	{
	rotate(revB,revC, 2);
	}
	else if(prevDir=='u')  // for down-up
	{			//boxed conditon
	if(u==99 && l==99 && r==99){
			moveBack(revB,revC,420);
			a[xs][ys] = 99;
		}else{
		if(min(u,99,l,r)=='u'){move(revB,revC,420);}
		else if(min(u,99,l,r)=='l'){rotate(revB,revC, 2);}
		else {rotate(revB,revC, 1);}}
	}
	else
	{
	move(revB,revC,420);
	}
	if(prevDir=='u'){
		if(u==99 && l==99 && r==99){
			prevDir='d';
       		  	xs = xs+1;	
		}else{
		a[xs+1][ys] = 99;
		if(min(u,99,l,r)=='u'){
			prevDir='u';
        		xs = xs-1;}
		else if(min(u,99,l,r)=='l'){
			prevDir='l';
        		ys = ys-1;}
		else {	prevDir='r';
        		ys = ys+1;}}
	}
	else {
		prevDir='d';	
		xs = xs+1;
	}
	disp(1, " dir: ", min(u,d,l,r));
	disp(1, " point: ", a[xs][ys]);
    }



     if(min(u,d,l,r)=='l'){
	if(prevDir=='u')
	{
	rotate(revB,revC, 2);
	}
	else if(prevDir=='d')
	{
	rotate(revB,revC, 1);
	}
	else if(prevDir=='r')  // for right-left
	{			//boxed conditon
	if(u==99 && d==99 && r==99){
			moveBack(revB,revC,420);
			a[xs][ys] = 99;
		}else{
		if(min(u,d,99,r)=='r'){move(revB,revC,420);}
		else if(min(u,d,99,r)=='u'){rotate(revB,revC, 2);}
		else {rotate(revB,revC, 1);}}
	}
	else
	{
	move(revB,revC,420);
	}
	if(prevDir=='r'){
		if(d==99 && u==99 && r==99){
			prevDir='l';
       		  	ys=ys-1;	
		}else{
		a[xs][ys-1]=99;
		if(min(u,d,99,r)=='r'){
			prevDir='r';
        		ys=ys+1;}
		else if(min(u,d,99,r)=='u'){
			prevDir='u';
        		xs=xs-1;}
		else {	prevDir='d';
        		xs=xs+1;}}
	}
	else {
		prevDir='l';
        	ys=ys-1;
	}
	
	
	disp(1, " dir: ", min(u,d,l,r));
	disp(1, " point: ", a[xs][ys]);
    }



     if(min(u,d,l,r)=='r'){
	if(prevDir=='u')
	{
	rotate(revB,revC, 1);
	}
	else if(prevDir=='d')
	{
	rotate(revB,revC, 2);
	}
	else if(prevDir=='l')  // for left-right
	{			//boxed conditon
	if(u==99 && d==99 && l==99){
			moveBack(revB,revC,420);
			a[xs][ys] = 99;
		}else{
		if(min(u,d,l,99)=='l'){move(revB,revC,420);}
		else if(min(u,d,l,99)=='u'){rotate(revB,revC, 1);}
		else {rotate(revB,revC, 2);}}
	}
	else
	{
	move(revB,revC,420);
	}
	if(prevDir=='l'){
	if(d==99 && u==99 && l==99){
			prevDir='r';
       		  	ys=ys+1;	
		}else{
		a[xs][ys+1]=99;
		if(min(u,d,l,99)=='l'){
			prevDir='l';
        		ys=ys-1;}
		else if(min(u,d,l,99)=='u'){
			prevDir='u';
        		xs=xs-1;}
		else {	prevDir='d';
        		xs=xs+1;}}
	}
	else {
 	prevDir='r';	
        ys=ys+1;
	}
	disp(1, " dir: ", min(u,d,l,r));
	disp(1, " point: ", a[xs][ys]);
    }
z=z+1;

}
//move(revB,revC,420);

nxt_motor_set_speed(NXT_PORT_C,0,1);
nxt_motor_set_speed(NXT_PORT_B,0,1);

TerminateTask();
}


