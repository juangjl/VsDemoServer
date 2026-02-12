/**
 * @file FuncMeas.h
 * @brief  Header for measurement function
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
#ifndef __FUNC_MEAS_H__
#define __FUNC_MEAS_H__ 

#include "Global.h"

extern void FuncMeasFolderCreate(JDevice * pDevice);

extern void FuncMeasStateSet(JDevice *pDevice, JINT iReportId, JFLOAT fReportType, JINT iMeasState);

#endif ///<  __FUNC_MEAS_H__
