/**
 * @file Gateway.cpp 
 * @brief Gateway related functions 
 * @version $Revision$
 * @author JL Juang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2022,  VitalSigns Technology Co., LTD., all rights reserved.
 * @note
*/
#include "Global.h"

void GatewayInfoPrint(JDevice *pDevice, JGatewayInfo *pGatewayInfo)
{
  JINT iDid = pDevice->dwDID;
   
  DBG_PRINTF(iDid, "------------------------ GATEWAY INFO ------------------------\r\n");
  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_year",                           pGatewayInfo->iYear);                  ///< 00
  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_month",                          pGatewayInfo->iMonth);                 ///< 01
  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_day",                            pGatewayInfo->iDay);                   ///< 02
  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_hour",                           pGatewayInfo->iHour);                  ///< 03
  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_min",                            pGatewayInfo->iMin);                   ///< 04
  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_sec",                            pGatewayInfo->iSec);                   ///< 05

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_did",                            pGatewayInfo->iGatewayDid);            ///< 06
  DBG_PRINTF(iDid, "\t %-30s = %0.2f oc\r\n",  "gateway_temp",                           pGatewayInfo->fGatewayTemp);           ///< 07
  DBG_PRINTF(iDid, "\t %-30s = %0.2f mV\r\n",  "gateway_volt",                           pGatewayInfo->fGatewayVolt);           ///< 08
  DBG_PRINTF(iDid, "\t %-30s = %d %%\r\n",     "gateway_batt_level",                     pGatewayInfo->iGatewayBattLevel);      ///< 09

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_utc",                            pGatewayInfo->dwGatewayUtc);           ///< 10
  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_online_time",                    pGatewayInfo->dwGatewayOnlineTime);    ///< 11

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_btn_event0",                     pGatewayInfo->iBtnEvent00);            ///< 12
  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_btn_event1",                     pGatewayInfo->iBtnEvent01);            ///< 13

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_btn_event2",                     pGatewayInfo->iSosBtnLongPressEvt);    ///< 14  SOS button
  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_alarm",                          pGatewayInfo->dwGatewayAlarm);         ///< 15

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_contol",                         pGatewayInfo->dwEventControl);         ///< 16
  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_status",                         pGatewayInfo->dwGatewayStatus);        ///< 17
  DBG_PRINTF(iDid, "\t %-30s = %s\r\n",        "gateway_name",                           pGatewayInfo->strGatewayName);         ///< 18-23

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_ecg_item_count",                 pGatewayInfo->iEcgItemCount);          ///< 24

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_temp_item_count",                pGatewayInfo->iTempItemCount);         ///< 25

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_long_btn_event",                 pGatewayInfo->iLongBtnEvent);          ///< 26

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_online",                         pGatewayInfo->iGatewayOnline);         ///< 27

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_version",                        pGatewayInfo->iVersion);               ///< 28

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_vsc_mode_queue_count",           pGatewayInfo->iVscModeQueueCnt);       ///< 29

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_ble_connect_status",             pGatewayInfo->iBleConnectStatus);      ///< 30 : 0 disconnected, 1: connected

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_monitor_file_utc",               pGatewayInfo->dwMonitorFileUtc);       ///< 31
  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_monitor_file_size",              pGatewayInfo->dwMonitorFileSize);      ///< 32

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_open_ble_did",                   pGatewayInfo->iBleOpenDid);            ///< 33: 0: no auto-connect, other: auto connected to the did

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_vsc_mode_on",                    pGatewayInfo->iVscModeOn);             ///< 34: 

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_vsc_mode_utc_start",             pGatewayInfo->dwVscModeStartUtc);      ///< 35: 

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_queue_header_utc",               pGatewayInfo->dwQueueHeaderUtc);       ///< 36: 

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_button_event_count",             pGatewayInfo->iButtonDeviceCnt);       ///< 37:

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_location_sensor_count",          pGatewayInfo->iLocationDeviceCount);   ///< 38: 
  
  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_oximeter_sensor_count",          pGatewayInfo->iOximeterCount);         ///< 39: 

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_weighing_scale_count",           pGatewayInfo->iWeighingScaleCount);    ///< 40: 

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_thermometer_ir_count",           pGatewayInfo->iThermometerIRCount);    ///< 41: 

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_bp_meter_count",                 pGatewayInfo->iBloodPressureMeterCnt); ///< 42:

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_glucose_meter_count",            pGatewayInfo->iGlucoseMeterCount);     ///< 43: 

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_notification",                   pGatewayInfo->iNotification);          ///< 44: 

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_notification",                   pGatewayInfo->iNotification);          ///< 44: 

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_info_status",                    pGatewayInfo->iNotification);          ///< 45:

  DBG_PRINTF(iDid, "\t %-30s = %d\r\n",        "gateway_notification_utc",               pGatewayInfo->iNotification);          ///< 46: 

  DBG_PRINTF(iDid, "\r\n");
}

JBOOL GatewayInfoCheck(JDevice *pDevice, JGatewayInfo * pGatewayInfo)
{
  JBOOL bValid = TRUE;
  JINT iDid = pDevice->dwDID;

  ///----------------------------------------------------------------///
  /// 1. Year check
  ///----------------------------------------------------------------///
  if((pGatewayInfo->iYear < 2022) ||(pGatewayInfo->iYear > 2100))
  {
    DBG_PRINTF(iDid, "\t [GATEWAY][VALID][CHECK][!!!!!!] YEAR is invalid\r\n");

    bValid = FALSE;
    return bValid;
  }

  ///----------------------------------------------------------------///
  /// 2. Month check
  ///----------------------------------------------------------------///
  if((pGatewayInfo->iMonth < 1) ||(pGatewayInfo->iMonth > 12))
  {
    DBG_PRINTF(iDid, "\t [GATEWAY][VALID][CHECK][!!!!!!] MONTH is invalid\r\n");

    bValid = FALSE;
    return bValid;
  }

  ///----------------------------------------------------------------///
  /// 3. Day check
  ///----------------------------------------------------------------///
  if((pGatewayInfo->iDay < 1) ||(pGatewayInfo->iDay > 31))
  {
    DBG_PRINTF(iDid, "\t [GATEWAY][VALID][CHECK][!!!!!!] DAY is invalid\r\n");

    bValid = FALSE;
    return bValid;
  }

  ///----------------------------------------------------------------///
  /// 4. Hour check
  ///----------------------------------------------------------------///
  if((pGatewayInfo->iHour < 0) ||(pGatewayInfo->iHour > 23))
  {
    DBG_PRINTF(iDid, "\t [GATEWAY][VALID][CHECK][!!!!!!] HOUR is invalid\r\n");

    bValid = FALSE;
    return bValid;
  }

  ///----------------------------------------------------------------///
  /// 5. Min check
  ///----------------------------------------------------------------///
  if((pGatewayInfo->iMin < 0) ||(pGatewayInfo->iMin > 59))
  {
    DBG_PRINTF(iDid, "\t [GATEWAY][VALID][CHECK][!!!!!!] MIN is invalid\r\n");

    bValid = FALSE;
    return bValid;
  }

  ///----------------------------------------------------------------///
  /// 5. Sec check
  ///----------------------------------------------------------------///
  if((pGatewayInfo->iSec < 0) ||(pGatewayInfo->iSec > 59))
  {
    DBG_PRINTF(iDid, "\t [GATEWAY][VALID][CHECK][!!!!!!] SEC is invalid\r\n");

    bValid = FALSE;
    return bValid;
  }

  ///----------------------------------------------------------------///
  /// 6. Gateway DID check
  ///----------------------------------------------------------------///
  if((pGatewayInfo->iGatewayDid < 0) ||(pGatewayInfo->iGatewayDid > 900000000))
  {
    DBG_PRINTF(iDid, "\t [GATEWAY][VALID][CHECK][!!!!!!] DID is invalid\r\n");

    bValid = FALSE;
    return  bValid;
  }

  ///----------------------------------------------------------------///
  /// 7. Gateway temp check
  ///----------------------------------------------------------------///
  if((pGatewayInfo->fGatewayVolt < 0) ||(pGatewayInfo->fGatewayTemp > 5000))
  {
    DBG_PRINTF(iDid, "\t [GATEWAY][VALID][CHECK][!!!!!!] VOLT is invalid\r\n");

    bValid = FALSE;
    return bValid;
  }
  
  return bValid;
}

void GatewayDataFolderSet(JDevice *pDevice)
{  
  /// add the Gateway folder
  sprintf((char *)&pDevice->strGatewayDataRoot[0], "%sGATEWAY", pGlobalVar->strDataRoot);
  UtilFolderCreate((char *)&pDevice->strGatewayDataRoot[0]);

  /// get the DID item folder
  sprintf((char *)&pDevice->strGatewayDataRoot[0], "%sGATEWAY/DID_%07d",  pGlobalVar->strDataRoot, pDevice->dwDID);
  UtilFolderCreate((char *)&pDevice->strGatewayDataRoot[0]);

  DBG_PRINTF(pDevice->dwDID, "[INFO]GATEWAY FOLDER : %s\r\n", &pDevice->strGatewayDataRoot[0]);
}

DeviceEcgType * GatewayDeviceEcgListItemGet(JDevice *pDevice, JDWORD dwDID)
{
  DeviceEcgType *pItem = NULL;
  JINT i = 0;
  JBOOL iFind = FALSE;
  
  for(i = 0 ; i < pDevice->ecgObjectList.iEcgObjectCount; i = i + 1)
  {
    pItem = &pDevice->ecgObjectList.ecgObjectArr[i];

    if(pItem->dwDid == dwDID)
    {
      iFind = TRUE;
      break;
    }
  }
  if(iFind == FALSE)
  {
    return NULL;
  }
  return pItem;
}

JBOOL GatewayMBleConnectCheck(JDevice *pDevice)
{
  JBOOL iRet = FALSE;

  iRet = PacketCmdGatewayMBleConnectCheck(pDevice);
  return iRet;
}


JBOOL GatewayMBleOpen(JDevice *pDevice, JDWORD dwDeviceEcgDid)
{
	char fileName[256];
  char folderPath[256];
	char strCmd[256];
	JINT idx;
  JINT i = 0;
	JINT iRet = 0;
  JINT iFind = FALSE;

  DeviceEcgType *pItem = NULL;

  sprintf((char*)folderPath, "%s%07d", GlobalVar.strCmdRoot, pDevice->dwDID);
  

  for(i = 0 ; i < pDevice->ecgObjectList.iEcgObjectCount; i = i + 1)
  {
    pItem = &pDevice->ecgObjectList.ecgObjectArr[i];

    if(pItem->dwDid == (JDWORD)dwDeviceEcgDid)
    {
      iFind = TRUE;
      break;
    }
  }
  if(iFind == FALSE)
  {
    return FALSE;
  }

  /// check item is connected
  if(pItem->ecgInfoItem.wConnected == TRUE)
  {
    return FALSE;
  }

  /// send command
  iRet = PacketCmdGatewayMBleOpen(pDevice, &pItem->bAddr[0]);
  if(iRet == TRUE)
  {    
    /// get the did seting strSNN
    UtilMemcpy((JBYTE *)&pDevice->strDeviceSSN[0], (JBYTE *)&pItem->strSSN[0], SSN_SIZE);
    UtilMemcpy((JBYTE *)&pDevice->bBleOpenAddr[0], (JBYTE *)&pItem->bAddr[0],  BLE_MAC_ADDR_SIZE);
    pDevice->dwBleOpenDid = pItem->dwDid; 
  }
  
  return iRet;
}

JBOOL GatewayMBleOpen(JDevice * pDevice)
{
	char fileName[256];
  char folderPath[256];
	char strCmd[256];
	JINT idx;
  JINT i = 0;
	JINT iRet = 0;
  JINT iFind = FALSE;

  DeviceEcgType *pItem = NULL;

  sprintf((char*)folderPath, "%s%07d", GlobalVar.strCmdRoot, pDevice->dwDID);
  /// get the address
  GatewayMBleOpenDidAddrGet(pDevice, &idx);
  /// get the item
  for(i = 0 ; i < pDevice->ecgObjectList.iEcgObjectCount; i = i + 1)
  {
    pItem = &pDevice->ecgObjectList.ecgObjectArr[i];

    if(pItem->dwDid == (JDWORD)idx)
    {
      iFind = TRUE;
      break;
    }
  }
  if(iFind == FALSE)
  {
    return FALSE;
  }

  /// check item is connected
  if(pItem->ecgInfoItem.wConnected == TRUE)
  {
    return FALSE;
  }

  /// send command
  iRet = PacketCmdGatewayMBleOpen(pDevice, &pItem->bAddr[0]);
  if(iRet == TRUE)
  {    
    /// get the did seting strSNN
    UtilMemcpy((JBYTE *)&pDevice->strDeviceSSN[0], (JBYTE *)&pItem->strSSN[0], SSN_SIZE);
    UtilMemcpy((JBYTE *)&pDevice->bBleOpenAddr[0], (JBYTE *)&pItem->bAddr[0],  BLE_MAC_ADDR_SIZE);
    pDevice->dwBleOpenDid = pItem->dwDid;
    
    DBG_PRINTF(pDevice->dwDID,"\t\tBle Connect SSN:%s\n",pDevice->strDeviceSSN); 
  }
  
  return iRet;
}

JBOOL GatewayMBleClose(JDevice *pDevice)
{
  JBOOL bRet = FALSE;

  bRet = PacketCmdGatewayMBleClose(pDevice);
  
  /// clear data
  pDevice->dwBleOpenDid = 0;
  UtilMemset((JBYTE *)&pDevice->strDeviceSSN[0], 0x00, SSN_SIZE);
  UtilMemset((JBYTE *)&pDevice->bBleOpenAddr[0], 0x00, BLE_MAC_ADDR_SIZE);

  if(bRet == TRUE)
  {

    DBG_PRINTF(pDevice->dwDID, "\t\t [BLE] closed OK\r\n");
  }
  else
  {
    DBG_PRINTF(pDevice->dwDID, "\t\t [BLE] closed failed\r\n");
  }

  return bRet;
}

void GatewayMBleOpenDidAddrGet(JDevice *pDevice, JINT * pDid)
{
  char line[1024];
  char *pStr = NULL;  
  JINT iRet = 0;
  JINT i = 0;
  JINT idx;
  JINT iBleMacAddr[6];
  char  fileName[256];
  JUINT dwDID = pDevice->dwDID;
  FILE *fp  = NULL;

  sprintf((char *) &fileName[0], "%s%07d/ble_open_did.csv", GlobalVar.strCmdRoot, dwDID);
  fp  = fopen(fileName, "rb");
  
  if(fp == NULL)
  {
    DBG_PRINTF(dwDID,"Open file fail %s\n",fileName);
    return;
  }

	while(1)
	{
		iRet = fscanf(fp, "%[^\n]\n", line);
		if(iRet <= 0)
		{
			break;
		}
    pStr = strstr(line, "DEVICE_BLE_DID");
    if(pStr != NULL)
    {
      pStr = strstr(line, ":");
      pStr = pStr + 1;
      sscanf(pStr, "%d\n", &idx);   
      continue;
    }
  }
  *pDid = idx;
  fclose(fp);
}

void GatewayInfoWrite(JUINT dwDID, char *fileName, JGatewayInfo *pGatewayInfo, JINT iGatewayDeviceMode)
{
  char msg[1024];
  JINT iRet = 0;
  JINT i = 0;
  
  FILE *fp  = fopen(fileName, "w+");
  if(fp == NULL)
  {
    DBG_PRINTF(dwDID,"Create file fail %s\n",fileName);
    return;
  }
  /// HEder
  sprintf(msg, "%s\n", "ITEM,VALUE");
  fprintf(fp,"%s", msg);

  /// #0
  sprintf(msg, "YEAR,%d\n", pGatewayInfo->iYear);
  fprintf(fp,"%s", msg);
  /// #1
  sprintf(msg, "MONTH,%d\n", pGatewayInfo->iMonth);
  fprintf(fp,"%s", msg);
  /// #2
  sprintf(msg, "DAY,%d\n", pGatewayInfo->iDay);
  fprintf(fp,"%s", msg);
  /// #3
  sprintf(msg, "HOUR,%d\n", pGatewayInfo->iHour);
  fprintf(fp,"%s", msg);
  /// #4
  sprintf(msg, "MIN,%d\n", pGatewayInfo->iMin);
  fprintf(fp,"%s", msg);
  /// #5
  sprintf(msg, "SEC,%d\n", pGatewayInfo->iSec);
  fprintf(fp,"%s", msg);   
  /// #6
  sprintf(msg, "GATEWAY_DID,%d\n", pGatewayInfo->iGatewayDid);
  fprintf(fp,"%s", msg);   
  /// #7
  sprintf(msg, "GATEWAY_TEMP,%0.3f\n", pGatewayInfo->fGatewayTemp);
  fprintf(fp,"%s", msg);  
  /// #8
  sprintf(msg, "GATEWAY_VOLT,%0.3f\n", pGatewayInfo->fGatewayVolt);
  fprintf(fp,"%s", msg);
  /// #9
  sprintf(msg, "GATEWAY_BAT_LEVEL,%d\n", pGatewayInfo->iGatewayBattLevel);
  fprintf(fp,"%s", msg);
  /// #10
  sprintf(msg, "GATEWAY_UTC,%d\n", pGatewayInfo->dwGatewayUtc);
  fprintf(fp,"%s", msg);  
  /// #11
  sprintf(msg, "GATEWAY_ONLINE_TIME,%d\n", pGatewayInfo->dwGatewayOnlineTime);
  fprintf(fp,"%s", msg);  
  /// #12
  sprintf(msg, "GATEWAY_BTN_EVENT_00,%d\n", pGatewayInfo->iBtnEvent00);
  fprintf(fp,"%s", msg);  
  /// #13
  sprintf(msg, "GATEWAY_BTN_EVENT_01,%d\n", pGatewayInfo->iBtnEvent01);
  fprintf(fp,"%s", msg);  
  /// #14
  sprintf(msg, "GATEWAY_LONG_PRESS,%d\n", pGatewayInfo->iLongBtnEvent);
  fprintf(fp,"%s", msg);  
  
  /// #15
  sprintf(msg, "GATEWAY_ONLINE,%d\n", pGatewayInfo->iGatewayOnline);
  fprintf(fp,"%s", msg); 

  sprintf(msg, "GATEWAY_STATUS,0x%08x\n", pGatewayInfo->dwGatewayStatus);
  fprintf(fp,"%s", msg); 
  
  sprintf(msg, "GATEWAY_ALARM,0x%08x\n", 		pGatewayInfo->dwGatewayAlarm);
  fprintf(fp,"%s", msg); 
  
  sprintf(msg, "GATEWAY_ECG_DEV_CNT,%d\n", 	pGatewayInfo->iEcgItemCount);
  fprintf(fp,"%s", msg); 
  
  /// #  
  sprintf(msg, "GATEWAY_TEMP_DEV_CNT,%d\n", pGatewayInfo->iTempItemCount);
  fprintf(fp,"%s", msg); 

  /// #
  if(pGatewayInfo->dwGatewayStatus & GW_SYS_STS_NOTICE)
  {
    sprintf(msg, "GATEWAY_STS_NOTICE,1\n");
    fprintf(fp,"%s", msg); 
  }
  else
  {
    sprintf(msg, "GATEWAY_STS_NOTICE,0\n");
    fprintf(fp,"%s", msg); 
  }
  
  ///#
  if(pGatewayInfo->dwGatewayStatus & GW_SYS_STS_ALWAYS_ON)
  {
    sprintf(msg, "GATEWAY_STS_ALWAYS_ON,1\n");
    fprintf(fp,"%s", msg); 
  }
  else
  {
    sprintf(msg, "GATEWAY_STS_ALWAYS_ON,0\n");
    fprintf(fp,"%s", msg); 
  }
  
  ///#
  if(pGatewayInfo->dwGatewayStatus & GW_SYS_STS_MAYDAY)
  {
    sprintf(msg, "GATEWAY_STS_MAYDAY,1\n");
    fprintf(fp,"%s", msg); 
  }
  else
  {
    sprintf(msg, "GATEWAY_STS_MAYDAY,0\n");
    fprintf(fp,"%s", msg); 
  }
  
  /// #
  if(pGatewayInfo->dwGatewayStatus & GW_SYS_STS_ON_DUTY)
  {
    sprintf(msg, "GATEWAY_STS_ON_DUTY,1\n");
    fprintf(fp,"%s", msg); 
  }
  else
  {
    sprintf(msg, "GATEWAY_STS_ON_DUTY,0\n");
    fprintf(fp,"%s", msg); 
  }
  
  /// #
  if(pGatewayInfo->dwGatewayStatus & GW_SYS_STS_CALL_HELP)
  {
    sprintf(msg, "GATEWAY_STS_CALL_HELP,1\n");
    fprintf(fp,"%s", msg); 
  }
  else
  {
    sprintf(msg, "GATEWAY_STS_CALL_HELP,0\n");
    fprintf(fp,"%s", msg); 
  }

  sprintf(msg, "GATEWAY_NAME,%s\n", &pGatewayInfo->strGatewayName[0]);
  fprintf(fp,"%s", msg); 

  
  sprintf(msg, "GATEWAY_DEV_MODE,%d\n", iGatewayDeviceMode);
  fprintf(fp,"%s", msg); 

  sprintf(msg, "GATEWAY_VERSION,%d\n", pGatewayInfo->iVersion);
  fprintf(fp,"%s", msg); 
  
  fclose(fp);
}
