#pragma once


#define __FILENAME__                    (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__) // file name from where it was printed

#define LOG_HELP(M, ...)                printf_s("[HELP] " M "\n", __VA_ARGS__)

#ifdef _DEBUG
#define dbg(M, ...)                 printf_s("[dbg] " M "\n", __VA_ARGS__)
#define LOG_INFO(M, ...)            printf_s("[INFO] " M "\n", __VA_ARGS__)
#define LOG_WARN(M, ...)            printf_s("[WARN] (%S:%d:%S) " M "\n" , __FILENAME__, __LINE__, __func__, __VA_ARGS__)
#define LOG_ERROR(err, M, ...)      printf_s("[ERROR:0x%08x] (%S:%d:%S) " M "\n", err, __FILENAME__, __LINE__, __func__, __VA_ARGS__)
#else
#define dbg(M, ...)                 (M)
#define LOG_INFO(M, ...)            printf_s("[INFO] " M "\n", __VA_ARGS__)
#define LOG_WARN(M, ...)            printf_s("[WARN] " M "\n", __VA_ARGS__)
#define LOG_ERROR(err, M, ...)      printf_s("[ERROR:0x%08x] " M "\n", err, __VA_ARGS__)
#endif

