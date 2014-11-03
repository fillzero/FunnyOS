#include "PC.h"
#include "struct.h"

#define MEM_ALLOC_START 0x4000

//#define MK_FP(seg,ofs)        ((void far *)(((LONG)(seg)<<16)|(UWORD)(ofs)))
// Task Func
extern void testTask1(void);
extern void testTask2(void);
extern void testTask3(void);
extern void task_select_sort(void);
extern void task_bubble_sort(void);
extern void task_quick_sort(void);
extern void gameSnake(void);
extern void gameTetris(void);
extern void taskReadKeyBoard(void);

// OS Func
extern void osSendMsg(osMessage *msg);
extern osMessage osGetMsg(int msgType);
extern int getRandom();

void init_queue(queue *q);
int enqueue(queue *q, int x);
int dequeue(queue *q);
int empty(queue *q);

// MEM 
void  *osMalloc(int len);
void free(void *p);