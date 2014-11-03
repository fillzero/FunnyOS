
typedef struct osTask {
	void (*ptask)(void);
	int stack_SS;
	int stack_SP;
	int isTaskStart;
} osTask;


typedef struct osMessage {
	int msgType;
	union content {
		char c;
		int i;
		long l;
		void *p;
	} msgContent;
} osMessage;


#define QUEUESIZE  4
typedef struct {
        int q[QUEUESIZE+1];		/* body of queue */
        int first;                      /* position of first element */
        int last;                       /* position of last element */
        int count;                      /* number of queue elements */
} queue;