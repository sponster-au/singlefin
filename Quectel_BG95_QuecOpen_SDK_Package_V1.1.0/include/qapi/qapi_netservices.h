/* Copyright (c) 2015-2019 Qualcomm Technologies, Inc.
All rights reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.
*/

/*
!! IMPORTANT NOTE: "DATA SERVICES" VERSION CATEGORY. 
*/

/** @file qapi_netservices.h
*/

#ifndef _QAPI_NETSERVICES_H_
#define _QAPI_NETSERVICES_H_


/*********************************************************************************
 * Status codes
**********************************************************************************/
#include "qapi_net_status.h"

/*********************************************************************************
 * Utilities
**********************************************************************************/
#include "qapi_ns_utils.h"
#include "qapi_netbuf.h"

/*********************************************************************************
 * General IPv4/IPv6 Network Services
**********************************************************************************/
/** @addtogroup qapi_net_services_datatypes
@{ */

/** Commands for routing QAPI net services. */
typedef enum {
    QAPI_NET_ROUTE_ADD_E,
    /**< Add route. */
    QAPI_NET_ROUTE_DEL_E,
    /**< Delete route. */
    QAPI_NET_ROUTE_SHOW_E,
    /**< Show routes. */
    QAPI_NET_ROUTE_MAX_E
} qapi_Net_Route_Command_t;

/** @} */

#include "qapi_ns_gen_v4.h"
#include "qapi_ns_gen_v6.h"

/*********************************************************************************
 * DHCPv6 Client
**********************************************************************************/

/*********************************************************************************
 * DHCPv4 Client
**********************************************************************************/

/*********************************************************************************
 * DHCPv4 Server
**********************************************************************************/

/*********************************************************************************
 * Bridging
**********************************************************************************/

/*********************************************************************************
 * HTTP Client
**********************************************************************************/
#include "qapi_httpc.h"

/*********************************************************************************
 * HTTP Server
**********************************************************************************/

/*********************************************************************************
 * DNS Client
**********************************************************************************/
#include "qapi_dnsc.h"

/*********************************************************************************
 * DNS Server
**********************************************************************************/

/*********************************************************************************
 * SNTP Client
**********************************************************************************/

#if !defined(QAPI_TXM_MODULE)


UINT qapi_Net_Handler(UINT id, UINT a1, UINT a2, UINT a3, UINT a4, UINT a5, UINT a6, UINT a7, UINT a8, UINT a9, UINT a10, UINT a11, UINT a12);

#endif

#endif /* _QAPI_NETSERVICES_H_ */
