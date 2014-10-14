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
	}
}

void testTask2() {
	int i = 0;
	int j = 0;
	int k = 10;
	long x = 0;
	PC_DispStr(0,9, "Task2:", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
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
		PC_DispChar(i+5, k, '=', DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		
		for(x = 0x05fffff; x > 0; x--);
	}
}

void testTask3() {
	int i = 0;
	int j = 0;
	int k = 17;
	long x = 0;
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
		PC_DispChar(i+5, k, '*', DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		
		for(x = 0x08fffff; x > 0; x--);
	}
}