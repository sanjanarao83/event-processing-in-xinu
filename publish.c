/* publish.c - publish */

#include <xinu.h>
extern void (*subscriber_handler[256][NPROC])(topic16, uint32);

extern topic16 arr_topic[256];
extern uint32 arr_data[256];
extern pid32 topic_subscribers[256][8];
extern topic16 arr_group[256];
extern topic16 arr_pidgroup[NPROC];
extern uint32 front;
extern uint32 rear;
extern sid32 sem1;
extern sid32 sem2;

syscall	publish(
		topic16 topic,
		uint32 data
	)
{
	intmask	mask;
	mask = disable();
	wait(sem1);
	arr_data[rear] = data;
	arr_topic[rear] = topic&255;
	arr_group[rear] = topic>>8;
	rear++;
	signal(sem2);
	restore(mask);		/* Restore interrupts */
	return OK;
}
