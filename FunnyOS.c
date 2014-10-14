/*
 *  Leafpad - GTK+ based simple text editor
 *  Copyright (C) 2014 fillzero <fillzero@163.com>
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "pc.h"
void TickISR(void);
void (*PC_TickISR)(void);
extern void testTask1(void);
extern void testTask2(void);
extern void testTask3(void);

//void *ptask1, *ptask2;
//void (*ptask1)(void);
//void (*ptask2)(void);
typedef struct osTask {
	void (*ptask)(void);
	int stack_SS;
	int stack_SP;
	int isTaskStart;
} osTask;
int taskCount = 0;
int isTaskAllStart = 0;

osTask task[4];//task1, task2;

void  main (void) {
    PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);      /* Clear the screen                         */

    PC_DispStr( 0,  0, "                                  Funny OS Demo                                 ", DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
	PC_SetTickRate(18);                      /* Reprogram tick rate                      */
	PC_TickISR = &TickISR;
	PC_VectSet(0x8, PC_TickISR); //asm {int 08h};
	
//	ptask1 = &testTask1;
//	ptask2 = &testTask2;
	task[0].ptask = &testTask1;
	task[0].stack_SS = 0x9000;
	task[0].stack_SP = 0x100;
	taskCount++;
	
	task[1].ptask = &testTask2;
	task[1].stack_SS = 0x8000;
	task[1].stack_SP = 0x100;
	taskCount++;

	task[2].ptask = &testTask3;
	task[2].stack_SS = 0x7000;
	task[2].stack_SP = 0x100;
	taskCount++;
	
	for(;;);
}

void TickISR(void) {
	static int i = 0;
	//static int j = 0;
	static int taskid = 0;
	static int taskSS, taskSP;
		
	asm {pop di; pop si;}
	asm cli;
	//outp(0x20,  0x20); 
	asm {
		push ax;
		push dx;
		mov dx,0x20;
		mov al,0x20;
		out dx,al;
		mov al,0x0;
		pop dx;
		pop ax;
	}
	asm sti;

	for(i = 0; i < taskCount; i++) {
		if(!task[i].isTaskStart) {
			task[i].isTaskStart = 1;
			
			//save pre task stack
			if(i) {
				asm {
					push ax; push bx; push cx; push dx; push si; push di; push bp; push ds; push es;
					mov taskSP, sp;
					mov taskSS, ss;
				};
				task[i-1].stack_SS = taskSS;
				task[i-1].stack_SP = taskSP;
			}
			
			taskSS = task[i].stack_SS;
			taskSP = task[i].stack_SP;
			asm {
				mov ss, taskSS;
				mov sp, taskSP;
			}
			(*(task[i].ptask))();
		}
	}
	if(!isTaskAllStart) {
		isTaskAllStart = 1;
		taskid = taskCount - 1; //taskid starts from 0
	}
	
	for(i = 0; i < taskCount; i++) {
		if(i == taskid) {
			//save task stack
			asm {
				push ax; push bx; push cx; push dx; push si; push di; push bp; push ds; push es;
				mov taskSP, sp;
				mov taskSS, ss;
			};
			task[i].stack_SS = taskSS;
			task[i].stack_SP = taskSP;

			//restore nextTaskid stack
			taskid = ((taskid+1) == taskCount) ? (0) : (taskid + 1);  //taskid = next task id
			taskSS = task[taskid].stack_SS;
			taskSP = task[taskid].stack_SP;
			asm {
				mov ss, taskSS;
				mov sp, taskSP;
				pop es; pop ds; pop bp; pop di; pop si; pop dx; pop cx; pop bx; pop ax;
				iret;
			}
		}
	}
	//asm iret;
}
