/**
 * @file Util.h 
 * @brief Utility header
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
#ifndef __UTIL_H__
#define __UTIL_H__ 

#define TIMEZONE_OFFSET	 (8 * 3600);

#define MEMCMP_SAME      (0)
#define MEMCMP_DIFF      (1)

///----------------------------------------------------------------------///
/// Util
///----------------------------------------------------------------------///
extern void JPrintfW( const wchar_t* format, ... );
extern void JPrintf(JUINT dwDID, const char* format, ... );

extern void   UtilErrPrint(const char* format, ... );

extern void   UtilMemcpyInt(JINT *dst, JINT *src, JINT iCnt);
extern void   UtilMemcpy(JBYTE *dst, JBYTE *src, JINT iCnt);
extern void   UtilMemset(JBYTE *dst, JBYTE bVal, JINT iCnt);

extern JINT   UtilNanoSleep(time_t sec, long nanosec);
extern JINT   UtilUsSleep(long us);
extern JINT   UtilMsSleep(long ms);

extern JINT   UtilFileExisted(char *folderPath);
extern JINT   UtilFileDelete(char *strFilePath);
extern JINT   UtilFileCopy(char *strSrcFilePath, char *strDstFilePath);
extern JINT   UtilFileSizeGet(char *strFilePath);

extern JINT   UtilFolderCreate(char *folderPath);
extern JINT   UtilFolderExisted(char *folderPath);
extern JINT   UtilFolderDelete(char *stFolder);

extern void   UtilDataPrint(JUINT dwDID, JBYTE *pbData, JINT iDataCnt);

extern void   UtilTimeStringGet(JFLOAT fTimeSec, char *strTime);

extern JINT   UtilTimeZoneGet(JDevice *pDevData);

extern void   UtilOSTypeGet(JINT *piOSType);
extern void 	UtilWinPathNameSet(char *pPathName);

extern JINT   UtilHexStringToBytes(char *str,JBYTE *out);
extern JBOOL  UtilMemcmp(JBYTE *dst, JBYTE *src, JINT iCnt);

extern JBOOL  UtilNumCharCheck(JDevice *pDevice, JBYTE *pbData, JINT iLen);

extern JFLOAT UtilLineAngleGet(JFLOAT x1, JFLOAT y1,JFLOAT x2, JFLOAT y2);
extern JINT   UtilAgeCalculate(JINT iYear, JINT iMonth, JINT iDay);

extern unsigned long TickCountGet();
extern unsigned long TickUSCountGet();
extern void UtilIdLevelGet(JINT iID, JINT *piLevel1, JINT *piLevel2, JINT *piLevel3);

extern void UtilFileFolderGet(char *strFilePath, char *pDir);
extern void UtilFileNameGet(char *strFilePath, char *pFileName);

extern void UtilSocketInfoPrint(JDevice *pDevice);

extern JINT UtilSpaceTrim(char * strLine);

#endif ///<  __UTIL_H__
