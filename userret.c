/* userret.c - userret */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  userret  -  Called when a process returns from the top-level function
 *------------------------------------------------------------------------
 */

extern void (*subscriber_handler[256][NPROC])(topic16, uint32);
extern pid32 topic_subscribers[256][8];
extern topic16 arr_pidgroup[NPROC];

void	userret(void)
{
	uint32 i;
	arr_pidgroup[getpid()] = 0;
	for(i=0;i<256;i++)
	{
		topic_subscribers[i][getpid()] = 0;
		subscriber_handler[i][getpid()] = NULL;	
	}
	kill(getpid());			/* Force process to exit */
}
