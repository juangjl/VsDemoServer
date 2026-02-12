/**
 * @file SystemCmd.h 
 * @brief  Packet command of system
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2025, VitalSigns Technology all rights reserved.
 * @note
*/
#ifndef __SYSTEM_CMD_H__
#define __SYSTEM_CMD_H__ 

#define  SYS_OFFLINE           (0)
#define  SYS_ONLINE_WIFI       (1)
#define  SYS_ONLINE_LTE        (2)

extern JBOOL PacketCmdSystemVersionGet(JDevice *pDevice);

#endif ///<  __SYSTEM_CMD_H__

