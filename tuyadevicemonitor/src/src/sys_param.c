
#include "sys_param.h"

enum {
	TOTAL_MEMORY,
	FREE_MEMORY,
    __MEMORY_MAX,
};

enum {
	MEMORY_DATA,
	__INFO_MAX,
};

static const struct blobmsg_policy memory_policy[__MEMORY_MAX] = {
	[TOTAL_MEMORY]	  = { .name = "total", .type = BLOBMSG_TYPE_INT64 },
	[FREE_MEMORY]	  = { .name = "free", .type = BLOBMSG_TYPE_INT64 },
};

static const struct blobmsg_policy info_policy[__INFO_MAX] = {
	[MEMORY_DATA] = { .name = "memory", .type = BLOBMSG_TYPE_TABLE },
};

static void board_cb(struct ubus_request *req, int type, struct blob_attr *msg) {
    struct MemData *memoryData = (struct MemData *)req->priv;
    struct blob_attr *table[__INFO_MAX];
    struct blob_attr *memory[__MEMORY_MAX];

    blobmsg_parse(info_policy, __INFO_MAX, table, blob_data(msg), blob_len(msg));

    if (table[MEMORY_DATA]) {
        blobmsg_parse(memory_policy, __MEMORY_MAX, memory, blobmsg_data(table[MEMORY_DATA]), blobmsg_data_len(table[MEMORY_DATA]));
        memoryData->total = blobmsg_get_u64(memory[TOTAL_MEMORY]);
        memoryData->free = blobmsg_get_u64(memory[FREE_MEMORY]);
    }
}

int get_ubus_data(struct MemData *memory)
{
    struct ubus_context *ctx;
    uint32_t id;
    int rc = 0;

    ctx = ubus_connect(NULL);

    if (!ctx){
        return -1;
    }
    
    if (ubus_lookup_id(ctx, "system", &id) || 
        ubus_invoke(ctx, id, "info", NULL, board_cb, memory, 3000))
        rc = -2;
    
    ubus_free(ctx);
    return rc;
}