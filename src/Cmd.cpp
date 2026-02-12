/**
 * @file Cmd.cpp 
 * @brief  Server Command function 
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology Co., Ltd., all rights reserved.
 * @note
*/
#include "Global.h"

///----------------------------------------------------------------///
/// Gateway Object
///----------------------------------------------------------------///
CmdObjType CmdObject;
CmdObjType * pCmdObjResult = &CmdObject;

///----------------------------------------------------------------///
/// Gateway Object
///----------------------------------------------------------------///
void CmdActBleConnectCheck(JDevice * pDevice, CmdObjType *pCmdObj)
{
  if(GatewayMBleConnectCheck(pDevice) == TRUE)
	{
	  pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
		sprintf(pCmdObj->strResult, GATEWAY_CMD_ACK);
		UtilMemcpy((JBYTE*)pCmdObjResult,(JBYTE*) pCmdObj, sizeof(CmdObjType));
	}
}

void CmdActBleSRegDataRead(JDevice * pDevice, CmdObjType *pCmdObj)
{
	SRegType *pSReg;
	SRegType sReg;
	JBOOL bRet = FALSE;
	JINT i 		 = 0;
	
	//// first check connection
  if(GatewayMBleConnectCheck(pDevice) == FALSE)
  {  	
    sprintf(pCmdObj->strResult, GATEWAY_CMD_NACK);
    pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] GW Ble Open fail\r\n");
    return;
  }

	pSReg = &sReg;
	bRet = PacketCmdGatewayMBleSRegCheck(pDevice, pSReg);
	if(bRet == FALSE)
	{
  	sprintf(pCmdObj->strResult, GATEWAY_CMD_NACK);
	}
	else
	{
  	sprintf(pCmdObj->strResult, GATEWAY_CMD_ACK);
		pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
		for(i = 0 ; i < SREG_DATA_SIZE ; i = i + 1)
		{
			sprintf(&pCmdObj->strData[i*2], "%02X", pSReg->bData[i]);
		}
	}
}

void CmdActBleSRegDataWrite(JDevice * pDevice, CmdObjType *pCmdObj)
{

	SRegType *pSReg;
	SRegType sReg;
	JBOOL bRet = FALSE;
	JINT i 		 = 0;

  //// first check connection
  if(GatewayMBleConnectCheck(pDevice) == FALSE)
  {  	
    sprintf(pCmdObj->strResult, GATEWAY_CMD_NACK);
    pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
    DBG_PRINTF(pDevice->dwDID,"\t\t [PACKET] GW Ble Open fail\r\n");
    return;
  }

	pSReg = &sReg;
	bRet = PacketCmdGatewayMBleSRegCheck(pDevice, pSReg);
  
  if(bRet == FALSE)
	{
  	sprintf(pCmdObj->strResult, GATEWAY_CMD_NACK);
	}
	else
	{
  	sprintf(pCmdObj->strResult, GATEWAY_CMD_ACK);
		pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
	}
}

void CmdActBleOpen(JDevice * pDevice, CmdObjType *pCmdObj)
{
  if(GatewayMBleOpen(pDevice, pCmdObj->iDeviceDid) == TRUE)
	{
	  pCmdObj->iStatus = GATEWAY_CMD_EXECUTING;
		sprintf(pCmdObj->strResult, GATEWAY_CMD_ACK);
		/// check the ble status after data then copy the command
		UtilMemcpy((JBYTE*)pCmdObjResult,(JBYTE*) pCmdObj, sizeof(CmdObjType));
    DBG_PRINTF(pDevice->dwDID, "\t\t [CMD] %s TRUE\r\n", pCmdObj->strCmdName);
	}
	else
	{
	  pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
		sprintf(pCmdObj->strResult, GATEWAY_CMD_NACK);
		/// check the ble status after data then copy the command
		UtilMemcpy((JBYTE*)pCmdObjResult,(JBYTE*) pCmdObj, sizeof(CmdObjType));
    DBG_PRINTF(pDevice->dwDID, "\t\t [CMD] %s FALSE\r\n", pCmdObj->strCmdName);
	}
}

void CmdActBleClose(JDevice * pDevice, CmdObjType *pCmdObj)
{
  if(GatewayMBleClose(pDevice) == TRUE)
	{
	  pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
		sprintf(pCmdObj->strResult, GATEWAY_CMD_ACK);
		/// check the ble status after data then copy the command
		UtilMemcpy((JBYTE*)pCmdObjResult,(JBYTE*) pCmdObj, sizeof(CmdObjType));
	}
	DBG_PRINTF(pDevice->dwDID, "\t\t [CMD] %s\r\n", pCmdObj->strCmdName);
}

void CmdExecutionControl(JDevice * pDevice, CmdObjType *pCmdObj)
{
  DBG_PRINTF(pDevice->dwDID, "\t\t [CMD] %s\r\n", pCmdObj->strCmdName);

	if(strcmp(&pCmdObj->strCmdName[0], GATEWAY_CMD_DEVICE_BLE_OPEN) == 0)  
  {
    CmdActBleConnectCheck(pDevice, pCmdObj);
    return;
  }
  if(strcmp(&pCmdObj->strCmdName[0], GATEWAY_CMD_DEVICE_SREG_WRITE) == 0)  
  {
		CmdActBleSRegDataWrite(pDevice, pCmdObj);
		return;
	}
  if(strcmp(&pCmdObj->strCmdName[0], GATEWAY_CMD_DEVICE_SREG_READ) == 0)  
  {
		CmdActBleSRegDataRead(pDevice, pCmdObj);
		return;
	}
}

void CmdDispatch(JDevice * pDevice, CmdObjType *pCmdObj)
{
  DBG_PRINTF(pDevice->dwDID, "\t\t [CMD]CMD DISPATCH : %s\r\n", pCmdObj->strCmdName);

  if(strcmp(&pCmdObj->strCmdName[0], GATEWAY_CMD_DEVICE_BLE_OPEN) == 0)  
  {
    CmdActBleOpen(pDevice, pCmdObj);
    return;
  }
  if(strcmp(&pCmdObj->strCmdName[0], GATEWAY_CMD_DEVICE_BLE_CLOSE) == 0)  
  {
    CmdActBleClose(pDevice, pCmdObj);
    return;
  }
  if(strcmp(&pCmdObj->strCmdName[0], GATEWAY_CMD_SREG_WRITE) == 0)  
  {
	  CmdPacketGatewaySRegWrite(pDevice, pCmdObj);
		return;
  }
  if(strcmp(&pCmdObj->strCmdName[0], GATEWAY_CMD_SREG_READ) == 0)  
  {
		CmdPacketGatewaySRegRead(pDevice, pCmdObj);
    return;
  }
  if(strcmp(&pCmdObj->strCmdName[0], GATEWAY_CMD_DEVICE_SREG_WRITE) == 0)  
  {
	  pCmdObj->iStatus = GATEWAY_CMD_EXECUTING;
  	CmdPacketDeviceSRegWrite(pDevice, pCmdObj);
    return;
  }
  if(strcmp(&pCmdObj->strCmdName[0], GATEWAY_CMD_DEVICE_SREG_READ) == 0)  
  {
    pCmdObj->iStatus = GATEWAY_CMD_EXECUTING;
  	CmdPacketDeviceSRegRead(pDevice, pCmdObj);
    return;
  }
}

void CmdCheck(JDevice * pDevice)
{
  JINT iDID = pDevice->dwDID;
  CmdObjType cmdObject;
  CmdObjType * pCmdObj = &cmdObject;
  JBOOL bRet = FALSE;
  
	UtilMemset((JBYTE*)&cmdObject, 0, sizeof(CmdObjType));
  
	bRet = FuncGatewayCmdGet(pDevice, pCmdObj);
  if(bRet == FALSE)
  {
    return;
  }

	/// check executing command is finished or not
	if(pCmdObj->iStatus == GATEWAY_CMD_EXECUTING)
	{
		CmdExecutionControl(pDevice, pCmdObj);
		pDevice->iGatewayCmdTimeOut = pDevice->iGatewayCmdTimeOut + 100;
		/// check timeout
		if(pDevice->iGatewayCmdTimeOut >= pCmdObj->iTimeout)
		{
			DBG_PRINTF(pDevice->dwDID, "\t\t [CMD] CmdCheck TIMEOUT CMD:%s\r\n", pCmdObj->strCmdName);
			pCmdObj->iStatus = GATEWAY_CMD_EXECUTED_DONE;
			sprintf(pCmdObj->strResult, GATEWAY_CMD_NACK);			
		}		
	}
  else
	{
		DBG_PRINTF(pDevice->dwDID, "\t\t [CMD] CmdCheck Get CMD:%s\r\n", pCmdObj->strCmdName);
		/// execute command 
		CmdDispatch(pDevice, pCmdObj);	
		pDevice->iGatewayCmdTimeOut = 0;
	}

  return;
}
