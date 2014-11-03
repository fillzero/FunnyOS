/*
 *  FunnyOS - Multitask OS for fun
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
#include "FunnyOS.h"

void TickISR(void);
void (*PC_TickISR)(void);
void osSendMsg(osMessage *msg);
osMessage osGetMsg(int msgType);




//void *ptask1, *ptask2;
//void (*ptask1)(void);
//void (*ptask2)(void);

int isFunnyOS = 0;	// 0 mean DOS, 1 meas FunnyOS
int taskCount = 0;
int isTaskAllStart = 0;
static int tickCnt = 0;

osTask task[4];//task1, task2;
osMessage message[6];
int msgIndex;

queue osqueue1, osqueue2;

void osSendMsg(osMessage *msg) {
	
	//PC_DispChar(4, 0, '0' + msgIndex, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	while(msgIndex>=5){;}
	asm cli;
	message[msgIndex].msgType = msg->msgType;
	message[msgIndex++].msgContent = msg->msgContent;
	//PC_DispChar(2, 0, '0' + msgIndex, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	
	asm sti;
}

osMessage osGetMsg(int msgType) {
	int i = 0;
	osMessage retMsg;
	
	//PC_DispChar(6, 0, '0' + msgIndex, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	while(msgIndex<0){;}
	asm cli;
	for(i=0; i < msgIndex; i++) {
		if(message[i].msgType == msgType) {
			retMsg = message[i];
			msgIndex--;
			//PC_DispChar(2, 0, '0' + msgIndex, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			break;
		}
	}
	asm sti;
	return retMsg;
}

void main (void) {
	int far *bootend;
	int result;
	int *test1, *test2;
	
	msgIndex = tickCnt = 0;
	init_queue(&osqueue1);
	init_queue(&osqueue2);
	bootend = MK_FP(0x7c0, 510);
	result = *bootend;
	if(result == 0xAA55)
		isFunnyOS = 1;

	//test1 = osMalloc(10);
	//strcpy(test1, "testmallcabcdefghi");
	//test2 = osMalloc(20);
	//strcpy(test2, "test2malloc");
	
    PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);      /* Clear the screen                         */

    PC_DispStr( 0,  0, "                                  Funny OS Demo                                 ", DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
	//PC_DispChar( 10,  0, '0' + isFunnyOS , DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
	//PC_DispStr( 15,  0, test1, DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);	
	//PC_DispStr( 15,  1, test2, DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);	
	//PC_DispChar( 14,  0, *bootend , DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
	PC_SetTickRate(18);                      /* Reprogram tick rate                      */
	//PC_TickISR = &TickISR;
	PC_TickISR   = PC_VectGet(0x8);  
	PC_VectSet(0x8, &TickISR); //asm {int 08h};
	
//	ptask1 = &testTask1;
//	ptask2 = &testTask2;
	task[0].ptask = &gameSnake;//&testTask1;
	task[0].stack_SS = 0x9000;
	task[0].stack_SP = 0x100;
	taskCount++;
	
	task[1].ptask = &gameTetris;//&task_bubble_sort;//&testTask2;
	task[1].stack_SS = 0x8000;
	task[1].stack_SP = 0x100;
	taskCount++;

	task[2].ptask = &taskReadKeyBoard; //&task_select_sort;
	task[2].stack_SS = 0x7000;
	task[2].stack_SP = 0x100;
	taskCount++;
	
	task[3].ptask = &task_quick_sort;
	task[3].stack_SS = 0x6000;
	task[3].stack_SP = 0x100;
	//taskCount++;
	for(;;);
}

void TickISR(void) {
	static int i = 0;
	static int taskid = 0;
	static int taskSS, taskSP;
	static int back_ax, back_bx, back_dx;
	tickCnt++;
	asm {mov back_ax, ax;  mov back_bx, bx; mov back_dx, dx;}
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
					mov ax, back_ax; mov bx, back_bx; mov dx, back_dx;
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
				mov ax, back_ax; mov bx, back_bx; mov dx, back_dx;
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
				mov sp, taskSP;pop es; pop ds; pop bp; pop di; pop si; pop dx; pop cx; pop bx; pop ax;
				iret;
			}
		}
	}
	//asm iret;
}

int getRandom() {
	static int randomVal;
	asm {
		push ax;
		mov ax, [0x46c];		
		add ax, [0x470];
		add randomVal, ax;
		add randomVal, bx;
		add randomVal, cx;
		add randomVal, dx;
		add randomVal, bp;
		add randomVal, sp;
		pop ax;
	}
	return randomVal*tickCnt + randomVal%tickCnt;
}
//////////////////////////// OS QUEUE //////////
void init_queue(queue *q) {
	q->first = 0;
	q->last = QUEUESIZE-1;
	q->count = 0;
}

int enqueue(queue *q, int x) {
	asm cli;
	//PC_DispChar(2, 0, '0' + q->count, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	if (q->count >= QUEUESIZE) {
		asm sti;
		return 0;
	}
	else {
		q->last = (q->last+1) % QUEUESIZE;
		q->q[ q->last ] = x;    
		q->count = q->count + 1;
		asm sti;
		return 1;
	}
}

int dequeue(queue *q) {
	int x;
	asm cli;
	//PC_DispChar(4, 0, '0' + q->count, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	if (q->count <= 0)  {
		asm sti;
		return -1;
	}
	else {
		x = q->q[ q->first ];
		q->first = (q->first+1) % QUEUESIZE;
		q->count = q->count - 1;
	}
	asm sti;
	return(x);
}

int empty(queue *q) {
	if (q->count <= 0) 
		return (1);
	else return (0);
}

////////////////////////////// OS MEM ////////////////////////
void *osMalloc(int len) {
	static int freeMemStart = MEM_ALLOC_START;
	int *p;
	if(!isFunnyOS) 
		return malloc(len);
	p = freeMemStart;
	freeMemStart += len;
	return p;
}