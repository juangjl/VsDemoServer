/**
 * @file JGateway.h 
 * @brief header of gateway functions
 * @version $Revision$
 * @author JL Juang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2023,  VitalSigns Technology Co., LTD., all rights reserved.
 * @note
*/
#ifndef __JGATEWAY_H__
#define __JGATEWAY_H__ 

#include "Global.h"

#define GATEWAY_DID             ("GATEWAY_DID")
#define GATEWAY_LTE_IP          ("GATEWAY_LTE_IP")
#define GATEWAY_LTE_PORT        ("GATEWAY_LTE_PORT")
#define GATEWAY_LTE_APN         ("GATEWAY_LTE_APN")     
#define GATEWAY_BTN_EVT_TIME    ("GATEWAY_BTN_EVT_TIME")  
#define GATEWAY_BLE_MAC         ("GATEWAY_BLE_MAC")
#define GATEWAY_BLE_MAC0        ("GATEWAY_BLE_MAC0")   
#define GATEWAY_BLE_MAC1        ("GATEWAY_BLE_MAC1")   
#define GATEWAY_GSEN_SEC        ("GATEWAY_GSEN_SEC")  
#define GATEWAY_GSEN_TH         ("GATEWAY_GSEN_TH")  
#define GATEWAY_TEMP_OFFSET     ("GATEWAY_TEMP_OFFSET")

#define GATEWAY_WIFI_SSID_NAME0 ("GATEWAY_WIFI_SSID_NAME0")
#define GATEWAY_WIFI_SSID_PWD0  ("GATEWAY_WIFI_SSID_PWD0")
#define GATEWAY_WIFI_SSID_NAME1 ("GATEWAY_WIFI_SSID_NAME1")
#define GATEWAY_WIFI_SSID_PWD1  ("GATEWAY_WIFI_SSID_PWD1")
#define GATEWAY_WIFI_SSID_NAME2 ("GATEWAY_WIFI_SSID_NAME2")
#define GATEWAY_WIFI_SSID_PWD2  ("GATEWAY_WIFI_SSID_PWD2")
#define GATEWAY_WIFI_SSID_NAME3 ("GATEWAY_WIFI_SSID_NAME3")
#define GATEWAY_WIFI_SSID_PWD3  ("GATEWAY_WIFI_SSID_PWD3")

#define GATEWAY_DEV_ALARM_ITEM  ("GATEWAY_DEV_ALARM_ITEM")

extern void   GatewayInfoPrint(JDevice *pDevice, JGatewayInfo * pGatewayInfo);

extern JBOOL  GatewayInfoCheck(JDevice *pDevice, JGatewayInfo * pGatewayInfo);

extern void   GatewayDataFolderSet(JDevice *pDevice);

extern void 	GatewayInfoWrite(JUINT dwDID, char *fileName, JGatewayInfo * pGatewayInfo, JINT iGatewayDeviceMode);

extern void   GatewayMBleOpenDidAddrGet(JDevice *pDevice, JINT *pDid);
extern JBOOL  GatewayMBleOpen(JDevice *pDevice);
extern JBOOL  GatewayMBleClose(JDevice *pDevice);
extern JBOOL  GatewayMBleConnectCheck(JDevice *pDevice);

extern DeviceEcgType * GatewayDeviceEcgListItemGet(JDevice *pDevice, JDWORD dwDID);

extern JBOOL  GatewayMBleOpen(JDevice *pDevice, JDWORD dwDeviceEcgDid);

#endif ///<  __JGATEWAY_H__

