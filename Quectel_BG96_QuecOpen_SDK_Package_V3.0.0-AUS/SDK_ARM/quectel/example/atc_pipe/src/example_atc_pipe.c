/******************************************************************************
*@file    example_atc_pipe.c
*@brief   example of atc pipe operation
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#if defined(__EXAMPLE_ATC_PIPE__)
#include "txm_module.h"
#include "qapi_diag.h"
#include "qapi_timer.h"
#include "qapi_uart.h"
#include "qapi_quectel.h"
#include "quectel_utils.h"
#include "quectel_uart_apis.h"
#include "example_atc_pipe.h"

/**************************************************************************
*                                 DEFINE
***************************************************************************/


/**************************************************************************
*                                 GLOBAL
***************************************************************************/
/* uart1 rx tx buffer */
static char tx1_buff[1024];

/* uart config para*/
static QT_UART_CONF_PARA uart1_conf =
{
	NULL,
	QT_UART_PORT_02,
	tx1_buff,
	sizeof(tx1_buff),
	NULL,
	NULL,
	115200
};

qapi_at_stream_id_t stream_id_0, stream_id_1;
static char at_cmd_rsp[3096] = {0};
TX_EVENT_FLAGS_GROUP* atc_pipe_sig_handle;
static qapi_at_pipe_data_t pipe_data;

/*===========================================================================

                           Static & global Variable Declarations

===========================================================================*/
static void callback_atc_pipe_0(qapi_at_pipe_data_t *data)
{
	ULONG sig_event = 0;

	/* Must consider the URC report firstly */

	/* Normal ATC response handler */
	tx_event_flags_get(atc_pipe_sig_handle, SIG_EVG_ATPIPE_SENDING_E, TX_OR, &sig_event, TX_NO_WAIT);
	if(sig_event & SIG_EVG_ATPIPE_SENDING_E)
	{
		tx_event_flags_set(atc_pipe_sig_handle, ~SIG_EVG_ATPIPE_SENDING_E, TX_AND);
		tx_event_flags_set(atc_pipe_sig_handle, SIG_EVG_ATPIPE_RECING_E, TX_OR);
		memset(at_cmd_rsp, 0, sizeof(at_cmd_rsp));
	}
	
	IOT_DEBUG("[DBG][ATC-1]Current Data: %s, %d",data->data, data->len);
	strcat(at_cmd_rsp, data->data);

	if(strstr(data->data,"\r\nOK") != NULL)
	{
		tx_event_flags_set(atc_pipe_sig_handle, SIG_EVG_ATPIPE_REC_OK_E, TX_OR);
	}
}


int quectel_task_entry
(
    void
)
{
	qapi_Status_t status = QAPI_OK;
	ULONG sig_event = 0;
	qapi_at_stream_id_t stream_id = stream_id_0;
	
	qapi_Timer_Sleep(3, QAPI_TIMER_UNIT_SEC, true);
    txm_module_object_allocate(&atc_pipe_sig_handle, sizeof(TX_EVENT_FLAGS_GROUP));
	/* Create event signal handle and clear signals */
	tx_event_flags_create(atc_pipe_sig_handle, "atc_pipe_sig_handle");
	tx_event_flags_set(atc_pipe_sig_handle, 0, TX_AND);

	/* uart 1 init */
	uart_init(&uart1_conf);
	
	/* prompt task running */
	qt_uart_dbg(uart1_conf.hdlr,"[atc pipe] start task ~");

	/* Open first ATC port */
 	status = qapi_QT_Apps_AT_Port_Open(QAPI_AT_PORT_0, &stream_id_0, callback_atc_pipe_0, &pipe_data);
	if(status != QAPI_QT_ERR_OK)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[atc pipe 0] status[0x%x]", status);
	}
	tx_event_flags_set(atc_pipe_sig_handle, SIG_EVG_ATPIPE_SENDING_E, TX_OR);
	status = qapi_QT_Apps_Send_AT(stream_id_0, "ATI\r\n");
	if(status != QAPI_QT_ERR_OK)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[atc pipe 0] status[0x%x]", status);
	}
	tx_event_flags_get(atc_pipe_sig_handle, SIG_EVG_ATPIPE_REC_OK_E, TX_OR, &sig_event, TX_WAIT_FOREVER);
	if(sig_event & SIG_EVG_ATPIPE_REC_OK_E)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[atc pipe 0] rsp[%s]", at_cmd_rsp);
		tx_event_flags_set(atc_pipe_sig_handle, ~SIG_EVG_ATPIPE_REC_OK_E, TX_AND);
		tx_event_flags_set(atc_pipe_sig_handle, SIG_EVG_ATPIPE_STANDBY_E, TX_OR);
	}

	while(1)
	{
		tx_event_flags_set(atc_pipe_sig_handle, SIG_EVG_ATPIPE_SENDING_E, TX_OR);
		
		/* loop test */
			
		status = qapi_QT_Apps_Send_AT(stream_id_0, "AT+CCLK?\r\n");
		if(status != QAPI_QT_ERR_OK)
		{
			qt_uart_dbg(uart1_conf.hdlr,"[atc pipe] status[0x%x]", status);
		}
		tx_event_flags_get(atc_pipe_sig_handle, SIG_EVG_ATPIPE_REC_OK_E, TX_OR, &sig_event, TX_WAIT_FOREVER);
		if(sig_event & SIG_EVG_ATPIPE_REC_OK_E)
		{
			qt_uart_dbg(uart1_conf.hdlr,"[atc pipe] rsp[%s]", at_cmd_rsp);
			tx_event_flags_set(atc_pipe_sig_handle, ~SIG_EVG_ATPIPE_REC_OK_E, TX_AND);
			tx_event_flags_set(atc_pipe_sig_handle, SIG_EVG_ATPIPE_STANDBY_E, TX_OR);
		}

		qapi_Timer_Sleep(3, QAPI_TIMER_UNIT_SEC, true);
	}
}

#endif /*__EXAMPLE_PSM__*/

