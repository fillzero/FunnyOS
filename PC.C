/*
*********************************************************************************************************
*                                          PC SUPPORT FUNCTIONS
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File : PC.C
* By   : Jean J. Labrosse
*********************************************************************************************************
*/

#include "PC.H"

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/
#define  DISP_BASE                  0xB800       /* Base segment of display (0xB800=VGA, 0xB000=Mono)  */
#define  DISP_MAX_X                     80       /* Maximum number of columns                          */
#define  DISP_MAX_Y                     25       /* Maximum number of rows                             */

#define  TICK_T0_8254_CWR             0x43       /* 8254 PIT Control Word Register address.            */
#define  TICK_T0_8254_CTR0            0x40       /* 8254 PIT Timer 0 Register address.                 */
#define  TICK_T0_8254_CTR1            0x41       /* 8254 PIT Timer 1 Register address.                 */
#define  TICK_T0_8254_CTR2            0x42       /* 8254 PIT Timer 2 Register address.                 */

#define  TICK_T0_8254_CTR0_MODE3      0x36       /* 8254 PIT Binary Mode 3 for Counter 0 control word. */
#define  TICK_T0_8254_CTR2_MODE0      0xB0       /* 8254 PIT Binary Mode 0 for Counter 2 control word. */
#define  TICK_T0_8254_CTR2_LATCH      0x80       /* 8254 PIT Latch command control word                */

#define  VECT_TICK                    0x08       /* Vector number for 82C54 timer tick                 */
#define  VECT_DOS_CHAIN               0x81       /* Vector number used to chain DOS                    */

/*$PAGE*/
/*
*********************************************************************************************************
*                           DISPLAY A SINGLE CHARACTER AT 'X' & 'Y' COORDINATE
*
* Description : This function writes a single character anywhere on the PC's screen.  This function
*               writes directly to video RAM instead of using the BIOS for speed reasons.  It assumed 
*               that the video adapter is VGA compatible.  Video RAM starts at absolute address 
*               0x000B8000.  Each character on the screen is composed of two bytes: the ASCII character 
*               to appear on the screen followed by a video attribute.  An attribute of 0x07 displays 
*               the character in WHITE with a black background.
*
* Arguments   : x      corresponds to the desired column on the screen.  Valid columns numbers are from
*                      0 to 79.  Column 0 corresponds to the leftmost column.
*               y      corresponds to the desired row on the screen.  Valid row numbers are from 0 to 24.
*                      Line 0 corresponds to the topmost row.
*               c      Is the ASCII character to display.  You can also specify a character with a 
*                      numeric value higher than 128.  In this case, special character based graphics
*                      will be displayed.
*               color  specifies the foreground/background color to use (see PC.H for available choices)
*                      and whether the character will blink or not.
*
* Returns     : None
*********************************************************************************************************
*/
void PC_DispChar (INT8U x, INT8U y, INT8U c, INT8U color)
{
    INT8U  far *pscr;
    INT16U      offset;


    offset  = (INT16U)y * DISP_MAX_X * 2 + (INT16U)x * 2;  /* Calculate position on the screen         */
    pscr    = (INT8U far *)MK_FP(DISP_BASE, offset);
    *pscr++ = c;                                           /* Put character in video RAM               */
    *pscr   = color;                                       /* Put video attribute in video RAM         */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                            CLEAR A COLUMN
*
* Description : This function clears one of the 80 columns on the PC's screen by directly accessing video 
*               RAM instead of using the BIOS.  It assumed that the video adapter is VGA compatible.  
*               Video RAM starts at absolute address 0x000B8000.  Each character on the screen is 
*               composed of two bytes: the ASCII character to appear on the screen followed by a video 
*               attribute.  An attribute of 0x07 displays the character in WHITE with a black background.
*
* Arguments   : x            corresponds to the desired column to clear.  Valid column numbers are from 
*                            0 to 79.  Column 0 corresponds to the leftmost column.
*
*               color        specifies the foreground/background color combination to use 
*                            (see PC.H for available choices)
*
* Returns     : None
*********************************************************************************************************
*/
void PC_DispClrCol (INT8U x, INT8U color)
{
    INT8U far *pscr;
    INT8U      i;


    pscr = (INT8U far *)MK_FP(DISP_BASE, (INT16U)x * 2);
    for (i = 0; i < DISP_MAX_Y; i++) {
        *pscr++ = ' ';                           /* Put ' ' character in video RAM                     */
        *pscr-- = color;                         /* Put video attribute in video RAM                   */
        pscr    = pscr + DISP_MAX_X * 2;         /* Position on next row                               */
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                             CLEAR A ROW
*
* Description : This function clears one of the 25 lines on the PC's screen by directly accessing video 
*               RAM instead of using the BIOS.  It assumed that the video adapter is VGA compatible.  
*               Video RAM starts at absolute address 0x000B8000.  Each character on the screen is 
*               composed of two bytes: the ASCII character to appear on the screen followed by a video 
*               attribute.  An attribute of 0x07 displays the character in WHITE with a black background.
*
* Arguments   : y            corresponds to the desired row to clear.  Valid row numbers are from 
*                            0 to 24.  Row 0 corresponds to the topmost line.
*
*               color        specifies the foreground/background color combination to use 
*                            (see PC.H for available choices)
*
* Returns     : None
*********************************************************************************************************
*/
void PC_DispClrRow (INT8U y, INT8U color)
{
    INT8U far *pscr;
    INT8U      i;


    pscr = (INT8U far *)MK_FP(DISP_BASE, (INT16U)y * DISP_MAX_X * 2);
    for (i = 0; i < DISP_MAX_X; i++) {
        *pscr++ = ' ';                           /* Put ' ' character in video RAM                     */
        *pscr++ = color;                         /* Put video attribute in video RAM                   */
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                              CLEAR SCREEN
*
* Description : This function clears the PC's screen by directly accessing video RAM instead of using
*               the BIOS.  It assumed that the video adapter is VGA compatible.  Video RAM starts at
*               absolute address 0x000B8000.  Each character on the screen is composed of two bytes:
*               the ASCII character to appear on the screen followed by a video attribute.  An attribute
*               of 0x07 displays the character in WHITE with a black background.
*
* Arguments   : color   specifies the foreground/background color combination to use 
*                       (see PC.H for available choices)
*
* Returns     : None
*********************************************************************************************************
*/
void PC_DispClrScr (INT8U color)
{
    INT8U  far *pscr;
    INT16U      i;


    pscr = (INT8U far *)MK_FP(DISP_BASE, 0x0000);
    for (i = 0; i < (DISP_MAX_X * DISP_MAX_Y); i++) { /* PC display has 80 columns and 25 lines        */
        *pscr++ = ' ';                                /* Put ' ' character in video RAM                */
        *pscr++ = color;                              /* Put video attribute in video RAM              */
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                 DISPLAY A STRING  AT 'X' & 'Y' COORDINATE
*
* Description : This function writes an ASCII string anywhere on the PC's screen.  This function writes
*               directly to video RAM instead of using the BIOS for speed reasons.  It assumed that the 
*               video adapter is VGA compatible.  Video RAM starts at absolute address 0x000B8000.  Each 
*               character on the screen is composed of two bytes: the ASCII character to appear on the 
*               screen followed by a video attribute.  An attribute of 0x07 displays the character in 
*               WHITE with a black background.
*
* Arguments   : x      corresponds to the desired column on the screen.  Valid columns numbers are from
*                      0 to 79.  Column 0 corresponds to the leftmost column.
*               y      corresponds to the desired row on the screen.  Valid row numbers are from 0 to 24.
*                      Line 0 corresponds to the topmost row.
*               s      Is the ASCII string to display.  You can also specify a string containing 
*                      characters with numeric values higher than 128.  In this case, special character 
*                      based graphics will be displayed.
*               color  specifies the foreground/background color to use (see PC.H for available choices)
*                      and whether the characters will blink or not.
*
* Returns     : None
*********************************************************************************************************
*/
void PC_DispStr (INT8U x, INT8U y, INT8U *s, INT8U color)
{
    INT8U  far *pscr;
    INT16U      offset;


    offset  = (INT16U)y * DISP_MAX_X * 2 + (INT16U)x * 2;   /* Calculate position of 1st character     */
    pscr    = (INT8U far *)MK_FP(DISP_BASE, offset);
    while (*s) {
        *pscr++ = *s++;                                     /* Put character in video RAM              */
        *pscr++ = color;                                    /* Put video attribute in video RAM        */
    }
}

/*
*********************************************************************************************************
*                                      SET THE PC'S TICK FREQUENCY
*
* Description: This function is called to change the tick rate of a PC.
*
* Arguments  : freq      is the desired frequency of the ticker (in Hz)
*
* Returns    : none
*
* Notes      : 1) The magic number 2386360 is actually twice the input frequency of the 8254 chip which
*                 is always 1.193180 MHz.
*              2) The equation computes the counts needed to load into the 8254.  The strange equation
*                 is actually used to round the number using integer arithmetic.  This is equivalent to
*                 the floating point equation:
*
*                             1193180.0 Hz
*                     count = ------------ + 0.5
*                                 freq
*********************************************************************************************************
*/
void PC_SetTickRate (INT16U freq)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    INT16U     count;


    if (freq == 18) {                            /* See if we need to restore the DOS frequency        */
        count = 0;
    } else if (freq > 0) {                        
                                                 /* Compute 8254 counts for desired frequency and ...  */
                                                 /* ... round to nearest count                         */
        count = (INT16U)(((INT32U)2386360L / freq + 1) >> 1); 
    } else {
        count = 0;
    }
    asm cli;
    outp(TICK_T0_8254_CWR,  TICK_T0_8254_CTR0_MODE3); /* Load the 8254 with desired frequency          */  
    outp(TICK_T0_8254_CTR0, count & 0xFF);            /* Low  byte                                     */
    outp(TICK_T0_8254_CTR0, (count >> 8) & 0xFF);     /* High byte                                     */
    asm sti;
}

/*
*********************************************************************************************************
*                                        INSTALL INTERRUPT VECTOR
*
* Description: This function sets an interrupt vector in the interrupt vector table.
*
* Arguments  : vect  is the desired interrupt vector number, a number between 0 and 255.
*              isr   is a pointer to a function to execute when the interrupt or exception occurs.
*
* Returns    : none
*********************************************************************************************************
*/
void PC_VectSet (INT8U vect, void (*isr)(void))
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    INT16U    *pvect;
    
    
    pvect    = (INT16U *)MK_FP(0x0000, vect * 4);     /* Point into IVT at desired vector location     */
    asm cli;
    *pvect++ = (INT16U)FP_OFF(isr);                   /* Store ISR offset                              */
    *pvect   = (INT16U)FP_SEG(isr);                   /* Store ISR segment                             */
    asm sti;
}

/*
*********************************************************************************************************
*                                        OBTAIN INTERRUPT VECTOR
*
* Description: This function reads the pointer stored at the specified vector.
*
* Arguments  : vect  is the desired interrupt vector number, a number between 0 and 255.
*
* Returns    : The address of the Interrupt handler stored at the desired vector location.
*********************************************************************************************************
*/
void *PC_VectGet (INT8U vect)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    INT16U    *pvect;
    INT16U     off;
    INT16U     seg;
    
    
    pvect = (INT16U *)MK_FP(0x0000, vect * 4);        /* Point into IVT at desired vector location     */
    asm cli;
    off   = *pvect++;                                 /* Obtain the vector's OFFSET                    */
    seg   = *pvect;                                   /* Obtain the vector's SEGMENT                   */
    asm sti;
    return (MK_FP(seg, off));
}
/*
*********************************************************************************************************
*                                        CHECK AND GET KEYBOARD KEY
*
* Description: This function checks to see if a key has been pressed at the keyboard and returns TRUE if
*              so.  Also, if a key is pressed, the key is read and copied where the argument is pointing
*              to.
*
* Arguments  : c     is a pointer to where the read key will be stored.
*
* Returns    : TRUE  if a key was pressed
*              FALSE otherwise
*********************************************************************************************************
*/
BOOLEAN PC_GetKey (INT16S *c)
{
    if (kbhit()) {                                         /* See if a key has been pressed            */
        *c = (INT16S)getch();                              /* Get key pressed                          */
        return (TRUE);
    } else {
        *c = 0x00;                                         /* No key pressed                           */
        return (FALSE);
    }
}