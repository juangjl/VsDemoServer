/**
 * @file GlobalVar.h 
 * @brief GlobalVar define variable type 
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/

#ifndef __GLOBALVAR_H__
#define __GLOBALVAR_H__
#include "Global.h"

typedef struct GlobalVarSt
{
	char    strInfoDataRoot[256];
  char    strDataRoot[256];
  char    strCmdRoot[256];
	JINT    iPrintFilterDID;
	JINT    iServerPort;

  ///------------------------------------------------------------///
  /// OS Type
  ///------------------------------------------------------------///
  JINT iOSType;

  ///------------------------------------------------------------///
  /// SubTask Data
  ///------------------------------------------------------------///
  time_t  t0;
  time_t  t1;
  JTM     jtm;
  JBOOL   bSubStart;

  JDevice * pDevice;

	///------------------------------------------------------------///
  /// IP
	///------------------------------------------------------------///
  char strIpAddr[32];
  JDWORD dwIp;

} GlobalVarType;

#endif ///< __TYPEDEF_H__
