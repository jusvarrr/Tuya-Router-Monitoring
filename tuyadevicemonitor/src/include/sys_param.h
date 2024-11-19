#ifndef SYS_PARAM_H
#define SYS_PARAM_H
#include <sys/sysinfo.h>
#include <libubox/blobmsg_json.h>
#include <libubus.h>

struct MemData {
    int total;
	int free;
};
int get_ubus_data(struct MemData *memory);

#endif