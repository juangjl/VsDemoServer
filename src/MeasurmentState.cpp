/**
 * @file MeasurementStateControl.cpp 
 * @brief  Measurement State control
 * @version $Revision$
 * @author JL Juang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2022,  VitalSigns Technology Co., LTD., all rights reserved.
 * @note
 */
#include "Global.h"

void MeasurementStateInit(JDevice *pDevice, MeasItemType * pMeasItem)
{
  JINT iDid = pDevice->dwDID;  
  JINT iDeviceEcgDid = 0;  
  JINT i = 0;
  JINT j = 0;
  DeviceEcgType * pEcgObject = NULL;

  JINT iRssiMax = -100;
  JINT iEcgIdx  = -1;
  JINT iMeasIdx  = -1;
  time_t tNow = time(NULL);

  if(pDevice->ecgObjectList.iEcgObjectCount <= 0)
  {
    
    pMeasItem->iBleDidTarget = -1;
    for(i = 0; i < 6; i = i + 1)
    {
      pMeasItem->bBleMacTarget[i] = 0;
    }    
    pMeasItem->iMeasId  = 0;    
    FuncGatewayDeviceDidGet(pDevice);	
  }
  
  DBG_PRINTF(iDid, "\t [MEAS][STATE][INIT] ECG-OBJECT-COUNT = %d\r\n", pDevice->ecgObjectList.iEcgObjectCount);       
 
  ///----------------------------------------------------------------/// 
  /// Find the Ecg Device DID in the reported measurement item
  ///----------------------------------------------------------------///
  for(i = 0; i < pDevice->ecgObjectList.iEcgObjectCount; i = i + 1)
  {
    pEcgObject =  &pDevice->ecgObjectList.ecgObjectArr[i];

    for(j = 0; j < MEAS_DEVICE_ECG_COUNT; j = j + 1)
    {
      iDeviceEcgDid = pMeasItem->iDeviceEcgDid[j];
      if(iDeviceEcgDid == pEcgObject->dwDid)
      {
        if(pEcgObject->iRssi > iRssiMax)
        {
          iRssiMax = pEcgObject->iRssi;
          iEcgIdx  = i;
        }
      }
    }    
  }
  
  if(iEcgIdx != -1)
  {    
    /// Copy the target BLE device information to measurement item
    pEcgObject = &pDevice->ecgObjectList.ecgObjectArr[iEcgIdx];    
    pMeasItem->iBleDidTarget = pEcgObject->dwDid;
    UtilMemcpy((JBYTE *)&pMeasItem->bBleMacTarget[0], (JBYTE *)&pEcgObject->bAddr[0], 6);     			

    /// measurement type is the measurement period
    for(i = 0; i < pMeasItem->iDeviceEcgCount; i = i + 1)
    {
      if(pMeasItem->iDeviceEcgDid[i] == pEcgObject->dwDid)
      {
        iMeasIdx = i;
      }      
    }
    pMeasItem->iMeasId     = tNow;
    pMeasItem->fMeasType   = pMeasItem->iMeasPeriodSec[iMeasIdx];
    pDevice->dwBleOpenDid  = pMeasItem->iBleDidTarget;
    pMeasItem->dwTotalTime = (JDWORD)pMeasItem->iMeasPeriodSec[iMeasIdx];
    strcpy(pMeasItem->strMeasToken, pMeasItem->strTokens[iMeasIdx]);

    pMeasItem->iWaitSec    = pMeasItem->iMeasWaitSec[iMeasIdx];
    pMeasItem->iWaitSecNow = 0;

    /// print debug log
    DBG_PRINTF(iDid, "\t [MEAS][STATE][INIT] MEAS ID = %ld, MEAS TYPE = %f, BLE ID = %d\r\n", pMeasItem->iMeasId, pMeasItem->fMeasType, pMeasItem->iBleDidTarget);       

    ///----------------------------------------------------------------/// 
    /// 3. Set mode to Connecting
    ///----------------------------------------------------------------///    
    FuncMeasStateSet(pDevice, pMeasItem->iMeasId, pMeasItem->fMeasType, MEAS_STATE_START);
  }  
  else
  {
    DBG_PRINTF(iDid, "\t [MEAS][STATE][INIT] MEAS ID not found\r\n");
  }
}

void MeasurementStateStart(JDevice *pDevice, MeasItemType * pMeasItem)
{
  JINT iDID = pDevice->dwDID;
  DeviceEcgType * pEcgObject = NULL;

  JINT iRet = 0;

  time_t  tNow = time(NULL);

  ///----------------------------------------------------------------/// 
  /// 1. BLE device target check
  ///----------------------------------------------------------------///
  if(pMeasItem->iBleDidTarget <= 0)
  {
    DBG_PRINTF(iDID, "%s", "\t\t BLE_DID_TARGET <= 0, return\r\n");
    return;
  }

  ///----------------------------------------------------------------/// 
  /// 2. BLE Connect
  ///----------------------------------------------------------------///
  iRet = PacketCmdGatewayMBleOpen(pDevice,  pMeasItem->bBleMacTarget);
  if(iRet == FALSE)
  {
    DBG_PRINTF(iDID, "\t\t [ERROR][!!!] GATEWAY MBLE CLOSE FAILED\r\n");    
    return;
  }

  ///----------------------------------------------------------------/// 
  /// 3. Set mode to Connecting
  ///----------------------------------------------------------------///
  pDevice->dwBleConnectingT0 = (JDWORD) tNow;
  
  FuncMeasStateSet(pDevice, pMeasItem->iMeasId, pMeasItem->fMeasType, MEAS_STATE_CONNECTING);
}

void MeasurementStateConnecting(JDevice * pDevice, MeasItemType *pMeasItem)
{    
  JINT iDID = pDevice->dwDID;
  time_t  tNow = time(NULL);
  JBOOL   bRet = FALSE;
  JINT    iRet = 0;
  JDWORD  dwTotalTime = 0;

  JINT iBleConnectStatus = 0;
  JINT iBleOpenDidRead = 0;
    
  JGatewayInfo * pGatewayObject = &pDevice->gatewayInfo;

  ///----------------------------------------------------------------/// 
  /// 1. Read Gateway Object information
  ///----------------------------------------------------------------///  
  iRet = PacketCmdGatewayBleConnectStatusGet(pDevice, &iBleConnectStatus, &iBleOpenDidRead);
	if(iRet == FALSE)
	{
		DBG_PRINTF(iDID, "\t\t [ERROR][!!!] GATEWAY INFO GET FAILED#2\r\n");		
		return;
	}
   
  pMeasItem->dwStartTime      = tNow;
  pMeasItem->dwLastUpdateTime = 0;
  pMeasItem->dwEndTime        = tNow + pMeasItem->dwTotalTime;
  dwTotalTime = pMeasItem->dwEndTime - pMeasItem->dwStartTime;

  ///----------------------------------------------------------------/// 
  /// 2. Set the connecting T0
  ///----------------------------------------------------------------/// 
  if(pDevice->dwBleConnectingT0 == 0)
  {
    pDevice->dwBleConnectingT0 = (JDWORD) tNow;
  }

  ///----------------------------------------------------------------/// 
  /// 3. Wait connected
  ///----------------------------------------------------------------///      
  if((iBleConnectStatus != 1) || (iBleOpenDidRead != pDevice->dwBleOpenDid))
  {
    if((tNow - pDevice->dwBleConnectingT0) > 10)
    {      
      FuncMeasStateSet(pDevice, pMeasItem->iMeasId, pMeasItem->fMeasType, MEAS_STATE_START);
    }
    return;
  }

  ///----------------------------------------------------------------/// 
  /// 4. VSC Mode init
  ///----------------------------------------------------------------/// 
  FuncVscModeInit(pDevice);
  
  ///----------------------------------------------------------------/// 
  /// 5. VSC Start
  ///----------------------------------------------------------------/// 
  bRet = PacketCmdGatewaySBleVscFileModeStart(pDevice, (JINT) dwTotalTime);    
  
  if(bRet == FALSE)
  {
    PacketCmdGatewayMBleClose(pDevice);
    FuncMeasStateSet(pDevice, pMeasItem->iMeasId, pMeasItem->fMeasType, MEAS_STATE_START);  	    
    return;
  }

  ///----------------------------------------------------------------/// 
  /// 5. Set report to running
  ///----------------------------------------------------------------///   
  pDevice->iVscModeDataCountLeft = 0;
  pDevice->dwBleConnectingT0 = 0;
  pGatewayObject->dwVscModeStartUtc = 0;

  FuncMeasStateSet(pDevice, pMeasItem->iMeasId, pMeasItem->fMeasType, MEAS_STATE_RUNNING);  	      
}

void MeasurementStateRunning(JDevice *pDevice, MeasItemType *pMeasItem)
{
  time_t tNow = time(NULL);  
  static time_t tPre = 0;

  JINT iRet = FALSE;
  JINT iDID = pDevice->dwDID;
  
  JGatewayInfo * pGatewayObject = &pDevice->gatewayInfo;

  static JDWORD dwUtcLastPre       = 0;
  static JDWORD dwUtcLastSameCount = 0;

  JBOOL bUtcLastSame = FALSE; 

  JINT iBleConnectStatus = 0;
  JINT iBleOpenDid = 0;
  JDWORD dwVscModeStartUtc = 0;
  
  if(pDevice->iVscModeDataCountLeft == 0)
	{	    
    ///----------------------------------------------------------------///
    /// Update the VSC mode and the iVscModeDataCountLeft
    ///----------------------------------------------------------------///
    iRet = PacketCmdGatewayVscModeStatusGet(pDevice, &pGatewayObject->iVscModeOn, 
                                              &dwVscModeStartUtc, 
                                              &pGatewayObject->iVscModeQueueCnt);
    if(iRet == FALSE)
	  {
      return;
    }  
    /// get the gateway vsc mode queue count                                            
    pDevice->iVscModeDataCountLeft = pGatewayObject->iVscModeQueueCnt;  
  }

  ///----------------------------------------------------------------///
  /// Use same UTC time to check stop
  ///----------------------------------------------------------------///  
  if(pMeasItem->dwLastUpdateTime == 0)
  {
    dwUtcLastSameCount = 0;
    dwUtcLastPre       = 0;
  }    
  else if(pDevice->pMeasItemActive->dwLastUpdateTime > 0)
  {
    if((pDevice->iVscModeIdxLast == 0) && (dwUtcLastPre == 0))
    {
      pDevice->iVscModeIdxLast = ((pDevice->pMeasItemActive->dwLastUpdateTime - pDevice->pMeasItemActive->dwStartTime) * 5);    
    }
  } 

  if(pMeasItem->dwLastUpdateTime == dwUtcLastPre)
  {
    bUtcLastSame = TRUE;
    if(dwUtcLastSameCount < 5)    
    {
      dwUtcLastSameCount = dwUtcLastSameCount + 1;
    }
  }
  else
  {
    dwUtcLastSameCount = 0;
  }
    
  if(pMeasItem->fMeasType != REPORT_TYPE_FOREVER)
  {
    ///----------------------------------------------------------------///
    /// Check stop
    ///----------------------------------------------------------------///
    if(tNow >= pMeasItem->dwEndTime)
    {
      if(pMeasItem->dwLastUpdateTime < pMeasItem->dwEndTime)
      {
        if(dwUtcLastSameCount >= 5)
        {          
          DBG_PRINTF(iDID, "\t [MEAS][STATE][RUNNING] SET TO STOP : SAME = %d, UTC-LAST-PRE = %ld \r\n", dwUtcLastSameCount, dwUtcLastPre);
          dwUtcLastPre       = 0;
          dwUtcLastSameCount = 0;
          FuncMeasStateSet(pDevice, pMeasItem->iMeasId, pMeasItem->fMeasType, MEAS_STATE_STOP);
        }
      }
      else
      {
        DBG_PRINTF(iDID, "\t [MEAS][STATE][RUNNING] SET TO STOP : SAME = %d, UTC-LAST-PRE = %ld \r\n", dwUtcLastSameCount, dwUtcLastPre);
        dwUtcLastPre       = 0;
        dwUtcLastSameCount = 0;
        FuncMeasStateSet(pDevice, pMeasItem->iMeasId, pMeasItem->fMeasType, MEAS_STATE_STOP);
      }
    }    
  }  

  ///----------------------------------------------------------------///
  /// Check Connected
  ///----------------------------------------------------------------///  
  iRet = PacketCmdGatewayBleConnectStatusGet(pDevice, &iBleConnectStatus, &iBleOpenDid);
  if(iRet == FALSE)
  {
    DBG_PRINTF(iDID, "\t\t [ERROR][!!!] GATEWAY INFO GET FAILED#2\r\n");		
    return;
  }    

  if(iBleConnectStatus != 1)
  {   
    dwUtcLastSameCount = 0;
    dwUtcLastPre = 0; 

    DBG_PRINTF(iDID, "\t\t [MEAS][STATE][RUNNING] [ERROR][!!!] GATEWAY BLE CONNECT STATUS = %d\r\n", iBleConnectStatus);

    iRet = PacketCmdGatewayMBleClose(pDevice);
		if(iRet == FALSE)
    {
      DBG_PRINTF(iDID, "\t\t [ERROR][!!!] GATEWAY MBLE CLOSE FAILED\r\n");      	
      return;
    }        
    FuncMeasStateSet(pDevice, pMeasItem->iMeasId, pMeasItem->fMeasType, MEAS_STATE_START);
    return;
  }
  
  if(iBleOpenDid != pDevice->dwBleOpenDid)
  {   
    dwUtcLastSameCount = 0;
    dwUtcLastPre = 0; 

    DBG_PRINTF(iDID, "\t\t [MEAS][STATE][RUNNING] [ERROR][!!!] OPEN DID  %d != %d\r\n", iBleOpenDid, pDevice->dwBleOpenDid);

    iRet = PacketCmdGatewayMBleClose(pDevice);
		if(iRet == FALSE)
    {
      DBG_PRINTF(iDID, "\t\t [ERROR][!!!] GATEWAY MBLE CLOSE FAILED\r\n");      	
      return;
    }        
    FuncMeasStateSet(pDevice, pMeasItem->iMeasId, pMeasItem->fMeasType, MEAS_STATE_START);
    return;
  }  

  dwUtcLastPre = pMeasItem->dwLastUpdateTime;
}

void MeasurementStateStop(JDevice * pDevice, MeasItemType * pMeasItem)
{
  JINT  iDID = pDevice->dwDID;
  JBOOL bRet = FALSE;

  DBG_PRINTF(iDID, "[MEAS][STATE] STOP\r\n");  
  bRet = PacketCmdGatewaySBleVscFileModeStop(pDevice);  	  

  if(bRet == FALSE)
  {
    return;
  }	
  
	///----------------------------------------------------------------///
	/// Wait BLE Send	
	///----------------------------------------------------------------///
  UtilMsSleep(400);

  bRet = PacketCmdGatewayMBleClose(pDevice);
  if(bRet == FALSE)
  {
    return;
  }  

  if(pMeasItem->dwLastUpdateTime < pMeasItem->dwStartTime)
  {
    pMeasItem->dwLastUpdateTime = pMeasItem->dwStartTime;
  }

  pDevice->gatewayInfo.iVscModeOn = FALSE;
  
  pMeasItem->iMeasId = 0;
  pDevice->dwBleConnectingT0 = 0;
        
  FuncMeasStateSet(pDevice, pMeasItem->iMeasId, pMeasItem->fMeasType, MEAS_STATE_FINISHED);
}

void MeasurementStateFinished(JDevice *pDevice, MeasItemType *pMeasItem)
{
  JINT  iDID = pDevice->dwDID;
  JBOOL bRet = FALSE;
  DBG_PRINTF(iDID, "[MEAS][STATE] FINISHED\r\n");

  ///----------------------------------------------------------------///
  /// if the wait sec is set, then wait for some seconds 
  /// to restart the measurement, otherwise, just stay at finished state
  ///----------------------------------------------------------------///
  if(pMeasItem->iWaitSec > 0)
  {
    pMeasItem->iWaitSecNow = pMeasItem->iWaitSecNow + 1;
    
    DBG_PRINTF(iDID, "[MEAS][STATE][FINISHED] Wait for restart: %d/%d\r\n", pMeasItem->iWaitSecNow, pMeasItem->iWaitSec);

    if(pMeasItem->iWaitSecNow >= pMeasItem->iWaitSec)
    {
      pMeasItem->iWaitSecNow = 0;      
      FuncMeasStateSet(pDevice, pMeasItem->iMeasId, pMeasItem->fMeasType, MEAS_STATE_INIT);
    }
  }
}

void MeasurementStateControl(JDevice * pDevice, MeasItemType *pMeasItem)
{
  if(pMeasItem->iMeasState == MEAS_STATE_INIT)
  {
    MeasurementStateInit(pDevice, pMeasItem);
  }
  if(pMeasItem->iMeasState == MEAS_STATE_START)
  {
    MeasurementStateStart(pDevice, pMeasItem);
  }
  if(pMeasItem->iMeasState == MEAS_STATE_CONNECTING)
  {
    MeasurementStateConnecting(pDevice, pMeasItem);
  }
  if(pMeasItem->iMeasState == MEAS_STATE_RUNNING)
  {
    MeasurementStateRunning(pDevice, pMeasItem);
  }
  if(pMeasItem->iMeasState == MEAS_STATE_STOP)
  {
    MeasurementStateStop(pDevice, pMeasItem);
  }
  if(pMeasItem->iMeasState == MEAS_STATE_FINISHED)
  {
    MeasurementStateFinished(pDevice, pMeasItem);
  }
}
