/**
 * @file util_thread.c
 * スレッドを扱うモジュール
 *
 * 実装側のみ複数のOS,ライブラリ用に変更できるよう
 * 各オブジェクトを typedef ではなく、構造体にしている。
 */
#ifndef UTIL_THREAD_H
#define UTIL_THREAD_H

#include <stdbool.h>

/* スレッド	*/
struct thread;
struct thread* thread_new(void (*start_routine)(void*), void* arg);
void thread_delete(struct thread* thread);
void thread_start(struct thread* thread);
bool thread_join(struct thread* thread);
bool thread_equals(struct thread* thread);

/* mutex */
struct mutex;
struct mutex* mutex_new(void);
bool mutex_delete(struct mutex* mutex);
void mutex_lock(struct mutex* mutex);
void mutex_unlock(struct mutex* mutex);

/* cond */
struct cond;
struct cond* cond_new(void);
bool cond_delete(struct cond* cond);
void cond_wait(struct cond* cond, struct mutex* mutex);
void cond_signal(struct cond* cond);
void cond_broadcast(struct cond* cond);


#endif	/* THREAD_H	*/

