/**
 * @file FuncGateway.h
 * @brief  Header for gateway function
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
#ifndef __FUNC_GATEWAY_H__
#define __FUNC_GATEWAY_H__ 

#include "Global.h"

extern JINT   FuncGatewayFolderCreate(JDevice * pDevice);
extern JINT   FuncGatewayOnlineSet(JDevice * pDevice);
extern JINT   FuncGatewayOfflineSet(JDevice * pDevice);

extern JINT   FuncGatewayDeviceDidGet(JDevice * pDevice);

extern void   FuncGatewayScanDeviceFileWrite(JDevice * pDevice);

extern JINT   FuncGatewayVersionGet(JDevice * pDevice);

extern JINT   FuncGatewayLocationInfoGet(JDevice * pDevice);
extern void   FuncGatewayLocationFileWrite(JDevice * pDevice);

extern void   FuncGatewayDeviceEcgListGet(JDevice *pDevice);

extern void   FuncGatewayInfoFileWrite(JDevice * pDevice,time_t tNow);

extern JINT   FuncGatewayCmdGet(JDevice * pDevice, CmdObjType * pCmdObj);

#endif ///<  __FUNC_GATEWAY_H__

