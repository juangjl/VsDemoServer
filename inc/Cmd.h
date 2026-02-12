/**
 * @file Cmd.h 
 * @brief  Server Cmd function 
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology Co., Ltd., all rights reserved.
 * @note
*/
#ifndef __CMD_H__
#define __CMD_H__ 

#define GATEWAY_CMD_NOT_EXECUTED          (0)
#define GATEWAY_CMD_EXECUTING             (1)
#define GATEWAY_CMD_EXECUTED_DONE         (2)

#define GATEWAY_CMD_DEVICE_BLE_OPEN       ("BLE_OPEN")
#define GATEWAY_CMD_DEVICE_BLE_CLOSE      ("BLE_CLOSE")
#define GATEWAY_CMD_SREG_WRITE            ("GATEWAY_SREG_WRITE")
#define GATEWAY_CMD_SREG_READ             ("GATEWAY_SREG_READ")
#define GATEWAY_CMD_DEVICE_SREG_WRITE     ("DEVICE_SREG_WRITE")
#define GATEWAY_CMD_DEVICE_SREG_READ      ("DEVICE_SREG_READ")

#define GATEWAY_CMD_ACK 									("ACK")
#define GATEWAY_CMD_NACK 								  ("NACK")


typedef struct CmdObjSt
{
	char 		strCmdName[32];
	char 		strArg[4][256];
	JINT		iGatewayDid;
	JINT		iDeviceDid;
	JINT 		iUserId;
	JDWORD 	dwUTC;
	JINT    iTimeout;
	JINT 		iStatus;
	char 		strResult[512];
	char 		strData[512];
	char    strToken[64];
} CmdObjType;

extern void CmdCheck(JDevice * pDevice);

extern JBOOL CmdPacketGatewaySRegWrite(JDevice *pDevice, CmdObjType *pCmdObj);

#endif ///<  __CMD_H__

