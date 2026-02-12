/**
 * @file Packet.cpp 
 * @brief Packet function 
 * @version $Revision$
 * @author JL Juang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2022,  VitalSigns Technology Co., LTD., all rights reserved.
 * @note
*/
#include "Global.h"

void PacketInit(PacketType *pPacket)
{
	JINT i = 0;
	JBYTE * pbData = (JBYTE *)pPacket;

	for(i = 0; i < sizeof(PacketType) ; i = i + 1)
	{
		pbData[i] = 0;
	}  
}

void PacketAckInit(PacketAckType *pPacketAck)
{
	JINT  i = 0;
	JBYTE * pbData = (JBYTE *)pPacketAck;
  
	for(i = 0 ; i < sizeof(PacketAckType) ; i = i + 1)
	{
		pbData[i] = 0;
	}  
}

void PacketAckCheckSumSet(PacketType *pPacketAck, JINT wLen)
{
	JINT 	i = 0;

	JBYTE bCheckSum = 0x00;
	JBYTE * pbData  = (JBYTE *)pPacketAck;
	JWORD wWrtLen   = wLen;  

	pPacketAck->bCheckSum = 0;
  pPacketAck->bPCode = pPacketAck->bCmd;
	
  for(i = 0 ; i < wWrtLen; i = i + 1)
	{			
		if(i == 3)
		{
			continue;
		}
		bCheckSum = bCheckSum + pbData[i];
	}
	
	pPacketAck->bCheckSum = bCheckSum;	
}

JBYTE PacketAckCheckSumCheck(JDevice *pDevice, PacketAckType *pPacketAck, JINT size, JBYTE bChkSum)
{
  char msg[256];
  JINT  i = 0;
  JBYTE bCheckSum = 0x00;  

  pPacketAck->bChkSum = 0;
  JBYTE *pbData = (JBYTE *)pPacketAck;
  
  for(i = 0 ; i < size; i = i + 1)
  {
    bCheckSum += pbData[i];
  }
  pPacketAck->bChkSum = bChkSum;

  if(bCheckSum != bChkSum)
  {
    sprintf(msg, "[PACKET][ERROR][!!!!!!!!] CHECK_SUM_CAL = 0x%02X, CHECK_SUM_SET = 0x%02X\r\n", bCheckSum, bChkSum);
    DBG_PRINTF(pDevice->dwDID, msg); 
    return FALSE;
  }
  else
  {
    return TRUE;
  }  
}

JBOOL  PacketSend(JDevice *pDevice, PacketType * pPacketOut, PacketAckType * pPacketIn, JINT iTimeout)
{
	JINT  errNo          = 0;
	JINT  iWrtLen        = 0;
  JINT  iReadLen       = 0;
	JINT  iRdLen         = 0;  /// slave back read len
  JINT  iRet           = 0;
	JBOOL bRet           = FALSE;
  JINT  iLenExp     = 0;
  JINT  iDislayCount   = 0;
  JINT  iRetryCountNow = 0;  
 
	iWrtLen  = pPacketOut->wMOSILen + PACKET_PARA_HEADER_SIZE + PACKET_PARA_BDATA_SIZE;
	iLenExp = PACKET_ACK_HEADER_SIZE + pPacketOut->wMISOLen;
  iReadLen = iLenExp;

  if(iWrtLen > PACKET_CMD_DATA_LEN)
  {
    iWrtLen = PACKET_ACK_DATA_LEN;
  }

  if(iReadLen > PACKET_ACK_DATA_LEN)
  {
    iReadLen = PACKET_ACK_DATA_LEN;
  }

  PacketAckCheckSumSet(pPacketOut, iWrtLen);  

  while(1)
  {    
    PacketAckInit(pPacketIn);
    
    iRet  = SockWriteCmd(pDevice->dwDID, &pDevice->iDevSocket, (JBYTE *) pPacketOut,  iWrtLen, (JBYTE *)pPacketIn,  iReadLen,  iTimeout);            

    UtilMsSleep(10);
    
    if(iRet > 0)
    {
      bRet =  TRUE;
    }
    else if(iRet == SOCKET_CMD_READ_TIMEOUT)
    {
      bRet =  FALSE;
      break;
    }
    else if(iRet == SOCKET_CMD_CONNECTION_CLOSED)
    {
      bRet =  FALSE;
      pDevice->iDevStatus |= STATUS_USER_CLOSE;
      DBG_PRINTF(pDevice->dwDID,  "\t\t\t [PACKET][!!!!!] SOCKET CLOSED BY PEER \r\n");
      break;
    }
    else if(iRet == SOCKET_CMD_NACK)
    {
      bRet =  FALSE;
      DBG_PRINTF(pDevice->dwDID,  "\t\t\t [PACKET][!!!!!] PACKET  NACK: EXPECTED = %d, READ = %d\r\n", iRdLen, iReadLen);                  
    }      
    else if(iRet == SOCKET_CMD_ERROR)
    {
      bRet =  FALSE;
      pDevice->iDevStatus |= STATUS_SOCKET_FAIL;
      DBG_PRINTF(pDevice->dwDID,  "\t\t\t [PACKET][!!!!!] PACKET  Command ERROR: EXPECTED = %d, READ = %d\r\n", iRdLen, iReadLen);                  
      return bRet;
    }

    if(bRet == TRUE)
    {
      iRdLen = PACKET_ACK_HEADER_SIZE + pPacketIn->wDataLen; 
      
      //----------------------------------------------------------------//
      // Check  return size  <= PACKET_ACK_DATA_LEN
      //----------------------------------------------------------------//
      if(iRdLen > PACKET_ACK_DATA_LEN)
      {
        bRet = FALSE;        

        DBG_PRINTF(pDevice->dwDID,  "\t\t\t [PACKET][!!!!!] PACKET SIZE > %d\r\n", iRdLen);      

        if(iRdLen > 64)
        {
          iDislayCount = 64;
        }
        else
        {
          iDislayCount = iRdLen;
        }
        UtilDataPrint(pDevice->dwDID, (JBYTE *) pPacketIn, iDislayCount);        
      }
      else if(iRdLen > iReadLen)
      {
        bRet =  FALSE;
        DBG_PRINTF(pDevice->dwDID,  "\t\t\t [PACKET][!!!!!]READ_LEN (%d) !=  EXPECTED_LEN (%d)\r\n", iRdLen, iReadLen);      
      }
      //----------------------------------------------------------------//
      // Check return size  = expected size
      //----------------------------------------------------------------//
      else if(iRdLen != iReadLen)
      {
        bRet = FALSE;    

        DBG_PRINTF(pDevice->dwDID,  "\t\t\t [PACKET][!!!!!] PACKET SIZE (%d) !=  EXPECTED (%d)\r\n", iRdLen, iReadLen);
        if(iRdLen > 64)
        {
          iDislayCount = 64;
        }
        else
        {
          iDislayCount = iRdLen;
        }
        UtilDataPrint(pDevice->dwDID, (JBYTE *) pPacketIn, iDislayCount);
      }

      if(bRet == TRUE)
      {
        bRet = PacketAckCheckSumCheck(pDevice, pPacketIn, iRdLen, pPacketIn->bChkSum);      
        if(bRet == TRUE)
        {
          break;
        }
      }
    }
    
    //----------------------------------------------------------------//
    // Send Fail --> Retry
    //----------------------------------------------------------------//
    if(bRet == FALSE)
    {         
      ///----------------------------------------------------------------///
      /// Retry
      ///----------------------------------------------------------------///
      iRetryCountNow = iRetryCountNow + 1;
      DBG_PRINTF(pDevice->dwDID,  "\t\t\t [PACKET][!!!!!] SEND FAILED RETRY :  %d / %d\r\n", iRetryCountNow, PACKET_SEND_RETRY_COUNT);      

      if(iRetryCountNow >= PACKET_SEND_RETRY_COUNT)
      {
        break;
      }            
    }    
  }  

  return bRet;
}

JBOOL SBlePacketSendEx(JDevice *pDevice, PacketType * pPacketOut, PacketAckType * pPacketIn, JINT iTimeout)
{
  PacketType    	PacketOut;
  PacketAckType   PacketIn;
 	JBOOL bRet 		 = FALSE;		
  JINT iWriteLen = 0;
		
  iWriteLen = pPacketOut->wMOSILen + PACKET_PARA_HEADER_SIZE + PACKET_PARA_BDATA_SIZE;
  PacketAckCheckSumSet(pPacketOut, iWriteLen);
  PacketInit(&PacketOut);
  PacketOut.bGroupId = (JBYTE)CMD_GROUP_ID_BLE;
  PacketOut.bCmd     = (JBYTE)CMD_BLE_PACKET_WRITE;
  PacketOut.bPCode   = (JBYTE)CMD_BLE_PACKET_WRITE;

  PacketOut.wMOSILen = pPacketOut->wMOSILen + PACKET_CMD_BDATA_SIZE + PACKET_ACK_HEADER_SIZE ;
  PacketOut.wMISOLen = pPacketOut->wMISOLen + PACKET_ACK_HEADER_SIZE;

  memcpy(&PacketOut.bCmdData[0], pPacketOut, iWriteLen);

  
  /// uart write cmd
	bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);
	if(bRet == FALSE)
	{
		DBG_PRINTF(pDevice->dwDID, " [SBLE] Device info read error (DID = %d)\r\n", pDevice->dwDID);
		return bRet;
	}	
  if(PacketIn.bAck != 'A')
  {
	  DBG_PRINTF(pDevice->dwDID, "[SBLE] [%07d] Device info read error, NACK (DID = %d)\r\n", pDevice->dwDID, pDevice->dwDID);
    return FALSE;
  }

  DBG_PRINTF(pDevice->dwDID, "[SBLE][SEND]LEN:%d G:%X, CMD:%X ACK:%c CHK:%X\r\n", 
    PacketIn.wDataLen, PacketIn.bGroupId, PacketIn.bPCode, PacketIn.bAck, PacketIn.bChkSum);
  
  return bRet;
}