/**
 * @file SystemCmd.cpp 
 * @brief Packet command function 
 * @version $Revision$
 * @author JL Juang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2022,  VitalSigns Technology Co., LTD., all rights reserved.
 * @note
*/
#include "Global.h"

JBOOL PacketCmdSystemVersionGet(JDevice *pDevice)
{  
  JINT iDid     = pDevice->dwDID;    
  JBOOL bRet    = FALSE;  
    
	PacketType 			PacketOut;
	PacketAckType 	PacketIn;

  JGatewayInfo * pGatewayInfoRead = NULL;

  JDWORD dwVersion;
  char strDeviceName[16];
  JINT iOnlineType = 0;

  PacketInit(&PacketOut);

  struct tm tm1 = {0};

	time_t  tNow = time(NULL);

  /// get the time zone 
  localtime_r(&tNow, &tm1); 
  
  PacketOut.bPCode    = CMD_SYS_VER;
	PacketOut.bGroupId  = CMD_GROUP_ID_SYSTEM;		
	PacketOut.bCmd      = CMD_SYS_VER;
	PacketOut.wMISOLen  = 24; ///< 8 + 24 = 32    
	PacketOut.wMOSILen  = 8;
      
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
 		return bRet;
 	}

  if(PacketIn.bPCode != CMD_SYS_VER)
  {
    DBG_PRINTF(iDid, "\t [CMD][SYS] PCODE ERROR \r\n");
    return FALSE;
  }
  if(PacketIn.bGroupId != CMD_GROUP_ID_SYSTEM)
  {
    DBG_PRINTF(iDid, "\t [CMD][SYS] GROUP ERROR \r\n");
    return FALSE;
  }

  UtilMemcpy((JBYTE *)&dwVersion,        &PacketIn.bData[0], 4);  
  UtilMemcpy((JBYTE *)&strDeviceName[0], &PacketIn.bData[4], 16);  
  UtilMemcpy((JBYTE *)&iOnlineType,      &PacketIn.bData[20], 4);

  pDevice->iSystemVersion = dwVersion;
  pDevice->iOnlineType    = iOnlineType;
     
  DBG_PRINTF(iDid, "\t\t [SYS][VERSION] Version       = %d\r\n", dwVersion);
  DBG_PRINTF(iDid, "\t\t [SYS][VERSION] Device name   = %s\r\n", strDeviceName);

  if(iOnlineType == SYS_OFFLINE)
  {
    DBG_PRINTF(iDid, "\t\t [SYS][VERSION] Online-type   = %s\r\n", "SYSTEM-OFFLINE");
  }
  else if(iOnlineType == SYS_ONLINE_WIFI)
  {
    DBG_PRINTF(iDid, "\t\t [SYS][VERSION] Online-type   = %s\r\n", "SYSTEM-ONLINE-WIFI");
  }
  else if(iOnlineType == SYS_ONLINE_LTE)
  {
    DBG_PRINTF(iDid, "\t\t [SYS][VERSION] Online-type   = %s\r\n", "SYSTEM-ONLINE-LTE");
  }  
      
  return TRUE;
}
