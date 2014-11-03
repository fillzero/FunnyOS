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

extern queue osqueue1, osqueue2;
extern int isFunnyOS;
extern void TickISR(void);
extern void (*PC_TickISR)(void);
void sort_draw(int *p, int row, int src, int dst);
void sort_draw_back(int *p, int row, int src, int dst);
void quick_sort(int *s, int l, int r, int *move_cnt, int *comp_cnt);

void testTask1() {
	int i = 0;
	int j = 0;
	int k = 2;
	long x = 0;
	PC_DispStr(0,1, "Task1:", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	for(;;) {
		++i;
		if(++j==70) {
			PC_DispChar(i+5, k, '-', DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			i = 1;
			j = 1;
			if(++k==5) {
				PC_DispClrRow(2, DISP_FGND_WHITE + DISP_BGND_BLACK);
				PC_DispClrRow(3, DISP_FGND_WHITE + DISP_BGND_BLACK);
				PC_DispClrRow(4, DISP_FGND_WHITE + DISP_BGND_BLACK);
				k = 2;
			}
		}
		PC_DispChar(i+6, k, '>', DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispChar(i+5, k, '-', DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		
		for(x = 0x01ffffff; x > 0; x--);
		//for(x = 0x0ffff; x > 0; x--);
	}
}

void testTask2() {
	int i = 0;
	int j = 0;
	int k = 10;
	long x = 0;
	osMessage taskmsg;
	taskmsg.msgType = 1;
	
	//osSendMsg(&taskmsg);
	
	PC_DispStr(0,9, "Task2:", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	
	//osqueue.count = 0;
	for(;;) {
		++i;
		if(++j==70) {
			PC_DispChar(i+5, k, '=', DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			i = 1;
			j = 1;
			if(++k==15) {
				PC_DispClrRow(10, DISP_FGND_WHITE + DISP_BGND_BLACK);
				PC_DispClrRow(11, DISP_FGND_WHITE + DISP_BGND_BLACK);
				PC_DispClrRow(12, DISP_FGND_WHITE + DISP_BGND_BLACK);
				PC_DispClrRow(13, DISP_FGND_WHITE + DISP_BGND_BLACK);
				PC_DispClrRow(14, DISP_FGND_WHITE + DISP_BGND_BLACK);
				k = 10;
			}
		}
		PC_DispChar(i+6, k, '>', DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		//PC_DispChar(i+5, k, '=', DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		//PC_DispChar(i+5, k, '0'+j, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		
		for(x = 0x2ffffff; x > 0; x--);
		taskmsg.msgContent.i = j;
		//osSendMsg(&taskmsg);
		//PC_DispChar(10, 9, '0' + osqueue.count, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		while(!(enqueue(&osqueue1,j)));
		PC_DispChar(i+5, k, '0'+j, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	}
}

void testTask3() {
	int i = 0;
	int j = 0;
	int k = 17;
	long x = 0;
	osMessage msg;
	PC_DispStr(0,16, "Task3:", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	for(;;) {
		++i;
		if(++j==70) {
			PC_DispChar(i+5, k, '*', DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			i = 1;
			j = 1;
			if(++k==24) {
				PC_DispClrRow(17, DISP_FGND_WHITE + DISP_BGND_BLACK);
				PC_DispClrRow(18, DISP_FGND_WHITE + DISP_BGND_BLACK);
				PC_DispClrRow(19, DISP_FGND_WHITE + DISP_BGND_BLACK);
				PC_DispClrRow(20, DISP_FGND_WHITE + DISP_BGND_BLACK);
				PC_DispClrRow(21, DISP_FGND_WHITE + DISP_BGND_BLACK);
				PC_DispClrRow(22, DISP_FGND_WHITE + DISP_BGND_BLACK);
				PC_DispClrRow(23, DISP_FGND_WHITE + DISP_BGND_BLACK);
				k = 17;
			}
		}
		
		PC_DispChar(i+6, k, '>', DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		//PC_DispChar(i+5, k, '*', DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		
		
		for(x = 0x8ffffff; x > 0; x--);
		//msg = osGetMsg(1);
		
		PC_DispChar(i+5, k, '0'+dequeue(&osqueue1), DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	}
}

void task_select_sort() {
	int num[20] = {35, 29, 17, 88, 44, 15, 25, 11, 49, 51, 61, 35, 22, 19, 99, 87, 28, 38, 45, 77};
	int i,j,tmp;
	static int move_cnt = 0;
	static int comp_cnt = 0;
	for(i = 0; i < 20; i++) {
		if(num[i] > 9) {
			PC_DispChar(i*3,16, '0'+num[i]/10, DISP_FGND_WHITE + DISP_FGND_BLACK);
			PC_DispChar(i*3+1,16, '0'+num[i]%10, DISP_FGND_WHITE + DISP_FGND_BLACK);
		}
		else
			PC_DispChar(i*3,16, '0' + num[i], DISP_FGND_WHITE + DISP_FGND_BLACK);
	}
	for(i = 0; i <19; i++) {
		tmp = num[i];
		for(j = i+1; j < 20 ; j++) {
			comp_cnt++;
			sort_draw_back(num, 16, i, j);
			if(num[j] < tmp) {
				tmp = num[j];
				num[j] = num[i];
				num[i] = tmp;
				sort_draw(num, 16, i, j);
				move_cnt++;
			}
		}
	}

	PC_DispStr(21*3, 15, "compare :", DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(75,15, '0'+comp_cnt/100, DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(76,15, '0'+comp_cnt%100/10, DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(77,15, '0'+comp_cnt%100%10, DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispStr(21*3, 16, "swap    :", DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(75,16, '0'+move_cnt/10, DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(76,16, '0'+move_cnt%10, DISP_FGND_WHITE + DISP_FGND_BLACK);
	for(;;);
}

void task_bubble_sort() {
	int num[20] = {35, 29, 17, 88, 44, 15, 25, 11, 49, 51, 61, 35, 22, 19, 99, 87, 28, 38, 45, 77};
	//int num[20] = {11, 12, 13, 14, 15, 22, 25, 33, 44, 55, 66, 67, 71, 72, 73, 87, 88, 89, 95, 97};
	int i,j,tmp;
	static int move_cnt = 0;
	static int comp_cnt = 0;
	for(i = 0; i < 20; i++) {
		if(num[i] > 9) {
			PC_DispChar(i*3,20, '0'+num[i]/10, DISP_FGND_WHITE + DISP_FGND_BLACK);
			PC_DispChar(i*3+1,20, '0'+num[i]%10, DISP_FGND_WHITE + DISP_FGND_BLACK);
		}
		else
			PC_DispChar(i*3,20, '0' + num[i], DISP_FGND_WHITE + DISP_FGND_BLACK);
	}
	for(i = 0; i <19; i++) {
		tmp = num[i];
		for(j = 0; j < 20-i-1; j++) {
			comp_cnt++;
			sort_draw_back(num, 20, j, j+1);
			if(num[j] > num[j+1]) {
				tmp = num[j];
				num[j] = num[j+1];
				num[j+1] = tmp;
				sort_draw(num, 20, j, j+1);
				move_cnt++;
			}
		}
	}
	PC_DispStr(21*3, 19, "compare :", DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(75,19, '0'+comp_cnt/100, DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(76,19, '0'+comp_cnt%100/10, DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(77,19, '0'+comp_cnt%100%10, DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispStr(21*3, 20, "swap    :", DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(75,20, '0'+move_cnt/10, DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(76,20, '0'+move_cnt%10, DISP_FGND_WHITE + DISP_FGND_BLACK);
	for(;;);
}


void task_quick_sort() {
	int num[20] = {35, 29, 17, 88, 44, 15, 25, 11, 49, 51, 61, 35, 22, 19, 99, 87, 28, 38, 45, 77};
	int i,j,tmp;
	static int move_cnt = 0;
	static int comp_cnt = 0;
	
	quick_sort(num, 0, 19, &move_cnt, &comp_cnt);
	PC_DispStr(21*3, 21, "compare :", DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(75,21, '0'+comp_cnt/100, DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(76,21, '0'+comp_cnt%100/10, DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(77,21, '0'+comp_cnt%100%10, DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispStr(21*3, 22, "swap    :", DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(75,22, '0'+move_cnt/10, DISP_FGND_WHITE + DISP_FGND_BLACK);
	PC_DispChar(76,22, '0'+move_cnt%10, DISP_FGND_WHITE + DISP_FGND_BLACK);

	for(i = 0; i < 20; i++) {
		if(num[i] > 9) {
			PC_DispChar(i*3,22, '0'+num[i]/10, DISP_FGND_WHITE + DISP_FGND_BLACK);
			PC_DispChar(i*3+1,22, '0'+num[i]%10, DISP_FGND_WHITE + DISP_FGND_BLACK);
		}
		else
			PC_DispChar(i*3,22, '0' + num[i], DISP_FGND_WHITE + DISP_FGND_BLACK);
	}
	for(;;);
}
void quick_sort(int *s, int l, int r, int *move_cnt, int *comp_cnt) {
    if (l < r) {
        int i = l, j = r, x = s[l];  
        while (i < j) {
            while(i < j && s[j] >= x) { // 从右向左找第一个小于x的数
				(*comp_cnt)++;
				sort_draw_back(s, 22, j, l);
                j--;
			}
			
            if(i < j) {
				(*comp_cnt)++;
				sort_draw_back(s, 22, j, l);
                s[i++] = s[j];
				(*move_cnt)++;
				sort_draw(s, 22, i-1, j);
			}
              
            while(i < j && s[i] < x) {// 从左向右找第一个大于等于x的数  
				(*comp_cnt)++;
				sort_draw_back(s, 22, i, l);
                i++;
			}
            if(i < j) {
				(*comp_cnt)++;
				sort_draw_back(s, 22, i, l);
                s[j--] = s[i];
				(*move_cnt)++;
				sort_draw(s, 22, j+1, i);
			}
        }  
        s[i] = x;  
        quick_sort(s, l, i - 1, move_cnt, comp_cnt); 
        quick_sort(s, i + 1, r, move_cnt, comp_cnt);
    }
}  

void sort_draw_back(int *p, int row, int src, int dst) {
	int i, color;
	long x;
	
	for(i = 0; i < 20; i++) {
		if(i == src || i == dst) {
			color = DISP_FGND_WHITE + DISP_BGND_LIGHT_GRAY;
		}
		else
			color = DISP_FGND_WHITE + DISP_FGND_BLACK;
		if(p[i] > 9) {
			PC_DispChar(i*3,row, '0'+p[i]/10, color);
			PC_DispChar(i*3+1,row, '0'+p[i]%10, color);
		}
		else
			PC_DispChar(i*3,row, '0' + p[i], color);
	}
	for(x = 0xffffff; x > 0; x--);
}

void sort_draw(int *p, int row, int src, int dst) {
	int i, color;
	long x;
	for(i = 0; i < 20; i++) {
		if(i == src || i == dst) {
			color = DISP_FGND_YELLOW + DISP_FGND_BLACK;
		}
		else
			color = DISP_FGND_WHITE + DISP_FGND_BLACK;
		if(p[i] > 9) {
			PC_DispChar(i*3,row, '0'+p[i]/10, color);
			PC_DispChar(i*3+1,row, '0'+p[i]%10, color);
		}
		else
			PC_DispChar(i*3,row, '0' + p[i], color);
	}
	for(x = 0xffffff; x > 0; x--);
}

void taskReadKeyBoard() {
	long x;
	int i = 0;
	int keychar, keyFlag;
	INT16S     key;
	while(1) {
		keyFlag = 0;
		/*
		asm {
			push ax; push bx; push cx; push dx; push si; push di; push bp; push ds; push es;
			int 16h;
			mov tmp, ax; 
			pop es; pop ds; pop bp; pop di; pop si; pop dx; pop cx; pop bx; pop ax;
		}*/
		/*if(!isFunnyOS)  {
			if (PC_GetKey(&key)) {
				PC_DispChar(i++,21, key, DISP_FGND_WHITE + DISP_FGND_BLACK);
			}
		} else*/ 
		{
			//PC_VectSet(0x8, PC_TickISR); 
			asm {
				
				push ax; push bx; push cx; push dx; push si; push di; push bp; push ds; push es;
				//cli;
				mov ax, 0x100;
				int 16h;
				jz  no_key;
				mov ax, 0x0;
				int 16h;
				mov keychar, ax; 
				mov keyFlag, 1;
				//sti;
			}
		no_key: 
			asm {
				pop es; pop ds; pop bp; pop di; pop si; pop dx; pop cx; pop bx; pop ax;
			}
			//PC_VectSet(0x8, &TickISR);
			if(keyFlag) {
				keychar &= 0xff;
				if(keychar == 'w' || keychar == 's'|| keychar == 'a' || keychar == 'd')
					while(!(enqueue(&osqueue1,keychar&0xff)));
				else if(keychar == '5' || keychar == '2'|| keychar == '1' || keychar == '3')
					while(!(enqueue(&osqueue2,keychar&0xff)));
				//PC_DispChar(i++,21, keychar, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
				//PC_DispChar(i++,21, keychar>>8, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
				//PC_DispChar(i++,21, '0'+(keychar>>12)&0xf, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
				//PC_DispChar(i++,21, '0'+(keychar>>8)&0xf, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
				//PC_DispChar(i++,21, '0'+(keychar>>4)&0xf, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
				//PC_DispChar(i++,21, '0'+(keychar)&0xf, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
				//PC_DispChar(i++,21, (keychar>>8)&0xff, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			}
			for(x = 0xffffff; x > 0; x--);
			//for(x = 0x0ffff; x > 0; x--);
		}
	}
}

////////////////////////// Game Snake//////////////////////////
#include "Game.h"
void gameSnake() {
	int i,j;
	int getmsg, direction;
	int foodx, foody;
	long x;
	snake mySnake[100];
	int snakeLen = 4;
	getmsg = -1;
	direction = 'd';
	foodx = (getRandom()&0xff % 20)+5;
	foody = getRandom()&0xff % 30;
	for(i = 0; i < snakeLen; i++) {
		mySnake[i].x = 10;
		mySnake[i].y = 10-i;
	}
	for(i=1; i < 25; i++) {
		PC_DispChar(32, i, ' ' , DISP_FGND_LIGHT_GREEN + DISP_BGND_BROWN);	
	}
	
	while(1) {
		for(j = 0; j < snakeLen; j++) 
			PC_DispChar(mySnake[j].y, mySnake[j].x, ' ' , DISP_FGND_BLACK + DISP_FGND_BLACK);
		getmsg = dequeue(&osqueue1);
		if(getmsg != -1) {
			if(getmsg == 'w' || getmsg == 's'|| getmsg == 'a' || getmsg == 'd')
				direction = getmsg;
		}
		/*
		PC_DispChar(0, 12, '0' + foodx/10 , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);	
		PC_DispChar(1, 12, '0' + foodx%10 , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);	
		
		PC_DispChar(4, 12, '0' + foody/10 , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);	
		PC_DispChar(5, 12, '0' + foody%10 , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);

		PC_DispChar(8, 12, '0' + mySnake[0].x/10 , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);	
		PC_DispChar(9, 12, '0' + mySnake[0].x%10 , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);

		PC_DispChar(11, 12, '0' + mySnake[0].y/10 , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);	
		PC_DispChar(12, 12, '0' + mySnake[0].y%10 , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		*/
		if(mySnake[0].x == foodx && mySnake[0].y == foody) {
			foodx = (getRandom()&0xff % 20)+5;
			foody = getRandom()&0xff % 30;
			snakeLen++;
			//PC_DispChar(snakeLen, 13, 'G' , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);	
		}
		for(j = snakeLen-1; j > 0; j--) {
			mySnake[j].y = mySnake[j-1].y;
			mySnake[j].x = mySnake[j-1].x;
		}
		
		if(direction == 's')
			mySnake[0].x += 1;
		else if(direction == 'd')
			mySnake[0].y += 1;
		else if(direction == 'w') 
			mySnake[0].x -= 1;
		else if(direction == 'a') 
			mySnake[0].y -= 1;
		
		for(j = 0; j < snakeLen; j++) 
			PC_DispChar(mySnake[j].y, mySnake[j].x, ' ' , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);

		//show food
		//foodx = getRandom() % 25;
		//foody = getRandom() % 80;
		PC_DispChar(foody, foodx, ' ' , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);	
		for(x = 0x1ffffff; x > 0; x--);	
		//for(x = 0x0ffff; x > 0; x--);
	}
	for(;;);
}

/////////////////////////// Game Tetris /////////////////////////
void newblock() {
    blockType = (getRandom()&0xf) % 7;
    turnState = (getRandom()&0xf) % 4;
	posx = 0;
	posy = 0;//47;
}

//draw wall
void drawwall() {
	int i, j;
	for (i = 0; i < TetrisWidth-1; i++) {
		map[i][TetrisHeight-2] = 2;
	}
	for (j = 0; j < TetrisHeight-1; j++) {
		map[TetrisWidth-2][j] = 2;
		map[0][j] = 2;
	}
}

void newmap() {
	int i, j;
	for (i = 0; i < TetrisWidth-1; i++) {
		for (j = 0; j < TetrisHeight-1; j++) {
			map[i][j] = 0;
		}
	}
}

int blow(int x, int y, int blockType, int turnState) {
	int a, b;
    for (a = 0; a < 4; a++) {
        for (b = 0; b < 4; b++) {
            if (((shapes[blockType][turnState][a * 4 + b] == 1) 
					&& (map[x+ b + 1][y + a] == 1))
				|| ((shapes[blockType][turnState][a * 4 + b] == 1) 
					&& (map[x+ b + 1][y + a] == 2))) {
                return 0;
            }
        }
    }
    return 1;
}

void delline() {
	int a, b, d, e;
    int c = 0;
    for (b = 0; b < TetrisHeight-1; b++) {
        for (a = 0; a < TetrisWidth-1; a++) {
            if (map[a][b] == 1) {

                c = c + 1;
                if (c == TetrisWidth-3) {
                    score += 10;
                    for (d = b; d > 0; d--) {
                        for (e = 0; e < TetrisWidth-2; e++) {
                            map[e][d] = map[e][d - 1];

                        }
                    }
                }
            }
        }
        c = 0;
    }
}

int gameover(int x, int y) {
    if (blow(x, y, blockType, turnState) == 0) {
        return 1;
    }
    return 0;
}

void add(int x, int y, int blockType, int turnState) {
	int a, b;
    int j = 0;
    for (a = 0; a < 4; a++) {
        for (b = 0; b < 4; b++) {
            if (map[x + b + 1][y + a] == 0) {
                map[x + b + 1][y + a] = shapes[blockType][turnState][j];
            }
            j++;
        }
    }
}

void paint() {
	int i,j;
	for (j = 0; j < TetrisHeight; j++) {
        for (i = 0; i < TetrisHeight; i++) {
			PC_DispChar(50+i, j, ' ' , DISP_FGND_BLACK + DISP_FGND_BLACK);
        }
    }
    for (j = 0; j < 16; j++) {
        if (shapes[blockType][turnState][j] == 1) {
            //g.fillRect((j % 4 + x + 1) * 10, (j / 4 + y) * 10, 10, 10);
			//PC_DispChar(j/4 + posy, j%4 + i+ posx, ' ' , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispChar(j%4 + posx+51, j/4 + posy, ' ' , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
        }
    }
    for (j = 0; j < TetrisHeight-1; j++) {
        for (i = 0; i < TetrisWidth-1; i++) {
            if (map[i][j] == 1 || map[i][j] == 2) {
                //g.fillRect(i * 10, j * 10, 10, 10);
				PC_DispChar(50+i, j, ' ' , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
            }
        }
    }
}

void left() {
    if (blow(posx - 1, posy, blockType, turnState) == 1) {
        posx = posx - 1;
    }
    paint();
}

void right() {
    if (blow(posx + 1, posy, blockType, turnState) == 1) {
        posx = posx + 1;
    }
    paint();
}

void down() {
    if (blow(posx, posy + 1, blockType, turnState) == 1) {
        posy = posy + 1;
        delline();
    }
    if (blow(posx, posy + 1, blockType, turnState) == 0) {
        add(posx, posy, blockType, turnState);
        newblock();
        delline();
    }
    paint();
}
	
void turn() {
    int tempturnState = turnState;
    turnState = (turnState + 1) % 4;
    if (blow(posx, posy, blockType, turnState) == 0) {
        turnState = tempturnState;
    }
    paint();
}

void gameTetris() {
	int i,j, val;
	int getmsg, direction;
	long x;
	int flag = 0;
	i = 0;
	getmsg = -1;
	direction = '2';
	//posx = 0;
	//posy = 44;
	
	newblock();
    newmap();
    drawwall();
		
	while(1) {
		getmsg = dequeue(&osqueue2);
		if(getmsg != -1) {
			if(getmsg == '5')
				turn();
			else if(getmsg == '3') 
				right();
			else if(getmsg == '1')
				left();
			else if(getmsg == '2')
				down();
		}
		paint();
		for(x = 0x2ffffff; x > 0; x--);	
		if (blow(posx, posy+1, blockType, turnState) == 1) {
			posy = posy + 1;
			delline();
		} else {
			if (flag == 1) {
				add(posx, posy, blockType, turnState);
				delline();
				newblock();
				flag = 0;
			}
			flag = 1;
		}
		//turn();
		//for(x = 0xffffff; x > 0; x--);	
	}
	
	while(1) {
		if(++i == 10) {
			val = getRandom()%7;
			i = 0;
		}
		for(j = 0; j < 16; j++)
			PC_DispChar(j/4 + posy, j%4 + i+ posx, ' ' , DISP_FGND_BLACK + DISP_FGND_BLACK);
		getmsg = dequeue(&osqueue2);
		if(getmsg != -1) {
			direction = getmsg;
			
			if(direction == '2')        //down
				posx += 1;
			//else if(direction == '5')   //up
	//			posx -= 1;
			else if(direction == '1')   //left
				posy -= 1;
			else if(direction == '3')   //right
				posy += 1;
		} 

		for(j = 0; j < 16; j++) {			
            if (shapes[6][3][j] == 1) {
                //g.fillRect((j % 4 + x + 1) * 10, (j / 4 + y) * 10, 10, 10);
				PC_DispChar(j/4+0 +posy, j%4+ i+posx, ' ' , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);	
            }
        }
		for(x = 0x8ffffff; x > 0; x--);	
	}

//	PC_DispChar(44, 10, ' ' , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);	
//	PC_DispChar(45, 10, ' ' , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);	
//	PC_DispChar(44, 11, ' ' , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);	
//	PC_DispChar(45, 11, ' ' , DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);	
	for(;;);
}