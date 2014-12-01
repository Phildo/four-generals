#ifndef _FG_LOGGER_H_
#define _FG_LOGGER_H_

#include "defines.h"
#include <stdio.h>

#ifdef FG_ANDROID
#include <android/log.h>
  #ifdef FG_DEBUG
    #define fg_log(t, ...) ({__android_log_print(ANDROID_LOG_INFO, "FG", t, ##__VA_ARGS__); char buff[FG_MAX_LOG_LEN]; sprintf(buff, t, ##__VA_ARGS__); MemLog::inst()->log(buff); })
  #else
    #define fg_log(t, ...) ({__android_log_print(ANDROID_LOG_INFO, "FG", t, ##__VA_ARGS__);})
  #endif
#endif

#ifdef FG_IOS
#endif

#ifdef FG_PC
  #ifdef FG_DEBUG
    #define fg_log(t, ...) ({ printf(t, ##__VA_ARGS__); printf("\n"); fflush(stdout); char buff[FG_MAX_LOG_LEN]; sprintf(buff, t, ##__VA_ARGS__); MemLog::inst()->log(buff); })
  #else
    #define fg_log(t, ...) ({ printf(t, ##__VA_ARGS__); printf("\n"); fflush(stdout); })
  #endif
#endif


#define FG_MAX_LOG_LEN 256
#define FG_MAX_NUM_LOGS 256

//singleton because I don't care about ugliness
class MemLog;
class MemLog
{
  private:
    static MemLog *sing;
    int log_i;
    int log_c;
    char logs[FG_MAX_NUM_LOGS*FG_MAX_LOG_LEN];
    MemLog();
  public:
    static MemLog *inst();
    void log(char *buff);
    char *logAt(int i);
};

#endif

