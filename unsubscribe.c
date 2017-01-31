/* unsubscribe.c - UnSubscribe */

#include <xinu.h>

extern void (*subscriber_handler[256][NPROC])(topic16, uint32);

extern topic16 arr_topic[256];
extern uint32 arr_data[256];
extern pid32 topic_subscribers[256][NPROC];
extern topic16 arr_group[256];
extern topic16 arr_pidgroup[NPROC];

syscall	unsubscribe(topic16 topic)
{
	intmask	mask;			/* Saved interrupt mask		*/
	mask = disable();

	topic_subscribers[topic&255][currpid] = 0;
	subscriber_handler[topic&255][currpid] = NULL;
	
	restore(mask);		/* Restore interrupts */
	return OK;
}
