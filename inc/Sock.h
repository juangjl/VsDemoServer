/**
 * @file Sock.h 
 * @brief Socket function headers
 * @version $Revision$
 * @author JL Juang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
#ifndef _SOCK_H_
#define _SOCK_H_ 

#include "Global.h"

#define SOCKET_CMD_OK                   (1)
#define SOCKET_CMD_NACK                 (-1)
#define SOCKET_CMD_READ_TIMEOUT         (-2)
#define SOCKET_CMD_CONNECTION_CLOSED    (-3)
#define SOCKET_CMD_ERROR                (-4)

///----------------------------------------------------------------------///
/// Socket Function
///----------------------------------------------------------------------///
extern void SockInit(JINT *pSocket);
extern void SockClose(JINT *pSocket);
extern void SockBind(JINT *pSocket, JINT port, struct sockaddr_in * pSockAddr);
extern void SockListen(JINT *pSocket);
extern JINT SockAccept(JINT *pSocket, struct sockaddr_in * pSockAddr);

extern void SockSet(JINT *pSocket);

extern JINT SockWrite(JINT *pSocket, JBYTE *pbData, JINT iWriteLen);
extern JINT SockPurge(JUINT dwDID, JINT *pSocket);

extern JINT SockWriteCmd(JUINT dwDID, JINT *pSocket, JBYTE *pbWData, JINT iWriteLen, JBYTE *pbRData, JINT iReadLen, JINT iTimeout);

extern JBYTE JAesKey[];

#endif ///<  _SOCK_H_

