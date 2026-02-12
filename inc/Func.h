/**
 * @file Func.h
 * @brief  Header for task function
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
#ifndef __FUNC_H__
#define __FUNC_H__ 

#include "Global.h"

extern void   FuncCmdFolderCreate(JDevice * pDevice);

extern void   FuncDataRootRead(void);
extern void   FuncDataCmdRead(void);

extern JINT   FuncServerPortRead(void);
extern void   FuncPrintFilterRead(void);

extern void   FuncGSenAccTilt(VscModeControlType *pVscMode);

#endif ///<  __FUNC_H__
