/**
 * @file Func.cpp
 * @brief  Function control
 * @version $Revision$
 * @author JLJuang <jl_juang@vsgntek.com>
 * @note Copyright (c) 2024, VitalSigns Technology Co., Ltd, all rights reserved.
 * @note
*/
#include "Global.h"
JFileListType DeviceFileList;

JINT FuncDeviceEcgDataSetEx(JDevice *pDevice, JDeviceEcg * pDeviceEcg, VscModeControlType * pVscModeCtl)
{
  JGatewayInfo *pGatewayObject = &pDevice->gatewayInfo;

  JFLOAT fGsenPower = sqrt(pVscModeCtl->fGsenXNow * pVscModeCtl->fGsenXNow + 
                           pVscModeCtl->fGsenYNow * pVscModeCtl->fGsenYNow + 
                           pVscModeCtl->fGsenZNow * pVscModeCtl->fGsenZNow);
        
  pDeviceEcg->iDid        			= pGatewayObject->iBleOpenDid;
  pDeviceEcg->iYear             = pVscModeCtl->jtm.iYear;
  pDeviceEcg->iMonth            = pVscModeCtl->jtm.iMonth;
  pDeviceEcg->iDay              = pVscModeCtl->jtm.iDay;
  pDeviceEcg->iHour             = pVscModeCtl->jtm.iHour;
  pDeviceEcg->iMin              = pVscModeCtl->jtm.iMin;
  pDeviceEcg->iSec              = pVscModeCtl->jtm.iSec;
  pDeviceEcg->iHR               = pVscModeCtl->fHRNow;
  pDeviceEcg->fTemp0            = pVscModeCtl->fTemp;
  pDeviceEcg->iBatLevel         = pVscModeCtl->fBattSoc;
  pDeviceEcg->fGsenPower        = fGsenPower;
  pDeviceEcg->iGSenX            = pVscModeCtl->fGsenXNow;
  pDeviceEcg->iGSenY            = pVscModeCtl->fGsenYNow;
  pDeviceEcg->iGSenZ            = pVscModeCtl->fGsenZNow;

  pDeviceEcg->fRRAvg            = pVscModeCtl->fHrvRR;
  pDeviceEcg->fSDNN             = pVscModeCtl->fHrvSDNN;
  pDeviceEcg->iNN50             = pVscModeCtl->fHrvNN50;
  pDeviceEcg->fRMSSD            = pVscModeCtl->fHrvRMSSD;
  pDeviceEcg->fVLF              = pVscModeCtl->fHrvVLF;
  pDeviceEcg->fLF               = pVscModeCtl->fHrvLF;
  pDeviceEcg->fHF               = pVscModeCtl->fHrvHF;
  pDeviceEcg->fTP               = pVscModeCtl->fHrvTP;
  pDeviceEcg->fLFHF             = pVscModeCtl->fHrvLFHF;
  pDeviceEcg->fLFTP             = pVscModeCtl->fHrvLFTP;
  pDeviceEcg->fHFTP             = pVscModeCtl->fHrvHFTP;
  pDeviceEcg->fQTTime           = pVscModeCtl->fQTTime;
  pDeviceEcg->fQRSTime          = pVscModeCtl->fQRSTime;
  pDeviceEcg->fPRTime           = pVscModeCtl->fPRTime;
  pDeviceEcg->fBar              = pVscModeCtl->fBar;
  pDeviceEcg->fBarTemp          = pVscModeCtl->fBarTemp;

  pDeviceEcg->iLeadOff          = pVscModeCtl->fLeadOff;

  pDeviceEcg->iTimeDelta        = 0;

  pDeviceEcg->dwConnected       = TRUE;

	pDeviceEcg->iGatewayDID       = pDevice->dwDID;        

  return TRUE;
}

JINT FuncDeviceEcgDataSet(JDevice *pDevice, JDeviceEcg* pDeviceEcg, DeviceEcgType * pEcgObject)
{
	pDeviceEcg->iDid        			= pEcgObject->dwDid;
  pDeviceEcg->iYear             = pEcgObject->ecgInfoItem.iYear;
  pDeviceEcg->iMonth            = pEcgObject->ecgInfoItem.iMonth;
  pDeviceEcg->iDay              = pEcgObject->ecgInfoItem.iDay;
  pDeviceEcg->iHour             = pEcgObject->ecgInfoItem.iHour;
  pDeviceEcg->iMin              = pEcgObject->ecgInfoItem.iMin;
  pDeviceEcg->iSec              = pEcgObject->ecgInfoItem.iSec;
  pDeviceEcg->iHR               = pEcgObject->ecgInfoItem.iHR;
  pDeviceEcg->fTemp0            = pEcgObject->ecgInfoItem.fTemp;
  pDeviceEcg->iBatLevel         = pEcgObject->ecgInfoItem.iBatLevel;
  pDeviceEcg->fGsenPower        = pEcgObject->ecgInfoItem.fPower;
  pDeviceEcg->iGSenX            = pEcgObject->ecgInfoItem.iGsenX;
  pDeviceEcg->iGSenY            = pEcgObject->ecgInfoItem.iGsenY;
  pDeviceEcg->iGSenZ            = pEcgObject->ecgInfoItem.iGsenZ;
  pDeviceEcg->iMaxGSenX         = pEcgObject->ecgInfoItem.iMaxGsenX;
  pDeviceEcg->iMaxGSenY         = pEcgObject->ecgInfoItem.iMaxGsenY;
  pDeviceEcg->iMaxGSenZ         = pEcgObject->ecgInfoItem.iMaxGsenZ;
  pDeviceEcg->fRRAvg            = pEcgObject->ecgInfoItem.fRRAvg;
  pDeviceEcg->fSDNN             = pEcgObject->ecgInfoItem.fSDNN;
  pDeviceEcg->iNN50             = pEcgObject->ecgInfoItem.iNN50;
  pDeviceEcg->fRMSSD            = pEcgObject->ecgInfoItem.fRMSSD;
  pDeviceEcg->fVLF              = pEcgObject->ecgInfoItem.fVLF;
  pDeviceEcg->fLF               = pEcgObject->ecgInfoItem.fLF;
  pDeviceEcg->fHF               = pEcgObject->ecgInfoItem.fHF;
  pDeviceEcg->fTP               = pEcgObject->ecgInfoItem.fTP;
  pDeviceEcg->fLFHF             = pEcgObject->ecgInfoItem.fLFHF;
  pDeviceEcg->fLFTP             = pEcgObject->ecgInfoItem.fLFTP;
  pDeviceEcg->fHFTP             = pEcgObject->ecgInfoItem.fHFTP;
  pDeviceEcg->fQTTime           = pEcgObject->ecgInfoItem.fQTTime;
  pDeviceEcg->fQRSTime          = pEcgObject->ecgInfoItem.fQRSTime;
  pDeviceEcg->fPRTime           = pEcgObject->ecgInfoItem.fPRTime;
  pDeviceEcg->fBar              = pEcgObject->ecgInfoItem.fBar;
  pDeviceEcg->fBarTemp          = pEcgObject->ecgInfoItem.fBarTemp;
  pDeviceEcg->fBarHeight        = pEcgObject->ecgInfoItem.fBarHeight;
  pDeviceEcg->iLeadOff          = pEcgObject->ecgInfoItem.iLeadOff;
  pDeviceEcg->iRssi             = pEcgObject->iRssi;
  pDeviceEcg->iTimeDelta        = pEcgObject->wTm;
	pDeviceEcg->dwAlarm           = pEcgObject->dwAlarm;
  pDeviceEcg->dwConnected       = pEcgObject->ecgInfoItem.wConnected;
  pDeviceEcg->iTimeZone         = pEcgObject->ecgInfoItem.iTimeZoneSec;
	pDeviceEcg->iVersion          = pEcgObject->ecgInfoItem.wVersion;
	pDeviceEcg->iBatVolt          = pEcgObject->ecgInfoItem.iBatVolt;
	pDeviceEcg->dwChargerStatus   = pEcgObject->ecgInfoItem.wChargerStatus; ///< the type are different

	pDeviceEcg->iGatewayDID       = pDevice->dwDID;

  UtilMemcpy((JBYTE *)&pDeviceEcg->strSSN[0], (JBYTE *)&pEcgObject->strSSN[0], SSN_SIZE);
  UtilMemcpy((JBYTE *)&pDeviceEcg->bAddr[0],  (JBYTE *)&pEcgObject->bAddr[0],  BLE_MAC_ADDR_SIZE);

  return TRUE;
}
