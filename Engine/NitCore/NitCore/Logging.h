#pragma once
#include <iostream>
#include <cstdio>

//TODO: Add date/time at the log beginning
//TODO: Add final config without logs and stats
//TODO: Set logging levels

#ifdef NIT_DEBUG
#define NIT_LOG(...) \
    printf("Nit Engine: "); \
    printf(__VA_ARGS__)

#define NIT_LOG_TRACE(...) \
    printf("\x1B[96m"); \
    NIT_LOG(__VA_ARGS__); \
    printf("\033[0m")

#define NIT_LOG_WARN(...) \
    printf("\x1B[93m"); \
    NIT_LOG(__VA_ARGS__); \
    printf("\033[0m")

#define NIT_LOG_ERR(...) \
    printf("\x1B[91m"); \
    NIT_LOG(__VA_ARGS__); \
    printf("\033[0m")

#define NIT_CHECK(_CONDITION, ...) \
    if (!_CONDITION) { NIT_LOG_ERR(__VA_ARGS__); __debugbreak(); }
#endif

#ifdef NIT_RELEASE

#define NIT_LOG(...)
#define NIT_LOG_TRACE(...)
#define NIT_LOG_WARN(...)
#define NIT_LOG_ERR(...)
#define NIT_CHECK(_CONDITION, ...)

#endif