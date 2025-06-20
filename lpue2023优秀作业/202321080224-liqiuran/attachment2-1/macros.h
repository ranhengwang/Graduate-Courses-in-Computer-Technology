#ifndef _MACROS_H_
#define _MACROS_H_

#ifdef DEBUG

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <cstddef>
#include <cstdint>

/// \brief PRINT_LOG printed info: file|line@function: info passed by user
///
/// \param color 颜色
/// \param fmt 格式化字符串
/// \param args... 参量表
///
/// \return 
#define PRINT_LOG(color, fmt, args...) \
    do \
    { \
        char buffer[256] = "\0"; \
        snprintf(buffer, sizeof(buffer) - 1, "\033[%sm>>> %s|%d@%s: \033[0m", color, __FILE__, __LINE__, __FUNCTION__); \
        snprintf(buffer + strlen(buffer), sizeof(buffer) - 1 - strlen(buffer), fmt, ##args); \
        buffer[strlen(buffer)] = '\n'; \
        write(STDOUT_FILENO, buffer, 256); \
    } while(0)

#define PRINTF_LOG(color, fmt, args...) \
    do \
    { \
        char buffer[256] = "\0"; \
        snprintf(buffer, sizeof(buffer) - 1, "\033[%sm>>> %s|%d@%s: \033[0m", color, __FILE__, __LINE__, __FUNCTION__); \
        snprintf(buffer + strlen(buffer), sizeof(buffer) - 1 - strlen(buffer), fmt, ##args); \
        write(STDOUT_FILENO, buffer, 256); \
    } while(0)

/// \brief ERROR 打印错误日志
///
/// \param fmt 格式化字符串
/// \param args... 参量表
///
/// \return 
#define ERROR(fmt, args...) \
    PRINT_LOG("0;31", fmt, ##args)

/// \brief WARNING 打印警告日志
///
/// \param fmt 格式化字符串
/// \param args... 参量表
///
/// \return 
#define WARNING(fmt, args...) \
    PRINT_LOG("0;33", fmt, ##args)

/// \brief INFO 打印普通信息日志
///
/// \param fmt 格式化字符串
/// \param args... 参量表
///
/// \return 
#define INFO(fmt, args...) \
    PRINT_LOG("0;32", fmt, ##args)

#define INFOF(fmt, args...) \
    PRINTF_LOG("0;32", fmt, ##args)

#else

#define ERROR(fmt, args...)

#define WARNING(fmt, args...)

#define INFO(fmt, args...)
#define INFOF(fmt, args...)

#endif
#endif





