#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

#include "logger.h"



// =============================================================================
//  内部でのみ使用する関数のプロトタイプ宣言
// =============================================================================
static void logger_log_output(int priority, const char* message);
static int  get_date(char* buff, size_t size);





// =============================================================================
//  関数定義
// =============================================================================

/**
 * 指定されたログレベルのログを出力可能か否かを返します。
 *
 * @param priority ログレベル
 * @return true/false (ログ出力可能/ログ出力不可)
 */
bool logger_is_logable(int priority)
{
  int current_logmask = setlogmask(0);
  return (current_logmask & priority);
}



/**
 * ログを出力します。
 *
 * @param priority syslog と同じログのレベルを指定します。
 * @param file __FILE__ を指定します。
 * @param line __LINE__ を指定します。
 * @param format 書式を指定します。
 */
void logger_log_(int priority, const char* file, int line, const char* format, ...)
{
  va_list ap;
  va_start(ap, format);
  bool is_logable = logger_is_logable(priority);
  if (is_logable)
  {
    char log_buffer[512];
    int  len = get_date(log_buffer, sizeof(log_buffer));
    len += snprintf(&log_buffer[len], sizeof(log_buffer) - len, " %s:%d: ", file, line);
    vsnprintf(&log_buffer[len], sizeof(log_buffer) - len, format, ap);
    logger_log_output(priority, log_buffer);
  }
  va_end(ap);
}


/**
 * 指定されたメッセージをログに出力します。
 *
 * @param priority ログレベル
 * @param message 出力するメッセージ
 */
static void logger_log_output(int priority, const char* message)
{
#ifdef USE_SYSLOG
  static bool is_logopend = false;
  if (!is_logopend)
  {
    openlog("ifconv", LOG_CONS | LOG_PID, LOG_USER);
    is_logopend = true;
  }
  syslog(priority, "%s", message);
#else
  pid_t current_pid = getpid();
  printf("ifconv[%d][pri=%d]: %s\n", current_pid, priority, message);
#endif
}


/**
 * 指定されたバッファに、現在の時刻(ナノ秒まで)を文字列形式で格納します。
 *
 * @param buff バッファ
 * @param size バッファサイズ
 * @return バッファに書き込んだ文字数
 */
static int get_date(char* buff, size_t size)
{
  // ナノ秒まで取得する。
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);

  // 秒を日時に変換する。
  struct tm* local_tm = localtime(&current_time.tv_sec);

  // YYYY/MM/DD HH:MM:SS.XXXXXXXXX に変換する。
  int len = snprintf(buff, size, "%04d/%02d/%02d %02d:%02d:%02d.%09ld",
        local_tm->tm_year + 1900,
        local_tm->tm_mon + 1,
        local_tm->tm_mday,
        local_tm->tm_hour,
        local_tm->tm_min,
        local_tm->tm_sec,
        current_time.tv_nsec);

  return len;
}


