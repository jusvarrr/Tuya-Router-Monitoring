#include "params_json.h"

void create_json_data(char *json_data, size_t size, struct MemData memoryData) {
    snprintf(json_data, size,
             "{\"totalram\":%d, \"freeram\":%d}",
             memoryData.total / 1048576,
             memoryData.free / 1048576);
}