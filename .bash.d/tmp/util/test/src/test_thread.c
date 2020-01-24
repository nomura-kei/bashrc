#include <stdio.h>
#include <unistd.h>

#include "util_thread.h"
#include "test_thread.h"



static void* thread_1_arg = NULL;
void thread_1(void* arg)
{
	thread_1_arg = arg;
	for (int i = 0; i < 5; i++)
	{
		sleep(1);
		printf("[%s] %d \n", (const char*)thread_1_arg, i);
	}
}
static void* thread_2_arg = NULL;
void thread_2(void* arg)
{
	thread_2_arg = arg;
	for (int i = 0; i < 5; i++)
	{
		printf("[%s] %d\n", (const char*)thread_2_arg, i);
		sleep(1);
	}
}

void test_thread(void)
{
	struct thread* thread1 = thread_new(thread_1, "thread_1");
	struct thread* thread2 = thread_new(thread_2, "thread_2");

	thread_start(thread1);
	thread_start(thread2);

	thread_join(thread1);
	thread_join(thread2);

}

