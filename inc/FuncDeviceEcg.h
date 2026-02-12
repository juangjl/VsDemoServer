/**
 * @file FuncDeviceEcg.h
 * @brief  Header for device ecg func function
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
#ifndef __FUNC_DEVICE_ECG_H__
#define __FUNC_DEVICE_ECG_H__ 

#include "Global.h"

#define JFILE_LIST_ITEM_COUNT	      (20)
#define JFILE_LIST_ITEM_SIZE        (8) ///< each item size UTC0/UTC1 in bytes
#define JFILE_LIST_ITEM_MAX_SIZE    ((JFILE_LIST_ITEM_COUNT * JFILE_LIST_ITEM_SIZE) + 4)  ///< total count + item size UTC0/UTC1

///-------------------------------------------------------------------------------------------------------------///
/// Device JFILE LIST
///-------------------------------------------------------------------------------------------------------------///
typedef struct JFileListItemSt
{
	JDWORD dwUTC0;
	JDWORD dwUTC1;
} JFileListItemType;

typedef struct JFileListSt
{
	JINT iCnt;
	JFileListItemType items[JFILE_LIST_ITEM_COUNT];			
} JFileListType;

///------------------------------------------------------------///
/// Device Ecg
///------------------------------------------------------------///
extern JINT   FuncDeviceEcgDataSet(JDevice *pDevice,  JDeviceEcg  *pDeviceEcg,  DeviceEcgType * pEcgObject);
extern JINT   FuncDeviceEcgDataSetEx(JDevice *pDevice, JDeviceEcg* pDeviceEcg,  VscModeControlType * pVscModeCtl);

extern JFileListType DeviceFileList;

#endif ///<  __FUNC_DEVICE_ECG_H__
