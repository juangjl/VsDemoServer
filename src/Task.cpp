/**
 * @file Task.cpp 
 * @brief Task function
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2024, VitalSigns Technology all rights reserved.
 * @note
*/
#include "Global.h"
MeasItemType MeasItem;

///--------------------------------------------------------///
/// Task init
///--------------------------------------------------------///
JINT TaskInit(JDevice *pDevice)
{
	JBOOL bRet = FALSE;
	JINT iWrtLen = 0;
	JINT iRdLen = 0;	
  char fileName[256];
	
	JINT i = 0;	  	
	
	pDevice->t0 = time(NULL);
	pDevice->t1 = pDevice->t0;	 
	pDevice->iSec    		= 0;
	pDevice->iSecPre 	 	= 0;	
	pDevice->iDevStatus = 0;
	pDevice->dwMID 		 	= INVALID_MID;
	pDevice->dwPID 		 	= INVALID_PID;
	pDevice->dwDID	   	= INVALID_DID;

	bRet = PacketCmdSockDeviceInfoRead(pDevice);
	
	if(bRet == TRUE)
	{				
		if((pDevice->dwDID < DEVICE_DID_MIN) || (pDevice->dwDID  > DEVICE_DID_MAX))
		{
			DBG_PRINTF(DBG_ID, "[TASK][INIT][ERROR] Invalid DID = %d\r\n", pDevice->dwDID);						
			bRet = FALSE;			
		}

		if(pDevice->dwMID != 0x45110164)
		{
			DBG_PRINTF(DBG_ID, "[TASK][INIT][ERROR] Invalid MID = %d\r\n", pDevice->dwMID);			
			bRet = FALSE;
		}
	}
	if(bRet == FALSE)
	{		
		DBG_PRINTF(DBG_ID, "[TASK][INIT][ERROR] Device Info Read failed !!!\r\n");		
		DBG_PRINTF(DBG_ID, "[TASK][INIT][EXIT][1] socket = %d\r\n", pDevice->iDevSocket);	
		close(pDevice->iDevSocket);	
		_exit(100);
	}
	
	DBG_PRINTF(DBG_ID,"MID = %08X, PID = %08X, DID = %07d\r\n",(JDWORD)pDevice->dwMID, (JDWORD)pDevice->dwPID, (JDWORD)pDevice->dwDID);	

	/// get the PID
	if(pDevice->dwPID == GATEWAY_TYPE_LTE)
	{
		pDevice->GatewayConfig.iGatewayType = GATEWAY_TYPE_LTE;
	}
	/// WIFI type
	else if(pDevice->dwPID == GATEWAY_TYPE_WIFI)
	{
		pDevice->GatewayConfig.iGatewayType = GATEWAY_TYPE_WIFI;
	}	

	if(PacketCmdSockDeviceTimeSet(pDevice) == FALSE)
	{
		DBG_PRINTF(DBG_ID, "[TASK][INIT][ERROR] Sock Device TimeSet failed !!!\r\n");
		close(pDevice->iDevSocket);
		_exit(101);
	}		
	
  UtilSocketInfoPrint(pDevice);
	
	for(i = 0; i < 10 ; i = i + 1)
	{		
  	/// add check device type  
		if(PacketCmdGatewayModeGet(pDevice) == FALSE)
		{			
			DBG_PRINTF(pDevice->dwDID, "[ERROR] gateway Mode get failed = %d  \r\n", pDevice->dwDID);
			if(i == 9)
			{
				close(pDevice->iDevSocket);
				_exit(102);
			}						
		}
		else
		{
			break;
		}
  } 

	/// Time zone get
  pDevice->iTimeZoneOffset = UtilTimeZoneGet(pDevice);

	/// system tick
	pDevice->dwTick     = TickCountGet();
	pDevice->dwTickPre  = pDevice->dwTick;
	pDevice->iTick100MS = 0;

	/// Create Device Command Folder
	FuncCmdFolderCreate(pDevice);

	/// Create Device Command Folder
	FuncMeasFolderCreate(pDevice);

	/// Gateway folder create
	FuncGatewayFolderCreate(pDevice);
		
	/// Set gateway online
	FuncGatewayOnlineSet(pDevice);	
	
  /// VSC Mode init 
  FuncVscModeInit(pDevice);
	
	///----------------------------------------------------------------///
	/// Task excution time wait
	///----------------------------------------------------------------///
	pDevice->iWaitTaskSystemVersionGet        = 2;
  pDevice->iWaitTaskGatewayInfoGet   			  = 60;
  pDevice->iWaitTaskGatewayEcgObjectListGet = 30;
	pDevice->iWaitTaskTaskLocationInfoGet			= 60;

	///----------------------------------------------------------------///
	/// Task excution time wait
	///----------------------------------------------------------------///
	pGlobalVar->pDevice = pDevice;

	///----------------------------------------------------------------///
	/// Data init
	///----------------------------------------------------------------///
  UtilMemset((JBYTE *)&pDevice->gatewayInfo, 0x00, sizeof(JGatewayInfo));
  pDevice->iGatewayInfoFileIdx    = -1;
  pDevice->iGatewayInfoFileIdxPre = -1;      
  pDevice->iUserId                = -1;  

	///----------------------------------------------------------------///
	/// Mapped device did get
	///----------------------------------------------------------------///	
	/// Init measurement data
	UtilMemset((JBYTE *)&MeasItem, 0x00, sizeof(MeasItemType));
	pDevice->pMeasItemActive = &MeasItem;  	
	pDevice->pMeasItemActive->iMeasState = MEAS_STATE_INIT;

	/// Get device did for measurement item
	FuncGatewayDeviceDidGet(pDevice);	
	
	return TRUE;
}

void TaskRoundInit(JDevice *pDevice)
{
	JINT iDid = pDevice->dwDID;
	pDevice->dwTick = TickCountGet();
	if((pDevice->dwTick - pDevice->dwTickPre) >= 100)
	{	
		pDevice->dwTickPre  = pDevice->dwTick ;
		pDevice->iTick100MS = pDevice->iTick100MS + 1;
		pDevice->dwDevEvent0 |= DEV_EVT0_100MS_EVENT;
		if((pDevice->iTick100MS % 2) == 0)
		{
			pDevice->dwDevEvent0 |= DEV_EVT0_200MS_EVENT;		
		}		
		if((pDevice->iTick100MS % 3) == 2)
    {
			pDevice->dwDevEvent0 |= DEV_EVT0_300MS_EVENT;		
    } 
		if((pDevice->iTick100MS % 4) == 3)
    {
			pDevice->dwDevEvent0 |= DEV_EVT0_400MS_EVENT;		
    } 
		if((pDevice->iTick100MS % 5) == 4)
    {
			pDevice->dwDevEvent0 |= DEV_EVT0_500MS_EVENT;		
    }  
    if(pDevice->iTick100MS >= 60)
    {
      pDevice->iTick100MS = 0;
      FuncPrintFilterRead();
    }
	}
	else
	{
		pDevice->dwDevEvent0 &= ~DEV_EVT0_100MS_EVENT;
		pDevice->dwDevEvent0 &= ~DEV_EVT0_200MS_EVENT;	
		pDevice->dwDevEvent0 &= ~DEV_EVT0_300MS_EVENT;	
		pDevice->dwDevEvent0 &= ~DEV_EVT0_400MS_EVENT;		
		pDevice->dwDevEvent0 &= ~DEV_EVT0_500MS_EVENT;						
	}
	
	/// Check Second event
	if(pDevice->iSecPre != pDevice->iSec)
	{
		pDevice->dwDevEvent0 |= DEV_EVT0_SEC_EVENT;	
	}
	else
	{
		pDevice->dwDevEvent0 &= ~DEV_EVT0_SEC_EVENT;
	}

	///----------------------------------------------------------------///
	/// 1. Wait time for task system version get
	///----------------------------------------------------------------///			
	if(pDevice->gatewayInfo.iVscModeOn == 0)
	{		
		pDevice->iWaitTaskSystemVersionGet = 3;
	}
	else
	{
		pDevice->iWaitTaskSystemVersionGet = 60;
	}

	///----------------------------------------------------------------///
	/// 2. Wait time for task Gateway information get
	///----------------------------------------------------------------///			
	pDevice->iWaitTaskGatewayInfoGet = 10;

	///----------------------------------------------------------------///
	/// 3. Wait time for task Gateway ECG Object List get
	///----------------------------------------------------------------///			
	if(pDevice->iVscModeDataCountLeft > 0)
	{
		pDevice->iWaitTaskGatewayEcgObjectListGet = 4;
	}
	else
	{
		pDevice->iWaitTaskGatewayEcgObjectListGet = 30;
	}	
}

void TaskSystemVersionGet(JDevice *pDevice)
{
	JINT iDid = pDevice->dwDID;		
	
	JTM jtm;
  JTM * pJtm = &jtm;
	time_t  tNow = time(NULL);
  JTMGmtTimeGet(pJtm, tNow);
	static JINT tPre = 0;

	JINT iWaitTime = 0;	

	if(pDevice->iDevStatus & STATUS_SOCKET_FAIL)
	{
		return;
	}

	iWaitTime = pDevice->iWaitTaskSystemVersionGet;

	if((tNow - tPre) < iWaitTime)
	{
		return;
	}
	tPre = tNow;

	///----------------------------------------------------------------///
	/// TASK VERSION
	///----------------------------------------------------------------///
	DBG_PRINTF(iDid, "[TASK] SYSTEM VERSION GET\r\n");		

	FuncGatewayVersionGet(pDevice);

	FuncGatewayOnlineSet(pDevice);

	DBG_PRINTF(iDid, "\t [TASK] SYSTEM VERSION GET - END\r\n\r\n");
}

void TaskGatewayInfoGet(JDevice *pDevice)
{
	JINT iDid  = pDevice->dwDID;		
	char fileName[256];
	char strCmd[256];
	JINT iRet = 0;  	

	static time_t tPre = -1;
	JTM jtm;
  JTM * pJtm = &jtm;
	time_t  tNow = time(NULL);
  JTMGmtTimeGet(pJtm, tNow);	

	JGatewayInfo * pGatewayObject = &pDevice->gatewayInfo;

	JINT iWaitSec = 0;

	pid_t pidChild  = 0;
	pid_t pidChild2 = 0;

	if(pDevice->iDevStatus & STATUS_SOCKET_FAIL)
	{
		return;
	}	
  
	iWaitSec = pDevice->iWaitTaskGatewayInfoGet;
	
	if((tNow - tPre) < iWaitSec)
	{
		return;
	}		
	tPre = tNow;

	///----------------------------------------------------------------///
	/// TASK PRINT
	///----------------------------------------------------------------///
	DBG_PRINTF(iDid, "[TASK] GATEWAY INFO GET\r\n");
	
	///----------------------------------------------------------------///
	/// Packet command send
	///----------------------------------------------------------------///	
	iRet = PacketCmdGatewayInfoGet(pDevice);	
	if(iRet == FALSE)
	{
		DBG_PRINTF(iDid, "\t\t [ERROR][!!!] GATEWAY INFO GET FAILED \r\n");					
		return;
	}
		
	///--------------------------------------------------------///
	/// Gateway information printing for debuging only
	///--------------------------------------------------------///
	GatewayInfoPrint(pDevice, &pDevice->gatewayInfo);

	///--------------------------------------------------------///
	/// Save the gateway information file for debuging only
	///--------------------------------------------------------///
	FuncGatewayInfoFileWrite(pDevice, tNow);
}

void TaskGatewayDeviceEcgListGet(JDevice * pDevice)
{			
	time_t  tNow = time(NULL);
	JTM jtm;
  JTM * pJtm = &jtm;	
	JINT iDid  = pDevice->dwDID;		

	static time_t tPre = -1;
	JINT iWaitTime = 0;

  JTMGmtTimeGet(pJtm, tNow);

	if(pDevice->iDevStatus & STATUS_SOCKET_FAIL)
	{
		return;
	}

	iWaitTime = pDevice->iWaitTaskGatewayEcgObjectListGet;
	
	if((tNow - tPre) < iWaitTime)
	{
		return;
	}	
	tPre = tNow;

	///----------------------------------------------------------------///
	/// TASK PRINT
	///----------------------------------------------------------------///
	DBG_PRINTF(iDid, "[TASK] DEVICE ECG INFO LIST GET\r\n");	
	
	FuncGatewayDeviceEcgListGet(pDevice);

	FuncGatewayScanDeviceFileWrite(pDevice);		
}

void TaskLocationInfoGet(JDevice *pDevice)
{
	char msg[256];
	JINT iDid = pDevice->dwDID;	

	static time_t tPre = 0;
	static time_t tNow = 0;	

	JINT iRet = FALSE;

	if(pDevice->iDevStatus & STATUS_SOCKET_FAIL)
	{
		return;
	}
	
	///----------------------------------------------------------------///
	/// Enter ther task every 30 seconds
	///----------------------------------------------------------------///
	tNow = time(NULL);
	if((tNow - tPre) < pDevice->iWaitTaskTaskLocationInfoGet)
	{
		return;
	}
	tPre = tNow ;
	
	///----------------------------------------------------------------///
	/// TASK PRINT
	///----------------------------------------------------------------///
	DBG_PRINTF(iDid, "[TASK] GATEWAY LOCATION GET\r\n");

	FuncGatewayLocationInfoGet(pDevice);

	FuncGatewayLocationFileWrite(pDevice);	
}

void TaskMeasurementMode(JDevice * pDevice)
{	
	JBOOL bRet   = FALSE;	
  JINT 	i 		 = 0;			
	JINT  idx 	 = 0;
	static JDWORD dwDeviceEcgDid[4];
	static JINT   iDeviceCnt;
  SRegType      sreg;
  SRegType * 	  pSReg = &sreg;

	static time_t tPre = 0;	
	time_t  tNow = time(NULL);
	JTM jtm;
  JTM * pJtm = &jtm;	
  JTMGmtTimeGet(pJtm, tNow);

  static JDWORD dwConnDID = 0;
	
	DeviceEcgType * pEcgObject = NULL;		
	MeasItemType * pMeasItem = NULL;

	time_t  tChkEndTime   = 0;						

	JINT iIdx = 0;
	JINT iCnt = MEAS_ITEM_LIST_CNT;

	char strRunningStatus[32];	

	if(pDevice->iDevStatus & STATUS_SOCKET_FAIL)
	{
		return;
	}

  if((pDevice->dwDevEvent0 & DEV_EVT0_SEC_EVENT) == 0)
	{
		return;
	}

	if(tPre == tNow)
	{
		return;
	}		
	tPre = tNow;
	
	///----------------------------------------------------------------///
	/// TASK PRINT
	///----------------------------------------------------------------///
	DBG_PRINTF(pDevice->dwDID, "[TASK] MEASUREMENT MODE\r\n");	
	pMeasItem = pDevice->pMeasItemActive;

	///----------------------------------------------------------------///
	/// Measurement running status
	///----------------------------------------------------------------///
	if(pMeasItem->iMeasState == MEAS_STATE_INIT)
	{
		sprintf(strRunningStatus, "%s", "INIT");
	}
	else 	if(pMeasItem->iMeasState == MEAS_STATE_FINISHED)
	{
		sprintf(strRunningStatus, "%s", "FINISHED");
	}
	else if(pMeasItem->iMeasState == MEAS_STATE_RUNNING)
	{
		sprintf(strRunningStatus, "%s", "RUNNING");
	}
	else if(pMeasItem->iMeasState == MEAS_STATE_STOP)
	{
		sprintf(strRunningStatus, "%s", "STOP");
	}
	else if(pMeasItem->iMeasState == MEAS_STATE_START)
	{
		sprintf(strRunningStatus, "%s", "START");
	}
	else if(pMeasItem->iMeasState == MEAS_STATE_CONNECTING)
	{
		sprintf(strRunningStatus, "%s", "CONNECTING");
	}	
  else if(pMeasItem->iMeasState == MEAS_STATE_FILE_UPLOAD)
  {
		sprintf(strRunningStatus, "%s", "UPLOADING");
  }
  else if(pMeasItem->iMeasState == MEAS_STATE_FILE_UPLOAD_FINISHED)
  {
		sprintf(strRunningStatus, "%s", "UPLOAD FINISHED");
  } 
	else
	{
		sprintf(strRunningStatus, "%s", "UNKNOWN");
	}

	///-------------------------------------------------------///
	/// Measurement Control
  ///-------------------------------------------------------///
	MeasurementStateControl(pDevice, pMeasItem);	
}

void TaskSocketCheck(JDevice * pDevice)
{
	int error = 0;
	socklen_t len = sizeof (error);
	int retval = getsockopt (pDevice->iDevSocket, SOL_SOCKET, SO_ERROR, &error, &len);
	if(retval != 0) 
	{
	  /// there was a problem getting the error code 
    DBG_PRINTF(DBG_ID, "[%d]error getting socket error code: %s\n", pDevice->dwDID, strerror(retval));
		pDevice->iDevStatus |= STATUS_SOCKET_FAIL;
		return;
  }

	if(error != 0) 
	{
		/// socket has a non zero error status 
		DBG_PRINTF(DBG_ID, "[%d]socket error: %s\n",pDevice->dwDID, strerror(error));
		pDevice->iDevStatus |= STATUS_SOCKET_FAIL;
		return;
	}	
}

void TaskDeviceEcgVscMode(JDevice *pDevice)
{
	JBOOL bRet = FALSE;
	
	JINT iDid = pDevice->dwDID;

	JINT iReadCnt  = 0;
  DeviceEcgType *pItem = NULL;
	JDWORD dwTick = 0;
	static JDWORD dwTickPre = 0;
	JINT iCnt = 0;

	JINT iRestCount = 0;
	JINT iReadCount = 0;

	JINT iReadBatchLen = 2; ///< 2
	time_t tNow = time(NULL);	

	JGatewayInfo * pGatewayObject = &pDevice->gatewayInfo;

	JDWORD dwVscModeStartUtc = 0;

	JINT iBatchCount1 = 1;
	JINT iBatchCount2 = 2;
	JINT iBatchCount3 = 2;

	if(pDevice->iDevStatus & STATUS_SOCKET_FAIL)
	{
		return;
	}

	if(pDevice->pMeasItemActive->iMeasState != MEAS_STATE_RUNNING)
	{
		return;
	}
		
	dwTick = TickCountGet();
	if((dwTick - dwTickPre) < 50)
	{
		return;
	}
	dwTickPre = dwTick;	

	///--------------------------------------------------------------------------------------///
	/// check the vsc mode queue count 
	///--------------------------------------------------------------------------------------///
	if((pDevice->dwBleOpenDid == 0)&&(pDevice->gatewayInfo.iBleOpenDid != 0))
	{
		pItem = GatewayDeviceEcgListItemGet(pDevice, pDevice->gatewayInfo.iBleOpenDid);
		if(pItem != NULL)
		{
			UtilMemcpy((JBYTE *)&pDevice->strDeviceSSN[0], 	(JBYTE *)&pItem->strSSN[0], SSN_SIZE);			
			UtilMemcpy((JBYTE *)&pDevice->bBleOpenAddr[0],  (JBYTE *)&pItem->bAddr[0],  BLE_MAC_ADDR_SIZE);
			pDevice->dwBleOpenDid = pItem->dwDid;    	
		}
	}
	
	if(pDevice->gatewayInfo.iVscModeOn == 0)
	{		
		return;
	}

	if(pDevice->iOnlineType == SYS_ONLINE_LTE)
	{
		///----------------------------------------------------------------///
		/// if LTE utilizes 1, 2, 2, it cannot read all the items in the queue
		/// Utilize the 2, 3, 4
		///----------------------------------------------------------------///
		iBatchCount1 = 1;
		iBatchCount2 = 2;
		iBatchCount3 = 2;
	}
	else if(pDevice->iOnlineType == SYS_ONLINE_WIFI)
	{
		///----------------------------------------------------------------///
		/// WIFI could be 1, 2, 2， it can read the queue out in normal status		
		///----------------------------------------------------------------///
		iBatchCount1 = 1;
		iBatchCount2 = 2;
		iBatchCount3 = 2;
	}

	///----------------------------------------------------------------///
	/// TASK PRINT
	///----------------------------------------------------------------///
	DBG_PRINTF(pDevice->dwDID, "[TASK] DEVICE ECG VSC MODE\r\n");	

	if(pDevice->iVscModeDataCountLeft == 0)
	{	      
    bRet = PacketCmdGatewayVscModeStatusGet(pDevice, &pGatewayObject->iVscModeOn, 
                                              &dwVscModeStartUtc, 
                                              &pGatewayObject->iVscModeQueueCnt);
    if(bRet == FALSE)
	  {						
      return;
    }                                              
    pDevice->iVscModeDataCountLeft = pGatewayObject->iVscModeQueueCnt;  
  }

  /// first get gateway report count
	if(pDevice->iVscModeDataCountLeft < pDevice->gatewayInfo.iVscModeQueueCnt)
	{
		pDevice->iVscModeDataCountLeft = pDevice->gatewayInfo.iVscModeQueueCnt;
	}
	
	/// get the queue count
	if(pDevice->iVscModeDataCountLeft <= 0)
	{		
		return;
	}	
	
	/// VSC_MODE queue data read  	
	while(1)
	{
		iRestCount = pDevice->iVscModeDataCountLeft;
		if(iRestCount <= 0)
		{
			break;
		}

		if(pDevice->iVscModeDataCountLeft <= 2)
		{
			iReadBatchLen = iBatchCount1;
		}
		else if(pDevice->iVscModeDataCountLeft < 5)
		{
			iReadBatchLen = iBatchCount2;
		}
		else
		{
			iReadBatchLen = iBatchCount3;
		}

		if(iRestCount >= iReadBatchLen)
		{
			iReadCount = iReadBatchLen;
		}
		else
		{
			iReadCount = iRestCount;
		}
		
		/// VSC_MODE queue data read 	  				
		bRet = FuncVscModeQueueRead(pDevice, iReadCount);
		if(bRet == FALSE)
		{
			DBG_PRINTF(pDevice->dwDID, "\t [ERROR][VSC][MODE] FAILED - 1\r\n");
			break;
		}
		
		iRestCount = iRestCount - iReadCount;
		
		FuncVscModeExecute(pDevice);

		if(pDevice->pMeasItemActive->iMeasState != MEAS_STATE_RUNNING)
		{
			break;
		}
		
		if(pDevice->iVscModeDataCountLeft <= 2)
		{
			break;
		}
	
		iCnt = iCnt + 1;
    if(iCnt >= 10)
		{
			break;
		}
	}	
}

void TaskCmdExecute(JDevice * pDevice)
{
	if((pDevice->dwDevEvent0 &  DEV_EVT0_100MS_EVENT) == 0)
	{
		return;
	}

	/// Check command
	CmdCheck(pDevice);	
}

void TaskCheckLeave(JDevice * pDevice)
{
  static JDWORD dwUpdateTick = 0;
  char fileName[256];
  JINT iDid = pDevice->dwDID;
  
	sprintf(fileName, "./leave");
	if(UtilFileExisted((char *)fileName) == TRUE)
	{
		DBG_PRINTF(DBG_ID, "%s", "[LEAVE] Leave \r\n");			
		FuncGatewayOfflineSet(pDevice);
		close(pDevice->iDevSocket);
		exit(0);
	}	
}

void TaskRoundEnd(JDevice *pDevice)
{
	pDevice->t1   = time(NULL);
	pDevice->tNow = time(NULL);

	pDevice->iSecPre =	pDevice->iSec;	
	if((pDevice->tNow - pDevice->tPre) >= 1)
	{
		pDevice->iSec = pDevice->iSec + (pDevice->tNow - pDevice->tPre);
	 	pDevice->tPre = pDevice->tNow;				
	}	
}

JINT Mainloop(JINT iSocket)
{
	JDevice  device;
	JDevice *pDevice = &device;
		
	JINT iRet = 0;
	JINT pid  = getpid();
  
	pDevice->tPre  = time(NULL);
	pDevice->tNow  = pDevice->tPre;	
  pDevice->dwDID = 0;
	
	pDevice->iDevSocket = iSocket;

	sleep(1);
	SockPurge(DBG_ID, &iSocket);
		
	iRet = TaskInit(pDevice);	
	if(iRet == FALSE)
	{			
		close(pDevice->iDevSocket);	
		_exit(1);
	}
		
	while(1)
	{	
		///---------------------------------///
		/// Task: Round Init
  	///---------------------------------///			
		TaskRoundInit(pDevice);	
    
    ///---------------------------------///
    /// Task: Cmd execute
    ///---------------------------------///  
		TaskCmdExecute(pDevice);

		///---------------------------------///
    /// Task: System Version
    ///---------------------------------///  
		TaskSystemVersionGet(pDevice);
		
    ///---------------------------------///
    /// Task: Gateway Info
    ///---------------------------------///  
		TaskGatewayInfoGet(pDevice);
		
		///---------------------------------///
    /// Task: Ecg list Info
    ///---------------------------------///  
		TaskGatewayDeviceEcgListGet(pDevice);
		
		///---------------------------------///
    /// Task: Gateway Location Get
    ///---------------------------------///  
		TaskLocationInfoGet(pDevice);

		///---------------------------------///
    /// Task: TaskMeasurementMode
    ///---------------------------------/// 
    TaskMeasurementMode(pDevice);		

		///---------------------------------///
		/// Task: Device ECG VSC Mode access
		///---------------------------------///   
		TaskDeviceEcgVscMode(pDevice);
		
		///---------------------------------///
		/// Task : TaskSocketCheck
  	///---------------------------------///		
	  TaskSocketCheck(pDevice);
		
		///---------------------------------///
		/// Task : TaskCheckLeave
  	///---------------------------------///		
	  TaskCheckLeave(pDevice);

		///---------------------------------///	
		/// Task:  Round final
  	///---------------------------------///		
		TaskRoundEnd(pDevice);

		///---------------------------------///
		/// Status check 
  	///---------------------------------///		
		if(pDevice->iDevStatus & STATUS_SOCKET_FAIL)
    {
      /// query file time is match current time then disconnect
      DBG_PRINTF(pDevice->dwDID,"[DEV] Status = %08x\r\n", pDevice->iDevStatus);  
      break;
    } 

		if(pDevice->iDevStatus & STATUS_USER_CLOSE)
		{
  	  DBG_PRINTF(DBG_ID, "[%06d] [DEV] Status = %08x USER CLOSE\r\n", pDevice->dwDID, pDevice->iDevStatus);  
			break;
		}
			
		UtilMsSleep(100);
	}

	FuncGatewayOfflineSet(pDevice);

	DBG_PRINTF(pDevice->dwDID, "[DEV] DID= %d offline \r\n", pDevice->dwDID);
	DBG_PRINTF(pDevice->dwDID, "DID = %d, Socket = %d Leaved!!!\n", pDevice->dwDID, iSocket);	
		
	close(pDevice->iDevSocket);

	return TRUE;
}
