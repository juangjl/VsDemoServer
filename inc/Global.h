/**
 * @file Global.h 
 * @brief  Global.h controls the features enabled or disabled 
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <wchar.h>
#include <locale.h>
#include <algorithm>

#include <dirent.h>

#include <unistd.h>

#include <vector>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <pthread.h>

#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include <pthread.h>

#include <unistd.h>

#include <termios.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <libgen.h> 

#include "JType.h"
#include "Define.h"
#include "Vendor.h"


#include "Sock.h"
#include "Jtm.h"
#include "TypeDef.h"
#include "Packet.h"
#include "JStat.h"

#include "Config.h"

#include "PacketCmd.h"
#include "SystemCmd.h"

#include "JGateway.h"
#include "JGatewayCmd.h"

#include "Patient.h"

#include "GlobalVar.h"

#include "Util.h"

#include "Cmd.h"
#include "SReg.h"

#include "Task.h"

#include "MeasurementState.h"

#include "Func.h"

#include "FuncVscMode.h"
#include "FuncGateway.h"
#include "FuncMeas.h"
#include "FuncDeviceEcg.h"

#include "JAtr.h"
#include "VarDef.h"
#include "FuncDef.h"

#include "Wfdb.h"
#include "VscMode.h"

#include "Aes.h"

#include "Location.h"
#include "LocationCmd.h"

#endif  ///< for #ifndef __GLOBAL_H__
