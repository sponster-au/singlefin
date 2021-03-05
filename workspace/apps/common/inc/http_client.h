
#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include <stdio.h>
#include "qapi_httpc.h"
#include "qapi_socket.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_CLIENT_SESSIONS_MAX	1
#define HTTP_CLIENT_BYTE_POOL_SIZE		1024*HTTP_CLIENT_SESSIONS_MAX
#define HTTP_CLIENT_BODY_LEN	1024
#define HTTP_CLIENT_HEADER_LEN	512
#define HTTP_CLIENT_TIMEOUT		10000 // secs

#define HTTP_CLIENT_DATA_EVT_DONE	(1 >> 0)

typedef struct http_client_ctx{
	qapi_Net_HTTPc_handle_t handle;
	TX_BYTE_POOL *byte_pool;
	qapi_Net_HTTPc_Config_t *httpc_cfg;
	TX_EVENT_FLAGS_GROUP *evt;
	qapi_Net_HTTPc_Response_t response;
	qapi_HTTPc_CB_t user_callback;
} http_client_ctx_t;

#define htpp_client_set_user_callback(ctx,cb) do{ ctx->user_callback = cb } while(0)

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

