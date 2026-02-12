/**
 * @file JStat.h 
 * @brief JStat.h statistic related function 
 * @version $Revision$
 * @author JL Juang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2023,  VitalSigns Technology Co., LTD., all rights reserved.
 * @note
*/
#ifndef __JSTAT_H__
#define __JSTAT_H__

#include "Global.h"

#define STAT_VALUE_MAX       (999999999)
#define STAT_VALUE_MIN       (-999999999)

///----------------------------------------------------------------///
///  Statictic definition
///----------------------------------------------------------------///
#define STAT_TYPE_MIN       (0)   ///< Statistic type
#define STAT_TYPE_HOUR      (1)
#define STAT_TYPE_DAY       (2)

///----------------------------------------------------------------///
///  Statictic object
///----------------------------------------------------------------///
#define STAT_TYPE_MIN       (0)   ///< Statistic type
#define STAT_TYPE_HOUR      (1)
#define STAT_TYPE_DAY       (2)
typedef struct JStatST
{
  char      strName[64];
  JINT      iCount; 
  JDOUBLE   fValSum;
  JDOUBLE   fValMax;
  JDOUBLE   fValMin;
  JDOUBLE   fValAvg;

  JBOOL     bTHMin;
  JBOOL     bTHMax;  
  JDOUBLE   fTHMax;
  JDOUBLE   fTHMin;  
} JStat;

///----------------------------------------------------------------///
///  Statictic function
///----------------------------------------------------------------///
extern void JStatAdd(JStat  * pStat, JDOUBLE fValue);
extern void JStatInit(JStat * pStat, JBOOL bTHMin, JBOOL bTHMax, JDOUBLE fTHMin, JDOUBLE fTHMax);

#endif ///< #define __JSTAT_H__
