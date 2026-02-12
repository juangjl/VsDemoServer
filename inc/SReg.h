/**
 * @file SReg.h 
 * @brief Software register control
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2025, VitalSigns Technology all rights reserved.
 * @note
*/
#ifndef _SREG_H_
#define _SREG_H_ 

#include "Global.h"

extern JBOOL CmdPacketGatewaySRegWrite(JDevice *pDevice, CmdObjType *pCmdObj);
extern JBOOL CmdPacketGatewaySRegRead(JDevice *pDevice,  CmdObjType *pCmdObj);

extern JBOOL CmdPacketDeviceSRegWrite(JDevice *pDevice,  CmdObjType *pCmdObj);
extern JBOOL CmdPacketDeviceSRegRead(JDevice *pDevice,   CmdObjType *pCmdObj);

#endif ///<  _SREG_H_

