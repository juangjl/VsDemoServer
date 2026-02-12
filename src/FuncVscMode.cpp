/**
 * @file FuncVscMode.cpp
 * @brief  Control Function for VSC Mode
 * @version $Revision$
 * @author JLJuang <jl_juang@vsgntek.com>
 * @note Copyright (c) 2024, VitalSigns Technology Co., Ltd, all rights reserved.
 * @note
*/
#include "Global.h"

void FuncVscModeInit(JDevice * pDevice)
{	
	pDevice->iVscModeQueueHead = 0;
	pDevice->iVscModeQueueTail = 0; 
  pDevice->iVscModeDataCount = 0;

  pDevice->iVscModeTotalSec  = 0;
  pDevice->iVscModeDataCountLeft = 0;
  pDevice->iVscModeIdx       = VSC_MODE_IDX_INIT;
	pDevice->iVscModeIdxLast   = 0;
 
  pDevice->tVscModeStartTime = 0;
	
  VscModeInit(pDevice->strDataRoot);

	pDevice->iVscAtrCnt	 	 	   = 0;	
	pDevice->iVscAtrCntPre	 	 = 0;
  
	JAtrDataClean(&pDevice->vscAtrNow);	
	JAtrDataClean(&pDevice->vscAtrPre);
	
	pDevice->iVscModeItemReadCount  = 0;
	pDevice->dwVscFileModeUtcGet		= 0;	
}

JBOOL FuncVscModeDayFolderCreate(JDevice *pDevice, time_t nowTime, char * strFolder)
{
  JTM   jtmNow;
  char  strFolderName[256];
	char strUserToken[256];
	MeasItemType * pMeasItem = pDevice->pMeasItemActive;

	strcpy(strUserToken, pMeasItem->strMeasToken);

  JTMGmtTimeGet(&jtmNow, nowTime);

  if(strlen(strUserToken) <= 0)
  {
    DBG_PRINTF(pDevice->dwDID, "\t [VSC][FOLDER][INFO][CREATE] User Token not set \r\n");
  }  

  sprintf(pDevice->strInfoDataRoot, "%sMEAS/%s/INFO/%04d/%02d/%02d",
          (char *)&pGlobalVar->strDataRoot[0],          
          (char *)&strUserToken[0],
          jtmNow.iYear, jtmNow.iMonth, jtmNow.iDay);
  
  if(UtilFolderExisted(pDevice->strInfoDataRoot) == TRUE)
  {
    sprintf(strFolder, "%s", pDevice->strInfoDataRoot);
    return FALSE;
  }

	///----------------------------------------------------------------///
  /// Set strDataRootEx: MEAS/USER/INFO/YYYY/MM/DD
  ///----------------------------------------------------------------///  
	/// Create MEAS
  sprintf(strFolderName, "%s/MEAS",pGlobalVar->strDataRoot);
  UtilFolderCreate(strFolderName);

	/// User folder
  sprintf(strFolderName, "%s/%s",     
					(char *)strFolderName,
          (char *)&strUserToken[0]);
  UtilFolderCreate(strFolderName);

	/// User folder
  sprintf(strFolderName, "%s/INFO",     
					(char *)strFolderName);
  UtilFolderCreate(strFolderName);

	/// Create date folder: YYYY
  sprintf(strFolderName, "%s/%04d",
					(char *)strFolderName,
          jtmNow.iYear);
  UtilFolderCreate(strFolderName);

	/// Create date folder: MM	
	sprintf(strFolderName, "%s/%02d",
					(char *)strFolderName,
          jtmNow.iMonth);
  UtilFolderCreate(strFolderName);

	/// Create date folder: DD
	sprintf(strFolderName, "%s/%02d",
					(char *)strFolderName,
          jtmNow.iDay);
  UtilFolderCreate(strFolderName);

  sprintf(strFolder, "%s", strFolderName);

  DBG_PRINTF(pDevice->dwDID, "\t [TASK][VSC][FOLDER][INFO][DAY] create path = '%s'\r\n",strFolder);
  
  return TRUE;
}

JBOOL FuncVscModeFolderCreate(JDevice * pDevice, time_t nowTime, time_t startTime, char * strFolder)
{
	JINT iDid = pDevice->dwDID;
  JTM   jtm;
  JTM   jtmNow;
  char  strFolderName[256];
	char  strUserToken[256];
	MeasItemType *pMeasItem = pDevice->pMeasItemActive;

  JTMGmtTimeGet(&jtm, startTime);
  JTMGmtTimeGet(&jtmNow, nowTime);

	sprintf(strUserToken, "%s", pMeasItem->strMeasToken);

	if(strlen(strUserToken) <= 0)
	{
		DBG_PRINTF(iDid, "\t [VSC][FOLDER][CREATE][!!!!!!] Meas Token is empty, use default '%s'\r\n", "UNKNOWN");
		sprintf(strUserToken, "UNKNOWN");
	}

  ///----------------------------------------------------------------///
  /// Set strDataRootEx: MEAS/USER/DATA/YYYY/MM/DD/HH/YYYYMMDD_HHMMSS
  ///----------------------------------------------------------------///
  sprintf(pDevice->strDataRootEx, "%sMEAS/%s/DATA/%04d/%02d/%02d/%02d/%04d%02d%02d_%02d%02d%02d",
          (char *)&pGlobalVar->strDataRoot[0],          
          (char *)&strUserToken[0],          
					jtm.iYear, jtm.iMonth, jtm.iDay, jtm.iHour,
          jtm.iYear, jtm.iMonth, jtm.iDay, 
					jtm.iHour, jtm.iMin, jtm.iSec);	

  /// Create MEAS
  sprintf(strFolderName, "%sMEAS", pGlobalVar->strDataRoot);
  UtilFolderCreate(strFolderName);

  sprintf(strFolderName, "%sMEAS/%s",
          (char *)&pGlobalVar->strDataRoot[0],          
          (char *)&strUserToken[0]);
  UtilFolderCreate(strFolderName);

  /// Create DATA folder
  sprintf(strFolderName, "%s/DATA", strFolderName);
  UtilFolderCreate(strFolderName);
	
  /// Create YYYY
  sprintf(strFolderName, "%s/%04d", strFolderName, jtm.iYear);
  UtilFolderCreate(strFolderName);

	/// Create MM
  sprintf(strFolderName, "%s/%02d", strFolderName, jtm.iMonth);
  UtilFolderCreate(strFolderName);

	/// Create DD
  sprintf(strFolderName, "%s/%02d", strFolderName, jtm.iDay);
  UtilFolderCreate(strFolderName);

	/// Create Hour
  sprintf(strFolderName, "%s/%02d", strFolderName, jtm.iHour);
  UtilFolderCreate(strFolderName);

	/// Create date folder: YYYYMMDD_HHMMSS
  sprintf(strFolderName, "%s/%04d%02d%02d_%02d%02d%02d",
          strFolderName, 
          jtm.iYear, jtm.iMonth, jtm.iDay, 
					jtm.iHour, jtm.iMin, jtm.iSec);
  UtilFolderCreate(strFolderName);

  /// Create date folder: YYYY-MM-DD
  sprintf(strFolderName, "%s/%04d%02d%02d",
          pDevice->strDataRootEx,
          jtmNow.iYear, jtmNow.iMonth, jtmNow.iDay);
  UtilFolderCreate(strFolderName);

  /// Create hour folder: YYYY-MM-DD/HH
  sprintf(strFolderName, "%s/%04d%02d%02d/%02d",
          pDevice->strDataRootEx,
          jtmNow.iYear, jtmNow.iMonth, jtmNow.iDay, jtmNow.iHour);
  UtilFolderCreate(strFolderName);

  sprintf(strFolder, "%s", strFolderName);
    
  return TRUE;
}

void FuncVscModeAtrFileSave(JDevice *pDevice, char *strFolderName )
{ 
	FILE *fp  = NULL;
	char msg[256];
	JINT idx = pDevice->iVscAtrCnt;
	char strFileName[256];
	char strBinFileName[256];
	JINT A = 0;
	JFLOAT fTimeSec  = 0;

	JINT APre = 0;
	JFLOAT fTimeSecPre  = 0;

	JFLOAT fDeltaSec = 0;
  JFLOAT fTime = 0;
	JFLOAT fmSec = 0;
	time_t tPreTime = 0;
	time_t tTime = 0;
	time_t tStartDeltaTime = 0;
	
	JAtrType *pAtrNow = &pDevice->vscAtrNow;
	JAtrType *pAtrPre = &pDevice->vscAtrPre;

	JAtrDataGet(pAtrNow, &A,    &fTimeSec);
	JAtrDataGet(pAtrPre, &APre, &fTimeSecPre);

	if(pAtrPre->bAtr == ATR_NONE)
	{
		fDeltaSec = 0;
	}
	else
	{
		fDeltaSec = fTimeSec - fTimeSecPre;
		if(fDeltaSec > 1.5)
		{
			fDeltaSec = 0;
		}
		if(fDeltaSec <= 0 )
		{
			fDeltaSec = 0;
		}
	}
	
	sprintf(strFileName, "%s/atr.csv", strFolderName);
  sprintf(strBinFileName, "%s/atr.bin", strFolderName);

	tPreTime = pAtrNow->dwMS / 1000;
	fmSec = pAtrNow->dwMS % 1000;

	tTime = tPreTime + pDevice->pMeasItemActive->dwStartTime;

	tStartDeltaTime = pDevice->tVscModeStartTime - pDevice->pMeasItemActive->dwStartTime;
	
	if((pDevice->tVscModeStartTime / 3600) == (tTime / 3600))
	{
		tPreTime = (tTime - pDevice->tVscModeStartTime);
		fTime = (tPreTime % 3600);
	}
	else
	{
		fTime = (tTime % 3600); ///< get the second in the hour
	}

	fmSec    = pAtrNow->dwMS % 1000;
	fTime    = fTime + (fmSec / 1000);
		
	JAtrFileCsvSave(strFileName,    idx, A, fTime, fDeltaSec);
	JAtrFileBinSave(strBinFileName, idx, A, fTime);
}

void FuncVscModeAtrSave(JDevice *pDevice, JAtrType *pAtrRead, char *strFolderName )
{	
	JINT A = 0;
	JFLOAT fTimeSec = 0;
	JAtrType *pAtr = NULL;
    
  UtilMemcpy((JBYTE *)&pDevice->vscAtrPre, (JBYTE *)&pDevice->vscAtrNow, sizeof(JAtrType));	
  UtilMemcpy((JBYTE *)&pDevice->vscAtrNow, (JBYTE *)pAtrRead, sizeof(JAtrType));		
  
  pDevice->iVscAtrCntPre = pDevice->iVscAtrCnt;
  pDevice->iVscAtrCnt    = pDevice->iVscAtrCnt + 1;
  
  FuncVscModeAtrFileSave(pDevice, strFolderName);  
}

void FuncVscModeGSensorDataSave(char *strFileName, VscModeControlType *pVscMode)
{
	char msg[256];
	JINT i = 0;
		
	FILE *fp = NULL;	
	JBOOL bCreate = FALSE;

	if(UtilFileExisted(strFileName) == FALSE)
	{
		bCreate = TRUE;
	}
	if(bCreate == TRUE)
	{
		fp = fopen(strFileName, "w+");
	}
	else
	{
		fp = fopen(strFileName, "a+");
	}
	if(fp == NULL)
	{
		return;
	}
	for(i = 0; i < 5; i = i + 1)
	{
		fprintf(fp, "%d,%d,%d\n", pVscMode->sGSenX[i], pVscMode->sGSenY[i], pVscMode->sGSenZ[i]);
	}
	fclose(fp);
}

void FuncVscModeEcgDataCsvSave(char *strFileName, VscModeControlType *pVscMode, JINT iCH)
{
	FILE *fp  = NULL;
	JINT 	i   = 0;		
	JINT iCnt = 0;
	char 	msg[256];
  JBOOL bCreate = FALSE;

	iCnt = (VSC_MODE_ITEM_DATA_SIZE / sizeof(JFLOAT)) / VscModeCtl.iChannelCount;

	if(UtilFileExisted(strFileName) == FALSE)
	{
		bCreate = TRUE;
	}

	if(bCreate == TRUE)
	{
		fp = fopen(strFileName, "w+");
	}
	else
	{
		fp = fopen(strFileName, "a+");
	}
	if(fp == NULL)
	{
		return;
	}

  for(i = 0 ; i < iCnt; i = i + 1)
  {		
    fprintf(fp, "%0.3f\n", pVscMode->fValueCH[iCH][i]);
  }
  fclose(fp);		
}

void FuncVscModeEcgDataBinSave(char *strFileName, VscModeControlType *pVscMode, JINT iCH)
{
	FILE *fp  = NULL;
	JINT 	i   = 0;		
	JINT iCnt = 0;
	char 	msg[256];
  JBOOL bCreate = FALSE;

	iCnt = (VSC_MODE_ITEM_DATA_SIZE / sizeof(JFLOAT)) / VscModeCtl.iChannelCount;

	if(UtilFileExisted(strFileName) == FALSE)
	{
		bCreate = TRUE;
	}

	if(bCreate == TRUE)
	{
		fp = fopen(strFileName, "wb+");
	}
	else
	{
		fp = fopen(strFileName, "ab+");
	}
	if(fp == NULL)
	{
		return;
	}

  for(i = 0 ; i < iCnt; i = i + 1)
  {		    
		fwrite(&pVscMode->fValueCH[iCH][i], 4, 1, fp);
  }
  fclose(fp);		
}

void FuncVscModeExecute(JDevice * pDevice)
{
  JINT i = 0;
	JINT j = 0;	
	JINT iSecCount = 0;
	JAtrType 	*pAtrRead = NULL;
	VscModeControlType *pVscMode = NULL;
	static JINT iPreSec = 0;
	char strDataFolder[512];
	char strDataFolderAtr[512];
	char strDataDayFolder[512];
	char strDataDayFolderAtr[512];
	char strFileName[256];
	time_t tStartTime = 0;
	time_t tLastUpdateTime = 0;
	time_t tAtrTime = 0;
  
	static JINT iVscCnt = 0;
	JAtrInfoType 	atrInfo;
	JINT iSec = 0;
	JINT iMS = 0;
  JINT atrTime;
	JDWORD dwTime   = 0;

	JINT iIDLast = 0;
	
	JINT iReportRecordCount		   = 0;
	time_t tReportStartTime 		 = 0;
	time_t tReportLastUpdateTime = 0;
	time_t tReportEndTime 	 = 0;
	time_t tTime = 0;

	MeasItemType * pMeasItem = pDevice->pMeasItemActive;
	
	iSecCount =	pDevice->iVscModeDataCount / VSC_MODE_SEC_ITEM_CNT;
  if(iSecCount == 0)
	{
		return;
	}
	
	DBG_PRINTF(pDevice->dwDID, "\t\t [VSC] Data Sec Cnt : %d, Total Count : %d, Eclapsed : %d \r\n",
					 									 iSecCount,
														 pDevice->iVscModeDataCount,
														 pDevice->iVscModeTotalSec);
  
	for(i = 0 ; i < iSecCount; i = i + 1)
	{						
		for(j = 0 ; j < VSC_MODE_SEC_ITEM_CNT; j = j + 1)
		{			
			pVscMode = &pDevice->vscModeQueue[pDevice->iVscModeQueueHead];			
					
			if(pDevice->iVscModeIdx == VSC_MODE_IDX_INIT)
			{				
				tStartTime = pVscMode->jtm.t;
			
				pDevice->tVscModeStartTime = pVscMode->jtm.t;				
        pDevice->iVscModeTotalSec = 0;
			
				pDevice->iVscModeIdx = pVscMode->item.wId;
			
				tReportStartTime 		  = tStartTime;
				tReportLastUpdateTime = tStartTime;
				tReportEndTime 	  		= tStartTime + (JINT)(pDevice->pMeasItemActive->fMeasType);

				pDevice->pMeasItemActive->dwStartTime = tReportStartTime;
				pDevice->pMeasItemActive->dwEndTime   = tReportEndTime;			
			}
      else if(pDevice->iVscModeIdx != pVscMode->item.wId)
			{
				tStartTime = pVscMode->jtm.t;
				
				pDevice->tVscModeStartTime = pVscMode->jtm.t;				
        pDevice->iVscModeTotalSec = 0;

				DBG_PRINTF(pDevice->dwDID, "\t\t [VSC][!!!!!][DIFF]VSD-MODE-ID-EXPECTED = %d, VSD-MODE-ID-READ = %d\r\n",
										pDevice->iVscModeIdx,
										pVscMode->item.wId
									);
				
				pDevice->iVscModeIdx = pVscMode->item.wId;
				
				DBG_PRINTF(pDevice->dwDID, "\t\t [VSC][!!!!!][DIFF]MEAS ID = %d,  CREATE-TIME: %d, SSN = '%s' \r\n",										
										pDevice->pMeasItemActive->iMeasId, 
										tStartTime, 
										(char *)&pDevice->strDeviceSSN[0]);
			}
			
			iVscCnt = iVscCnt + 1;		

			FuncVscModeFolderCreate(pDevice, pVscMode->jtm.t, pDevice->tVscModeStartTime, &strDataFolder[0]);						
			FuncVscModeDayFolderCreate(pDevice, pVscMode->jtm.t, &strDataDayFolder[0]);

			pDevice->iVscModeIdx = (pDevice->iVscModeIdx + 1) % VSC_MODE_IDX_MAX;								
			
			sprintf(strFileName, "%s/ch0.csv", strDataFolder);
			FuncVscModeEcgDataCsvSave(strFileName, pVscMode, VSC_MODE_CH0);
		
			sprintf(strFileName, "%s/gsen.csv", strDataFolder);
			FuncVscModeGSensorDataSave(strFileName, pVscMode);

			sprintf(strFileName, "%s/ch0.bin", strDataFolder);
			FuncVscModeEcgDataBinSave(strFileName, pVscMode, VSC_MODE_CH0);
			
			pAtrRead = &pVscMode->atrNow;	
			if((pAtrRead->bAtr != ATR_NONE) && (pAtrRead->dwMS > 0))
			{					
				tAtrTime = pAtrRead->dwMS/1000;
      	
        tAtrTime = tAtrTime + pDevice->pMeasItemActive->dwStartTime;

 			  FuncVscModeFolderCreate(pDevice, tAtrTime, pDevice->tVscModeStartTime, &strDataFolderAtr[0]);
  			FuncVscModeAtrSave(pDevice, pAtrRead, strDataFolderAtr);
			}
			
			if(pVscMode->jtm.iMS == 0)
			{
        FuncGSenAccTilt(pVscMode);  				      

				pVscMode->fPRTime  = pDevice->deviceEcgInfo.fPRTime;
				pVscMode->fQRSTime = pDevice->deviceEcgInfo.fQRSTime;
				pVscMode->fQTTime  = pDevice->deviceEcgInfo.fQTTime;				
				       
			  sprintf(strFileName, "%s/info.csv", strDataFolder);
			  FuncVscModeInfoSave(strFileName, pVscMode);

			  sprintf(strFileName, "%s/info.csv", strDataDayFolder);
			  FuncVscModeInfoSave(strFileName, pVscMode);
				
				if(pVscMode->jtm.iSec == 0)
				{
					sprintf(strFileName, "%s/loc.csv", strDataFolder);
					LocationInfoFileWrite(pDevice->dwDID, strFileName, TRUE);
				}
			}
			
			pDevice->iVscModeQueueHead = (pDevice->iVscModeQueueHead + 1) % VSC_MODE_QUEUE_SIZE;	
			pDevice->iVscModeDataCount = pDevice->iVscModeDataCount - 1;
		}
		
		pDevice->iVscModeTotalSec =  (pVscMode->jtm.t - pDevice->pMeasItemActive->dwStartTime) + 1;
	}
		
	if(iPreSec != pDevice->iVscModeTotalSec)
	{		
		tLastUpdateTime = pDevice->pMeasItemActive->dwStartTime + (pDevice->iVscModeIdxLast + 1) / 5;
		pDevice->pMeasItemActive->dwLastUpdateTime = tLastUpdateTime;
		iIDLast = ((tLastUpdateTime  - pDevice->pMeasItemActive->dwStartTime) * 5) % 1000;		
		iPreSec = pDevice->iVscModeTotalSec;
			
		FuncDeviceEcgDataSetEx(pDevice, &pDevice->deviceEcgRunning, pVscMode);		
	}	
}

void FuncVscModeInfoSave(char *strFileName, VscModeControlType *pVscMode)
{
	char msg[256];
		
	FILE *fp = NULL;	
	JBOOL bHeaderGen = FALSE;
  
	if(UtilFileExisted(strFileName) == FALSE)
	{
		bHeaderGen = TRUE;
	}
	
	fp  = fopen(strFileName, "a+");
	if(fp == NULL)
	{
		return;
	}
	if(bHeaderGen == TRUE)
	{
		VscModeInfoHeaderSave(fp);
	}

	VscModeInfoDataSave(pVscMode, fp);
		
	fclose(fp);
}

JINT FuncVscModeQueueRead(JDevice *pDevice, JINT iReadCount)
{
	JBOOL bRet 		= FALSE;
	JINT i = 0;
	JDWORD dwReportStartUtc = 0;
	JDWORD dwReportLastUtc  = 0;
	JINT iDid = pDevice->dwDID;	

	if(pDevice->pMeasItemActive->iMeasId == 0)
	{		
		return FALSE;
	}

	dwReportStartUtc = pDevice->pMeasItemActive->dwStartTime;
	dwReportLastUtc  = pDevice->pMeasItemActive->dwLastUpdateTime;	
	
	if(pDevice->iVscModeItemReadCount == 0)
	{
		if(dwReportLastUtc == 0)
		{
			pDevice->dwVscFileModeUtcStart = dwReportStartUtc;
			pDevice->dwVscFileModeUtcGet   = 0;
			pDevice->iVscFileModeMSGet     = 0;
		}
		else
		{
			pDevice->dwVscFileModeUtcStart = dwReportStartUtc;
			pDevice->dwVscFileModeUtcGet   = dwReportLastUtc;
			pDevice->iVscFileModeMSGet  	 = 0;
		}
	}		
	
  bRet = PacketCmdGatewaySBleVscFileModeQueueRead(pDevice, 	pDevice->dwVscFileModeUtcGet, pDevice->iVscFileModeMSGet, iReadCount);
  if(bRet == FALSE)
	{
		DBG_PRINTF(pDevice->dwDID, "\t [TASK][VSC][ERROR] -->  VSC-FILE-MODE-QUEUE-READ FAILED\r\n");		
		return FALSE;
	}		
	return bRet;
}