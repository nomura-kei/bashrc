#include <stdio.h>
#include "util_queue.h"

extern void queue_print_entries(struct queue* queue);


bool handler(void* data, size_t size, int index, bool enabled, void* arg)
{
	printf("[%02d][enabled=%d][size=%03ld] %s \n", index, enabled, size, (char*)data);
	(void) arg;
	return true;
}

bool simple_handler(void* data, size_t size)
{
	printf("##DATA:[size=%03ld] %s\n", size, (char*) data);
	return true;
}


int main(void)
{
	char tmp[256];
	size_t size;
	bool is_success;
	struct queue* queue = queue_new(4,256);
	is_success = queue_push(queue, "ABC", 4);
	printf("push ABC : %d\n", is_success);

	queue_entries_full(queue, handler, NULL);
	queue_entries(queue, simple_handler);

	is_success = queue_push(queue, "DEF", 4);
	printf("push DEF : %d\n", is_success);

	queue_entries_full(queue, handler, NULL);
	queue_entries(queue, simple_handler);

	is_success = queue_push(queue, "XYZ111", 7);
	printf("push XYZ111: %d\n", is_success);

	queue_entries_full(queue, handler, NULL);
	queue_entries(queue, simple_handler);

	size = queue_peek(queue, tmp, sizeof(tmp));
	printf("peek : size=%ld [%s]\n", size, tmp);

	queue_entries_full(queue, handler, NULL);
	queue_entries(queue, simple_handler);

	size = queue_peek(queue, tmp, sizeof(tmp));
	printf("peek : size=%ld [%s]\n", size, tmp);

	queue_entries_full(queue, handler, NULL);
	queue_entries(queue, simple_handler);

	size = queue_pop(queue, tmp, sizeof(tmp));
	printf("pop : size=%ld [%s]\n", size, tmp);

	queue_entries_full(queue, handler, NULL);
	queue_entries(queue, simple_handler);

	size = queue_pop(queue, tmp, sizeof(tmp));
	printf("pop : size=%ld [%s]\n", size, tmp);
	queue_entries_full(queue, handler, NULL);
	queue_entries(queue, simple_handler);

	size = queue_pop(queue, tmp, sizeof(tmp));
	printf("pop : size=%ld [%s]\n", size, tmp);
	queue_entries_full(queue, handler, NULL);
	queue_entries(queue, simple_handler);

	size = queue_pop(queue, tmp, sizeof(tmp));
	printf("pop : size=%ld [%s]\n", size, tmp);
	queue_entries_full(queue, handler, NULL);
	queue_entries(queue, simple_handler);

	is_success = queue_push(queue, "ABC", 4);
	printf("push ABC : %d\n", is_success);
	queue_entries_full(queue, handler, NULL);
	queue_entries(queue, simple_handler);

	is_success = queue_push(queue, "DEF", 4);
	printf("push DEF : %d\n", is_success);
	queue_entries_full(queue, handler, NULL);
	queue_entries(queue, simple_handler);

	is_success = queue_push(queue, "GHIJKLMN", 9);
	printf("push GHIJKLMN: %d\n", is_success);
	queue_entries_full(queue, handler, NULL);
	queue_entries(queue, simple_handler);

	is_success = queue_push(queue, "O", 2);
	printf("push O: %d\n", is_success);
	queue_entries_full(queue, handler, NULL);
	queue_entries(queue, simple_handler);

	is_success = queue_push(queue, "P", 2);
	printf("push P: %d\n", is_success);
	queue_entries_full(queue, handler, NULL);
	queue_entries(queue, simple_handler);


	return 0;
}

