#ifndef __GATEWAY_CMD_H__
#define __GATEWAY_CMD_H__ 

#include "Global.h"

///----------------------------------------------------------------////
/// Gateway Packet command
///----------------------------------------------------------------///
extern JBOOL PacketCmdGatewayInfoGet(JDevice *pDevice);

extern JBOOL PacketCmdSockDeviceTimeSet(JDevice *pDevice);
	  
extern JBOOL PacketCmdGatewayModeGet(JDevice *pDevice);

extern JBOOL PacketCmdGatewayDeviceEcgListGet(JDevice *pDevice, JINT cnt, JINT idx);
extern JBOOL PacketCmdGatewayDeviceEcgListCountGet(JDevice *pDevice, JINT *piEcgListCount);

extern JBOOL PacketCmdGatewaySRegWrite(JDevice *pDevice, SRegType *pData);
extern JBOOL PacketCmdGatewaySRegRead(JDevice *pDevice, SRegType *pData);

extern JBOOL PacketCmdGatewayMBleOpen(JDevice *pDevice, JBYTE *pAddr);
extern JBOOL PacketCmdGatewayMBleClose(JDevice *pDevice);
extern JBOOL PacketCmdGatewayMBleConnectCheck(JDevice *pDevice);
extern JBOOL PacketCmdGatewayMBleSRegCheck(JDevice *pDevice,  SRegType *pSReg);
extern JBOOL PacketCmdGatewayMBleFileListRead(JDevice *pDevice,  JINT iRecSize, JBYTE *pBuf);

extern JBOOL PacketCmdGatewaySBleVscModeStop(JDevice *pDevice);
extern JBOOL PacketCmdGatewaySBleVscModeStart(JDevice *pDevice);
extern JBOOL PacketCmdGatewaySBleVscModeQueueRead(JDevice *pDevice, JINT iReadCount, JINT iIDStart);

extern JBOOL PacketCmdGatewaySBleVscFileModeStop(JDevice *pDevice);
extern JBOOL PacketCmdGatewaySBleVscFileModeStart(JDevice *pDevice, JINT dwTotalSec);
extern JBOOL PacketCmdGatewaySBleVscFileModeQueueRead(JDevice *pDevice, JDWORD dwUtcGet, JINT iMSGet, JINT iReadCount);
extern JBOOL PacketCmdGatewaySBleVscFileModeQueueClear(JDevice *pDevice);

extern JBOOL PacketCmdGatewayBleConnectStatusGet(JDevice *pDevice, JINT *piBleConnectStatus, JINT *piBleOpenDid);
extern JBOOL PacketCmdGatewayVscModeStatusGet(JDevice *pDevice, JINT *piVscModeStatus, JDWORD *pdwVscModeStartUtc, JINT *piVscModeQueueCount);

#endif ///<  __PACKET_CMD_H__
