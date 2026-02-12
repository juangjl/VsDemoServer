/**
 * @file FuncVscMode.h
 * @brief  Header for VSC mode function
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
#ifndef __FUNC_VSC_MODE_H__
#define __FUNC_VSC_MODE_H__ 

#include "Global.h"

///------------------------------------------------------------///
/// Vsc Mode Save data
///------------------------------------------------------------///
extern JBOOL  FuncVscModeFolderCreate(JDevice *pDevice, time_t nowTime, time_t startTime, char *strFolder);

extern void   FuncVscModeEcgDataBinSave(char *strFileName, VscModeControlType *pVscMode, JINT iCH);
extern void   FuncVscModeEcgDataCsvSave(char *strFileName, VscModeControlType *pVscMode, JINT iCH);

extern void   FuncVscModeGSensorDataSave(char *strFileName, VscModeControlType *pVscMode);
extern void   FuncVscModeAtrSave(JDevice *pDevice, JAtrType *pAtrRead, char *strFolderName );
extern void   FuncVscModeInfoSave(char *strFileName, VscModeControlType *pVscMode);
extern void   FuncVscModeExecute(JDevice *pDevice);
extern void   FuncVscModeInit(JDevice * pDevice);

extern JINT   FuncVscModeQueueRead(JDevice *pDevice, JINT iReadCount);

#endif ///<  __FUNC_VSC_MODE_H__

