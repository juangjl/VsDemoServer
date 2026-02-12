/**
 * @file LocationCmd.cpp 
 * @brief Packet command function for location
 * @version $Revision$
 * @author JL Juang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2025,  VitalSigns Technology Co., LTD., all rights reserved.
 * @note
*/
#include "Global.h"

///------------------------------------------------------------------------------------------------///
///
/// Packet Gateway Command
///
///------------------------------------------------------------------------------------------------///
JBOOL PacketCmdLocLocationGet(JDevice * pDevice)
{  
  JINT iDid     = pDevice->dwDID;    
  JBOOL bRet    = FALSE;  
    
	PacketType 			PacketOut;
	PacketAckType 	PacketIn;  

  PacketInit(&PacketOut);
  
  PacketOut.bPCode    = CMD_LOC_LOCATION_GET;
	PacketOut.bGroupId  = CMD_GROUP_ID_LOC;		
	PacketOut.bCmd      = CMD_LOC_LOCATION_GET;
	PacketOut.wMISOLen  = sizeof(LocationDataType);    
	PacketOut.wMOSILen  = 8; 
  
  bRet = PacketSend(pDevice, &PacketOut, &PacketIn, SOCK_CMD_TIMEOUT);	
	if(bRet == FALSE)
	{
 		return bRet;
 	}  

  UtilMemcpy((JBYTE *)&LocationData, &PacketIn.bData[0], sizeof(LocationDataType));  
      
  return TRUE;
}
