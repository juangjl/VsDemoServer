/**
 * @file FuncMeas.cpp
 * @brief  Control function for measurement
 * @version $Revision$
 * @author JLJuang <jl_juang@vsgntek.com>
 * @note Copyright (c) 2024, VitalSigns Technology Co., Ltd, all rights reserved.
 * @note
*/
#include "Global.h"

void FuncMeasFolderCreate(JDevice * pDevice)
{
  FILE *fp = NULL;
  char  filePath[256];
  char  folderPath[256];
  char  line[256];
  
  char  strCmd[256];
  JINT  iRet = 0;

  ///------------------------------------------------------------------------///
  /// Folder Create
  ///------------------------------------------------------------------------///
	sprintf((char *)&folderPath[0], "%s%s", GlobalVar.strDataRoot, "MEAS");
	UtilFolderCreate(folderPath);

	sprintf(strCmd, "chmod 777 %s", folderPath);
  sprintf(strCmd, "%s", strCmd);
  iRet = system(strCmd);  
}

void FuncMeasStateSet(JDevice *pDevice, JINT iReportId, JFLOAT fReportType, JINT iMeasState)
{
  JINT iDID = pDevice->dwDID;
  MeasItemType * pMeasItem = pDevice->pMeasItemActive;
  pMeasItem->iMeasState = iMeasState;
}
