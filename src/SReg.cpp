/**
 * @file SReg.cpp 
 * @brief  Software Register (SREG)
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2025, VitalSigns Technology Co., Ltd., all rights reserved.
 * @note
*/
#include "Global.h"

JBOOL CmdPacketGatewaySRegWrite(JDevice *pDevice, CmdObjType *pCmdObj)
{
  JBOOL  bRet = FALSE;
  SRegType      sreg;
  SRegType      * pSReg = &sreg;
  PacketType    PacketOut;
  PacketAckType PacketIn;
	JINT iLen = 0;
     
	PacketInit(&PacketOut);
  
  /// output packet set
  PacketOut.bGroupId 	= CMD_GROUP_ID_SREG;
  PacketOut.bCmd 			= CMD_SREG_WRITE;
  PacketOut.bPCode 		= CMD_SREG_WRITE;
  PacketOut.wMOSILen 	= SREG_DATA_SIZE + 8; ///< 8 + 64  + 8 = 80
  PacketOut.wMISOLen 	= 8;                  ///< 8 + 8 = 16 

  /// Clear SREG
  UtilMemset((JBYTE *) &pSReg->strName[0], 0x00, SREG_SIZE);
  UtilMemset((JBYTE *) &pSReg->bData[0],   0x00, SREG_DATA_SIZE);
 
  /// SReg Name Set
	sprintf(&pSReg->strName[0], "%s", pCmdObj->strArg[0]);

	/// VALUE
	iLen = UtilHexStringToBytes(pCmdObj->strArg[1], &pSReg->bData[0]);

  /// SReg Name and Data Set
  UtilMemcpy((JBYTE *) &PacketOut.bData[0], (JBYTE *)pSReg, SREG_SIZE);
      
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
		pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
		sprintf(pCmdObj->strResult, "NACK");
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET][!!!!!!] SREG WRITE failed to send fail\r\n");
 		return bRet;
 	}

  if(PacketIn.bAck != 'A')
  {
		pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
		sprintf(pCmdObj->strResult, "NACK");
	  DBG_PRINTF(pDevice->dwDID, "\t\t [PACKET][!!!!!!] SREG WRITE NACK fail DID = %d\r\n", pDevice->dwDID);
    return FALSE;
  }
  
	pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
	sprintf(pCmdObj->strResult, "ACK");

  return bRet;
}

JBOOL CmdPacketGatewaySRegRead(JDevice *pDevice, CmdObjType *pCmdObj)
{
  JBOOL  bRet      = FALSE;
  
  SRegType      sreg;
  SRegType      *pSReg;
  PacketType    PacketOut;
  PacketAckType PacketIn;
 	JINT i;
	char *pData;

  pSReg = &sreg;
  PacketInit(&PacketOut);

  /// output packet set
  PacketOut.bGroupId 	= CMD_GROUP_ID_SREG;
  PacketOut.bCmd 			= CMD_SREG_READ;
  PacketOut.bPCode 		= CMD_SREG_READ;
  PacketOut.wMOSILen 	= SREG_NAME_SIZE + 8; ///< output data size = 32 + 8  +8 = 48 
  PacketOut.wMISOLen 	= SREG_NAME_SIZE + 8; ///< input  data size = 32 + 8 + 8 = 48

  /// Clear SREG
  UtilMemset((JBYTE *) &pSReg->strName[0], 0x00, SREG_SIZE);
 
  /// SReg Name Set
	sprintf(&pSReg->strName[0], "%s", pCmdObj->strArg[0]);

  /// SReg Name and Data Set
  UtilMemcpy((JBYTE *) &PacketOut.bData[0], (JBYTE *)pSReg, SREG_NAME_SIZE);

  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
  	sprintf(pCmdObj->strResult, "NACK");
    pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
	  DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET][!!!!!!] SREG READ send failed\r\n");
 		return bRet;
 	}

  if(PacketIn.bAck != 'A')
  {
  	sprintf(pCmdObj->strResult, "NACK");
    pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
	  DBG_PRINTF(pDevice->dwDID, "[%06d] SREG READ NACK fail %s\r\n", pDevice->dwDID, pCmdObj->strArg);
    return FALSE;
  }
	
	sprintf(pCmdObj->strResult, "ACK");

  pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
	for(i = 0 ; i < SREG_NAME_SIZE ; i = i + 1)
	{
		sprintf(&pCmdObj->strData[i*2], "%02X", PacketIn.bData[i]);
	}
	
  return bRet;
}

JBOOL CmdPacketDeviceSRegWrite(JDevice *pDevice, CmdObjType *pCmdObj)
{
  JBOOL  bRet      = FALSE;
  SRegType      sreg;
  SRegType      * pSReg;
  PacketType    PacketOut;
  PacketAckType PacketIn;
	JINT iLen = 0;
  pSReg = &sreg;
	PacketInit(&PacketOut);

  //// first check connection
  if(GatewayMBleConnectCheck(pDevice) == FALSE)
  {  	
    sprintf(pCmdObj->strResult, "NACK");
    pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] GW Ble Open fail\r\n");
    return FALSE;
  }
  
  /// output packet set
  PacketOut.bGroupId 	= CMD_GROUP_ID_SREG;
  PacketOut.bCmd 			= CMD_SREG_WRITE;
  PacketOut.bPCode 		= CMD_SREG_WRITE;
  PacketOut.wMOSILen 	= SREG_DATA_SIZE;
  PacketOut.wMISOLen 	= 16;

  /// Clear SREG
  UtilMemset((JBYTE *) &pSReg->strName[0], 0x00, SREG_SIZE);
  UtilMemset((JBYTE *) &pSReg->bData[0],   0x00, SREG_DATA_SIZE);
 
  /// SReg Name Set
	sprintf(&pSReg->strName[0], "%s", pCmdObj->strArg[0]);

	/// VALUE
	iLen = UtilHexStringToBytes(pCmdObj->strArg[1], &pSReg->bData[0]);

  /// SReg Name and Data Set
  UtilMemcpy((JBYTE *) &PacketOut.bData[0], (JBYTE *)pSReg, SREG_SIZE);     

  bRet = SBlePacketSendEx(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
  DBG_PRINTF(pDevice->dwDID,"\t\t [CMD][BLE][DEVICE] SREG WRITE %d, P:0x%X, Ack:0x%X, Chk:0x%X, Idx:%d, Len:%d\r\n", bRet,
  PacketIn.bPCode, PacketIn.bAck, PacketIn.bChkSum, PacketIn.wDataIdx, PacketIn.wDataLen);

	if(bRet == FALSE)
	{
  	sprintf(pCmdObj->strResult, "NACK");
    pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] GW Ble Open fail\r\n");
 		return bRet;
 	}
 
  return bRet;
}

JBOOL CmdPacketDeviceSRegRead(JDevice *pDevice, CmdObjType *pCmdObj)
{
  JBOOL  bRet   = FALSE;
  SRegType     	sreg;
  SRegType      * pSReg  = &sreg;
  JINT i = 0;  

  /// SReg Data Clear
  UtilMemset((JBYTE *) &pSReg->strName[0], 0x00, sizeof(SRegType));

  /// SReg Name Set
	sprintf((char*)&pSReg->strName[0], "%s", pCmdObj->strArg[0]);

	bRet = PacketMBleCmdSRegRead(pDevice, pSReg);

  return bRet;
}
