/**
 * @file Sock.cpp 
 * @brief  Socket Command 
 * @version $Revision$
 * @author JL Juang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
#include "Global.h"
#include <netinet/tcp.h>
#include <sys/poll.h>

JBYTE JAesKey[] = { 
										0x68, 0x74, 0x74, 0x70, 0x73, 0x3A, 0x2F, 0x2F,
									  0x77, 0x77, 0x77, 0x2E, 0x76, 0x73, 0x69, 0x67,
                    0x6E, 0x74, 0x65, 0x6B, 0x2E, 0x63, 0x6F, 0x6D,
										0x2F, 0x76, 0x73, 0x2D, 0x65, 0x63, 0x67, 0x2F 
									};

JDOUBLE SocketTotalCount 			= 0;
JDOUBLE SocketErrorCount 		  = 0;
JDOUBLE SocketErrorRate  		  = 0;

JDOUBLE SocketTotalReadBytes  = 0;
JDOUBLE SocketTotalWriteBytes = 0;

JDOUBLE SocketWriteTime  			= 0;
JDOUBLE SocketReadTime   			= 0;

JDOUBLE SocketT0 							= 0;
JDOUBLE SocketT1 							= 0;
JDOUBLE SocketT2  						= 0;
JDOUBLE SocketT3  						= 0;

JDOUBLE SocketWriteBytes      = 0;
JDOUBLE SocketReadBytes       = 0;

JDOUBLE SocketTotalWriteTime  = 0;
JDOUBLE SocketTotalReadTime   = 0;

JDOUBLE SocketReadSpeed       = 0;
JDOUBLE SocketWriteSpeed      = 0;

JDOUBLE SocketReadAvgSpeed    = 0;
JDOUBLE SocketWriteAvgSpeed   = 0;

JDOUBLE SocketErrorRateMax  	= 0;

void SocketStatisticPrint(JDWORD dwDid)	
{	
	DBG_PRINTF(dwDid,  "\t [SOCKET][STAT] PER = %0.2f%%, TOTAL = %0.0f, ERROR = %0.0f \r\n", SocketErrorRate, SocketTotalCount, 		SocketErrorCount);
	DBG_PRINTF(dwDid,  "\t [SOCKET][STAT] TOTAL-READ = %0.0f bytes , TOTAL-WRITE = %0.0f bytes \r\n", 				SocketTotalReadBytes, SocketTotalWriteBytes);		
	DBG_PRINTF(dwDid,  "\t [SOCKET][STAT] WRITE-TIME = %0.0f us, READ-TIME = %0.3f s\r\n", 										SocketWriteTime, 			SocketReadTime);
	DBG_PRINTF(dwDid,  "\t [SOCKET][STAT] WRITE-TIME-TOTAL = %0.0f us, READ-TIME-TOTAL = %0.3f s\r\n", 				SocketTotalWriteTime, SocketTotalReadTime);
	DBG_PRINTF(dwDid,  "\t [SOCKET][STAT] READ-SPEED-NOW = %0.3f KB /s,  WRITE-SPEED-NOW = %0.3f MB/s \r\n", 	SocketReadSpeed, 			SocketWriteSpeed);
	DBG_PRINTF(dwDid,  "\t [SOCKET][STAT] READ-SPEED-AVG = %0.3f KB /s,  WRITE-SPEED-AVG = %0.3f MB/s \r\n", 	SocketReadAvgSpeed, 	SocketWriteAvgSpeed);		
}

void SocketStatistic(JINT iStatus)
{	
	if((SocketTotalCount <= 0) || (SocketTotalWriteBytes <= 0) || (SocketTotalReadBytes <= 0))
	{		
		SocketErrorCount       = 0;
		SocketTotalWriteBytes  = 0;
		SocketTotalReadBytes   = 0;
	}

	SocketTotalCount = SocketTotalCount + 1;
	
	if(iStatus == SOCKET_CMD_OK)
	{
		SocketTotalWriteBytes = SocketTotalWriteBytes + SocketWriteBytes;
		SocketTotalReadBytes  = SocketTotalReadBytes  + SocketReadBytes;				
	}
	else
	{
		SocketErrorCount 		= SocketErrorCount + 1;		
	}  

	if(SocketTotalCount > 0)
	{
		SocketErrorRate = SocketErrorCount * 100 / SocketTotalCount;				
	}

	SocketWriteTime =  (SocketT1 - SocketT0);
	SocketReadTime  =  (SocketT3 - SocketT2) / 1000.0;
	
	SocketTotalWriteTime = SocketTotalWriteTime + SocketWriteTime;
	SocketTotalReadTime  = SocketTotalReadTime  + SocketReadTime;
	if(SocketTotalWriteTime > 0)
	{
		SocketWriteSpeed = SocketWriteBytes  / (SocketWriteTime);		
		SocketWriteAvgSpeed = SocketTotalWriteBytes / (SocketTotalWriteTime);		
	}
	if(SocketTotalReadTime > 0)
	{
		SocketReadSpeed    = SocketReadBytes  / (1000.0 * SocketReadTime);
		SocketReadAvgSpeed = SocketTotalReadBytes / (1000.0 *SocketTotalReadTime);		
	}
}

void SockInit(JINT * pSocket)
{ 	
	/// First call to socket() function 
	*pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	/// Check socket ready 
	if(*pSocket < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	}	
}

void SockBind(JINT *pSocket, JINT port, struct sockaddr_in * pSockAddr)
{
	/// Initialize socket structure
	bzero((char *) pSockAddr, sizeof(*pSockAddr));
	 
	pSockAddr->sin_family      = AF_INET;
	pSockAddr->sin_addr.s_addr = INADDR_ANY;
	pSockAddr->sin_port        = htons(port);	

	/// Now bind the host address using bind() call.
	if(bind(*pSocket, (struct sockaddr *) pSockAddr, sizeof(*pSockAddr)) < 0)
	{
		DBG_PRINTFW(L"ERROR on binding %d", port);
		exit(1);
	}		
}

void SockListen(JINT *pSocket)
{
	DBG_PRINTFW(L"Listen\r\n"); 
	listen(*pSocket, 1024);
}

JINT SockAccept(JINT *pSocket, struct sockaddr_in * pSockAddr)
{
	JINT iSocket;
	socklen_t iLen = sizeof(*pSockAddr);
	/// Accept actual connection from the client
	iSocket = accept(*pSocket, (struct sockaddr *)pSockAddr, (socklen_t *)&iLen);

	if(	iSocket != -1)
	{
		DBG_PRINTFW(L"client ip = %s sock=%d\r\n", inet_ntoa(pSockAddr->sin_addr), 	iSocket);
	}
	return  iSocket;
}

void SockSet(JINT *pSocket)
{
 	struct timeval timeout;  

	JINT iKeepAlive    = 1;
	JINT iKeepIdle     = 10;
	JINT iKeepInterval = 5;   
	JINT iKeepCount    = 3;
	
	JINT iReuse 		= 1;
	JINT iNoDelay 	= 1;
	JINT iBufSize	  = 1024 * 1024;

	JINT iFlags = 0;

	timeout.tv_sec  = 3;
	timeout.tv_usec = 0;

	if(setsockopt(*pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
	{
    perror("setsockopt failed\n");
    exit(1);
	}

	if(setsockopt(*pSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
	{
	  perror("setsockopt failed\n");
	  exit(1);
	}

	if(setsockopt(*pSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&iReuse, sizeof(iReuse)) < 0)
	{
    perror("setsockopt failed\n");
    exit(1);
	}

	/// no delay for TX
  if(setsockopt(*pSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&iNoDelay, sizeof(iNoDelay)) < 0)
	{
	  perror("setsockopt failed\n");
	  exit(1);
	}

	/// set the RX buffer size
	setsockopt(*pSocket, SOL_SOCKET, SO_RCVBUF, &iBufSize, sizeof(iBufSize));

	/// set the TX buffer size
	setsockopt(*pSocket, SOL_SOCKET, SO_SNDBUF, &iBufSize, sizeof(iBufSize));

	/// Keepalive
	setsockopt(*pSocket, SOL_SOCKET,  SO_KEEPALIVE,  &iKeepAlive, 	 sizeof(iKeepAlive));
	
	#if OS_TYPE == OS_TYPE_UBUNTU
	setsockopt(*pSocket, IPPROTO_TCP, TCP_KEEPIDLE,  &iKeepIdle, 		 sizeof(iKeepIdle));
	#endif ///< for OS_TYPE_UBUNTU

	setsockopt(*pSocket, IPPROTO_TCP, TCP_KEEPINTVL, &iKeepInterval, sizeof(iKeepInterval));
	setsockopt(*pSocket, IPPROTO_TCP, TCP_KEEPCNT, 	 &iKeepCount, 	 sizeof(iKeepCount));
}

JINT SockWrite(JINT *pSocket, JBYTE *pbData, JINT iWriteLen)
{
	JINT len = 0;    
	
	len = send(*pSocket, pbData, iWriteLen, MSG_NOSIGNAL | MSG_DONTWAIT);
	if(len > 0)
	{
		return len;
	}
	else if(len < 0)
	{
		if(errno == EINTR)
		{
			printf("Sock Write: EINTR\r\n");
		}
		if((errno == EAGAIN) || (errno == EWOULDBLOCK))
		{
			printf("Sock Write: EAGAIN | EWOULDBLOCK\r\n");		
		}
		printf("Sock Write failed\r\n");
		return -1;
	}

	return len;
}

JINT SockPurge(JUINT dwDID, JINT *pSocket)
{
	JBYTE bBuffer[8192];
	JINT iReadCount = 0;
	JINT iReadTotal = 0;
	
	JINT iFlags = fcntl(*pSocket, F_GETFL, 0);
	fcntl(*pSocket, F_SETFL, iFlags | O_NONBLOCK);

	while(1)
	{
		iReadCount = read(*pSocket, bBuffer, sizeof(bBuffer));
		if(iReadCount <= 0)
		{
			break;
		}
		iReadTotal = iReadTotal + iReadCount;
	}

	fcntl(*pSocket, F_SETFL, iFlags);

	if(iReadTotal > 0)
	{
			DBG_PRINTF(dwDID, "\t\t [SOCK][UPRGE] PURGE-SIZE = %d\r\n", iReadTotal);  
			UtilMsSleep(1);
	}

	return iReadTotal;
}

JINT SockWriteCmd(JUINT dwDID, JINT *pSocket, JBYTE *pbWData, JINT iWriteLen, JBYTE *pbRData, JINT iReadLen, JINT iTimeout)
{
	JINT iWrtLen = 0;
	JINT iRdLen = 0;	
	JINT iTotalReadLen = 0;		
	JULONG t0 = TickCountGet();
	JULONG t1 = TickCountGet(); 
	JULONG tStart = t0;
	
  JINT 	i = 0;
  JINT 	idx = 0;
  JINT 	iRArr[10];
	JINT 	iRet = -1;	
  PacketAckType  PacketAckTmp;
	
	struct pollfd fdPoll;
	
	JINT iRecvLen = 0;
	JINT iRestLen = 0;

	JINT iBatchLen = 0;
  	
	JINT iTimeOutMS = iTimeout;	

	JINT iPurgeCount 			= 0;
	JINT iPurgeCountTotal = 0;

	///----------------------------------------------------------------///
	/// Encode and decode data
	///----------------------------------------------------------------///
	JWORD wOriginalSize   = 0;
	
	///----------------------------------------------------------------///
	/// AES
	///----------------------------------------------------------------///
	AesCtx ctx;
	JWORD wLenAes256  = 0;
	
	JBYTE bDataEncrypted[20000];
	JBYTE bDataDecrypted[20000];

	static JINT iPacketFailCount = 0;				
		
	// Software init ctx
	AesCtxInit(&ctx, JAesKey);		

	SocketT0 = TickUSCountGet();			

	///----------------------------------------------------------------///
	/// Socket Purge
	///----------------------------------------------------------------///
	SockPurge(dwDID, pSocket);

	///----------------------------------------------------------------///
	/// Socket Write Encryption
	///----------------------------------------------------------------///
	if(iWrtLen % 16 != 0)
	{
		DBG_PRINTF(dwDID,"\t[SOCK][ERROR][!!!!!!] Write iWrtLen  % 16 != 0 ERROR: %d\r\n", iWriteLen);  
		return FALSE;
	}
	UtilMemcpy(&bDataEncrypted[0], &pbWData[0], iWriteLen);		
	AesEcbBufferEncrypt(&ctx, &bDataEncrypted[0], iWriteLen);
		
	///----------------------------------------------------------------///
	/// Socket Write
	///----------------------------------------------------------------///
	iWrtLen = SockWrite(pSocket, (JBYTE *)&bDataEncrypted[0], iWriteLen);
	if(iWrtLen != iWriteLen)
	{
		DBG_PRINTF(dwDID,"\t[SOCK]Write iWrtLen ERR: %d, %d\r\n", iWriteLen, iWrtLen);  
	}
	SocketT1 = TickUSCountGet();
	SocketT2 = TickCountGet();

	if(iReadLen == 0)
	{
		return TRUE;
	}
		
	///----------------------------------------------------------------///
	/// Socket Read with poll
	///----------------------------------------------------------------///
	if(fdPoll.fd != *pSocket)
	{
		fdPoll.fd		  = *pSocket; ///< your socket handler 
		fdPoll.events = POLLIN;
	}

	iRet = poll(&fdPoll, 1, SOCK_CMD_TIMEOUT); // 1 second for timeout	

	idx = 0;
	iTotalReadLen = 0;
	iRestLen = iReadLen;
	while(1)
	{
		iRecvLen = 0;
		iRet = poll(&fdPoll, 1, SOCK_CMD_TIMEOUT); // 1 second for timeout		
		switch (iRet)
		{
		case -1:
			/// Error							
			DBG_PRINTF(dwDID, "\t[SOCK] Socket Read Error!!!\r\n");  
			iRet = SOCKET_CMD_ERROR;  
			return iRet;  			
		case 0:
			/// Timeout 
			//DBG_PRINTF(dwDID, "\t[SOCK] Socket Read Timeout!!!\r\n");	    
			iPacketFailCount = iPacketFailCount + 1;
			if(iPacketFailCount >= 8)
			{				
				DBG_PRINTF(dwDID, "\t[SOCK] Socket Failed 8 times!!!\r\n");	    			
				iPacketFailCount = 0;
				return SOCKET_CMD_ERROR;
			}			
			break;
		default:				
			iBatchLen = 20000;
			iRecvLen = recv(*pSocket, &bDataDecrypted[idx], iBatchLen, 0); ///< get your data						

			if(iRecvLen == 0)  ///< Client closed connection
			{				
				iRet = SOCKET_CMD_CONNECTION_CLOSED;							
			}
			t0 = TickCountGet();
			iPacketFailCount = 0;
			break;
		}		
		idx = idx + iRecvLen;
		iTotalReadLen = iTotalReadLen + iRecvLen;

		iRestLen = iReadLen - iTotalReadLen;
									
		if((iRestLen <= 0))	
		{							
			if(!AesEcbBufferDecrypt(&ctx, &bDataDecrypted[0], iTotalReadLen))
			{
			  DBG_PRINTF(DBG_ID, "[AES][ECB][DECRYPT][ERROR]AES TOTAL LENGTH ERROR: %d, DID:%d\n", iTotalReadLen, dwDID);
			}
			UtilMemcpy(&pbRData[0], &bDataDecrypted[0], iTotalReadLen);

			t1 = TickCountGet();			
			iRet = SOCKET_CMD_OK;
			break;
		}   

		///---------------------------------------///
		///check command packet nack 
		///---------------------------------------///
		if((iTotalReadLen == PACKET_CMD_HEADER_SIZE) || (iTotalReadLen == 16))
		{		
			/// check the data
			memcpy((void *)&PacketAckTmp, &pbRData[0], iTotalReadLen);
			
			/// NACK 
			if(PacketAckTmp.bAck == CMD_NACK)
			{	
				DBG_PRINTF(dwDID, "\t\t[SOCK][!!!!!] CMD_NACK \r\n");	
				break;				
			}
			iRet = SOCKET_CMD_NACK;
		}
				
		t1 = TickCountGet();
		if((t1 - t0)> iTimeout)
		{
			iRet = SOCKET_CMD_READ_TIMEOUT;
			DBG_PRINTF(dwDID, "\t\t [SOCK][ERROR] Socket Read timeout (%d ms), exepected %d, total = %d\r\n", iTimeout, iReadLen,  iTotalReadLen);
			if((iTotalReadLen < 64)	 && (iTotalReadLen > 0))
			{
				UtilDataPrint(dwDID, (JBYTE * )&pbRData[0], iTotalReadLen);
			}
			break;
		}		
		UtilUsSleep(200);
	}
  SocketT3 = TickCountGet();

	//----------------------------------------------------------------//
	// Socket statistics
	//----------------------------------------------------------------//
	SocketWriteBytes  = iWriteLen;
  SocketReadBytes   = iReadLen;
	SocketStatistic(iRet);
	if(iRet != SOCKET_CMD_OK)
	{
		SocketStatisticPrint(dwDID);
	}
	else
	{
		if(((long)SocketTotalCount % 100) == 0)
		{
			SocketStatisticPrint(dwDID);
		}	
	}

	return iRet;
}
