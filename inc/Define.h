/**
 * @file Define.h
 * @brief  Header for system
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
#ifndef __DEFINE_H__
#define __DEFINE_H__ 

//#define FEATURE_SERVER_REQUEST
///-----------------------------------------------------------------------///
/// DBG PRINTF
///-----------------------------------------------------------------------///
#define DBG_PRINTF        JPrintf
#define DBG_PRINTF2(x)    JPrintf(DBG_ID, x)
#define ERR_PRINTF				UtilErrPrint
#define DBG_PRINTFW				JPrintfW

#define DBG_ID	          (0)

#define TRUE						  (1)
#define FALSE 						(0)

#ifndef PI
	#define PI	(3.1415926)
#endif ///< PI

///----------------------------------------------------------------///
/// DID
///----------------------------------------------------------------///
#define DEVICE_DID_MIN     (1)
#define DEVICE_DID_MAX     (4294967295) ///< 4294967295

#define GATEWAY_TYPE_LTE   (0x100)
#define GATEWAY_TYPE_WIFI  (0x101)

#define ONLINE						(1)
#define OFFLINE						(0)

#ifndef NULL
#define NULL (0)
#endif ///< for NULL

///-----------------------------------------------------------------------///
/// OS_TYPE
///-----------------------------------------------------------------------///
#define OS_TYPE_WINDOWS               (0)
#define OS_TYPE_UBUNTU                (1)
#define OS_TYPE_MAC_OS                (2)
#define OS_TYPE_UNKNOWN               (100)

///----------------------------------------------------------------///
/// Device Data
///----------------------------------------------------------------///
#define DEVICE_OFFLINE	              (0)
#define DEVICE_ONLINE	                (1)
#define DEVICE_ONLINE_OFFLINE    	    (2)

///----------------------------------------------------------------///
/// Device Data
///----------------------------------------------------------------///
#define DEVICE_ECG_OFFLINE	          (0)
#define DEVICE_ECG_ONLINE	            (1)
#define DEVICE_ECG_ONLINE_OFFLINE    	(2)
  
///----------------------------------------------------------------///
/// Global Status
///----------------------------------------------------------------///
#define STATUS_ONLINE			    	      (1<<0)
#define STATUS_MEASUREMENT			      (1<<1)
#define STATUS_QUIT                   (1<<2)
#define STATUS_SOCKET_FAIL            (1<<3)
#define STATUS_USER_CLOSE             (1<<4)

///----------------------------------------------------------------///
/// Wait time
///----------------------------------------------------------------///
#define WAIT_100US							(100)
#define WAIT_1MS								(1000)
#define WAIT_10MS								(10 * WAIT_1MS)
#define WAIT_100MS							(100 * WAIT_1MS)
#define WAIT_500MS							(500 * WAIT_1MS)
#define WAIT_1SEC								(1000 * WAIT_1MS)
#define WAIT_3SEC								(3 * WAIT_1SEC)

///----------------------------------------------------------------///
/// File path
///----------------------------------------------------------------///
#define DATA_TYPE_CSV_CH1       (100)
#define DATA_TYPE_CSV_CH2       (101)
#define DATA_TYPE_CSV_CH3       (102)
#define DATA_TYPE_CSV_CH4       (103)
#define DATA_TYPE_CSV_CH5       (104)
#define DATA_TYPE_CSV_CH6       (105)
#define DATA_TYPE_CSV_CH7       (106)
#define DATA_TYPE_CSV_CH8       (107)
#define DATA_TYPE_CSV_CH9       (108)
#define DATA_TYPE_CSV_CH10      (109)
#define DATA_TYPE_BIN_CH1       (200)
#define DATA_TYPE_BIN_CH2       (201)
#define DATA_TYPE_BIN_CH3       (202)
#define DATA_TYPE_BIN_CH4       (203)
#define DATA_TYPE_BIN_CH5       (204)
#define DATA_TYPE_BIN_CH6       (205)
#define DATA_TYPE_BIN_CH7       (206)
#define DATA_TYPE_BIN_CH8       (207)
#define DATA_TYPE_BIN_CH9       (208)
#define DATA_TYPE_BIN_CH10      (209)
#define DATA_TYPE_INFO_01       (300)
#define DATA_TYPE_INFO_02       (301)
#define DATA_TYPE_INFO_03       (302)
#define DATA_TYPE_INFO_04       (303)
#define DATA_TYPE_INFO_05       (304)
#define DATA_TYPE_INFO_06       (305)
#define DATA_TYPE_INFO_07       (306)
#define DATA_TYPE_INFO_08       (307)
#define DATA_TYPE_INFO_09       (308)
#define DATA_TYPE_INFO_10       (309)

#define SYSTIME_INIT_CHECK      (1652770000)  ///< 2022/5/17 
#define INVALID_FILE			   	  (-1)

///----------------------------------------------------------------///
/// Vender define
///----------------------------------------------------------------///
#define GW_SYS_ECG_MODE                   			(0)
#define DEV_ECG_INFO_TIMEOUT              			(30)

///----------------------------------------------------------------///
/// DEVICE EVENT0
///----------------------------------------------------------------///
#define DEV_EVT0_SEC_EVENT											(1 << 0)
#define DEV_EVT0_100MS_EVENT										(1 << 1)
#define DEV_EVT0_200MS_EVENT										(1 << 2)
#define DEV_EVT0_300MS_EVENT										(1 << 3)
#define DEV_EVT0_400MS_EVENT										(1 << 4)
#define DEV_EVT0_500MS_EVENT										(1 << 5)

///----------------------------------------------------------------///
/// SOCKET
///----------------------------------------------------------------///
#define SOCK_CMD_TIMEOUT												(399)   	///< 4 packets LTE costs 350~ 380 ms, wifi costs 433 ms

///----------------------------------------------------------------///  
///  Gateway status 
///----------------------------------------------------------------///
#define GW_SYS_STS_CLEAN                  			(0 << 0)
#define GW_SYS_STS_ALWAYS_ON              			(1 << 0)  ///< long press
#define GW_SYS_STS_MAYDAY                 			(1 << 1)  
#define GW_SYS_STS_NOTICE                  			(1 << 2)
#define GW_SYS_STS_ON_DUTY                			(1 << 3)  ///< check - in 
#define GW_SYS_STS_CALL_HELP              			(1 << 4)  
                                                                                           
///----------------------------------------------------------------///  
///  MEASUREMENT TYPE
///----------------------------------------------------------------///
#define REPORT_TYPE_FOREVER                     (0)

///----------------------------------------------------------------///  
///  MEASUREMENT STATE
///----------------------------------------------------------------///
#define MEAS_STATE_INIT	                    		(10)

#define MEAS_STATE_FINISHED                 		(0)
#define MEAS_STATE_RUNNING                  		(1)
#define MEAS_STATE_STOP                     		(2)
#define MEAS_STATE_START                    		(3)
#define MEAS_STATE_CONNECTING               		(4)
#define MEAS_STATE_FILE_UPLOAD_FINISHED	    		(5)
#define MEAS_STATE_FILE_UPLOAD_ANNOTATION   		(6)
#define MEAS_STATE_FILE_UPLOAD       						(7)

#endif ///<  __DEFINE_H__
