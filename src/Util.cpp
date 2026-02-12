/**
 * @file util.cpp 
 * @brief utility library 
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
#include "Global.h"

void JPrintf(JUINT dwDID, const char* format, ... )
{
  JTM jtm;
  JTM * pJtm = &jtm;
  char buffer[2048];
  va_list args;
	char fileName[256];

  time_t t = time(NULL);
  
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long ms = tp.tv_usec / 1000;

  JTMLocalTimeGet(pJtm, t);

	/// Print Filter
	if(dwDID != 0) 
	{		
		if(GlobalVar.iPrintFilterDID != dwDID)
		{
			return;
		}		
	}

  if(dwDID == 0)
	{
	  printf("[%02d-%02d %02d:%02d:%02d.%03d] ", 
              pJtm->iMonth, pJtm->iDay, 
              pJtm->iHour, pJtm->iMin, pJtm->iSec,
              (JINT)ms);
	}
  else
	{
	  printf("[%02d-%02d %02d:%02d:%02d.%03d][DID_%07d] ", 
              pJtm->iMonth, pJtm->iDay, 
              pJtm->iHour, pJtm->iMin, pJtm->iSec,
              (JINT)ms, dwDID
              );
	}

  va_start(args, format);
  vsprintf(buffer, format, args);
  fputs(buffer, stdout);
  va_end(args);
}

void JPrintfW(const wchar_t* format, ... )
{
  JTM jtm;
  JTM * pJtm = &jtm;
  wchar_t buffer[10000];
  va_list args;

  time_t t = time(NULL);
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long ms = tp.tv_usec / 1000;
  
  JTMLocalTimeGet(pJtm, t);

  wprintf(L"[%02d-%02d %02d:%02d:%02d.%03d] ", 
              pJtm->iMonth, 
              pJtm->iDay,
              pJtm->iHour, pJtm->iMin, pJtm->iSec,
              ms);

  va_start(args, format);
  vswprintf(buffer, 2048, format, args);
  fputws(buffer, stdout);
  va_end(args);
}

unsigned long TickCountGet()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
  return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

unsigned long TickUSCountGet()
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (ts.tv_sec * 1000000UL + ts.tv_nsec / 1000);
}

///-------------------------------------------------///
/// @Function: UtilFileExisted
/// @Brief: folder create
///-------------------------------------------------///
JINT UtilFileExisted(char *folderPath)
{
	if(access( folderPath, F_OK ) != -1 )
	{
		/// file exists
		return TRUE;
	}
	else
	{
	  /// file doesn't exist
    return FALSE;
	}

	return TRUE;
}

///-------------------------------------------------///
/// @Function: UtilFileDelete
/// @Brief: file delete
///-------------------------------------------------///
JINT UtilFileDelete(char *strFilePath)
{
  char strCmd[512];
  sprintf(strCmd, "rm -f %s", strFilePath);
  system(strCmd);

	return TRUE;
}

///-------------------------------------------------///
/// @Function: UtilFileCopy
/// @Brief: file copy (src -> dst)
///-------------------------------------------------///
JINT UtilFileCopy(char *strSrcFilePath, char *strDstFilePath)
{
  FILE  *fpSrc = NULL;
  FILE  *fpDst = NULL;
  char   buf[4096];
  size_t rdSize = 0;
  size_t wrSize = 0;

  /// open source file
  fpSrc = fopen(strSrcFilePath, "rb");
  if(fpSrc == NULL)
  {
    return FALSE;
  }

  /// open/create dest file
  fpDst = fopen(strDstFilePath, "wb");
  if(fpDst == NULL)
  {
    fclose(fpSrc);
    return FALSE;
  }

  /// copy loop
  while(1)
  {
    rdSize = fread(buf, 1, sizeof(buf), fpSrc);
    if(rdSize > 0)
    {
      wrSize = fwrite(buf, 1, rdSize, fpDst);
      if(wrSize != rdSize)
      {
        fclose(fpSrc);
        fclose(fpDst);
        return FALSE;
      }
    }

    /// if read size < buffer, check if EOF or error
    if(rdSize < sizeof(buf))
    {
      if(feof(fpSrc))
      {
        break;  ///< normal end of file
      }
      if(ferror(fpSrc))
      {
        fclose(fpSrc);
        fclose(fpDst);
        return FALSE;
      }
    }
  }

  fclose(fpSrc);
  fclose(fpDst);

  return TRUE;
}

///-------------------------------------------------///
/// @Function: UtilFileSizeGet
/// @Brief: get file size in bytes
///-------------------------------------------------///
JINT UtilFileSizeGet(char *strFilePath)
{
  struct stat st;
  JINT iFileSize = -1;

  if(strFilePath == NULL)
  {
    return iFileSize;
  }

  /// check file existed
  if(access(strFilePath, F_OK) != -1)
  {
    /// get stat info
    if(stat(strFilePath, &st) == 0)
    {
      iFileSize = (JINT)st.st_size;
      return iFileSize;
    }
    else
    {
      /// stat failed      
      return iFileSize;
    }
  }
  else
  {
    /// file not existed    
    return iFileSize;
  }

  return iFileSize;
}

///-------------------------------------------------///
/// @Function: UtilFolderCreateRecursive
/// @Brief: recursively create parent folders
///-------------------------------------------------///
static JINT UtilFolderCreateRecursive(char *folderPath)
{
  char strPath[512];
  char *parent = NULL;
  struct stat st = {0};
  mode_t oldMask = 0;

  /// Already exists
  if(stat(folderPath, &st) == 0)
  {
    return TRUE;
  }

  strncpy(strPath, folderPath, sizeof(strPath));
  strPath[sizeof(strPath)-1] = 0;

  parent = dirname(strPath);

  /// Recursively create parent
  if((strlen(parent) > 1) && (strcmp(parent, "/") != 0))
  {
    if(stat(parent, &st) != 0)
    {
      UtilFolderCreateRecursive(parent);
    }
  }

  /// Create this folder
  oldMask = umask(0);
  if(mkdir(folderPath, 0775) != 0)
  {
    umask(oldMask);
    return FALSE;
  }
  umask(oldMask);

  return TRUE;
}

///-------------------------------------------------///
/// @Function: UtilFolderCreate
/// @Brief: folder create
///-------------------------------------------------///
JINT UtilFolderCreate(char *folderPath)
{
  char strPath[512];
  gid_t parentGid = 0;
  struct stat pst;
  char *parent = NULL;
  mode_t oldMask = 0;
  struct stat st = {0};

  if(stat(folderPath, &st) != -1) 
  {
    return FALSE;
  }

  strncpy(strPath, folderPath, sizeof(strPath));
  strPath[sizeof(strPath)-1] = 0;

  parent = dirname(strPath);

  if(stat(parent, &pst) != 0) 
  {
    UtilFolderCreateRecursive(parent);
    if(stat(parent, &pst) != 0)
    {
      perror("stat parent fail after recursive create");
      return FALSE;
    }
  }

  parentGid = pst.st_gid;

  oldMask = umask(0);
  if(mkdir(folderPath, 0775) != 0)
  {
    perror("mkdir failed");
    umask(oldMask);
    return FALSE;
  }
  umask(oldMask);

  // setting same group www-data
  if(chown(folderPath, (uid_t)33, parentGid) != 0)
  {
    perror("chown failed");
  }

	return TRUE;
}

///-------------------------------------------------///
/// @Function: UtilFolderDelete
/// @Brief: folder delete
///-------------------------------------------------///
JINT UtilFolderDelete(char *strFolder)
{
  char strCmd[512];
  sprintf(strCmd, "rm -fR %s", strFolder);
  system(strCmd);

	return TRUE;
}

///-------------------------------------------------///
/// @Function: UtilFolderExisted
/// @Brief: folder existed check
///-------------------------------------------------///
JINT UtilFolderExisted(char *folderPath)
{
  struct stat st= {0};

  if(stat(folderPath, &st) == 0 && S_ISDIR(st.st_mode)) 
  {
    return TRUE;
  }
  else 
  {
    return FALSE;
  }
}

void UtilMemcpy(JBYTE *dst, JBYTE *src, JINT iCnt)
{
	JINT i;
	for(i = 0; i < iCnt ; i = i + 1)
	{
		*dst = *src;
		dst++;
		src++;		
	}
}

JBOOL UtilMemcmp(JBYTE *dst, JBYTE *src, JINT iCnt)
{
  JINT i;
  for(i = 0; i < iCnt ; i = i + 1)
  {
    if(*dst != *src)
    {
      return MEMCMP_DIFF;
    }
    dst++;
    src++;
  }
  return MEMCMP_SAME;
}

void UtilMemset(JBYTE *dst, JBYTE bVal, JINT iCnt)
{
	JINT i;
	for(i = 0; i < iCnt ; i = i + 1)
	{
		*dst = bVal;
		dst++;	
	}
}

JINT UtilNanoSleep(time_t sec, long nanosec)
{
   /// Setup timespec
   struct timespec req;
   req.tv_sec  = sec;
   req.tv_nsec = nanosec;
   nanosleep( &req, &req );  
   return 0; 
}

JINT UtilUsSleep(long us)
{
   /// Setup timespec
   struct timespec req;
   req.tv_sec  = 0;
   req.tv_nsec = us * 1000;
   nanosleep( &req, &req ); 
	return 0;
}

JINT UtilMsSleep(long ms)
{
	/// Setup timespec
	struct timespec req;
	req.tv_sec  = (ms / 1000);
	req.tv_nsec = (ms % 1000) * 1000000;
	nanosleep(&req, &req); 
	return 0;
}

void UtilErrPrint(const char* format, ... )
{
  JTM  jtm;
  JTM  *pJtm = &jtm;
  time_t  t = time(NULL);

  char buffer[2048];
  va_list args;
	char fileName[256];
	
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long  ms = tp.tv_usec / 1000;

	JTMLocalTimeGet(pJtm, t);	

  printf("[%02d-%02d %02d:%02d:%02d.%03d] ", 
            pJtm->iMonth, pJtm->iDay,
            pJtm->iHour, pJtm->iMin, pJtm->iSec,
            (JINT)ms);
  
  va_start ( args, format );
  vsprintf ( buffer, format, args );
  fputs ( buffer, stdout );
  va_end ( args );
  
	sprintf(fileName, "./log/err-%04d-%02d-%02d.log", 
				  jtm.iYear,
					jtm.iMonth,
				  jtm.iDay);
	FILE *fp = fopen(fileName, "a+");

	if(fp!= NULL)
	{
	
		fprintf(fp, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n", 
							jtm.iYear,
							jtm.iMonth,
							jtm.iDay,
							jtm.iHour, 
							jtm.iMin,
							jtm.iSec,
 						  buffer);
		fclose(fp);
	}
	else
	{
		printf("Fail to open the err print\r\n");	
	}
}

void UtilDataPrint(JUINT dwDID, JBYTE *pbData, JINT iDataCnt)
{
	char 	msg[256];
	JINT 	i = 0;	
	JINT 	iDisplayCnt = 0;
	char  ch = 0;
	char  strText[256];

	if((iDataCnt % 16) != 0)
	{
		iDisplayCnt = iDataCnt - (iDataCnt % 16) + 16;
	}
	else
	{
		iDisplayCnt = iDataCnt;
	}
	
	/// print title
	sprintf(msg, "\t\tData count = %d\r\n", iDataCnt);
	DBG_PRINTF(dwDID, msg);
	
	for(i = 0 ; i < iDisplayCnt; i = i + 1)
	{
		if((i % 16) == 0)
		{
			msg[0] = 0;
			strText[0] = 0;
			sprintf(msg, "\t\t%04X-%04X :", i, i+ 15);
		}
		if(i < iDataCnt)
		{
			sprintf(msg + strlen(msg), "%02X ", pbData[i]);
			ch = pbData[i];
			if((ch >= 48) && (ch<= 127))
			{
				sprintf(strText + strlen(strText), "%c", pbData[i]);
			}
			else
			{
				sprintf(strText + strlen(strText), "%c", '.');
			}
		}
		else
		{
			sprintf(msg + strlen(msg), "%c%c ", 'x', 'x');
			sprintf(strText + strlen(strText), "%s", ".");						
		}

		if((i % 16) == 7)
		{
			sprintf(msg + strlen(msg), "%s", "- ");
			sprintf(strText + strlen(strText), "%s", " ");									
		}

		if((i % 16) == 15)
		{
			sprintf(msg + strlen(msg), "%s\r\n", strText);
			DBG_PRINTF(dwDID, msg);
		}
	}

	msg[0] = 0;
	sprintf(msg, "%s", "\r\n");
	DBG_PRINTF(dwDID, msg);	
}

void UtilTimeStringGet(JFLOAT fTimeSec, char *strTime)
{
	JINT iMSTotal  	= (JINT) (fTimeSec * 1000);
	JINT iMS  		  = (iMSTotal % 1000);
	JINT iSecTotal  = ((iMSTotal  - iMS)) / 1000;	

	JINT iSec      	= (iSecTotal) % 60;	
	JINT iMinTotal 	= (iSecTotal - iSec) / 60;		
	JINT iMin		 		= (iMinTotal) % 60;		
	JINT iHour 			= (iMinTotal - iMin) / 60;		

	sprintf(strTime, "%02d:%02d:%02d_%03d", iHour, iMin, iSec, iMS);
}

JINT UtilHexStringToBytes(char * pStrIn, JBYTE * pStrOut)
{
  JINT iLen = 0;
  JINT i = 0;
  char ch = 0;
  JINT iCnt = 0;
  JINT iValue = 0;  
  char pStrBuf[512];
  
  JBYTE bL = 0;
  JBYTE bH = 0;

  strcpy(pStrBuf, pStrIn);
  iLen = strlen(pStrBuf);
  if(iLen == 1)
  {
    pStrBuf[1] = pStrIn[0];
    pStrBuf[0] = '0';
    iLen = 2;
  }

  for(i = 0 ; i < iLen ; i = i + 1)
  {
    ch = pStrBuf[i];
    /// check the char data
    if((ch >= '0') && (ch <='9'))
    {
      iValue = (ch - '0');
    }
    else if((ch >= 'A') && (ch <= 'F'))
    {
      iValue = (ch - 'A') + 10;
    }
    else if((ch >= 'a') && (ch <= 'f'))
    {
      iValue = (ch - 'a') + 10;
    }
    else
    {
      /// error input data return zero
      return 0;
    }    
    
    if((i % 2) == 0)
    {
      bH = iValue;
      bL = 0;
    }
    else
    {
      bL = iValue;
      pStrOut[iCnt] = (bH << 4) + bL;    
      iCnt = iCnt + 1;        
    }    
  }  	
  
  return iCnt;
}

JINT UtilTimeZoneGet(JDevice *pDevice)
{
  time_t now;
  struct tm tm_local;
  struct tm tm_utc;

  time(&now);

  localtime_r(&now, &tm_local);
  gmtime_r(&now, &tm_utc);

  time_t t_local = mktime(&tm_local);
  time_t t_utc   = mktime(&tm_utc);

  JINT iOffsetSec = (JINT)difftime(t_local, t_utc);

  DBG_PRINTF(pDevice->dwDID,
             "[INFO]TIMEZONE OFFSET = %d SEC (%d HOURS)\r\n",
             iOffsetSec, iOffsetSec / 3600);

  return iOffsetSec;
}

void UtilOSTypeGet(JINT *piOSType)
{		
	#if OS_TYPE == OS_TYPE_UBUNTU
	*piOSType= OS_TYPE_UBUNTU;
	DBG_PRINTF(DBG_ID, "OS = UBUNTU\r\n");
	#elif OS_TYPE == OS_TYPE_MAC_OS
	*piOSType= OS_TYPE_MAC_OS;
	DBG_PRINTF(DBG_ID, "OS = MAC\r\n");
	#elif OS_TYPE == OS_TYPE_WINDOWS
	*piOSType = OS_TYPE_WINDOWS;
	DBG_PRINTF(DBG_ID, "OS = WINDOWS\r\n");
	#else
	*piOSType= OS_TYPE_UNKNOWN;
	DBG_PRINTF(DBG_ID, "\t OS = UNKNOWN\r\n");
	#endif ///< for #if
}

void UtilWinPathNameSet(char *pPathName)
{
#ifdef ARCH_WIN32
	JINT i =0;
	JINT iLen = strlen(pPathName);
	for(i = 0 ; i < iLen ; i = i + 1)
	{
		if(pPathName[i] == '/')
		{
			pPathName[i] = '\\';
		}
	}
#endif ///<for ARCH_WIN32	
}

JBOOL UtilNumCharCheck(JDevice *pDevice, JBYTE  * pbData, JINT iLen)
{
  JINT iRet = TRUE;
	JINT i = 0;
  JBYTE ch = 0;

  JBYTE bNotNum = FALSE;

  for(i = 0; i < iLen ;i = i + 1)
  {
    ch = pbData[i];
    
    /// default the SSN is 00-000-0000 , bypass the '-'
    if(ch == '-')
    {
      continue;
    }
    
    if((ch >= 48) && (ch <= 57)) ///< '0'~ '9'
    {
      bNotNum = FALSE;
    }
    else if((ch >= 65) && (ch <= 90)) ///< 'A'~ 'Z'
    {
      bNotNum = FALSE;
    }
    else if((ch >= 97) && (ch <= 122)) ///< 'a'~ 'z'
    {
      bNotNum = FALSE;
    }
    else if(ch == 0x00)
    {
      break;
    }
    else
    {
      bNotNum = TRUE;
			iRet  = FALSE;
      break;
    }

    if(ch == 0x00)
    {
      break;
    }
  }

  return iRet;
}

JFLOAT UtilLineAngleGet(JFLOAT x1, JFLOAT y1,JFLOAT x2, JFLOAT y2)
{
  // const JFLOAT PI = 3.1415926;

  // Find Vector
  JFLOAT fDX = x2 - x1;
  JFLOAT fDY = y2 - y1;
  
  // Get the vector angle
  JDOUBLE fAngle  = atan2(fDY, fDX);

  fAngle = (fAngle  * 180) / PI;
      
  return fAngle;
}

JINT UtilAgeCalculate(JINT iYear, JINT iMonth, JINT iDay)
{
  JINT iAge = 0;
  JTM jtm;
  time_t tNow;

  /// Get current time
  tNow = time(NULL);
  JTMLocalTimeGet(&jtm, tNow);

  /// Calculate age
  iAge = jtm.iYear - iYear;

  /// Check if birthday has passed this year
  if((jtm.iMonth < iMonth) || ((jtm.iMonth == iMonth) && (jtm.iDay < iDay)))
  {
    iAge = iAge - 1;
  }

  /// Ensure age is not negative
  if(iAge < 0)
  {
    iAge = 0;
  }

  return iAge;
}

void UtilIdLevelGet(JINT iID, JINT *piLevel1, JINT *piLevel2, JINT *piLevel3)
{
  if((piLevel1 == NULL) || (piLevel2 == NULL) || (piLevel3 == NULL))
  {
    return;
  }

  *piLevel3 = (iID % 1000);
  *piLevel2 = (iID / 1000) % 1000;  
  *piLevel1 = (iID / 1000000) % 1000;
}

void UtilFileFolderGet(char *strFilePath, char *pDir)
{
  JINT len = strlen(strFilePath);  
  strcpy(pDir, strFilePath);
  while(len > 0)
  {
    len = len - 1;
    if(pDir[len] == '\\' || pDir[len] == '/')
    {
      pDir[len] = '\0';
      break;
    }
  }
}

void UtilFileNameGet(char *strFilePath, char *pFileName)
{
  JINT len = strlen(strFilePath);  
  JINT idx = 0;
  
  strcpy(pFileName, strFilePath);
  pFileName[len] = 0;		
  while(1)
  {
    if(len < 0)
    {	
      break;
    }
    len = len - 1;
    if(pFileName[len] == '\\' || pFileName[len] == '/')
    {
      pFileName[len] = '\0';
      break;
    }
  }
  /// file name
  while(1)
  {
    len = len + 1;
    if(len >= (JINT)strlen(strFilePath))
    {
      break;
    }		
    pFileName[idx] = pFileName[len];
    pFileName[idx + 1] = 0;
    if(pFileName[len] == 0)
    {
      break;
    }
    idx = idx + 1;
  }
}

void UtilSocketInfoPrint(JDevice *pDevice)
{
  struct sockaddr_in c;
  struct sockaddr_in s;

  JBYTE ip[4];

  socklen_t cLen = sizeof(c);
  socklen_t sLen = sizeof(s);
  
  getsockname(pDevice->iDevSocket, (struct sockaddr*)&s, &sLen);
  getpeername(pDevice->iDevSocket, (struct sockaddr*)&c, &cLen);
  memcpy(&ip, &c.sin_addr.s_addr, 4);

  DBG_PRINTF(pDevice->dwDID, "CLIENT = %s, SERVER = %s, ADDR = %08X, iP = %d:%d:%d:%d\r\n", 
              inet_ntoa(c.sin_addr), 
              inet_ntoa(s.sin_addr), 
              c.sin_addr.s_addr, ip[0],ip[1],ip[2],ip[3]);
}

JINT UtilSpaceTrim(char * strLine)
{
  JINT i = 0;
  JINT j = 0;

  ///----------------------------------------------------------------///
  /// trim ' ' char in line
  ///----------------------------------------------------------------///
  i = 0;
  while(1)
  {
    if(strLine[i] == 0)
    {
      break;
    }
    if(strLine[i] == ' ')
    {
      j = i + 1;
      while(1)
      {
        if((j - 1) > 0)
        {
          strLine[j - 1] = strLine[j];
        }        
        if(strLine[j] == 0)
        {
          break;
        }        
        j = j + 1;
      }            
    }
    else
    {
      i = i + 1;
    }
  }

   ///----------------------------------------------------------------///
  /// trim '\t' char in line
  ///----------------------------------------------------------------///
  i = 0;
  while(1)
  {
    if(strLine[i] == 0)
    {
      break;
    }
    if(strLine[i] == '\t')
    {
      j = i + 1;
      while(1)
      {
        strLine[i] = strLine[j];
        if(strLine[j] == 0)
        {
          break;
        }        
        j = j + 1;
      }            
    }
    i = i + 1;
  }

  return TRUE;
}