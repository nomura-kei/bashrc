#ifndef UTIL_BLOCKINGQUEUE_H
#define UTIL_BLOCKINGQUEUE_H

#include <stdbool.h>


/**
 * ブロッキングキューのインスタンス構造体
 */
struct blockingqueue;

struct blockingqueue* blockingqueue_new(int cap);
void   blockingqueue_destroy(struct blockingqueue* queue);
bool   blockingqueue_push(struct blockingqueue* queue, void* data, size_t size);
bool   blockingqueue_push_blocking(struct blockingqueue* queue, void* data, size_t size);
size_t blockingqueue_pop( struct blockingqueue* queue, void* data, size_t size);
size_t blockingqueue_pop_blocking( struct blockingqueue* queue, void* data, size_t size);
size_t blockingqueue_peek(struct blockingqueue* queue, void* data, size_t size);
size_t blockingqueue_size(struct blockingqueue* queue);
void   blockingqueue_notify(struct blockingqueue* queue);


#endif	/* UTIL_BLOCKINGQUEUE_H	*/

