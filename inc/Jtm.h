/**
 * @file Jtm.h 
 * @brief TM function
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
#ifndef __JTM_H__
#define __JTM_H__ 

#include "Global.h"

typedef struct JTMSt
{
	JINT    iYear;
	JINT    iMonth;
	JINT    iDay;
	JINT    iHour;
	JINT    iMin;
	JINT    iSec;	
	JINT    iMS;			
	JINT 		iTimeZoneSec;
	JDWORD 	t;	
} JTM;

extern void   JTMPrint(JTM *pJtm);
extern void   JTMTimeZoneSecSet(JTM *pJtm, JINT iTimeZoneSec);
extern void 	JTMLocalTimeGet(JTM *pJtm, time_t t);
extern void 	JTMGmtTimeGet(JTM *pJtm, time_t t);

extern JINT 	JTMLocalTimeShitGet(JTM *pJtm);
extern void 	JTMMakeTimeLocal(JTM *pJtm, JINT iYear, JINT iMonth, JINT iDay, JINT iHour, JINT iMin, JINT iSec);
extern void 	JTMMakeTimeGmt(JTM *pJtm, JINT iYear, JINT iMonth, JINT iDay, JINT iHour, JINT iMin, JINT iSec);

#endif ///<  __JTM_H__
