/*
 *  Duktape public API for Duktape @DUK_VERSION_FORMATTED@.
 *
 *  See the API reference for documentation on call semantics.  The exposed,
 *  supported API is between the "BEGIN PUBLIC API" and "END PUBLIC API"
 *  comments.  Other parts of the header are Duktape internal and related to
 *  e.g. platform/compiler/feature detection.
 *
 *  Git commit 429ad41966f7ba337cd5112722cf7bf0b70f38de (429ad41-dirty).
 *  Git branch single-header.
 *
 *  See Duktape AUTHORS.rst and LICENSE.txt for copyright and
 *  licensing information.
 */



#if !defined(SINGLEFIN_H_INCLUDED)
#define SINGLEFIN_H_INCLUDED

#define FIN_SINGLE_FILE


/*
 *  Version and Git commit identification
 */

/* Duktape version, (major * 10000) + (minor * 100) + patch.  Allows C code
 * to #if (DUK_VERSION >= NNN) against Duktape API version.  The same value
 * is also available to ECMAScript code in Duktape.version.  Unofficial
 * development snapshots have 99 for patch level (e.g. 0.10.99 would be a
 * development version after 0.10.0 but before the next official release).
 */
#define FIN_VERSION                       10000L

/* Git commit, describe, and branch for Duktape build.  Useful for
 * non-official snapshot builds so that application code can easily log
 * which Duktape snapshot was used.  Not available in the ECMAScript
 * environment.
 */
#define FIN_GIT_COMMIT                    "429ad41966f7ba337cd5112722cf7bf0b70f38de"
#define FIN_GIT_DESCRIBE                  "429ad41-dirty"
#define FIN_GIT_BRANCH                    "single-header"



/* External fin_config.h provides platform/compiler/OS dependent
 * typedefs and macros, and FIN_USE_xxx config options so that
 * the rest of SingleFin doesn't need to do any feature detection.
 * FIN_VERSION is defined before including so that configuration
 * snippets can react to it.
 */
#include "fin_config.h"

/*
 *  Avoid C++ name mangling
 */

#if defined(__cplusplus)
extern "C" {
#endif

/* ADC */

typedef enum fin_adc_channel{	
	ADC0,
	ADC1,
	ADC_MAX_NO
}fin_adc_channel_t;

int fin_adc_config(fin_adc_channel_t adc_num);
uint32_t fin_adc_read_microvolts(fin_adc_channel_t adc_num);
uint32_t fin_adc_read_physical(fin_adc_channel_t adc_num);
uint32_t fin_adc_read_percent(fin_adc_channel_t adc_num);
uint32_t fin_adc_read_raw(fin_adc_channel_t adc_num);


/* CLI */
typedef int (*fin_cli_callback_t)(int args, char *argv[]);

typedef	struct fin_cli_cmd{
	char *name;
	fin_cli_callback_t callback;
	struct fin_cli_cmd *next;
} fin_cli_cmd_t;

typedef struct fin_cli{
	char *in;
	int head;
	int tail;
	int count;
	fin_cli_cmd_t *cmds;
} fin_cli_t;


void fin_cli_free(fin_cli_t *ctx);
fin_cli_t *fin_cli_new(void);
void fin_cli_input(fin_cli_t *ctx, char c);
int fin_cli_register(fin_cli_t *ctx, const char *name, fin_cli_callback_t func);


/* DSS */

typedef const void (*fin_dss_data_call_status_cb_t)(qapi_DSS_Net_Evt_t evt);

typedef struct fin_dss_notify{
	TX_EVENT_FLAGS_GROUP *evt;
	char addr[DSS_ADDR_SIZE];
	fin_dss_data_call_status_cb_t cb;
	bool timed_out;
}fin_dss_notify_t;

typedef struct fin_dss_ctx{ 
	char apn[QAPI_DSS_CALL_INFO_APN_MAX_LEN];
	char username[QAPI_DSS_CALL_INFO_USERNAME_MAX_LEN];
	char password[QAPI_DSS_CALL_INFO_PASSWORD_MAX_LEN];
	TX_EVENT_FLAGS_GROUP *dss_event_handle;
	fin_dss_notify_t *notify;
	TX_THREAD	*thread_handle; 
	TX_BYTE_POOL *thread_byte_pool;
	void *thread_stack;
	qapi_DSS_Hndl_t dss_handle;
	struct{
		TX_EVENT_FLAGS_GROUP *nw_event_handle;
		qapi_Device_Info_Hndl_t device_info_handle;
	};
} fin_dss_ctx_t;


#define fin_dss_set_notify(ctx, tobenotified) do { (ctx)->notify = &(tobenotified)->notify; } while (0) 
#define fin_dss_wait_conn_notify(N,T) \
do { 	\
	uint32_t N##_received_sigs = 0; \
	N->notify->timed_out = tx_event_flags_get(N->notify->evt, QAPI_DSS_EVT_NET_IS_CONN_E, TX_OR_CLEAR, &N##_received_sigs, T); \
} while (0)

fin_dss_ctx_t *fin_dss_new(const char *apn, const char *password, const char *username);
int fin_dss_start(fin_dss_ctx_t *ctx);
int fin_dss_restart(fin_dss_ctx_t *ctx);
int fin_dss_stop(fin_dss_ctx_t *ctx);
int fin_dss_free(fin_dss_ctx_t *ctx);


/* GPIO */
typedef void (*fin_gpio_interrupt_cb_t)(uint32_t arg);

int fin_gpio_pin_on(uint32_t pin, uint32_t trigger, fin_gpio_interrupt_cb_t interrupt_cb) ;
int fin_gpio_pin_trigger(uint32_t pin);
int fin_gpio_pin_disable_irq(uint32_t pin);
int fin_gpio_pin_enable_irq(uint32_t pin);
int fin_gpio_pin_config(uint32_t pin, uint32_t pull, uint32_t drive, uint32_t type);
int fin_gpio_pin_output(uint32_t pin, uint32_t pull, uint32_t drive);
int fin_gpio_pin_input(uint32_t pin, uint32_t pull, uint32_t drive);
int fin_gpio_pin_release(uint32_t pin);
int fin_gpio_pin_write(uint32_t pin, bool val);
int fin_gpio_pin_read(uint32_t pin);
void fin_gpio_config_dump(uint32_t pin);

/* HTTP */

typedef struct http_client_ssl {
	qapi_Net_SSL_Obj_Hdl_t ctx; 
	qapi_Net_SSL_Con_Hdl_t con; 
	qapi_Net_SSL_Config_t config; 
	qapi_Net_SSL_Role_t role;
} http_client_ssl_t;


typedef struct http_client_ctx{
	qapi_Net_HTTPc_handle_t handle;
	TX_BYTE_POOL *byte_pool;
	qapi_Net_HTTPc_Config_t *httpc_cfg;
	struct linger so_linger;
	TX_EVENT_FLAGS_GROUP *evt;
	struct list_head list;
	qapi_HTTPc_CB_t user_callback;
	uint32_t timeout;
	http_client_ssl_t ssl;
	bool use_https;
} fin_http_client_ctx_t;

#define fin_htpp_client_set_user_callback(ctx,cb) do{ ctx->user_callback = cb } while(0)
#define fin_http_client_for_each(p,ctx) list_for_each(p,&ctx->list)
#define fin_htpp_client_has_data(ctx) list_first_entry_or_null(&ctx->list, http_client_entry_t,head)
#define fin_http_client_get_data(iter) list_entry(iter,http_client_entry_t, head )->data
#define fin_http_client_get_data_length(iter) (uint32_t)list_entry(iter,http_client_entry_t, head )->data_len
#define fin_http_client_get_header(iter) list_entry(iter,http_client_entry_t, head )->header
#define fin_http_client_get_header_length(iter) (uint32_t)list_entry(iter,http_client_entry_t, head )->header_len
#define fin_http_client_free_resource(iter,data)	\
do{ \
	list_del(iter); \
	free(data); \
}while (0)

#define fin_http_client_set_timeout(ctx, t) do{ if(ctx){ ctx->timeout = t; } } while(0)


fin_http_client_ctx_t *fin_htpp_client_new(void);
int fin_htpp_client_free(fin_http_client_ctx_t *ctx);
int fin_htpp_client_set_header(fin_http_client_ctx_t *ctx, const char *key, const char *value);
int fin_htpp_client_set_parameter(fin_http_client_ctx_t *ctx, const char *key, const char *value);
int fin_htpp_client_set_body(fin_http_client_ctx_t *ctx, const char *body, uint32_t len);
int fin_htpp_client_get(fin_http_client_ctx_t *ctx, const char *host, int port, const char *path);

/* Hardware Random */
void fin_hwrandombytes(uint8_t *buf, uint64_t size );


/* I2C */

typedef enum fin_i2c_num{
	I2C1,
	I2C2,
	I2C_MAX_NO
} fin_i2c_num_t;


int fin_i2c_transfer(fin_i2c_num_t i2c_num, uint16_t addr, uint8_t *tx_data, size_t tx_data_size, 
	uint8_t *rx_data, size_t rx_data_size, uint32_t delay_us);
int fin_i2c_start(fin_i2c_num_t i2c_num);
int fin_i2c_stop(fin_i2c_num_t i2c_num);
int fin_i2c_config(fin_i2c_num_t i2c_num);
int fin_i2c_deconfig(fin_i2c_num_t i2c_num);
int fin_i2c_set_frequency(fin_i2c_num_t i2c_num, uint32_t frequency);
int fin_i2c_power_down(fin_i2c_num_t i2c_num);
int fin_i2c_power_up(fin_i2c_num_t i2c_num);


/* NET */

#define fin_dump_net_interfaces() \
do{ \
	qapi_Net_Ifnameindex_t itf[10]; \
	int itf_no = qapi_Net_Get_All_Ifnames (&itf); \
	for(int i=0; i < itf_no; i++){ \
		const char * address = 0; \
		uint32_t address_length = 0; \
		printf("Interface = %d, %s, %d\r\n",i,itf[i].interface_Name,itf[i].if_Is_Up); \
       	if(qapi_Net_Interface_Get_Physical_Address(itf[i].interface_Name, &address, &address_length) == 0) \
       		printf("physical address = %c.%c.%c.%c\r\n",address[0],address[1],address[2],address[3]); \
	} \
}while(0); \

#define fin_dump_dns_server_list() \
do { \
	qapi_Net_DNS_Server_List_t svr_list; \
	/* iface_index can be 0,1,2,or 3 */ \
	if (qapi_Net_DNSc_Get_Server_List(&svr_list,0) == 0){ \
		int i; \
		char ip_str[48]; \
		for (i = 0; i < MAX_DNS_SVR_NUM; ++i){ \
			if (svr_list.svr[i].type != AF_UNSPEC){ \
				printf("DNS Server: %s\r\n",	inet_ntop(svr_list.svr[i].type,	&svr_list.svr[i].a, ip_str, sizeof(ip_str))); \
			} \
		} \
	} \
} while(0)

const char *fin_resolve_host_itf(const char *domain, const char *itf, char *buf, size_t buf_len);

#define fin_resolve_host(domain, itf) \
({char itf##_ip_str[48]; \
	fin_resolve_host_itf(domain, #itf, itf##_ip_str, sizeof(itf##_ip_str)); \
    itf##_ip_str;\
})

/* TIMER */

#define fin_sleep(T) qapi_Timer_Sleep(T, QAPI_TIMER_UNIT_MSEC, true)


/* SPI */
typedef enum {
	SPI1,
	SPI2,
	SPI_MAX_NO
} fin_spi_num_t;

typedef enum{
	SPI_CS_DEASSERT,
	SPI_CS_KEEP_ASSERTED,
	SPI_CS_MODE_INVALID = 0x7FFFFFFF
} spi_cs_mode_t;

typedef enum{
	SPI_CS_ACTIVE_LOW,
	SPI_CS_ACTIVE_HIGH
} spi_cs_polarity_t;

typedef enum{
	SPI_BYTE_ORDER_NATIVE = 0,
	SPI_BYTE_ORDER_LITTLE_ENDIAN = 0,
	SPI_BYTE_ORDER_BIG_ENDIAN
}spi_endian_t;

typedef enum{
	SPI_MODE_0, //CPOL = 0, CPHA = 0.
	SPI_MODE_1, //CPOL = 0, CPHA = 1.
	SPI_MODE_2, //CPOL = 1, CPHA = 0.
	SPI_MODE_3 //CPOL = 1, CPHA = 1.
} spi_mode_t;

typedef enum{
	SPI_BPW_3 = 3,
	SPI_BPW_4,
	SPI_BPW_5,
	SPI_BPW_6,
	SPI_BPW_7,
	SPI_BPW_8,
	SPI_BPW_9,
	SPI_BPW_10,
	SPI_BPW_11,
	SPI_BPW_12,
	SPI_BPW_13,
	SPI_BPW_14,
	SPI_BPW_15,
	SPI_BPW_16,
	SPI_BPW_17,
	SPI_BPW_18,
	SPI_BPW_19,
	SPI_BPW_20,
	SPI_BPW_21,
	SPI_BPW_22,
	SPI_BPW_23,
	SPI_BPW_24,
	SPI_BPW_25,
	SPI_BPW_26,
	SPI_BPW_27,
	SPI_BPW_28,
	SPI_BPW_29,
	SPI_BPW_30,
	SPI_BPW_31
}spi_bits_per_word_t;



int fin_spi_power_down(fin_spi_num_t spi_num);
int fin_spi_power_up(fin_spi_num_t spi_num);
int fin_spi_config(fin_spi_num_t spi_num);
int fin_spi_deconfig(fin_spi_num_t spi_num);
int fin_spi_set_frequency(fin_spi_num_t spi_num, uint32_t frequency);
int fin_spi_set_endianess(fin_spi_num_t spi_num, spi_endian_t endian);
int fin_spi_set_bit_per_word(fin_spi_num_t spi_num, spi_bits_per_word_t bpw);
int fin_spi_set_mode(fin_spi_num_t spi_num, spi_mode_t mode);
int fin_spi_set_inter_word_delay(fin_spi_num_t spi_num, uint8_t delay);
int fin_spi_set_loopback(fin_spi_num_t spi_num, bool enable);
int fin_spi_set_chip_select_mode(fin_spi_num_t spi_num, spi_cs_mode_t mode);
int fin_spi_set_chip_select_polarity(fin_spi_num_t spi_num, spi_cs_polarity_t polarity);
int fin_spi_set_chip_select_delay(fin_spi_num_t spi_num, uint8_t delay);
int fin_spi_set_slaves_num(fin_spi_num_t spi_num, uint8_t num);
int fin_spi_send(fin_spi_num_t spi_num, uint8_t *tx_buf, size_t tx_len);
int fin_spi_send_receive(fin_spi_num_t spi_num, uint8_t *tx_buf, uint8_t *rx_buf, size_t rx_len);
void fin_spi_config_dump(fin_spi_num_t spi_num);
const char *fin_spi_get_name(fin_spi_num_t spi_num);

/* UART */

typedef enum fin_uart{
	UART1,
	UART2,
	UART3	
} fin_uart_t;

int fin_uart_config(fin_uart_t uart, uint32_t baud_rate, uint32_t stop_bits, uint32_t bits_per_char, uint32_t parity, uint32_t flow_ctrl);
int fin_uart_write(fin_uart_t uart, uint8_t *buf, size_t nbytes);
int fin_uart_read(fin_uart_t uart, uint8_t *buf, size_t nbytes);
int fin_uart_deconfig(fin_uart_t uart);
int fin_uart_power_up(fin_uart_t uart);
int fin_uart_power_down(fin_uart_t uart);


/*
 *  C++ name mangling
 */

#if defined(__cplusplus)
/* end 'extern "C"' wrapper */
}
#endif

/*
 *  END PUBLIC API
 */

#endif  /* SINGLEFIN_H_INCLUDED */