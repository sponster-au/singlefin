
#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include <stdio.h>
#include "qapi_httpc.h"
#include "qapi_socket.h"
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_CLIENT_SESSIONS_MAX	1
#define HTTP_CLIENT_BYTE_POOL_SIZE		1024*HTTP_CLIENT_SESSIONS_MAX
#define HTTP_CLIENT_BODY_LEN	1024
#define HTTP_CLIENT_HEADER_LEN	512
#define HTTP_CLIENT_TIMEOUT		10000 // secs

#define HTTP_CLIENT_DATA_EVT_DONE	(1 >> 0)

typedef struct http_entry_list {
	struct list_head head;
	char *data;
	uint32_t data_len;
	char *header;
	uint32_t header_len;
	uint32_t code;
} http_client_entry_t;

typedef struct http_client_ctx{
	qapi_Net_HTTPc_handle_t handle;
	TX_BYTE_POOL *byte_pool;
	qapi_Net_HTTPc_Config_t *httpc_cfg;
	TX_EVENT_FLAGS_GROUP *evt;
	struct list_head list;
	qapi_HTTPc_CB_t user_callback;
} http_client_ctx_t;

#define htpp_client_set_user_callback(ctx,cb) do{ ctx->user_callback = cb } while(0)
#define http_client_for_each(p,ctx) list_for_each(p,&ctx->list)
#define htpp_client_has_data(ctx) list_first_entry_or_null(&ctx->list, http_client_entry_t,head)
#define http_client_get_data(iter) list_entry(iter,http_client_entry_t, head )->data
#define http_client_get_data_length(iter) (uint32_t)list_entry(iter,http_client_entry_t, head )->data_len
#define http_client_get_header(iter) list_entry(iter,http_client_entry_t, head )->header
#define http_client_get_header_length(iter) (uint32_t)list_entry(iter,http_client_entry_t, head )->header_len
#define http_client_free_resource(iter,data)	\
do{ \
	list_del(iter); \
	free(data); \
}while (0)


http_client_ctx_t *htpp_client_new(void);
int htpp_client_free(http_client_ctx_t *ctx);
int htpp_client_set_header(http_client_ctx_t *ctx, const char *key, const char *value);
int htpp_client_set_parameter(http_client_ctx_t *ctx, const char *key, const char *value);
int htpp_client_set_body(http_client_ctx_t *ctx, const char *body, uint32_t len);
int htpp_client_get(http_client_ctx_t *ctx, const char *host, int port, const char *path);



#ifdef __cplusplus
}
#endif

#endif 

