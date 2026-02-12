/**
 * @file Jtm.cpp 
 * @brief Generaic Function 
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology Co., Ltd., all rights reserved.
 * @note
*/
#include "Global.h"

void JTMPrint(JTM *pJtm)
{
	printf("\tJTM = %04d/%02d/%02d-%02d:%02d:%02d\r\n", 
				pJtm->iYear,
				pJtm->iMonth,
				pJtm->iDay,
				pJtm->iHour,
				pJtm->iMin,
				pJtm->iSec);
}

void JTMTimeZoneSecSet(JTM *pJtm, JINT iTimeZoneSec)
{
	pJtm->iTimeZoneSec = iTimeZoneSec;
}

void JTMLocalTimeGet(JTM *pJtm, time_t t)
{	
	struct tm tmNow  =  *localtime(&t); 

	///-----------------------------------///
	/// Set current time
	///-----------------------------------///  
	pJtm->iYear  = tmNow.tm_year + 1900;
	pJtm->iMonth = tmNow.tm_mon  + 1;
	pJtm->iDay   = tmNow.tm_mday;
	pJtm->iHour  = tmNow.tm_hour;
	pJtm->iMin   = tmNow.tm_min;
	pJtm->iSec   = tmNow.tm_sec;
	pJtm->t      = t;	
}

void JTMGmtTimeGet(JTM *pJtm, time_t t)
{	
	struct tm tmNow  =  *gmtime(&t);  
	///-----------------------------------///
	/// Set current time
	///-----------------------------------///  
	pJtm->iYear  = tmNow.tm_year + 1900;
	pJtm->iMonth = tmNow.tm_mon  + 1;
	pJtm->iDay   = tmNow.tm_mday;
	pJtm->iHour  = tmNow.tm_hour;
	pJtm->iMin   = tmNow.tm_min;
	pJtm->iSec   = tmNow.tm_sec;
	pJtm->t      = t;
}

JINT JTMLocalTimeShitGet(JTM *pJtm)
{	
	struct tm tmLocal;
	struct tm * ptmGmt;
	time_t tLocal = 0;
	time_t tGmt   = 0;
	JINT iYear 	= 2023;
	JINT iMonth = 9;
	JINT iDay 	= 22;
	JINT iHour 	= 14;
	JINT iMin 	= 0;
	JINT iSec 	= 0;

	JINT iShift  = 0;

	tmLocal.tm_year = iYear  - 1900;
  tmLocal.tm_mon 	= iMonth - 1;
  tmLocal.tm_mday = iDay;
  tmLocal.tm_hour = iHour;
  tmLocal.tm_min 	= iMin;
  tmLocal.tm_sec 	= iSec; 
	tmLocal.tm_isdst = 0;    

  tLocal = mktime(&tmLocal);
	ptmGmt = gmtime(&tLocal);   
  tGmt   = mktime(ptmGmt);

	iShift  = tLocal - tGmt;
	//printf("\t [JTM] LOCAL =%ld, GMT= %ld, SHIFT = %d\r\n", tLocal, tGmt, iShift);

	return iShift;
}

void JTMMakeTimeLocal(JTM *pJtm, JINT iYear, JINT iMonth, JINT iDay, JINT iHour, JINT iMin, JINT iSec)
{	
	struct tm tmTime;
	
	pJtm->iYear  = iYear;
	pJtm->iMonth = iMonth;
	pJtm->iDay   = iDay;
	pJtm->iHour  = iHour;
	pJtm->iMin   = iMin;
	pJtm->iSec   = iSec;

  tmTime.tm_year 	= pJtm->iYear  - 1900;
  tmTime.tm_mon 	= pJtm->iMonth - 1;
  tmTime.tm_mday 	= pJtm->iDay;
  tmTime.tm_hour 	= pJtm->iHour;
  tmTime.tm_min 	= pJtm->iMin;
  tmTime.tm_sec 	= pJtm->iSec; 
	tmTime.tm_isdst = 0;    

  pJtm->t = mktime(&tmTime);

	//printf("\t [JTM] LOCAL TIME: %04d/%02d/%02d-%02d:%02d:%02d, UTC = %d\r\n" ,
	//					iYear, iMonth, iDay,
	//					iHour, iMin,iSec,
	//					(JINT) pJtm->t	
	//			);
}

void JTMMakeTimeGmt(JTM *pJtm, JINT iYear, JINT iMonth, JINT iDay, JINT iHour, JINT iMin, JINT iSec)
{	
	struct tm tmTime;

	JINT iShift = JTMLocalTimeShitGet(pJtm);
	
	pJtm->iYear  = iYear;
	pJtm->iMonth = iMonth;
	pJtm->iDay   = iDay;
	pJtm->iHour  = iHour;
	pJtm->iMin   = iMin;
	pJtm->iSec   = iSec;

  tmTime.tm_year 	= pJtm->iYear  - 1900;
  tmTime.tm_mon 	= pJtm->iMonth - 1;
  tmTime.tm_mday 	= pJtm->iDay;
  tmTime.tm_hour 	= pJtm->iHour;
  tmTime.tm_min 	= pJtm->iMin;
  tmTime.tm_sec 	= pJtm->iSec; 
	tmTime.tm_isdst = 0;    

  pJtm->t = mktime(&tmTime) + iShift;

	//printf("\t [JTM] GMT TIME: %04d/%02d/%02d-%02d:%02d:%02d, UTC = %d\r\n" ,
	//					iYear, iMonth, iDay,
	//					iHour, iMin,iSec,
	//					(JINT) pJtm->t	
	//			);
}
