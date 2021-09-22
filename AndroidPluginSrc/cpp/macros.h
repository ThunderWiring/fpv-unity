//
// Created by bassam on 02/09/2021.
//

#ifndef OPENNATIVEC_MACROS_H
#define OPENNATIVEC_MACROS_H

#include <stddef.h>
#include <android/log.h>

#define EXPORTED extern "C"
#define LOGD(tag, ...) (__android_log_print(ANDROID_LOG_DEBUG, (tag), __VA_ARGS__))
#define LOGE(tag, ...) (__android_log_print(ANDROID_LOG_ERROR, (tag), __VA_ARGS__))
#define STR_BOOL(b) (b) ? "true" : "false"

#if defined(__CYGWIN32__)
#define UNITY_INTERFACE_API __stdcall
    #define UNITY_INTERFACE_EXPORT __declspec(dllexport)
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WINAPI_FAMILY)
#define UNITY_INTERFACE_API __stdcall
    #define UNITY_INTERFACE_EXPORT __declspec(dllexport)
#elif defined(__MACH__) || defined(__ANDROID__) || defined(__linux__)
#define UNITY_INTERFACE_API
#define UNITY_INTERFACE_EXPORT
#else
#define UNITY_INTERFACE_API
    #define UNITY_INTERFACE_EXPORT
#endif

#endif //OPENNATIVEC_MACROS_H
