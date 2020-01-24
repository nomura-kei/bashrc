#ifndef UTIL_QUEUE_H
#define UTIL_QUEUE_H

#include <stdbool.h>


/** キュー */
struct queue;
struct queue* queue_new(size_t cap, size_t size);
void   queue_destroy(struct queue* queue);
bool   queue_push(struct queue* queue, void* data, size_t size);
size_t queue_pop( struct queue* queue, void* data, size_t size);
size_t queue_peek(struct queue* queue, void* data, size_t size);
size_t queue_size(struct queue* queue);
void   queue_entries(struct queue* queue, bool (*handler)(void* data, size_t size));
void   queue_entries_full(struct queue* queue,
   	bool (*handler)(void* data, size_t size, int index, bool enabled, void* arg),
	void* arg);

#endif	/* UTIL_QUEUE_H	*/

