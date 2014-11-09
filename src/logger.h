#ifndef _FG_LOGGER_H_
#define _FG_LOGGER_H_

#include "defines.h"

#ifdef FG_ANDROID
#include <android/log.h>
#define fg_log(t, ...) __android_log_print(ANDROID_LOG_INFO, "FG", t, ##__VA_ARGS__)
#endif

#ifdef FG_IOS
#endif

#ifdef FG_PC
#include <stdio.h>
#define fg_log(t, ...) ({ printf(t, ##__VA_ARGS__); printf("\n"); fflush(stdout); })
#endif

#endif

