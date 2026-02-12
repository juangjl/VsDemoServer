#ifndef __PACKET_CMD_H__
#define __PACKET_CMD_H__ 

#include "Global.h"

extern JBOOL PacketCmdSockDeviceInfoRead(JDevice *pDevice);

extern JBOOL PacketCmdSockDeviceTimeSet(JDevice *pDevice);

extern JBOOL PacketMBleCmdSRegWrite(JDevice *pDevice, SRegType *pSReg);
extern JBOOL PacketMBleCmdSRegRead(JDevice *pDevice, SRegType *pSReg);
extern JBOOL PacketMBleCmdFileListRead(JDevice *pDevice);

#endif ///<  __PACKET_CMD_H__

