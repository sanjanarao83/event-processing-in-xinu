/*  main.c  - main */

#include <xinu.h>

//Variables to store topic and handler

extern void (*subscriber_handler[256][NPROC])(topic16, uint32);

extern topic16 arr_topic[256];
extern uint32 arr_data[256];
extern pid32 topic_subscribers[256][NPROC];
extern topic16 arr_group[256];
extern topic16 arr_pidgroup[NPROC];
extern uint32 front;
extern uint32 rear;

sid32 sub;
extern sid32 sem1;
extern sid32 sem2;

pid32 subscriber_id;
pid32 subscriber_id1;
pid32 publisher_id;
pid32 broker_id;

void mutex_acquire(sid32 mutex)
{
	wait(mutex);
}

void mutex_release(sid32 mutex)
{
	signal(mutex);
}

void subscribeProcHandler(topic16 topic, uint32 data)
{
	mutex_acquire(sub);	
	printf("Function subscribeProcHandler is called with arguments %d and %d\n", topic, data);
	mutex_release(sub);
}

//Subscribing for a topic
process subscribeProc(void)
{
	subscribe(350, subscribeProcHandler);
	mutex_acquire(sub);
	printf("Process subscribeProc subscribes to topic 350 with handler subscribeProcHandler()\n");
	mutex_release(sub);
	sleepms(8000);
	return OK;
}

void subscribeProcHandler1(topic16 topic, uint32 data)
{
	mutex_acquire(sub);
	printf("Function subscribeProcHandler1 is called with arguments %d and %d\n", topic, data);
	mutex_release(sub);
}

process subscribeProc1(void)
{
	subscribe(350, subscribeProcHandler1);
	mutex_acquire(sub);
	printf("Process subscribeProc1 subscribes to topic 350 with handler subscribeProcHandler1()\n");
	mutex_release(sub);
	sleepms(8000);
	return OK;
}

process publisher(void)
{
	publish(350, 77);
	mutex_acquire(sub);
	printf("Process publisher publishes 77 to topic 350\n");
	mutex_release(sub);

	publish(350, 42);
	mutex_acquire(sub);
	printf("Process publisher publishes 42 to topic 350\n");
	mutex_release(sub);

	publish(512, 15);
	mutex_acquire(sub);
	printf("Process publisher publishes 15 to topic 512\n");
	mutex_release(sub);

	publish(24, 8);
	mutex_acquire(sub);
	printf("Process publisher publishes 8 to topic 24\n");
	mutex_release(sub);
	
	return OK;
}

process	main(void)
{
	recvclr();
	
	front = 0;
	rear = 0;

	sub = semcreate(1);

	//1. Sending and receiving one message at a time
	subscriber_id = create(subscribeProc, 4096, 50, "subscribeProc", 0);
	subscriber_id1 = create(subscribeProc1, 4096, 50, "subscribeProc1", 0);
	publisher_id = create(publisher, 4096, 50, "publisher", 0);

	resched_cntl(DEFER_START);

	resume(subscriber_id);
	resume(subscriber_id1);
	resume(publisher_id);

	resched_cntl(DEFER_STOP);

	return OK;
}
