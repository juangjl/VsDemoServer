/**
 * @file Location.h
 * @brief  Location functions
 * @version $Revision$
 * @author JL JUANG <jl_juang@vsigntek.com>
 * @note Copyright 2025, VitalSigns Technology Co., Ltd., all rights reserved.
 * @note
*/ 
#include "Global.h"

#ifndef __LOCATION_H__
#define __LOCATION_H__

///----------------------------------------------------------------------------------------------------------------///
///
/// Macro
///
///----------------------------------------------------------------------------------------------------------------///


///----------------------------------------------------------------------------------------------------------------///
///
/// Type
///
///----------------------------------------------------------------------------------------------------------------///
typedef struct LocationDataST
{
  ///----------------------------------------------------------------///
  /// GPS
  ///----------------------------------------------------------------///
  JINT    iGpsFixed;      ///< 00 -> 00-04 : Fixed 1 , not fixed. 0
  JDWORD  dwGpsUtc;       ///< 01 -> 04-08 : UTC now 
  JDWORD  dwGpsUtcFixed;  ///< 02 -> 08-12 : Latest fixed UTC 
  JFLOAT  fGpsLat;        ///< 03 -> 12-16 : Latitude of GPS
  JFLOAT  fGpsLon;        ///< 04 -> 16-20 : Longitude of GPS
  JFLOAT  fGpsAlt;        ///< 05 -> 20-24 : Altitude
  JFLOAT  fGpsVel;        ///< 06 -> 24-28 : Velocity
  JFLOAT  fGpsCog;        ///< 07 -> 28-32 : Course Over Ground (COG)

  ///----------------------------------------------------------------///
  /// AGPS
  ///----------------------------------------------------------------///
  JINT    iAGpsFixed;     ///< 08 -> 32-36 : Fixed 1 , not fixed. 0
  JDWORD  dwAGpsUtc;      ///< 09 -> 36-40 : UTC now     
  JDWORD  dwAGpsUtcFixed; ///< 10 -> 40-44 : Latest fixed UTC
  JFLOAT  fAGpsLat;       ///< 11 -> 44-48 : Latitude
  JFLOAT  fAGpsLon;       ///< 12 -> 48-52 : Longitude
  JFLOAT  fAGpsAlt;       ///< 13 -> 52-56 : Altitude
  JFLOAT  fAGpsError;     ///< 14 -> 56-60 : Error of AGPS  
  JINT    iDummy0;        ///< 15 -> 60-64         
  JINT    iDummy1;        ///< 16 -> 64-68         
  JINT    iDummy2;        ///< 17 -> 68-72         
} LocationDataType;

///----------------------------------------------------------------------------------------------------------------///
///
/// Variables
///
///----------------------------------------------------------------------------------------------------------------///
extern LocationDataType LocationData;

///----------------------------------------------------------------------------------------------------------------///
///
/// Functions
///
///----------------------------------------------------------------------------------------------------------------///
extern void LocationGpsDataPrint(JDevice *pDevice);

extern void LocationInfoFileWrite(JUINT dwDID, char *strFileName, JBOOL bAppend);

#endif ///< __LOCATION_H__
