/**<-Quectel TXM API IDs Start with 0x20000->*/
TXM_OEM_API_QT_BASE                 = TXM_QC_OEM_API_BASE,

/****************** System APIs *****************/
TXM_OEM_API_QT_SYS_START            = TXM_OEM_API_QT_BASE + 1,
TXM_OEM_API_QT_SYS_RESET_DEVICE     = TXM_OEM_API_QT_BASE + 2,
TXM_OEM_API_QT_SYS_SHUTDOWN_DEVICE  = TXM_OEM_API_QT_BASE + 3,
TXM_OEM_API_QT_SYS_SAHARA_GET       = TXM_OEM_API_QT_BASE + 4,
TXM_OEM_API_QT_SYS_SAHARA_SET       = TXM_OEM_API_QT_BASE + 5,
TXM_OEM_API_QT_SYS_NMEA_SIO_OPEN    = TXM_OEM_API_QT_BASE + 6,
TXM_OEM_API_QT_SYS_NMEA_SIO_CLOSE   = TXM_OEM_API_QT_BASE + 7,
TXM_OEM_API_QT_SYS_NMEA_SIO_SEND    = TXM_OEM_API_QT_BASE + 8,
TXM_OEM_API_QT_SYS_MP_FW_VER_GET    = TXM_OEM_API_QT_BASE + 10,
TXM_OEM_API_QT_SYS_AP_FW_VER_GET    = TXM_OEM_API_QT_BASE + 11,
TXM_OEM_API_QT_SYS_IMEI_GET         = TXM_OEM_API_QT_BASE + 12,
TXM_OEM_API_QT_SYS_MP_CORE_INFO_GET = TXM_OEM_API_QT_BASE + 13,
TXM_OEM_API_QT_SYS_AP_CORE_INFO_GET = TXM_OEM_API_QT_BASE + 14,
TXM_OEM_API_QT_SYS_MANU_INFO_GET    = TXM_OEM_API_QT_BASE + 15,
TXM_OEM_API_QT_SYS_PRNG_DATA_GET    = TXM_OEM_API_QT_BASE + 16,
TXM_OEM_API_QT_SYS_GET_USB_EVENT    = TXM_OEM_API_QT_BASE + 17,
TXM_OEM_API_QT_SYS_END              = TXM_OEM_API_QT_BASE + 49,

/****************** Network APIs *****************/
TXM_OEM_API_QT_NW_START             	= TXM_OEM_API_QT_BASE + 50,
TXM_OEM_API_QT_PHONE_FUNC_SET			= TXM_OEM_API_QT_BASE + 51,
TXM_OEM_API_QT_PHONE_FUNC_GET			= TXM_OEM_API_QT_BASE + 52,
TXM_OEM_API_QT_REAL_TIME_CLOCK_SET		= TXM_OEM_API_QT_BASE + 53,
TXM_OEM_API_QT_REAL_TIME_CLOCK_GET		= TXM_OEM_API_QT_BASE + 54,
TXM_OEM_API_QT_NW_BAND_PREF_SET			= TXM_OEM_API_QT_BASE + 55,
TXM_OEM_API_QT_NW_BAND_PREF_GET			= TXM_OEM_API_QT_BASE + 56,
TXM_OEM_API_QT_NW_RAT_PREF_SET			= TXM_OEM_API_QT_BASE + 57,
TXM_OEM_API_QT_NW_RAT_PREF_GET			= TXM_OEM_API_QT_BASE + 58,
TXM_OEM_API_QT_NW_RAT_SCAN_PREF_SET		= TXM_OEM_API_QT_BASE + 59,
TXM_OEM_API_QT_NW_RAT_SCAN_PREF_GET		= TXM_OEM_API_QT_BASE + 60,
TXM_OEM_API_QT_NW_SRV_DOMAIN_PREF_SET	= TXM_OEM_API_QT_BASE + 61,
TXM_OEM_API_QT_NW_SRV_DOMAIN_PREF_GET	= TXM_OEM_API_QT_BASE + 62,
TXM_OEM_API_QT_NW_PDP_CFG_SET			= TXM_OEM_API_QT_BASE + 63,
TXM_OEM_API_QT_NW_PDP_CFG_GET			= TXM_OEM_API_QT_BASE + 64,
TXM_OEM_API_QT_NW_GSM_MEAS_INFO_GET		= TXM_OEM_API_QT_BASE + 65,
TXM_OEM_API_QT_NW_LTE_MEAS_INFO_GET		= TXM_OEM_API_QT_BASE + 66,
TXM_OEM_API_QT_NW_PSM_CFG_SET			= TXM_OEM_API_QT_BASE + 67,
TXM_OEM_API_QT_NW_PSM_CFG_GET			= TXM_OEM_API_QT_BASE + 68,
TXM_OEM_API_QT_NW_EDRX_CFG_SET			= TXM_OEM_API_QT_BASE + 69,
TXM_OEM_API_QT_NW_EDRX_CFG_GET			= TXM_OEM_API_QT_BASE + 70,
TXM_OEM_API_QT_NW_EXTEND_BAND_PREF_SET	= TXM_OEM_API_QT_BASE + 71,
TXM_OEM_API_QT_NW_EXTEND_BAND_PREF_GET	= TXM_OEM_API_QT_BASE + 72,

TXM_OEM_API_QT_NW_END               = TXM_OEM_API_QT_BASE + 99,

/****************** SIM APIs *****************/
TXM_OEM_API_QT_SIM_START            = TXM_OEM_API_QT_BASE + 100,
TXM_OEM_API_QT_SIM_RDY_Check        = TXM_OEM_API_QT_BASE + 101,
TXM_OEM_API_QT_SIM_IMSI_Get         = TXM_OEM_API_QT_BASE + 102,
TXM_OEM_API_QT_SIM_CCID_Get         = TXM_OEM_API_QT_BASE + 103,
TXM_OEM_API_QT_SIM_MSISDN_Get       = TXM_OEM_API_QT_BASE + 104,
TXM_OEM_API_QT_SIM_END              = TXM_OEM_API_QT_BASE + 149,

/****************** SMS APIs *****************/
TXM_OEM_API_QT_SMS_START            = TXM_OEM_API_QT_BASE + 150,
TXM_OEM_API_QT_SMS_SEND             = TXM_OEM_API_QT_BASE + 151,
TXM_OEM_API_QT_SMS_READ             = TXM_OEM_API_QT_BASE + 152,
TXM_OEM_API_QT_SMS_DELETE           = TXM_OEM_API_QT_BASE + 153,
TXM_OEM_API_QT_SMS_READY            = TXM_OEM_API_QT_BASE + 154,
TXM_OEM_API_QT_SMS_END              = TXM_OEM_API_QT_BASE + 199,

/****************** FTP APIs *****************/
TXM_OEM_API_QT_FTP_START            = TXM_OEM_API_QT_BASE + 200,
TXM_OEM_API_QT_FTP_END              = TXM_OEM_API_QT_BASE + 249,

/****************** URC APIs *****************/
TXM_OEM_API_QT_URC_START            = TXM_OEM_API_QT_BASE + 250,
TXM_OEM_API_QT_URC_END              = TXM_OEM_API_QT_BASE + 299,

/*****************  ATC Pipe *****************/
TXM_OEM_API_AT_CMD_START            = TXM_OEM_API_QT_BASE + 300,
TXM_OEM_API_AT_PORT_OPEN            = TXM_OEM_API_QT_BASE + 301,
TXM_OEM_API_AT_SEND                 = TXM_OEM_API_QT_BASE + 302,
TXM_OEM_API_AT_SEND_HEXBYTE         = TXM_OEM_API_QT_BASE + 303,
TXM_OEM_API_AT_SEND_GENERAL         = TXM_OEM_API_QT_BASE + 304,
TXM_OEM_API_AT_PORT_CLOSE           = TXM_OEM_API_QT_BASE + 305,
TXM_OEM_API_AT_CMD_END              = TXM_OEM_API_QT_BASE + 310,


/**<-Quectel TXM API IDs End with 0x30000->*/
TXM_OEM_API_QT_END                  = TXM_QC_OEM_API_END
