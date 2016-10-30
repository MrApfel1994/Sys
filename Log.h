#ifndef LOG_H
#define LOG_H

#ifdef __ANDROID__
#include <android/log.h>
#define  LOG_TAG    "libjni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#include <cstdio>
//#define  LOGI(...) fprintf(stdout, "\n" __VA_ARGS__)
#define  LOGI(...) {fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n");}
#define  LOGE(...) fprintf(stderr, "\n" __VA_ARGS__)
#endif

#endif
