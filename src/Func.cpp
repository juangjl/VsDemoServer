/**
 * @file Func.cpp
 * @brief  Function control
 * @version $Revision$
 * @author JLJuang <jl_juang@vsgntek.com>
 * @note Copyright (c) 2024, VitalSigns Technology Co., Ltd, all rights reserved.
 * @note
*/
#include "Global.h"

void FuncPrintFilterRead(void)
{
	FILE *fp = NULL;
	JINT iRet = 0;
	char line[256];
	JINT iDID = DBG_ID;
	///-----------------------------------------------///
	/// Fetch the PRINT FILTER
	///-----------------------------------------------///
	strcpy(line, "");
	fp = fopen("./cfg/print_filter.txt", "r");
	if(fp != NULL)
	{
		fscanf(fp, "%[^\n]\n", &line[0]);		
		fclose(fp);
	}	
	if(strlen(line) > 0)
	{
		sscanf(&line[4], "%d", &iDID);
	}
  if(GlobalVar.iPrintFilterDID  != iDID)
  {
  	GlobalVar.iPrintFilterDID = iDID;
  	DBG_PRINTF(DBG_ID, "PRINT_FILTER = DID_%07d\r\n", 	GlobalVar.iPrintFilterDID);
  }
}

void FuncDataRootRead(void)
{
	ConfigStrGet((char *)CONFIG_ITEM_DATA_ROOT, &GlobalVar.strDataRoot[0]);		
	DBG_PRINTF(DBG_ID, "DATA_ROOT = '%s'\r\n", (char *)&GlobalVar.strDataRoot[0]);

  /// add the ECG Data Info path
  sprintf(&GlobalVar.strInfoDataRoot[0], "%sVS", &GlobalVar.strDataRoot[0]);

  /// Create the data root folder if not existed
  if(UtilFileExisted(GlobalVar.strDataRoot) == FALSE)
  {
    UtilFolderCreate(GlobalVar.strDataRoot);
  }
}

void FuncDataCmdRead(void)
{
	ConfigStrGet((char *)CONFIG_ITEM_CMD_ROOT, &GlobalVar.strCmdRoot[0]);		
	DBG_PRINTF(DBG_ID, "CMD_ROOT  = '%s'\r\n", (char *)&GlobalVar.strCmdRoot[0]);
}

JINT FuncServerPortRead(void)
{
	JINT iPort = 0;
	ConfigIntGet((char *)CONFIG_ITEM_SERVER_PORT, &iPort);			
	DBG_PRINTF(DBG_ID, "SERVER PORT = %d \r\n", iPort);
	return iPort;	
}

void FuncCmdFolderCreate(JDevice * pDevice)
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
	sprintf((char *)&folderPath[0], "%s%07d", GlobalVar.strCmdRoot, pDevice->dwDID);
	UtilFolderCreate(folderPath);

	sprintf(strCmd, "chmod 755 %s", folderPath);
  sprintf(strCmd, "%s", strCmd);
  iRet = system(strCmd);

  ///------------------------------------------------------------------------///
  /// VsGwCmd copy
  ///------------------------------------------------------------------------///
  sprintf(strCmd, "cp -f ./prj/VsGwCmd.o %s/", folderPath);  
  iRet = system(strCmd);

  sprintf(strCmd, "chmod 755 %s/VsGwCmd.o ", folderPath);  
  iRet = system(strCmd);

  ///------------------------------------------------------------------------///
  /// Create the gw_light_on.sh
  ///------------------------------------------------------------------------///
  sprintf(filePath, "%s/gw_light_on.sh", folderPath);
  fp = fopen(filePath, "w+");
  if(fp!= NULL)
  {
    fprintf(fp, "rm -f GCMD.TXT\n");
    fprintf(fp, "./VsGwCmd.o GATEWAY_SREG_WRITE SREG_LIGHT_EVENT 1\n");
    fclose(fp);
  }
  sprintf(strCmd, "chmod 755 %s", filePath);
  sprintf(strCmd, "%s", strCmd);
  iRet = system(strCmd);

  ///------------------------------------------------------------------------///
  /// Create the gw_light_off.sh
  ///------------------------------------------------------------------------///
  sprintf(filePath, "%s/gw_light_off.sh", folderPath);
  fp = fopen(filePath, "w+");
  if(fp!= NULL)
  {
    fprintf(fp, "rm -f GCMD.TXT\n");
    fprintf(fp, "./VsGwCmd.o GATEWAY_SREG_WRITE SREG_LIGHT_EVENT 0\n");
    fclose(fp);
  }
  sprintf(strCmd, "chmod 755 %s", filePath);
  sprintf(strCmd, "%s", strCmd);
  iRet = system(strCmd);

  ///------------------------------------------------------------------------///
  /// Create the gw_sound_on.sh
  ///------------------------------------------------------------------------///
  sprintf(filePath, "%s/gw_sound_on.sh", folderPath);
  fp = fopen(filePath, "w+");
  if(fp!= NULL)
  {
    fprintf(fp, "rm -f GCMD.TXT\n");
    fprintf(fp, "./VsGwCmd.o GATEWAY_SREG_WRITE SREG_SOUND_EVENT 1\n");
    fclose(fp);
  }
  sprintf(strCmd, "chmod 755 %s", filePath);
  sprintf(strCmd, "%s", strCmd);
  iRet = system(strCmd);

  ///------------------------------------------------------------------------///
  /// Create the gw_sound_off.sh
  ///------------------------------------------------------------------------///
  sprintf(filePath, "%s/gw_sound_off.sh", folderPath);
  fp = fopen(filePath, "w+");
  if(fp!= NULL)
  {
    fprintf(fp, "rm -f GCMD.TXT\n");
    fprintf(fp, "./VsGwCmd.o GATEWAY_SREG_WRITE SREG_SOUND_EVENT 0\n");
    fclose(fp);
  }
  sprintf(strCmd, "chmod 755 %s", filePath);
  sprintf(strCmd, "%s", strCmd);
  iRet = system(strCmd);
}

void FuncGSenAccTilt(VscModeControlType *pVscMode)
{  
  JFLOAT fN = 0;
  JFLOAT fD = 0;
  JFLOAT fG = 0;

  JFLOAT fPitchPI = 0;
  JFLOAT fRollPI = 0;
  
  JFLOAT p = 0;
  JFLOAT r = 0;
  
  JFLOAT fPitch = 0;
  JFLOAT fRoll = 0;

  JFLOAT fAX = pVscMode->fGsenXNow;
  JFLOAT fAY = pVscMode->fGsenYNow;
  JFLOAT fAZ = pVscMode->fGsenZNow;

	///------------------------------------------------------------------------///
	/// PITCH
	///------------------------------------------------------------------------///
	fN = pVscMode->fGsenXNow;
	fD = sqrt((pVscMode->fGsenYNow * pVscMode->fGsenYNow) + (pVscMode->fGsenZNow * pVscMode->fGsenZNow));
  if(fD != 0)
  {
    p = atan2(fN, fD);
    fPitchPI = p;
  }

  fG = sqrt((fAX * fAX)  + (fAY * fAY) + (fAZ * fAZ)) / 1000.0;

	///------------------------------------------------------------------------///
	/// ROLL
	///------------------------------------------------------------------------///
	fN = pVscMode->fGsenYNow;
	fD = pVscMode->fGsenZNow;
  r  = atan2(fN, fD); 
  fRollPI = r;
  
  fPitch = fPitchPI * 180 / PI;
  fRoll  = fRollPI * 180 / PI;

	///----------------------------------------------------------------////
	/// Normal
	///----------------------------------------------------------------///
  // pVscMode->fGsenPoll  = fRoll;
  // pVscMode->fGsenPitch = fPitch;
  
	///----------------------------------------------------------------////
	/// Pitch Roll swap
	///----------------------------------------------------------------///
  pVscMode->fGsenPoll  = fPitch;
  pVscMode->fGsenPitch = fRoll * -1;

  pVscMode->fGsenG = fG;
}
