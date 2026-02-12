/**
 * @file Main.cpp 
 * @brief  Main Code 
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
///------------------------------------------///
///  Include Area
///------------------------------------------///
#include "Global.h"

///------------------------------------------///
///  Variable Area
///------------------------------------------///
GlobalVarType   GlobalVar;
GlobalVarType * pGlobalVar = &GlobalVar;

///------------------------------------------///
///  Device Name
///------------------------------------------///
void HelloPrint()
{
	printf("\r\n");
	printf("--------------------------------------------------------------------------------\r\n");
	printf("\t VITALSIGNS ECG SERVER DEMO(V1.0)\r\n");
	printf("\t programmed by JL JUANG\r\n");	
	printf("\t Copyright (c) 2026, VitalSigns Technology all rights reserved.\r\n");
	printf("--------------------------------------------------------------------------------\r\n");
	return;
}

JINT MainMonitor()
{
	struct sockaddr_in dev_addr;
	pid_t  childPID  = 0;
	pid_t  childPID2 = 0;
	JINT 	 socket;
	JINT 	*pSocket = &socket;
	JINT 	 port = 0;
	JINT 	 pid  = getpid();
	JINT 	 iSocket = -1;
	JINT 	 idx;
  JINT 	 status;
  struct sockaddr_in * pSockAddr;
	char strFileName[256];

  JINT	 iDID = GlobalVar.iPrintFilterDID;	

	///  Zombie process kill
	signal(SIGCHLD, SIG_IGN);

	port = FuncServerPortRead();
	pGlobalVar->iServerPort = port;

	/// SOCKET INIT
	SockInit(pSocket);
	
	/// add the socket option
	SockSet(pSocket);

	/// SOCKET BIND
	SockBind(pSocket, port, &dev_addr);

	/// SOCKET Listen
	SockListen(pSocket);

	DBG_PRINTF(DBG_ID, "pid = %d, port = %d \r\n", pid, port);
  pSockAddr = &dev_addr;
	sprintf(strFileName, "./leave");
	while(1)
	{	
		if(UtilFileExisted((char *)strFileName) == TRUE)
		{
				break;
		}
		/// SOCKET SERVER ACCEPT
		iSocket = SockAccept(pSocket, &dev_addr);
		if(iSocket  == -1)
		{
			UtilMsSleep(5);
			continue;
		}		
		DBG_PRINTF(DBG_ID, "Accept\r\n");		
		
		/// fork process
		signal(SIGCHLD, SIG_IGN);
		childPID = fork();

		if(childPID == 0) ///< Fork was successful, child process
		{
			close(*pSocket);
		 	UtilMsSleep(50);
			signal(SIGCHLD, SIG_DFL); ///< change the process to default
			childPID2 = fork();
			if(childPID2 == 0)
			{
	 			snprintf(pGlobalVar->strIpAddr, sizeof(pGlobalVar->strIpAddr), "%s", inet_ntoa(pSockAddr->sin_addr));
        pGlobalVar->dwIp = *(JDWORD*)&pSockAddr->sin_addr;
	 			Mainloop(iSocket);
	 			_exit(0);
			}
			_exit(0);
	 	}
		else if(childPID < 0)
		{
			DBG_PRINTF(DBG_ID, "[PID][%d]FORK ERROR\r\n", pid);
		}
		else if(childPID > 0)
		{
      /// close connection socket
			close(iSocket);         

			/// parent process
      waitpid(-1, &status, 0);
      DBG_PRINTF(DBG_ID, "[PID][%d] DEVICE-IN\r\n", pid);
		}
		waitpid(-1, NULL, WNOHANG);		
	}	
	ConfigSave();
	DBG_PRINTF(DBG_ID, "BYEBYE (%d)\r\n", childPID);
	return 0;
}

///-------------------------------------------------///
/// @Function: Main
/// @Brief: Main Entry
///-------------------------------------------------///
void MainInit()
{  
  HelloPrint();  

	///-------------------------------------------------------------------------///
	/// OS Type Get
	///-------------------------------------------------------------------------///
	UtilOSTypeGet(&GlobalVar.iOSType);

	///-------------------------------------------------------------------------///
	/// Config Load
	///-------------------------------------------------------------------------///
	ConfigLoad();
			
  FuncDataRootRead();  
	FuncDataCmdRead();
	FuncPrintFilterRead();	
			
	/// Folcder create  
	UtilFolderCreate((char *)"./log");	
	UtilFolderCreate((char *)GlobalVar.strCmdRoot);	
}

JINT main(JINT argc, char *argv[])
{	
  MainInit();
	
	MainMonitor();
  
	return 0;
}

