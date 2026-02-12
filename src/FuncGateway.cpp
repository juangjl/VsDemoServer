/**
 * @file FuncGateway.cpp
 * @brief  Control Function for Gateway
 * @version $Revision$
 * @author JLJuang <jl_juang@vsgntek.com>
 * @note Copyright (c) 2024, VitalSigns Technology Co., Ltd, all rights reserved.
 * @note
*/
#include "Global.h"

JINT FuncGatewayFolderCreate(JDevice * pDevice)
{
	char strFileName[256];

	/// create device folder
	GatewayDataFolderSet(pDevice);
  	
  sprintf(strFileName, "%s/GW_INFO", pDevice->strGatewayDataRoot);
  UtilFolderCreate((char *) strFileName);

	sprintf(strFileName, "%s/LOC_INFO", pDevice->strGatewayDataRoot);
  UtilFolderCreate((char *) strFileName);

	return TRUE;
}

JINT FuncGatewayOnlineSet(JDevice * pDevice)
{
	FILE *fp = NULL;
	char  strFilePath[256];
	time_t tNow = time(NULL);
	sprintf(strFilePath, "%s/GW_INFO/GW_ONLINE.txt", pDevice->strGatewayDataRoot);
	fp = fopen(strFilePath, "w+");
	if(fp == NULL)
	{
		DBG_PRINTF(pDevice->dwDID, "\t [ERROR] Gateway Online File Create Failed\r\n");
		return FALSE;
	}
	fprintf(fp, "%ld", (long)tNow);
	fclose(fp);

	return TRUE;
}

JINT FuncGatewayOfflineSet(JDevice * pDevice)
{
	char  strFilePath[256];	
	sprintf(strFilePath, "%s/GW_INFO/GW_ONLINE.txt", pDevice->strGatewayDataRoot);	
	UtilFileDelete(strFilePath);
	return TRUE;
}

JINT FuncGatewayDeviceDidGet(JDevice * pDevice)
{
  FILE *fp = NULL;
  char strLine[256];
	JINT iRet = 0;
	char * pStr = 0;
	char msg[256];

	JINT iGatewayDidRead = 0;
	JINT iDeviceDidRead = 0;
	JINT iPeriodRead = 0;
	JINT iTimeWaitRead = 0;
	char strMeasTokenRead[256];

	JINT iCount = 0;	

  fp = fopen("./cfg/map.txt", "r");
  if(fp == NULL)
  {
    return -1;
  }

	while(1)
	{
		iRet = fscanf(fp, "%[^\n]\n", strLine);
		if(iRet <= 0)
		{
			break;
		}
		
		///----------------------------------------------------------------///
		/// Skip '#' line
		///----------------------------------------------------------------///
		pStr = strstr(strLine, "#");
		if(pStr != NULL)
		{
			*pStr = 0;
		}

		///----------------------------------------------------------------///
		/// Trim line
		///----------------------------------------------------------------///
		strcpy(msg, strLine);
		UtilSpaceTrim(strLine);
		if(strlen(strLine) <= 0)
		{
			continue;
		}			
		iRet = sscanf(strLine, "%d,%d,%d,%d,%s", &iGatewayDidRead, &iDeviceDidRead, &iPeriodRead, &iTimeWaitRead, &strMeasTokenRead[0]);
		if(iRet != 5)
		{
			DBG_PRINTF(pDevice->dwDID, "\t [ERROR] Gateway Device Map File Line Parse Failed before: '%s'\r\n", msg);
			DBG_PRINTF(pDevice->dwDID, "\t [ERROR] Gateway Device Map File Line Parse Failed after : '%s'\r\n", strLine);
			continue;
		}
		strMeasTokenRead[127] = 0;

		if(iGatewayDidRead == pDevice->dwDID)
		{			
			if(iCount < 4)
			{
				pDevice->pMeasItemActive->iDeviceEcgDid[iCount]  = iDeviceDidRead;
				pDevice->pMeasItemActive->iMeasPeriodSec[iCount] = iPeriodRead;				
				pDevice->pMeasItemActive->iMeasWaitSec[iCount]   = iTimeWaitRead;
				strcpy(&pDevice->pMeasItemActive->strTokens[iCount][0], &strMeasTokenRead[0]);

				iCount = iCount + 1;
				pDevice->pMeasItemActive->iDeviceEcgCount = iCount;

				sprintf(msg, "[INFO] Gateway DID = %d, Device DID = %d, Period = %d sec, Token = %s\r\n", iGatewayDidRead, iDeviceDidRead, iPeriodRead, strMeasTokenRead);
				DBG_PRINTF(pDevice->dwDID, "%s", msg);			
			}			
		}	
	}
  fclose(fp);
  return -1;
}

void FuncGatewayScanDeviceFileWrite(JDevice * pDevice)
{
	JINT i = 0;
	char msg[256];
	char strLine[1024];
	DeviceEcgType * pEcgObject = NULL;
	FILE *fp = NULL;
	char strFilePath[256];
	JINT iEcgTotalCount = pDevice->ecgObjectList.iEcgObjectCount;

	sprintf(strFilePath, "%s/GW_INFO/GW_SCAN_DEVICE.csv", pDevice->strGatewayDataRoot);

	///----------------------------------------------------------------///
	/// Scan device count check
	///----------------------------------------------------------------///	
	if(iEcgTotalCount<= 0)
	{
		UtilFileDelete(strFilePath);
		return;
	}

	///----------------------------------------------------------------///
	/// file open
	///----------------------------------------------------------------///
	fp = fopen(strFilePath, "w+");
	if(fp == NULL)	
	{
		DBG_PRINTF(pDevice->dwDID, "\t [ERROR] Gateway Scan Device File Create Failed\r\n");
		return;
	}
	
	///----------------------------------------------------------------///
	/// header print
	///----------------------------------------------------------------///
	fprintf(fp, "ID,DID,SSN,RSSI,CONN,HR,TEMP\r\n");

	for(i = 0; i < iEcgTotalCount; i = i + 1)
	{
		pEcgObject = &pDevice->ecgObjectList.ecgObjectArr[i];
		fprintf(fp, "%d,%d,%s,%d,%d,%d,%0.3f\r\n", i, pEcgObject->dwDid, pEcgObject->strSSN, pEcgObject->iRssi, pEcgObject->ecgInfoItem.wConnected, pEcgObject->ecgInfoItem.iHR, pEcgObject->ecgInfoItem.fTemp);
	}	

	///----------------------------------------------------------------///
	/// file close
	///----------------------------------------------------------------///
	fclose(fp);
}

JINT FuncGatewayVersionGet(JDevice * pDevice)
{
	JINT iRet = 0;
	JINT iDid = pDevice->dwDID;

	iRet = PacketCmdSystemVersionGet(pDevice);	
	if(iRet == FALSE)
	{
		DBG_PRINTF(iDid, "\t\t [ERROR][!!!] SYSTEM VERSION GET FAILED \r\n");				
	}	

	return iRet;
}

void FuncGatewayInfoFileWrite(JDevice * pDevice, time_t tNow)
{
  JTM jtm;
  JTM * pJtm = &jtm;

  char  strFileName[256];
  char  strCmd[256];  
  JUINT uTNow = 0;

  static time_t tPreTime = 0;
  if(((tNow - tPreTime) < 1) &&
      (tNow != 0))
  {
    return;
  }

  tPreTime = tNow;
  
  if(tNow != 0)
  {
    JTMGmtTimeGet(pJtm, tNow);
    /// get the system time
    uTNow = tNow ;
  }
     
	sprintf(strFileName, "%s/GW_INFO/GW_INFO.csv", pDevice->strGatewayDataRoot);  
  GatewayInfoWrite(pDevice->dwDID, strFileName, &pDevice->gatewayInfo, pDevice->GatewayConfig.iGatewayDeviceMode);
}

void FuncGatewayDeviceEcgListGet(JDevice *pDevice)
{
  JINT 	iRet 		 = 0;				
	JINT 	iDid 		 = pDevice->dwDID;	
	JBOOL bUpdate  = FALSE;
	
	JINT idx  		 = 0;
	JINT iCnt 		 = 0;
	JINT iTotalCnt = 0;

  JINT iDeltaSec = 0;

	JINT i = 0;

	DeviceEcgType * pEcgObject = NULL;	
	
	JDeviceEcg * pDeviceEcg = &pDevice->deviceEcgInfo;	

	MeasItemType * pMeasItem = pDevice->pMeasItemActive;

	JINT iEcgListCount = 0;

	time_t tNow = time(NULL);

	iRet = PacketCmdGatewayDeviceEcgListCountGet(pDevice, &iEcgListCount);
	if(iRet == FALSE)
	{
		DBG_PRINTF(iDid, "\t [PACKET] Device ECG List Count Get Failed\r\n");						
		return;
	}		
  
  if(iEcgListCount <= 0)
	{   	
		return;
	}  

	if(iEcgListCount > ECG_ITEM_LIST_TOTAL_CNT)
	{
		DBG_PRINTF(iDid, "\t [WARNING] The device ecg list size is bigger than %d \r\n", iEcgListCount);
		return;
	}
	
	///----------------------------------------------------------------///
	/// ECG Object get from gateway
	///----------------------------------------------------------------///	
	idx = 0;
	iTotalCnt = iEcgListCount;
	while(1)
	{
		if(iTotalCnt == 0)
		{
			break;
		}

		iCnt = iTotalCnt;

		if(iCnt > ECG_ITEM_LIST_FETCH_MAX_CNT)
		{
			iCnt = ECG_ITEM_LIST_FETCH_MAX_CNT;
		}		

		/// ECG list get with retry mechanism
		DBG_PRINTF(iDid, "\t\t ECG ITEM LIST : TOTAL COUNT = %d, IDX = %d, CND = %d\r\n", iEcgListCount, idx, iCnt);
		iRet = PacketCmdGatewayDeviceEcgListGet(pDevice, iCnt, idx);
		if(iRet == FALSE)
		{
			DBG_PRINTF(iDid, "\t [PACKET] Device ECG List Get Failed\r\n");						
			return;
		}		

		iTotalCnt = iTotalCnt - iCnt;		
		idx = idx + iCnt;
	}
		
	pDevice->ecgObjectList.iEcgObjectCount = iEcgListCount;
	DBG_PRINTF(iDid, "\t\t ECG LIST ITEM COUNT = %d \r\n",iEcgListCount);

	///----------------------------------------------------------------//	
	/// add the device info report list data to data base
	///----------------------------------------------------------------//	
	for(i = 0; i < pDevice->ecgObjectList.iEcgObjectCount; i = i + 1)
  {				
		/// GW get item
	  pEcgObject = &pDevice->ecgObjectList.ecgObjectArr[i];	
		
		///----------------------------------------------------------------///
		/// Skip the device with did is 0
		///----------------------------------------------------------------///
		if(pEcgObject->dwDid == 0)
		{	  	
			continue;
		}

		/// setting the utc time is gateway fetch time
    pDeviceEcg->dwUTC   = (tNow - pEcgObject->wTm);		

		///----------------------------------------------------------------///
		/// Copy the data from ECG object to DeviceEcg 
		///----------------------------------------------------------------///
		if(pEcgObject->dwDid == pMeasItem->iBleDidTarget)
		{
			FuncDeviceEcgDataSet(pDevice, pDeviceEcg, pEcgObject);										
		}		
	}	
}

JINT FuncGatewayLocationInfoGet(JDevice * pDevice)
{
	JINT iRet = 0;
	JINT iDid = pDevice->dwDID;

	iRet = PacketCmdLocLocationGet(pDevice);
	if(iRet == FALSE)
	{
		DBG_PRINTF(iDid, "\t\t [ERROR][!!!] LOCATION INFO GET FAILED \r\n");							
		return iRet;
	}

	LocationGpsDataPrint(pDevice);
	return iRet;
}

void FuncGatewayLocationFileWrite(JDevice * pDevice)
{  
	time_t tNow = time(NULL);
  char  strstrFileName[256];
  static time_t tPre = 0;
	char msg[256];
	JBOOL bAppend = FALSE;

  if((tNow - tPre) < 1)
  {
    return;
  }
	tPre = tNow;
       
	sprintf(strstrFileName, "%s/LOC_INFO/LOC_INFO.csv", pDevice->strGatewayDataRoot);
	
	LocationInfoFileWrite(pDevice->dwDID, strstrFileName, bAppend);
}

JINT FuncGatewayCmdGet(JDevice * pDevice, CmdObjType * pCmdObj)
{
	JINT iDid = pDevice->dwDID;	
	char strFilePath[256];
	FILE *fp = NULL;
	char strLine[1024];
	char strArg[4][1024];
	JINT iLineCount = 0;
	JINT iArgCount = 0;

	JINT iRet = 0;
	MeasItemType * pMeasItem = pDevice->pMeasItemActive;

	///----------------------------------------------------------------///
	/// Only when the device is connected, the command can be get from file
	///----------------------------------------------------------------///
	if(pMeasItem->iBleDidTarget  == 0)
	{		
		pCmdObj->iDeviceDid  = 0;
		return FALSE;
	}	

	///----------------------------------------------------------------///
	/// Check the command file exist or not, if not exist, return false
	///----------------------------------------------------------------///
	sprintf(strFilePath, "%s/%07d/GCMD.TXT", GlobalVar.strCmdRoot, iDid);
	if(UtilFileExisted(strFilePath) == FALSE)
	{
		return FALSE;
	}
	fp = fopen(strFilePath, "r");
	if(fp == NULL)	
	{
		DBG_PRINTF(iDid, "\t [ERROR] Gateway Command File Open Failed\r\n");
		return FALSE;
	}
	while(1)
	{
		iRet = fscanf(fp, "%[^\n]\n", strLine);
		if(iRet <= 0)
		{
			break;
		}

		if(iLineCount == 0)
		{
			strcpy(pCmdObj->strCmdName, strLine);
		}
		else
		{
			strcpy(&pCmdObj->strArg[iArgCount][0], strLine);
			iArgCount = iArgCount + 1;
		}
		iLineCount = iLineCount + 1;
	}		
	fclose(fp);

	pCmdObj->iGatewayDid = iDid;
	pCmdObj->iDeviceDid  = pMeasItem->iBleDidTarget;
	pCmdObj->dwUTC			 = time(NULL);
	pCmdObj->iTimeout		 = 5000; ///< default timeout is 5000 ms
	pCmdObj->iStatus		 = GATEWAY_CMD_NOT_EXECUTED;

	///----------------------------------------------------------------///
	/// Command file delete after read
	///----------------------------------------------------------------///
	UtilFileDelete(strFilePath);
		
	return TRUE;
}