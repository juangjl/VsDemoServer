/**
 * @file PacketCmd.cpp 
 * @brief  Packet command function 
 * @version $Revision$
 * @author JL Juang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2022,  VitalSigns Technology Co., LTD., all rights reserved.
 * @note
*/
#include "Global.h"

///------------------------------------///
/// CMD: Device Info Read
///------------------------------------///
JBOOL PacketCmdSockDeviceInfoRead(JDevice *pDevice)
{
	JBOOL bRet 		= FALSE;		
  
	PacketType 			PacketOut;
	PacketAckType 	PacketIn;

  pDevice->dwDID = DBG_ID;
	
  PacketInit(&PacketOut);  
	
	///  SET OUTPUT MODE			
	PacketOut.bPCode    = 0;
	PacketOut.bGroupId  = CMD_GROUP_ID_SOCK;		
	PacketOut.bCmd      = CMD_SOCK_DEVICE_INFO;
	PacketOut.bPCode    = CMD_SOCK_DEVICE_INFO;
	PacketOut.wMOSILen  = 8;
	PacketOut.wMISOLen  = 24;
  
	bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
		DBG_PRINTF(DBG_ID, " [ERROR] Gateway info read error (DID = %d)\r\n", pDevice->dwDID);
		return bRet;
	}	
  if(PacketIn.bAck != 'A')
  {
	  DBG_PRINTF(DBG_ID, "[ERROR] [%07d] Gateway device info read error, NACK (DID = %d)\r\n", pDevice->dwDID, pDevice->dwDID);
    return FALSE;
  }

  pDevice->dwMID = *(JDWORD *) &PacketIn.bData[0];
	pDevice->dwPID = *(JDWORD *) &PacketIn.bData[4];
	pDevice->dwDID = *(JDWORD *) &PacketIn.bData[8];

	return bRet;
}

JBOOL PacketCmdSockDeviceTimeSet(JDevice *pDevice)
{
	JBOOL bRet 		= FALSE;		
  
	PacketType 			PacketOut;
	PacketAckType 	PacketIn;
	
  PacketInit(&PacketOut);  


	JTM jtm;
	time_t t = time(NULL);
	JTMGmtTimeGet(&jtm, t);	
  
	///  SET OUTPUT MODE			
	PacketOut.bPCode    = 0;
	PacketOut.bGroupId  = CMD_GROUP_ID_SYSTEM;		
	PacketOut.bCmd      = CMD_SYS_TIME_SET;
	PacketOut.bPCode    = CMD_SYS_TIME_SET;
	PacketOut.wMOSILen  = 8; ///< output data size = 8 + 8  = 16
	PacketOut.wMISOLen  = 8; ///< input data size  = 8 + 8  = 16

  UtilMemcpy(&PacketOut.bData[0], (JBYTE*)&jtm.t, 4);

	bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
		DBG_PRINTF(DBG_ID, " [ERROT] Device Time set error (DID = %d)\r\n", pDevice->dwDID);
		return bRet;
	}	

  if(PacketIn.bAck != 'A')
  {
	  DBG_PRINTF(DBG_ID, "[ERROR] [%07d] Gateway device Time Set, NACK (DID = %d)\r\n", pDevice->dwDID, pDevice->dwDID);
    return FALSE;
  }

	return bRet;
}

JBOOL PacketMBleCmdFileListRead(JDevice *pDevice)
{  
  JBOOL  bRet     = FALSE;
  JINT   iReadLen = 0;
  JINT   iCnt     = 0;
  PacketType    PacketOut;
  PacketAckType PacketIn;
  
  PacketInit(&PacketOut);
  
  iCnt = JFILE_LIST_ITEM_MAX_SIZE;
  
  /// output packet set
  PacketOut.bGroupId 	= CMD_GROUP_ID_JFILE;
  PacketOut.bCmd 			= CMD_VSC_FILE_LIST_READ;
  PacketOut.bPCode 		= CMD_VSC_FILE_LIST_READ;
  PacketOut.wMOSILen 	= 16;
  PacketOut.wMISOLen 	= 176;  

  bRet = SBlePacketSendEx(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	

	if(bRet == FALSE)
	{
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] GW Ble Open fail\r\n");
 		return bRet;
 	}

  // if(PacketIn.bAck != 'A')
  // {
	//   DBG_PRINTF(pDevice->dwDID, "[%06d] [BLE][DEVICE][FILE LIST] NACK\r\n", pDevice->dwDID);
  //   return FALSE;
  // }

  return bRet;
}

JBOOL PacketMBleCmdSRegRead(JDevice *pDevice, SRegType *pSReg)
{
  JBOOL  bRet      = FALSE;
  
  PacketType    PacketOut;
  PacketAckType PacketIn;
  
  PacketInit(&PacketOut);
  
  /// output packet set
  PacketOut.bGroupId 	= CMD_GROUP_ID_SREG;
  PacketOut.bCmd 			= CMD_SREG_READ;
  PacketOut.bPCode 		= CMD_SREG_READ;
  PacketOut.wMOSILen 	= SREG_NAME_SIZE;
  PacketOut.wMISOLen 	= SREG_DATA_SIZE;

  /// SReg Data Clear
  UtilMemset((JBYTE *) &pSReg->bData[0], 0x00, SREG_DATA_SIZE);
	
  /// SReg Name Set
  UtilMemcpy((JBYTE *) &PacketOut.bData[0],(JBYTE *)pSReg, SREG_NAME_SIZE);


  bRet = SBlePacketSendEx(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	

	if(bRet == FALSE)
	{
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] GW Ble Open fail\r\n");
 		return bRet;
 	}

  if(PacketIn.bAck != 'A')
  {
	  DBG_PRINTF(pDevice->dwDID, "[%06d] [BLE][DEVICE] SREG FAIL command fail %s\r\n", pDevice->dwDID, pSReg->strName);
    return FALSE;
  }
  
  /// SReg Data Set
  UtilMemcpy((JBYTE *) &pSReg->bData[0],(JBYTE *)&PacketIn.bData[0], SREG_DATA_SIZE);

  return bRet;
}

JBOOL PacketMBleCmdSRegWrite(JDevice *pDevice, SRegType *pSReg)
{
  JBOOL  bRet      = FALSE;
  
  PacketType    PacketOut;
  PacketAckType PacketIn;
   
  PacketInit(&PacketOut);
  
  /// output packet set
  PacketOut.bGroupId 	= CMD_GROUP_ID_SREG;
  PacketOut.bCmd 			= CMD_SREG_WRITE;
  PacketOut.bPCode 		= CMD_SREG_WRITE;
  PacketOut.wMOSILen 	= SREG_DATA_SIZE;
  PacketOut.wMISOLen 	= 8;

  /// SReg Name and Data Set
  UtilMemcpy((JBYTE *) &PacketOut.bData[0], (JBYTE *)pSReg, SREG_DATA_SIZE);
      
  bRet = SBlePacketSendEx(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] GW Ble Open fail\r\n");
 		return bRet;
 	}

  if(PacketIn.bAck != 'A')
  {
	  DBG_PRINTF(pDevice->dwDID, "[%06d] [BLE][DEVICE] SREG WRITE FAIL command fail %s\r\n", pDevice->dwDID, pSReg->strName);
    return FALSE;
  }
  DBG_PRINTF(pDevice->dwDID, "[%06d] [BLE][DEVICE] SREG WRITE OK, %d\r\n", pDevice->dwDID, bRet);
  
  return bRet;
}
