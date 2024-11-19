#ifndef TUYA_FUNCTIONS1
#define TUYA_FUNCTIONS1


#define LOG_TRACE_TUYA  LOG_TRACE
#define LOG_DEBUG_TUYA  LOG_DEBUG
#define LOG_INFO_TUYA   LOG_INFO
#define LOG_WARN_TUYA   LOG_WARN
#define LOG_ERROR_TUYA  LOG_ERROR
#define LOG_FATAL_TUYA  LOG_FATAL

#undef LOG_TRACE
#undef LOG_DEBUG
#undef LOG_INFO
#undef LOG_WARN
#undef LOG_ERROR
#undef LOG_FATAL

#include "syslog.h"
#include "tuya_error_code.h"

int connect_to_tuya_cloud(char *deviceId, char *deviceSecret);
int send_data_to_cloud();
int disconnect();

#endif