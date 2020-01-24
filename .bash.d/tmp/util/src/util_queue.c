#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util_thread.h"
#include "util_queue.h"



/* =============================================================================
 *  構造体定義
 * =============================================================================
 */

/**
 * キューのエントリ構造体
 */
struct queue_entry {
	size_t size;		/**< 実データサイズ			*/
	char*  data;		/**< データバッファ			*/
};
 
 
/**
 * キューのインスタンス構造体
 */
struct queue {
	size_t              capacity;			/**< キューの容量		*/
	size_t              size;				/**< キューのサイズ		*/
	size_t              max_data_size;		/**< データの最大サイズ	*/
	struct queue_entry* entries;			/**< データのエントリ	*/
	struct queue_entry* head;				/**< キューの取出位置	*/
	struct queue_entry* tail;				/**< キューの挿入位置	*/
};



/* =============================================================================
 *  内部関数プロトタイプ宣言
 * =============================================================================
 */
static struct queue_entry* queue_next_entry(struct queue* queue, struct queue_entry* entry);
static bool queue_is_enabled(struct queue* queue, struct queue_entry* entry);



/* =============================================================================
 *  公開関数
 * =============================================================================
 */

/**
 * キューを生成します。
 * インスタンスを生成できない場合、NULL を返します。
 *
 * [注意]
 * このキューは、同期化されません。
 * 同期化が必要な場合は、blocking_queue を使用してください。
 *
 * @param cap  キューの容量(格納可能なデータの個数)
 * @param size キューに格納する1つあたりのデータサイズ
 * @return キューのインスタンス
 */
/* [実装メモ]
 * キューのデータ管理構造
 * メモリ確保を 1 回の malloc でできるようにしている。
 * +-------------------+
 * | queue 構造体      |
 * +-------------------+ 
 * | entry[0]          |
 * +-------------------+
 *           :
 * +-------------------+
 * | entry[cap-1]      |
  +-------------------+
 * | entry[0].data     |
 * | 用バッファ        |
 * +-------------------+
 *           :
 * +-------------------+
 * | entry[cap-1].data |
 * | 用バッファ        |
 * +-------------------+
 */
struct queue* queue_new(size_t cap, size_t size)
{
	size_t queue_size   = sizeof(struct queue)
						+ ((sizeof(struct queue_entry) + size) * cap);
	struct queue* queue = (struct queue*) malloc(queue_size);
	if (queue != NULL)
	{
		queue->capacity      = cap;
		queue->size          = 0;
		queue->max_data_size = size;
		queue->entries       = (struct queue_entry*) (queue + 1);
		queue->head          = queue->entries;
		queue->tail          = queue->entries;

		
		char*  tmp_data_ptr           = (char*) (queue->entries + queue->capacity);
		struct queue_entry* tmp_entry = queue->entries;
		for (size_t i = 0; i < queue->capacity; i++)
		{
			tmp_entry->data = tmp_data_ptr;
			tmp_entry++;
			tmp_data_ptr += queue->max_data_size;
		}
	}
	return queue;
}



/**
 * 指定されたキューを破棄します。
 *
 * @param queue キューのインスタンス
 */
void queue_destroy(struct queue* queue)
{
	free(queue);
}


/**
 * キューにデータを追加します。
 *
 * @param queue キューのインスタンス
 * @param data  キューに追加するエントリ(データ)
 * @param size  キューに追加するエントリ(データ)のサイズ
 * @return true/false (追加成功/追加失敗)
 */
bool queue_push(struct queue* queue, void* data, size_t size)
{
	bool result = false;

	if (size < queue->max_data_size)
	{
		if (queue->size < queue->capacity)
		{
			memset(queue->tail->data, 0x00, sizeof(struct queue_entry));
			memcpy(queue->tail->data, data, size);
			queue->tail->size = size;
			queue->tail++;
			if (queue->tail >= (queue->entries + queue->capacity))
			{
				queue->tail = queue->entries;
			}
			queue->size++;
			result = true;
		}
	}
	return result;
}


/**
 * キューの先頭よりデータを取り出します。
 * データの取り出しに失敗した場合、0を返します。
 *
 * @param queue  キューのインスタンス
 * @param buf    取り出したデータ格納用バッファ
 * @param buflen バッファのサイズ
 * @return 取り出したデータのサイズ
 */
size_t queue_pop( struct queue* queue, void* buf, size_t buflen)
{
	size_t size = 0;
	if (queue->size > 0)
	{
		if (queue->head->size < buflen)
		{
			size = queue->head->size;
			memset(buf, 0x00, buflen);
			memcpy(buf, queue->head->data, size);

			// データクリア (なくても動作上問題ない)
			memset(queue->head->data, 0x00, queue->max_data_size);
			queue->head->size = 0;

			queue->head++;
			if (queue->head >= (queue->entries + queue->capacity))
			{
				queue->head = queue->entries;
			}
			queue->size--;
		}
	}
	return size;
}


/**
 * キューの先頭よりデータを取り出します。
 * データの取り出しに失敗した場合、0を返します。
 * queue_pop と異なり、キューからデータは削除されません。
 *
 * @param queue  キューのインスタンス
 * @param buf    取り出したデータ格納用バッファ
 * @param buflen バッファのサイズ
 * @return 取り出したデータのサイズ
 */
size_t queue_peek( struct queue* queue, void* buf, size_t buflen)
{
	size_t size = 0;
	if (queue->size > 0)
	{
		if (queue->head->size < buflen)
		{
			size = queue->head->size;
			memset(buf, 0x00, buflen);
			memcpy(buf, queue->head->data, size);
		}
	}
	return size;
}


/**
 * キューのサイズ(キューに入っているデータの個数)を取得します。
 *
 * @param queue キューのインスタンス
 * @return キューのサイズ
 */
size_t queue_size(struct queue* queue)
{
	return queue->size;
}


/**
 * キューに格納されている全エントリーを引数に、指定された handler を呼び出します。
 *
 * handler の引数:
 * - data : データ
 * - size : データのサイズ
 *
 * @param queue キューのインスタンス
 * @param handler ハンドラ
 */
void queue_entries(struct queue* queue, bool (*handler)(void* data, size_t size))
{
	bool   is_continue = true;
	size_t counter     = queue->size;
	struct queue_entry* entry = queue->head;
	while (is_continue && (counter > 0))
	{
		is_continue = handler(entry->data, entry->size);
		entry       = queue_next_entry(queue, entry);
		counter--;
	}
}


/**
 * キューインスタンスの全エントリを引数に、指定された handler を呼び出します。
 * この関数は、デバッグ用の関数です。
 * handler には、実際にデータが入っていないキューのエントリも渡されます。
 *
 * handler の引数:
 * - data    : データ
 * - size    : データのサイズ
 * - index   : キューの管理上のインデックス
 * - enabled : true/false (有効なデータ/無効なデータ)
 * - arg     : ユーザーデータ
 *
 * @param queue   キューのインスタンス
 * @param handler ハンドラ
 * @param arg     ハンドラに渡すユーザーデータ
 */
void queue_entries_full(struct queue* queue,
   	bool (*handler)(void* data, size_t size, int index, bool enabled, void* arg),
	void* arg)
{
	bool is_enabled;
	bool is_continue = true;
	struct queue_entry* entry = queue->entries;
	for (int i = 0; is_continue && (i < (int) queue->capacity); i++)
	{
		is_enabled = queue_is_enabled(queue, entry);
		is_continue = handler(entry->data, entry->size, i, is_enabled, arg);
		entry++;
	}
}



/* =============================================================================
 *  内部関数
 * =============================================================================
 */


/**
 * 指定されたエントリの次のエントリーを取得します。
 * エントリー配列の末尾に到達した場合、先頭のエントリを返します。
 *
 * @param queue  キューのインスタンス
 * @param entry  エントリ
 * @return 次のエントリ
 */
static
struct queue_entry* queue_next_entry(struct queue* queue, struct queue_entry* entry)
{
	struct queue_entry* next_entry = entry;
	next_entry++;
	if (next_entry >= (queue->entries + queue->capacity))
	{
		next_entry = queue->entries;
	}
	return next_entry;
}


/**
 * 指定されたエントリが、有効か否かを返します。
 *
 * @param queue キューのインスタンス
 * @param entry エントリ
 * @return true/false (有効/無効)
 */
static
bool queue_is_enabled(struct queue* queue, struct queue_entry* entry)
{
	bool is_enabled;
	if ((queue->size != queue->capacity) && (queue->head <= queue->tail))
	{	/* キューのエントリ配列が以下のような状態の場合
		 * [- |- |HT-|- |- |- |- |- ]
		 * [- |- |Ho |o |o |T-|- |- ]  (-:空/o:データ有/H:Head/T:Tail)
		 */
		is_enabled = ((queue->head <= entry) && (entry < queue->tail));
	}
	else
	{	/* キューのエントリ配列が以下のような状態の場合
		 * [o |o |HTo|o |o |o |o |o ]
		 * [o |o |T- |- |- |Ho|o |o ]  (-:空/o:データ有/H:Head/T:Tail)
		 */
		is_enabled = ((entry < queue->tail) || (queue->head <= entry));
	}
	return is_enabled;
}
