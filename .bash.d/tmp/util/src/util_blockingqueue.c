#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "util_queue.h"
#include "util_blockingqueue.h"
 
/**
 * ブロッキングキューのインスタンス構造体
 */
struct blockingqueue {
	struct queue*   queue;							/**< キュー							*/
	struct mutex*   data_lock;						/**< データロック					*/
	struct cond*    empty_block;					/**< pop のブロック用				*/
	struct cond*    full_block;						/**< push のブロック用				*/
};


/**
 * ブロッキングキューを生成します。
 * インスタンスを生成できない場合、NULL を返します。
 *
 * @param cap  キューの容量
 * @param size キューに格納するデータサイズ
 * @return イベントキューのインスタンス
 */
struct blockingqueue* blockingqueue_new(size_t cap, size_t size)
{
	struct blockingqueue* blockingqueue = (struct blockingqueue*) malloc(sizeof(struct blockingqueue));
	if (blockingqueue != NULL)
	{
		blockingqueue->queue = queue_new(cap,4096);
		if (blockingqueue->queue != NULL)
		{	// pthread_mutex_lock ですでに保持している mutex をロックしようとしたとき、
			// デフォルトのスレッドを停止させる動作とするため、NULL (デフォルト設定)を指定する。
			pthread_mutex_init(&blockingqueue->data_lock, NULL);

			// データがない場合 pop をブロックするための cond オブジェクトを初期化する。
			pthread_cond_init(&blockingqueue->pop_block, NULL);
		}
		else
		{
			free(blockingqueue);
			blockingqueue = NULL;
		}

	}
	return blockingqueue;
}




/**
 * 指定されたイベントキューを破棄します。
 *
 * @param blockingqueue イベントキューのインスタンス
 */
void blockingqueue_destroy(struct blockingqueue* blockingqueue)
{
	if (blockingqueue != NULL)
	{
		// T.B.D.
		queue_destroy(blockingqueue->queue);
		blockingqueue->queue = NULL;
	}
	free(blockingqueue);
}


/**
 * 指定された mutex をロックします。
 *
 * @param mutex mutexオブジェクト
 */
static
void blockingqueue_mutex_lock(pthread_mutex_t* mutex)
{
	int errcode = pthread_mutex_lock(mutex);
	if (errcode != 0)
	{	// エラーが発生する条件は、下記の何れかであり、本プログラムの設定では発生しない。
		// => 発生するのであれば、プログラム誤り。
		// EINVAL  : mutex が適切に初期化されていない。
		// EDEADLK : mutex は既に呼び出しスレッドによりロックされている (「エラー検査を行う」mutexes のみ)
		printf("###!!! [ERROR] pthred_mutex_lock !!!###\n");
	}
}


/**
 * 指定された mutex をアンロックします。
 *
 * @param mutex mutexオブジェクト
 */
static
void blockingqueue_mutex_unlock(pthread_mutex_t* mutex)
{
	int errcode = pthread_mutex_unlock(mutex);
	if (errcode != 0)
	{	// エラーが発生する条件は、下記の何れかであり、本プログラムの設定では発生しない。
		// => 発生するのであれば、プログラム誤り。
		// EINVAL  : mutex が適切に初期化されていない。
		// EDEADLK : mutex は既に呼び出しスレッドによりロックされている (「エラー検査を行う」mutexes のみ)
		printf("###!!! [ERROR] pthred_mutex_lock !!!###\n");
	}
}


/**
 * イベントキューにデータを追加します。
 *
 * @param blockingqueue キューのインスタンス
 * @param data  キューに追加するエントリ(データ)
 * @param size  キューに追加するエントリ(データ)のサイズ
 * @return true/false (追加成功/追加失敗)
 */
bool blockingqueue_push(struct blockingqueue* blockingqueue, void* data, size_t size)
{
	blockingqueue_mutex_lock(&blockingqueue->data_lock);

	// キューにデータがない間ブロックする。
	size_t data_count = queue_size(blockingqueue->queue);
	if (data_count == 0)
	{
		pthread_cond_signal(&blockingqueue->pop_block);
	}

	bool result = queue_push(blockingqueue->queue, data, size);

	blockingqueue_mutex_unlock(&blockingqueue->data_lock);

	return result;
}


/**
 * キューの先頭よりデータを取り出します。
 * キューにデータがない場合、ブロックされます。
 *
 * @param blockingqueue  キューのインスタンス
 * @param buf    取り出したデータ格納用バッファ
 * @param buflen バッファのサイズ
 * @return 取り出したデータのサイズ
 */
size_t blockingqueue_pop( struct blockingqueue* blockingqueue, void* buf, size_t buflen)
{
	blockingqueue_mutex_lock(&blockingqueue->data_lock);

	// キューにデータがない間ブロックする。
	size_t data_count = queue_size(blockingqueue->queue);
	while (data_count == 0)
	{
		pthread_cond_wait(&blockingqueue->pop_block, &blockingqueue->data_lock);
		data_count = queue_size(blockingqueue->queue);
	}

	size_t data_size = queue_pop(blockingqueue->queue, buf, buflen);
	blockingqueue_mutex_unlock(&blockingqueue->data_lock);
	return data_size;
}


/**
 * キューの先頭よりデータを取り出します。
 * データの取り出しに失敗した場合、0を返します。
 * blockingqueue_pop と異なり、キューからデータは削除されません。
 *
 * @param blockingqueue  キューのインスタンス
 * @param buf    取り出したデータ格納用バッファ
 * @param buflen バッファのサイズ
 * @return 取り出したデータのサイズ
 */
size_t blockingqueue_peek( struct blockingqueue* blockingqueue, void* buf, size_t buflen)
{
	blockingqueue_mutex_lock(&blockingqueue->data_lock);
	size_t size = queue_peek(blockingqueue->queue, buf, buflen);
	blockingqueue_mutex_unlock(&blockingqueue->data_lock);
	return size;
}



/**
 * キューのサイズ(キューに入っているデータの個数)を取得します。
 *
 * @param blockingqueue キューのインスタンス
 * @return キューのサイズ
 */
size_t blockingqueue_size(struct blockingqueue* blockingqueue)
{
	size_t size;
	blockingqueue_mutex_lock(&blockingqueue->data_lock);
	size = queue_size(blockingqueue->queue);
	blockingqueue_mutex_unlock(&blockingqueue->data_lock);
	return size;
}


#endif
