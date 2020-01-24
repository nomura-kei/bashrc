/**
 * @file util_thread.c
 * スレッドを扱うモジュール。
 */
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "util_thread.h"



/* /////////////////////////////////////////////////////////////////////////////
//
// スレッド
//
*/


/* ============================================================================
 *  構造体定義
 * ============================================================================
 */

/**
 * スレッドインスタンス構造体。
 */
struct thread {
	void      (*start_routine)(void*);	/**< スレッドとして実行する関数	*/
	void*     arg;						/**< スレッドに渡すデータ		*/
	pthread_t tid;						/**< スレッドのID				*/
};



/* ============================================================================
 *  プロトタイプ宣言 (内部でのみ利用する関数)
 * ============================================================================
 */
static void* thread_run(void* args);



/* ============================================================================
 *  公開関数
 * ============================================================================
 */

/**
 * スレッドを生成します。
 * スレッドの生成に失敗した場合、NULL を返します。
 *
 * @param start_routine スレッドで実行する関数
 * @param arg           スレッドに渡すデータへのポインタ
 * @return スレッドインスタンス
 */
struct thread* thread_new(void (*start_routine)(void*), void* arg)
{
	struct thread* thread = (struct thread*) malloc(sizeof(struct thread));
	if (thread != NULL)
	{
		thread->start_routine = start_routine;
		thread->arg           = arg;
	}
	return thread;
}


/**
 * スレッドインスタンスのリソースを開放します。
 * スレッド自体が終了するわけではありません。
 *
 * @param thread スレッドインスタンス
 */
void thread_delete(struct thread* thread)
{
	free(thread);
}


/**
 * スレッドを開始します。
 *
 * @param thread スレッドインスタンス
 */
void thread_start(struct thread* thread)
{
	pthread_create(&thread->tid, NULL, thread_run, thread);
}


/**
 * 指定されたスレッドが終了するのを待ちます。
 *
 * @param thread スレッドインスタンス
 */
bool thread_join(struct thread* thread)
{
	int ret = pthread_join(thread->tid, NULL);
	return (ret == 0);
}


/**
 * 現在のスレッドが指定されたスレッドと同一か否かを返します。
 *
 * @param thread スレッドインスタンス
 * @return true/false (一致/不一致)
 */
bool thread_equals(struct thread* thread)
{
	pthread_t tid = pthread_self();
	return (tid == thread->tid);
}



/* ============================================================================
 *  内部関数
 * ============================================================================
 */

/**
 * スレッドとして実行される関数。
 * この関数の中で、スレッド起動に指定された関数を実行します。
 *
 * @param arg スレッドインスタンス
 */
static
void* thread_run(void* arg)
{
	struct thread* thread = (struct thread*) arg;
	thread->start_routine(thread->arg);
	return NULL;
}



/* /////////////////////////////////////////////////////////////////////////////
//
// mutex
//
*/


/* ============================================================================
 *  構造体定義
 * ============================================================================
 */

/**
 * mutex インスタンス構造体。
 */
struct mutex {
	pthread_mutex_t mutex;		/**< mutex オブジェクト						*/
};



/* ============================================================================
 *  公開関数
 * ============================================================================
 */

/**
 * mutex を生成します。
 * mutex の生成に失敗した場合、NULL を返します。
 *
 * @return mutex
 */

struct mutex* mutex_new(void)
{
	struct mutex* mutex = (struct mutex*) malloc(sizeof(struct mutex));
	if (mutex != NULL)
	{
		pthread_mutex_init(&mutex->mutex, NULL);
	}
	return mutex;
}


/**
 * mutex を破棄します。
 * mutex がロックされている場合は、破棄に失敗しエラーを返します。
 *
 * @param mutex 破棄する　mutex
 * @return true/false (破棄成功/破棄失敗 [mutex がロックされている])
 */
bool mutex_delete(struct mutex* mutex)
{
	if (mutex != NULL)
	{
		int res = pthread_mutex_destroy(&mutex->mutex);
		if (res == 0)
		{
			free(mutex);
			return true;
		}
	}
	return false;
}


/**
 * mutex をロックします。
 *
 * @param mutex ロックする mutex
 */
void mutex_lock(struct mutex* mutex)
{
	// mutex は、デフォルトの種別で初期化済みのため、
	// EINVAL, EDEADLK のエラーは発生しない。
	pthread_mutex_lock(&mutex->mutex);
}


/**
 * mutex をアンロックします。
 *
 * @param mutex アンロックする mutex
 */
void mutex_unlock(struct mutex* mutex)
{
	// mutex は、デフォルトの種別で初期化済みのため、
	// EINVAL, EPERM のエラーは発生しない。
	pthread_mutex_unlock(&mutex->mutex);
}



/* /////////////////////////////////////////////////////////////////////////////
//
// cond
//
*/


/* ============================================================================
 *  構造体定義
 * ============================================================================
 */

/**
 * cond インスタンス構造体。
 */
struct cond {
	pthread_cond_t cond;		/**< cond オブジェクト						*/
};



/* ============================================================================
 *  公開関数
 * ============================================================================
 */

/**
 * cond を生成します。
 * cond の生成に失敗した場合、NULL を返します。
 *
 * @return cond
 */

struct cond* cond_new(void)
{
	struct cond* cond = (struct cond*) malloc(sizeof(struct cond));
	if (cond != NULL)
	{
		pthread_cond_init(&cond->cond, NULL);
	}
	return cond;
}


/**
 * cond を破棄します。
 * cond が条件変数を待っている場合、破棄に失敗しエラーを返します。
 *
 * @param cond 破棄する cond
 * @return true/false (破棄成功/破棄失敗 [cond が条件変数を待っている])
 */
bool cond_delete(struct cond* cond)
{
	if (cond!= NULL)
	{
		int res = pthread_cond_destroy(&cond->cond);
		if (res == 0)
		{
			free(cond);
			return true;
		}
	}
	return false;
}


/**
 * 指定された mutex のアンロックと、条件変数 cond の送信に対する待機を
 * アトミックに行います。条件変数が送信されるまで、スレッドの実行は停止され、
 * CPU時間を消費しません。
 *
 * 本関数を実行する前に、 mutex はロックされている必要があります。
 * 本関数を呼び出しスレッドが、条件変数の待機完了により動作する際、
 * mutex は再びロックされます。
 *
 * @param cond  cond  インスタンス
 * @param mutex mutex ロック済みの mutex
 */
void cond_wait(struct cond* cond, struct mutex* mutex)
{
	pthread_cond_wait(&cond->cond, &mutex->mutex);
}


/**
 * 条件変数 cond に備えて待機しているスレッドの一つの実行を再開させます。
 * cond を待機しているスレッドがなければ何もしません。
 * 複数のスレッドが cond を待機している場合、どのスレッドが再開されるかはわからない。
 *
 * @param cond 再開させる cond
 */
void cond_signal(struct cond* cond)
{
	pthread_cond_signal(&cond->cond);
}


/**
 * 条件変数 cond に備えて待機しているすべてのスレッドを再開させます。
 * cond を待機しているスレッドがなければ何もしません。
 *
 * @param cond 再開させる cond
 */
void cond_broadcast(struct cond* cond)
{
	pthread_cond_broadcast(&cond->cond);
}


