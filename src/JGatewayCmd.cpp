/**
 * @file PacketCmd.cpp 
 * @brief Packet command function 
 * @version $Revision$
 * @author JL Juang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2022,  VitalSigns Technology Co., LTD., all rights reserved.
 * @note
*/
#include "Global.h"

///------------------------------------------------------------------------------------------------///
///
/// Packet Gateway Command
///
///------------------------------------------------------------------------------------------------///
JBOOL PacketCmdGatewayInfoGet(JDevice *pDevice)
{  
  JINT iDid     = pDevice->dwDID;    
  JBOOL bRet    = FALSE;  
    
	PacketType 			PacketOut;
	PacketAckType 	PacketIn;

  JGatewayInfo * pGatewayInfoRead = NULL;

  PacketInit(&PacketOut);

  struct tm tm1 = {0};

	time_t  tNow = time(NULL);

  /// get the time zone 
  localtime_r(&tNow, &tm1); 
  
  PacketOut.bPCode    = CMD_GW_GATEWAY_INFO_GET;
	PacketOut.bGroupId  = CMD_GROUP_ID_GATEWAY;		
	PacketOut.bCmd      = CMD_GW_GATEWAY_INFO_GET;
	PacketOut.wMISOLen  = sizeof(JGatewayInfo);    
	PacketOut.wMOSILen  = 24; ///< 8 + 24 = 32
  
  ///----------------------------------------------------------------///
  /// UTC
  ///----------------------------------------------------------------///
  *(JDWORD *) &PacketOut.bData[0] = (JDWORD)tNow;
  *(JDWORD *) &PacketOut.bData[4] = (JDWORD)tNow;

  ///----------------------------------------------------------------///
  /// TIME ZONE
  ///----------------------------------------------------------------///
  *(JINT *) &PacketOut.bData[8]  = tm1.tm_gmtoff;
  *(JINT *) &PacketOut.bData[12] = tm1.tm_gmtoff;
    
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
 		return bRet;
 	}  

  pGatewayInfoRead = (JGatewayInfo * ) &PacketIn.bData[0];

  if(GatewayInfoCheck(pDevice, pGatewayInfoRead) == TRUE)
  {
    UtilMemcpy((JBYTE *)&pDevice->gatewayInfo, &PacketIn.bData[0], sizeof(JGatewayInfo));
    /// check the ECG file data 
    if(pDevice->gatewayInfo.dwMonitorFileUtc > SYSTIME_INIT_CHECK)
    {
      pDevice->iGatewayInfoFileIdx  = pDevice->gatewayInfo.dwMonitorFileUtc;
      pDevice->iGatewayInfoFileSize = pDevice->gatewayInfo.dwMonitorFileSize;
    }  
  }
  else
  {
    GatewayInfoPrint(pDevice, pGatewayInfoRead);
    DBG_PRINTF(iDid, "\t [CMD][GATEWAY] gateway info read, data is not valid\r\n\r\n");
    return FALSE;
  }    
      
  return TRUE;
}

///------------------------------------------------------------------------------------------------///
///
/// Previous Functions
///
///------------------------------------------------------------------------------------------------///

///----------------------------------------------------------------///
/// CMD: Gateway mode set
///----------------------------------------------------------------///
JBOOL PacketCmdGatewayModeGet(JDevice *pDevice)
{
  JBOOL  bRet = FALSE;
  
	PacketType 			PacketOut;
	PacketAckType 	PacketIn;

  PacketInit(&PacketOut);   

  PacketOut.bGroupId  = CMD_GROUP_ID_SYSTEM;		
	PacketOut.bCmd      = CMD_SYS_GW_MODE_GET;
	PacketOut.wMOSILen  = 8;
	PacketOut.wMISOLen  = 8;
  
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
 		return bRet;
 	}
  
  /// no this command then setting default
  if(PacketIn.bAck != CMD_ACK)
  {
    pDevice->GatewayConfig.iGatewayDeviceMode = GW_SYS_ECG_MODE;
  }
  else
  {
    UtilMemcpy((JBYTE*)&pDevice->GatewayConfig.iGatewayDeviceMode, &PacketIn.bData[0], 4);
  }  
  return TRUE;
}

JBOOL PacketCmdGatewaySRegRead(JDevice *pDevice, SRegType *pSReg)
{
  JBOOL  bRet      = FALSE;
  
  PacketType    PacketOut;
  PacketAckType PacketIn;
  
  PacketInit(&PacketOut);
  
  /// output packet set
  PacketOut.bGroupId 	= CMD_GROUP_ID_SREG;
  PacketOut.bCmd 			= CMD_SREG_READ;
  PacketOut.bPCode 		= CMD_SREG_READ;
  PacketOut.wMOSILen 	= SREG_NAME_SIZE + 8; ///< output data size = 32 + 8  +8 = 48
  PacketOut.wMISOLen 	= SREG_SIZE + 8;      ///< input  data size = 96 + 8 + 8 = 112

  /// SReg Data Clear
  UtilMemset((JBYTE *) &pSReg->bData[0], 0x00, SREG_DATA_SIZE);
	
  /// SReg Name Set
  UtilMemcpy((JBYTE *) &PacketOut.bData[0],(JBYTE *)pSReg, SREG_NAME_SIZE);


  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] GW Ble Open fail\r\n");
 		return bRet;
 	}

  if(PacketIn.bAck != 'A')
  {
	  DBG_PRINTF(pDevice->dwDID, "[%06d] GW SREG FAIL command fail %s\r\n", pDevice->dwDID, pSReg->strName);
    return FALSE;
  }
  
  /// SReg Data Set
  UtilMemcpy((JBYTE *) &pSReg->bData[0],(JBYTE *)&PacketIn.bData[0], SREG_DATA_SIZE);

  return bRet;
}

JBOOL PacketCmdGatewaySRegWrite(JDevice *pDevice, SRegType *pSReg)
{
  JBOOL  bRet      = FALSE;
  
  PacketType    PacketOut;
  PacketAckType PacketIn;
   
  PacketInit(&PacketOut);
  
  /// output packet set
  PacketOut.bGroupId 	= CMD_GROUP_ID_SREG;
  PacketOut.bCmd 			= CMD_SREG_WRITE;
  PacketOut.bPCode 		= CMD_SREG_WRITE;
  PacketOut.wMOSILen 	= SREG_SIZE + 8;  ///< 96 + 8 + 8
  PacketOut.wMISOLen 	= 8;              ///< 8 + 8 = 16

  /// SReg Name and Data Set
  UtilMemcpy((JBYTE *) &PacketOut.bData[0], (JBYTE *)pSReg, SREG_SIZE);
      
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] GW Ble Open fail\r\n");
 		return bRet;
 	}

  if(PacketIn.bAck != 'A')
  {
	  DBG_PRINTF(pDevice->dwDID, "[%06d] GW Ble Open command fail \r\n", pDevice->dwDID);
    return FALSE;
  }
  
  return bRet;
}

JBOOL PacketCmdGatewayMBleSRegCheck(JDevice *pDevice,  SRegType *pSReg)
{
  JBOOL bRet = FALSE;

  PacketType    PacketOut;
  PacketAckType PacketIn;

  PacketInit(&PacketOut);
  
  PacketOut.bGroupId = CMD_GROUP_ID_BLE;
  PacketOut.bCmd     = CMD_BLE_PACKET_READ;
  PacketOut.bPCode   = CMD_BLE_PACKET_READ;
  PacketOut.wMOSILen = 8;
  PacketOut.wMISOLen = SREG_DATA_SIZE+8;  


  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
  if(bRet == FALSE)
  {
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] Gateway BLE Command Check failed \r\n");
 		return bRet;
 	}

  if(PacketIn.bAck != 'A')
  {
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET][MBLE] BLE COMMAND CHECK NACK\r\n");
    return FALSE;
  }

  /// SReg Data Set
  UtilMemcpy((JBYTE *) &pSReg->bData[0],(JBYTE *)&PacketIn.bData[0], SREG_DATA_SIZE);
  
  return bRet;
}

JBOOL PacketCmdGatewayMBleFileListRead(JDevice *pDevice,  JINT iRecSize, JBYTE *pBuf)
{
  JBOOL bRet = FALSE;

  PacketType    PacketOut;
  PacketAckType PacketIn;

  PacketInit(&PacketOut);
  
  PacketOut.bGroupId = CMD_GROUP_ID_BLE;
  PacketOut.bCmd     = CMD_BLE_PACKET_READ;
  PacketOut.bPCode   = CMD_BLE_PACKET_READ;
  PacketOut.wMOSILen = 8;
  PacketOut.wMISOLen = iRecSize;  


  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
  if(bRet == FALSE)
  {
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] Gateway BLE Command Check failed \r\n");
 		return bRet;
 	}

  if(PacketIn.bAck != 'A')
  {
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET][MBLE] BLE COMMAND CHECK NACK\r\n");
    return FALSE;
  }

  UtilMemcpy((JBYTE *) &pBuf[0],(JBYTE *)&PacketIn.bData[0], iRecSize);

  return bRet;
}

JBOOL PacketCmdGatewayMBleConnectCheck(JDevice *pDevice)
{
  JBOOL bRet = FALSE;
  JBOOL bConnected = FALSE;

  PacketType    PacketOut;
  PacketAckType PacketIn;

  PacketInit(&PacketOut);
  
  PacketOut.bGroupId = CMD_GROUP_ID_BLE;
  PacketOut.bCmd     = CMD_BLE_CONNECT_STATUS;
  PacketOut.bPCode   = CMD_BLE_CONNECT_STATUS;
  PacketOut.wMOSILen = 8;
  PacketOut.wMISOLen = 8;  

  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
  if(bRet == FALSE)
  {
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] Gateway BLE Status failed \r\n");
 		return bRet;
 	}

  if(PacketIn.bAck != 'A')
  {
	  DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET][MBLE] BLE OPEN COMMAND NACK\r\n");
    return FALSE;
  }
  
  /// SReg Data Set
  bConnected = PacketIn.bData[0];

  DBG_PRINTF(pDevice->dwDID, "[GW][BLE] Connect : %d \r\n", bConnected);

  return bConnected;
}

JBOOL PacketCmdGatewayMBleOpen(JDevice *pDevice, JBYTE *pAddr)
{
  JBOOL bRet = FALSE;
  PacketType    PacketOut;
  PacketAckType PacketIn;
  JINT  i = 0;

  PacketInit(&PacketOut);
  
	PacketOut.bGroupId = CMD_GROUP_ID_BLE;
	PacketOut.bCmd     = CMD_BLE_OPEN;
	PacketOut.bPCode   = CMD_BLE_OPEN;
	PacketOut.wMOSILen = 24;
	PacketOut.wMISOLen = 8;  
  
	///--------------------------------------------------------///
	/// set mac for connection
	///--------------------------------------------------------///	
	for(i = 0; i < 6 ; i = i + 1)
	{	
		PacketOut.bCmdData[i] = (JBYTE) pAddr[i];
	}
                
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET][MBLE] BLE OPEN FAILED\r\n");
 		return bRet;
 	}

  if(PacketIn.bAck != 'A')
  {
	  DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET][MBLE] BLE OPEN COMMAND NACK\r\n");
    return FALSE;
  }  

  return bRet;
} 

JBOOL PacketCmdGatewayMBleClose(JDevice *pDevice)
{
  JINT i = 0;
  JBOOL bRet = FALSE;

  PacketType    PacketOut;
  PacketAckType PacketIn;

  PacketInit(&PacketOut);
  
	PacketOut.bGroupId = CMD_GROUP_ID_BLE;
	PacketOut.bCmd     = CMD_BLE_CLOSE;
	PacketOut.bPCode   = CMD_BLE_CLOSE;
	PacketOut.wMOSILen = 8;
	PacketOut.wMISOLen = 8;  

  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] Gateway BLE Close failed \r\n");
 		return bRet;
 	}

  return bRet;
}

JBOOL PacketCmdGatewaySBleVscModeStart(JDevice *pDevice)
{
  JINT i = 0;
  JBOOL bRet = FALSE;

  PacketType    PacketOut;
  PacketAckType PacketIn;

  PacketInit(&PacketOut);
  
	PacketOut.bGroupId = CMD_GROUP_ID_BLE;
	PacketOut.bCmd     = CMD_BLE_VSC_MODE_START;
	PacketOut.bPCode   = CMD_BLE_VSC_MODE_START;
	PacketOut.wMOSILen = 8;
	PacketOut.wMISOLen = 8;  

  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] GW  Ble VSC Mode Start fail\r\n");
 		return bRet;
 	}

  if(PacketIn.bAck != 'A')
  {
	  DBG_PRINTF(pDevice->dwDID, "[%06d] GW  Ble VSC Mode Start fail \r\n", pDevice->dwDID);
    return FALSE;
  }
    
  return bRet;
}

JBOOL PacketCmdGatewaySBleVscModeStop(JDevice *pDevice)
{
  JINT i = 0;
  JBOOL bRet = FALSE;

  PacketType    PacketOut;
  PacketAckType PacketIn;

  PacketInit(&PacketOut);
  
	PacketOut.bGroupId = CMD_GROUP_ID_BLE;
	PacketOut.bCmd     = CMD_BLE_VSC_MODE_STOP;
	PacketOut.bPCode   = CMD_BLE_VSC_MODE_STOP;
	PacketOut.wMOSILen = 8;
	PacketOut.wMISOLen = 8;  

  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] GW  Ble VSC Mode Start fail\r\n");
 		return bRet;
 	}

  if(PacketIn.bAck != 'A')
  {
	  DBG_PRINTF(pDevice->dwDID, "[%06d] GW  Ble VSC Mode Start fail \r\n", pDevice->dwDID);
    return FALSE;
  }

  return bRet;
}

JBOOL PacketCmdGatewaySBleVscModeQueueRead(JDevice *pDevice, JINT iReadCount, JINT iIDStart)
{
  JINT i = 0;
  JINT bRet = FALSE;
  JINT iSize = 0;
  
  JINT idx = 0;

  JINT iVscModeSize = (VSC_MODE_ITEM_INFO_SIZE + VSC_MODE_TYPE1_DATA_SIZE + 4);
  JINT iVscModeType = VSC_MODE_INFO_TYPE1;

	VscModeItemType *pItem = NULL;
  VscModeControlType * pVscMode = NULL;
  VscModeItemType vscModeType;

  PacketType    PacketOut;
  PacketAckType PacketIn;

  JINT iIdxTemp = 0;

  JINT iTimeOut = 400; //< defult is SOCK_CMD_TIMEOUT, here is 400 ms


  DBG_PRINTF(pDevice->dwDID,"\t [CMD][VSC] VSC_MODE_COUNT LEFT = %d, READ_COUNT = %d, ID-START = %d\r\n", pDevice->iVscModeDataCountLeft, iReadCount, iIDStart) ;
  iSize = (iReadCount * iVscModeSize) + sizeof(JINT);

  PacketInit(&PacketOut);    

  PacketOut.bGroupId = CMD_GROUP_ID_BLE;
  PacketOut.bCmd     = CMD_BLE_VSC_MODE_QUEUE_READ;
  PacketOut.bPCode   = CMD_BLE_VSC_MODE_QUEUE_READ;
  PacketOut.wMOSILen = 4 * sizeof(JINT);
  PacketOut.wMISOLen = iSize;  
  
  /// get count
  UtilMemcpy((JBYTE *)&PacketOut.bData[0], (JBYTE *)&iReadCount,  sizeof(JINT));
  UtilMemcpy((JBYTE *)&PacketOut.bData[4], (JBYTE *)&iIDStart,    sizeof(JINT));

  UtilMemcpy((JBYTE *)&PacketOut.bData[8], (JBYTE *)&iReadCount,  sizeof(JINT));
  UtilMemcpy((JBYTE *)&PacketOut.bData[12], (JBYTE *)&iIDStart,   sizeof(JINT));  
  
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, iTimeOut);	
  if(bRet == FALSE)
  {
    DBG_PRINTF(pDevice->dwDID,"\t [CMD][ERROR] VSC Mode read fail\r\n");
    return bRet;
  }

  if(PacketIn.bAck != 'A')
  {
    DBG_PRINTF(pDevice->dwDID, "\t [CMD][ERROR][%06d] VSC Mode NACK \r\n", pDevice->dwDID);
    return FALSE;
  }
  
  /// get back the queue data count
  pDevice->iVscModeDataCountLeft = PacketIn.bData[idx];
  DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] VSC MODE : READ_COUNT =  %d, DATA_LEN = %d bytes, LEFT_COUNT= %d\r\n",
                                  iReadCount, PacketIn.wDataLen,  pDevice->iVscModeDataCountLeft);

  idx = idx + sizeof(JINT);
  /// decode data
  for(i = 0; i < iReadCount; i = i + 1)
  {
    /// get the vsc mode queue data
    pItem = (VscModeItemType*)&PacketIn.bData[idx];
    vscModeType.wId  = pItem->wId;
    vscModeType.wLen = pItem->wLen;

    /// Info Data Copy
    UtilMemcpy((JBYTE *)&vscModeType.fInfo[0], (JBYTE *)&PacketIn.bData[idx + (iVscModeSize - VSC_MODE_ITEM_INFO_SIZE)], VSC_MODE_ITEM_INFO_SIZE);
    /// Ecg Data Copy
    UtilMemcpy((JBYTE *)&vscModeType.bData[0], (JBYTE *)&PacketIn.bData[idx + 4], iVscModeSize - VSC_MODE_ITEM_INFO_SIZE);

    DBG_PRINTF(pDevice->dwDID, "\t\t #%02d: VSC Mode ID READ = %-4d, START-IDX = %-4d\r\n", i, pItem->wId, iIDStart);
    
    /// check input index
    if(pItem->wId <= VSC_MODE_IDX_MAX)
    {
      iIdxTemp =  (pDevice->iVscModeIdxLast + 1) % VSC_MODE_IDX_MAX;
      if(iIdxTemp == pItem->wId)
      {
        pDevice->iVscModeIdxLast = pItem->wId;
      }
      else
      {
        DBG_PRINTF(pDevice->dwDID,"\t\t\t  [VSC] INVALID-IDX =  %-4d, START-IDX = %-4d\r\n", pItem->wId, iIDStart);
        continue;
      }
      
      /// vsc mode decode 
      VscModeDecodeEx(pDevice->dwDID, &vscModeType, iVscModeType);
      
      /// copy to main
      pVscMode = &pDevice->vscModeQueue[pDevice->iVscModeQueueTail];
      UtilMemcpy((JBYTE *)pVscMode, (JBYTE *)&VscModeCtl, sizeof(VscModeControlType));      

      pDevice->iVscModeQueueTail = (pDevice->iVscModeQueueTail + 1) % VSC_MODE_QUEUE_SIZE;      

      /// get the 200 ms count
      pDevice->iVscModeDataCount = pDevice->iVscModeDataCount + 1;
    }
    /// next data index
    idx = idx + iVscModeSize;
  }  
    
  return bRet;
}

JBOOL PacketCmdGatewaySBleVscFileModeStart(JDevice *pDevice, JINT dwTotalSec)
{
  JINT i = 0;
  JBOOL bRet = FALSE;

  PacketType    PacketOut;
  PacketAckType PacketIn;

  PacketInit(&PacketOut);
  
	PacketOut.bGroupId = CMD_GROUP_ID_BLE;
	PacketOut.bCmd     = CMD_BLE_VSC_FILE_MODE_START;
	PacketOut.bPCode   = CMD_BLE_VSC_FILE_MODE_START;
	PacketOut.wMOSILen = 24;
	PacketOut.wMISOLen = 8;  
  
  ///--------------------------------------------------------///
  /// Total time #1
  ///--------------------------------------------------------///
  UtilMemcpy(&PacketOut.bData[0], (JBYTE *)&dwTotalSec, 4);

  ///--------------------------------------------------------///
  /// Last fetch time #1
  ///--------------------------------------------------------///
  UtilMemcpy(&PacketOut.bData[4], (JBYTE *)&pDevice->pMeasItemActive->dwLastUpdateTime, 4);

  ///--------------------------------------------------------///
  /// Total time #2 (duplicated parameter)
  ///--------------------------------------------------------///
  UtilMemcpy(&PacketOut.bData[8], (JBYTE *)&dwTotalSec, 4);

  ///--------------------------------------------------------///
  /// Last fetch time #2 (duplicated parameter)
  ///--------------------------------------------------------///
  UtilMemcpy(&PacketOut.bData[12], (JBYTE *)&pDevice->pMeasItemActive->dwLastUpdateTime, 4);

  ///--------------------------------------------------------///
  /// start time #1
  ///--------------------------------------------------------///
  UtilMemcpy(&PacketOut.bCmdData[0], (JBYTE *)&pDevice->pMeasItemActive->dwStartTime, 4);

  ///--------------------------------------------------------///
  /// start time #2
  ///--------------------------------------------------------///
  UtilMemcpy(&PacketOut.bCmdData[4], (JBYTE *)&pDevice->pMeasItemActive->dwStartTime, 4);
  
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET][GATEWAY][MBLE] VSC-FILE-MODE Start failed \r\n");
 		return bRet;
 	}

  if(PacketIn.bAck != 'A')
  {
	  DBG_PRINTF(pDevice->dwDID, "%s", "[PACKET][GATEWAY][MBLE] VSC-FILE-MODE Start NACK \r\n");
    return FALSE;
  }

  if(pDevice->pMeasItemActive->dwLastUpdateTime > 0)
  {
    pDevice->iVscModeIdxLast = ((pDevice->pMeasItemActive->dwLastUpdateTime - pDevice->pMeasItemActive->dwStartTime) * 5);    
  }  
  
  return bRet;
}

JBOOL PacketCmdGatewaySBleVscFileModeStop(JDevice *pDevice)
{
  JINT i = 0;
  JBOOL bRet = FALSE;

  PacketType    PacketOut;
  PacketAckType PacketIn;

  PacketInit(&PacketOut);
  
	PacketOut.bGroupId = CMD_GROUP_ID_BLE;
	PacketOut.bCmd     = CMD_BLE_VSC_FILE_MODE_STOP;
	PacketOut.bPCode   = CMD_BLE_VSC_FILE_MODE_STOP;
	PacketOut.wMOSILen = 8;
	PacketOut.wMISOLen = 8;  

  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] Ble VSC-FILE-MODE Start fail\r\n");
 		return bRet;
 	}

  if(PacketIn.bAck != 'A')
  {
	  DBG_PRINTF(pDevice->dwDID, "[PACKET] [%06d] Ble VSC-FILE-MODE command NACK \r\n", pDevice->dwDID);
    return FALSE;
  }

  return bRet;
}

JBOOL PacketCmdGatewaySBleVscFileModeQueueRead(JDevice * pDevice, JDWORD dwUtcGet, JINT iMSGet, JINT iReadCount)
{
  JINT i = 0;
  JINT bRet = FALSE;
  JINT iSize = 0;
  
  JINT idx      = 0;
  
  JINT iVscModeSize = (VSC_MODE_ITEM_INFO_SIZE + VSC_MODE_TYPE1_DATA_SIZE + 4);
  JINT iVscModeType = VSC_MODE_INFO_TYPE1;

  JINT iIDLast = 0;

	VscModeItemType * pItem = NULL;
  VscModeControlType * pVscMode = NULL;
  VscModeItemType vscModeType;

  PacketType    PacketOut;
  PacketAckType PacketIn;

  JINT iIdxTemp = 0;

  JINT iTimeOut = SOCK_CMD_TIMEOUT; //< defult is SOCK_CMD_TIMEOUT, here is 400 ms

  static JINT iNotFoundCount = 0; ///< last id for vsc file mode queue read


  iIDLast = (pDevice->dwVscFileModeUtcGet - pDevice->dwVscFileModeUtcStart) * 5 + iMSGet / 200;
  if(iIDLast < 0)  
  {
    iIDLast  = 0;
  }
  
  iSize = (iReadCount * iVscModeSize) + sizeof(JINT);

  if(((iSize + 8) % 16) != 0)
  {
    iSize = iSize + (16 - ((iSize + 8) % 16)); ///< make it 16 byte aligned
  }
  
  PacketInit(&PacketOut);    

  PacketOut.bGroupId = CMD_GROUP_ID_BLE;
  PacketOut.bCmd     = CMD_BLE_VSC_FILE_MODE_QUEUE_READ;
  PacketOut.bPCode   = CMD_BLE_VSC_FILE_MODE_QUEUE_READ;
  PacketOut.wMOSILen = 10 * sizeof(JINT); ///< 40 + 8  = 48
  PacketOut.wMISOLen = iSize;             ///<  
  
  /// UTC-GET
  UtilMemcpy((JBYTE *)&PacketOut.bData[0],     (JBYTE *)&dwUtcGet,    sizeof(JINT));  
  UtilMemcpy((JBYTE *)&PacketOut.bData[4],     (JBYTE *)&dwUtcGet,    sizeof(JINT));  

  /// MS-GET
  UtilMemcpy((JBYTE *)&PacketOut.bData[8],     (JBYTE *)&iMSGet,      sizeof(JINT));  
  UtilMemcpy((JBYTE *)&PacketOut.bData[12],    (JBYTE *)&iMSGet,      sizeof(JINT));  
  
  /// READ COUNT
  UtilMemcpy((JBYTE *)&PacketOut.bCmdData[0],  (JBYTE *)&iReadCount,  sizeof(JINT));
  UtilMemcpy((JBYTE *)&PacketOut.bCmdData[4],  (JBYTE *)&iReadCount,  sizeof(JINT));

  /// UTC-START
  UtilMemcpy((JBYTE *)&PacketOut.bCmdData[8],  (JBYTE *)&pDevice->dwVscFileModeUtcStart,  sizeof(JDWORD));
  UtilMemcpy((JBYTE *)&PacketOut.bCmdData[12], (JBYTE *)&pDevice->dwVscFileModeUtcStart,  sizeof(JDWORD));

  /// DATA-COUNT
  UtilMemcpy((JBYTE *)&PacketOut.bCmdData[16], (JBYTE *)&pDevice->iVscModeDataCount ,  sizeof(JDWORD));
  UtilMemcpy((JBYTE *)&PacketOut.bCmdData[20], (JBYTE *)&pDevice->iVscModeDataCount ,  sizeof(JDWORD));
  
  
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, iTimeOut);	
  if(bRet == FALSE)
  {
    DBG_PRINTF(pDevice->dwDID,"\t [CMD][ERROR] VSC FILE MODE QUEUE READ fail\r\n");
    return bRet;
  }

  if(PacketIn.bAck != 'A')
  {
    DBG_PRINTF(pDevice->dwDID, "\t [CMD][ERROR][%06d] VSC FILE MODE QUEUE REA NACK \r\n", pDevice->dwDID);
    return FALSE;
  }
  
  /// get back the queue data count
  pDevice->iVscModeDataCountLeft = PacketIn.bData[idx];

  idx = idx + sizeof(JINT);
  /// decode data
  for(i = 0; i < iReadCount; i = i + 1)
  {
    /// get the vsc mode queue data
    pItem = (VscModeItemType*)&PacketIn.bData[idx];
    vscModeType.wId  = pItem->wId;
    vscModeType.wLen = pItem->wLen;

    /// Info Data Copy
    UtilMemcpy((JBYTE *)&vscModeType.fInfo[0], (JBYTE *)&PacketIn.bData[idx + (iVscModeSize - VSC_MODE_ITEM_INFO_SIZE)], VSC_MODE_ITEM_INFO_SIZE);
    /// Ecg Data Copy
    UtilMemcpy((JBYTE *)&vscModeType.bData[0], (JBYTE *)&PacketIn.bData[idx + 4], iVscModeSize - VSC_MODE_ITEM_INFO_SIZE);    

    if(pItem->wId == VSC_MODE_IDX_INVALID)
    {
      pDevice->iNotFoundCount = pDevice->iNotFoundCount + 1;
      continue;
    }
    
    /// check input index
    if(pItem->wId <= VSC_MODE_IDX_MAX)
    {
      pDevice->iNotFoundCount = 0;
      ///----------------------------------------------------------------///
      /// VSC FILE MODE TIME GET UPDATE
      ///----------------------------------------------------------------///      
      iIdxTemp = pDevice->iVscModeIdxLast  % VSC_MODE_IDX_MAX;      
      if(iIdxTemp == pItem->wId)
      {
        pDevice->iVscModeIdxLast = pDevice->iVscModeIdxLast + 1;
        iNotFoundCount = 0;
      }
      else
      {
        DBG_PRINTF(pDevice->dwDID,"\t\t\t  [VSC][FILE] INVALID-IDX =  %-4d, LAST-IDX = %-4d, IDX-TEMP = %d\r\n", pItem->wId, iIDLast, iIdxTemp);
        DBG_PRINTF(pDevice->dwDID,"\t\t\t  [VSC][FILE] DEVICE-LAST-IDX = %-4d\r\n",       pDevice->iVscModeIdxLast);
        
        ///----------------------------------------------------------------///
        /// VSC FILE MODE QUEUE CLEAR
        ///----------------------------------------------------------------///        
        iNotFoundCount = iNotFoundCount + 1;
        if(iNotFoundCount == 3)
        {
          DBG_PRINTF(pDevice->dwDID,"\t\t\t  [VSC][FILE] VSC FILE MODE QUEUE CLEAR!!!\r\n", pDevice->iVscModeIdxLast);
          iNotFoundCount = 0;
          PacketCmdGatewaySBleVscFileModeQueueClear(pDevice);
        }
        continue;
      } 
      
      /// update the start UTC 
      if(pDevice->iVscModeItemReadCount == 0)
      {
        pDevice->dwVscFileModeUtcStart =  *(JDWORD *)&vscModeType.fInfo[VSC_MODE_INFO_VSC_START_UTC];  
        pDevice->dwVscFileModeUtcGet   =  *(JDWORD *)&vscModeType.fInfo[VSC_MODE_INFO_TYPE0_TIME_UTC];  

        pDevice->pMeasItemActive->dwLastUpdateTime = pDevice->dwVscFileModeUtcGet;
      }
      
      /// update get time
      pDevice->iVscFileModeMSGet = (pDevice->iVscFileModeMSGet + 200);
      if(pDevice->iVscFileModeMSGet == 1000)
      {
        pDevice->iVscFileModeMSGet = 0;
        pDevice->dwVscFileModeUtcGet = pDevice->dwVscFileModeUtcGet + 1;
      }
      
      /// vsc mode decode 
      VscModeDecodeEx(pDevice->dwDID, &vscModeType, iVscModeType);
      
      /// copy to main
      pVscMode = &pDevice->vscModeQueue[pDevice->iVscModeQueueTail];
      UtilMemcpy((JBYTE *)pVscMode, (JBYTE *)&VscModeCtl, sizeof(VscModeControlType));      

      pDevice->iVscModeQueueTail = (pDevice->iVscModeQueueTail + 1) % VSC_MODE_QUEUE_SIZE;      

      /// get the 200 ms count
      pDevice->iVscModeDataCount = pDevice->iVscModeDataCount + 1;

      pDevice->iVscModeItemReadCount = pDevice->iVscModeItemReadCount + 1;
    }
    /// next data index
    idx = idx + iVscModeSize;
  }  
    
  return bRet;
}

JBOOL PacketCmdGatewaySBleVscFileModeQueueClear(JDevice * pDevice)
{
  JINT  iIdx = 0;
  JBOOL  bRet = FALSE;
  
	PacketType 			PacketOut;
	PacketAckType 	PacketIn;
  
  PacketInit(&PacketOut);

  JDWORD dwLastUtc = pDevice->pMeasItemActive->dwLastUpdateTime;
  JDWORD dwLastMS  = 0;
    

  ///--------------------------------------------------------------------///
  ///  Clear VSC File Mode Queue
  ///--------------------------------------------------------------------///
  PacketOut.bGroupId  = CMD_GROUP_ID_BLE;		
	PacketOut.bCmd      = CMD_BLE_VSC_FILE_MODE_QUEUE_CLEAR;
	PacketOut.wMOSILen  = 8;
	PacketOut.wMISOLen  = 0;
  
  
	UtilMemcpy((JBYTE *)&PacketOut.bData[0], (JBYTE *)&dwLastUtc, 4);
	UtilMemcpy((JBYTE *)&PacketOut.bData[4], (JBYTE *)&dwLastMS, 4);
  
  
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET][VSC][FILE] QUEUE CLEAR Failed\r\n");
 		return bRet;
 	}
  DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET][VSC][FILE] QUEUE CLEAR OK\r\n");
  return TRUE;
}

JBOOL PacketCmdGatewayDeviceEcgListGet(JDevice *pDevice, JINT iCnt, JINT iIdx)
{  
	PacketType 			PacketOut;
	PacketAckType 	PacketIn;

  JINT    iDid = pDevice->dwDID;

  JBOOL   bRet  = FALSE;
  JINT    iSize = 0;

  JINT    iIdxRead    = 0;
  JINT    iCountTotal = 0;
  JDWORD  dwUTcRead   = 0;

  PacketInit(&PacketOut);
  
  iSize = iCnt * sizeof(DeviceEcgType) + (sizeof(JINT) * 2);
  
  PacketOut.bPCode    = CMD_GW_DEVICE_ECG_LIST_GET;
  PacketOut.bGroupId  = CMD_GROUP_ID_GATEWAY;		
	PacketOut.bCmd      = CMD_GW_DEVICE_ECG_LIST_GET;
	PacketOut.wMOSILen  = 24;
	PacketOut.wMISOLen  = iCnt * sizeof(DeviceEcgType) + (sizeof(JINT) * 2);

  UtilMemcpy((JBYTE*)&PacketOut.bData[0],  (JBYTE *)&iIdx, 4);
  UtilMemcpy((JBYTE*)&PacketOut.bData[4],  (JBYTE *)&iCnt, 4);

  UtilMemcpy((JBYTE*)&PacketOut.bData[8],  (JBYTE *)&iIdx, 4);
  UtilMemcpy((JBYTE*)&PacketOut.bData[12], (JBYTE *)&iCnt, 4);
  
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
 		return bRet;
 	}

  ///----------------------------------------------------------------///
  /// Total Count Check
  ///----------------------------------------------------------------///
  UtilMemcpy((JBYTE*)&iCountTotal, (JBYTE *)&PacketIn.bData[4], 4);     
  if(iCountTotal > ECG_ITEM_LIST_TOTAL_CNT)
  {
    DBG_PRINTF(iDid, "\t [ERROR] ECG ITEM LIST READ ERROR,  TOTAL_COUNT = %d\r\n", iCountTotal);
    return FALSE;
  }

  ///----------------------------------------------------------------///
  /// UTC shall not be less than current time
  ///----------------------------------------------------------------///
  UtilMemcpy((JBYTE*)&dwUTcRead, (JBYTE *)&PacketIn.bData[0], 4);     
  if(dwUTcRead < pDevice->ecgObjectList.dwGatewayUtc)
  {
    DBG_PRINTF(iDid, "\t [ERROR] ECG ITEM  LIST READ ERROR,  UTC = %d\r\n", iCountTotal);
    return FALSE;
  }
  
  UtilMemcpy((JBYTE*)&pDevice->ecgObjectList.ecgObjectArr[iIdx],  &PacketIn.bData[8], iCnt * sizeof(DeviceEcgType));

  return TRUE;
}

JBOOL PacketCmdGatewayDeviceEcgListCountGet(JDevice *pDevice, JINT *piEcgListCount)
{  
	PacketType 			PacketOut;
	PacketAckType 	PacketIn;

  JINT    iDid = pDevice->dwDID;

  JBOOL   bRet  = FALSE;

  PacketInit(&PacketOut);
    
  PacketOut.bPCode    = CMD_GW_DEVICE_ECG_LIST_COUNT_GET;
  PacketOut.bGroupId  = CMD_GROUP_ID_GATEWAY;		
	PacketOut.bCmd      = CMD_GW_DEVICE_ECG_LIST_COUNT_GET;
	PacketOut.wMOSILen  = 8;
	PacketOut.wMISOLen  = 8;
    
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
 		return bRet;
 	}

  ///----------------------------------------------------------------///
  /// UTC shall not be less than current time
  ///----------------------------------------------------------------///
  UtilMemcpy((JBYTE *)piEcgListCount, (JBYTE *)&PacketIn.bData[0], 4);     
  
  return TRUE;
}

JBOOL PacketCmdGatewayBleConnectStatusGet(JDevice *pDevice, JINT *piBleConnectStatus, JINT *piBleOpenDid)
{  
  JINT iDid     = pDevice->dwDID;    
  JBOOL bRet    = FALSE;  
    
	PacketType 			PacketOut;
	PacketAckType 	PacketIn;

  JGatewayInfo * pGatewayInfoRead = NULL;

  PacketInit(&PacketOut);

  struct tm tm1 = {0};

	time_t  tNow = time(NULL);

  /// get the time zone 
  localtime_r(&tNow, &tm1); 
  
  PacketOut.bPCode    = CMD_GW_GATEWAY_BLE_CONNECT_STATUS_GET;
	PacketOut.bGroupId  = CMD_GROUP_ID_GATEWAY;		
	PacketOut.bCmd      = CMD_GW_GATEWAY_BLE_CONNECT_STATUS_GET;
	PacketOut.wMISOLen  = 24;    
	PacketOut.wMOSILen  = 8;
      
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
 		return bRet;
 	}  

  if(PacketIn.bAck != 'A')
  {
    DBG_PRINTF(iDid, "[%06d] Gateway BLE connect status get  : NACK \r\n", iDid);
    return FALSE;
  }

  UtilMemcpy((JBYTE *)piBleConnectStatus, &PacketIn.bData[0], 4);
  UtilMemcpy((JBYTE *)piBleOpenDid, &PacketIn.bData[4], 4);  
      
  return TRUE;
}

JBOOL PacketCmdGatewayVscModeStatusGet(JDevice *pDevice, JINT *piVscModeStatus, JDWORD *pdwVscModeStartUtc, JINT *piVscModeQueueCount)
{  
  JINT iDid     = pDevice->dwDID;    
  JBOOL bRet    = FALSE;  
    
	PacketType 			PacketOut;
	PacketAckType 	PacketIn;

  JGatewayInfo * pGatewayInfoRead = NULL;

  PacketInit(&PacketOut);

  struct tm tm1 = {0};

	time_t  tNow = time(NULL);

  /// get the time zone 
  localtime_r(&tNow, &tm1); 
  
  PacketOut.bPCode    = CMD_GW_GATEWAY_VSC_MODE_STATUS_GET;
	PacketOut.bGroupId  = CMD_GROUP_ID_GATEWAY;		
	PacketOut.bCmd      = CMD_GW_GATEWAY_VSC_MODE_STATUS_GET;
	PacketOut.wMISOLen  = 24;    
	PacketOut.wMOSILen  = 8;
      
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
 		return bRet;
 	}  

  if(PacketIn.bAck != 'A')
  {
    DBG_PRINTF(iDid, "[%06d] Gateway BLE connect status get  : NACK \r\n", iDid);
    return FALSE;
  }

  UtilMemcpy((JBYTE *)piVscModeStatus,      &PacketIn.bData[0], 4);
  UtilMemcpy((JBYTE *)pdwVscModeStartUtc,   &PacketIn.bData[4], 4);
  UtilMemcpy((JBYTE *)piVscModeQueueCount,  &PacketIn.bData[8], 4);
        
  return TRUE;
}
