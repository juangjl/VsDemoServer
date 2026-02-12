/**
 * @file JAtr.cpp 
 * @brief ATR related function 
 * @version $Revision$
 * @author JL Juang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021,  VitalSigns Technology Co., LTD., all rights reserved.
 * @note
*/
#include "Global.h"

JAtrQueueType AtrQueue;

///----------------------------------------///
/// [JL] for JCNN
///----------------------------------------///
const char * JAtrLabel[] =
{
  LABEL_NOTQRS	 ,  ///< 00 
  LABEL_NORMAL	 ,  ///< 01 
  LABEL_LBBB		 ,  ///< 02 
  LABEL_RBBB		 ,  ///< 03 
  LABEL_ABERR		 ,  ///< 04 
  LABEL_PVC		   ,  ///< 05 
  LABEL_FUSION	 ,  ///< 06 
  LABEL_NPC		   ,  ///< 07 
  LABEL_APC			 , 	///< 08 
  LABEL_SVPB		 , 	///< 09 
  LABEL_VESC		 , 	///< 10 
  LABEL_NESC		 , 	///< 11 
  LABEL_PACE		 , 	///< 12 
  LABEL_UNKNOWN	 , 	///< 13 
  LABEL_NOISE		 , 	///< 14 
  LABEL_ATR_15	 ,  ///< 15 
  LABEL_ARFCT		 , 	///< 16 
  LABEL_ATR_17	 ,  ///< 17 
  LABEL_STCH		 , 	///< 18 
  LABEL_TCH			 , 	///< 19 
  LABEL_SYSTOLE	 , 	///< 20 
  LABEL_DIASTOLE , 	///< 21 
  LABEL_NOTE		 , 	///< 22 
  LABEL_MEASURE  , 	///< 23 
  LABEL_PWAVE		 , 	///< 24 
  LABEL_BBB			 , 	///< 25 
  LABEL_PACESP	 , 	///< 26 
  LABEL_TWAVE		 , 	///< 27 
  LABEL_RHYTHM	 , 	///< 28 
  LABEL_UWAVE		 , 	///< 29 
  LABEL_LEARN		 , 	///< 30 
  LABEL_FLWAV		 , 	///< 31 
  LABEL_VFON		 , 	///< 32 
  LABEL_VFOFF		 , 	///< 33 
  LABEL_AESC		 , 	///< 34 
  LABEL_SVESC		 , 	///< 35 
  LABEL_LINK     , 	///< 36 
  LABEL_NAPC		 , 	///< 37 
  LABEL_PFUS		 , 	///< 38 
  LABEL_WFON		 , 	///< 39 
  LABEL_WFOFF		 , 	///< 40 
  LABEL_RONT		 ,  ///< 41 
	LABEL_ATR_42	 ,  ///< 42
	LABEL_ATR_43	 ,  ///< 43
	LABEL_ATR_44	 ,  ///< 44
	LABEL_ATR_45	 ,  ///< 45
	LABEL_ATR_46	 ,  ///< 46
	LABEL_ATR_47	 ,  ///< 47
	LABEL_ATR_48	 ,  ///< 48
	LABEL_ATR_49	 ,  ///< 49

	LABEL_ATR_50	 ,  ///< 50
	LABEL_ATR_51	 ,  ///< 51
	LABEL_ATR_52	 ,  ///< 52
	LABEL_ATR_53	 ,  ///< 53 
	LABEL_ATR_54	 ,  ///< 54
	LABEL_ATR_55	 ,  ///< 55
	LABEL_ATR_56	 ,  ///< 56
	LABEL_ATR_57	 ,  ///< 57
	LABEL_ATR_58	 ,  ///< 58

  LABEL_ATR_SKIP ,  ///< 59 

	LABEL_ATR_NUM  ,  ///< 60 
	LABEL_ATR_SUB  ,  ///< 61 
	LABEL_ATR_CHN  ,  ///< 62 
	LABEL_ATR_AUX  ,  ///< 63 
};

const char * strAtrAbbrev[] =
{
  ABBREV_NOTQRS	  ,   ///< 0 not-QRS (not a getann/putann code) 
  ABBREV_NORMAL	  ,   ///< 1 normal beat 
  ABBREV_LBBB		  ,   ///< 2 left bundle branch block beat 
  ABBREV_RBBB		  ,   ///< 3 right bundle branch block beat 
  ABBREV_ABERR		,   ///< 4 aberrated atrial premature beat 
  ABBREV_PVC		  ,   ///< 5 premature ventricular contraction 
  ABBREV_FUSION	  ,   ///< 6 fusion of ventricular and normal beat 
  ABBREV_NPC		  ,   ///< 7 nodal (junctional) premature beat 
  ABBREV_APC			, 	///< 8 atrial premature contraction 
  ABBREV_SVPB		  , 	///< 9 premature or ectopic supraventricular beat 
  ABBREV_VESC		  , 	///< 10 ventricular escape beat 
  ABBREV_NESC		  , 	///< 11 nodal (junctional) escape beat 
  ABBREV_PACE		  , 	///< 12 paced beat 
  ABBREV_UNKNOWN	, 	///< 13 unclassifiable beat 
  ABBREV_NOISE		, 	///< 14 signal quality change 
  ABBREV_ATR_15	  ,   ///< 15 ATR = 15
  ABBREV_ARFCT		, 	///< 16 isolated QRS-like artifact 
  ABBREV_ATR_17	  ,   ///< 17 ATR = 17
  ABBREV_STCH		  , 	///< 18 ST change 
  ABBREV_TCH			, 	///< 19 T-wave change 
  ABBREV_SYSTOLE	, 	///< 20 systole 
  ABBREV_DIASTOLE , 	///< 21 diastole 
  ABBREV_NOTE		  , 	///< 22 comment annotation 
  ABBREV_MEASURE  , 	///< 23 measurement annotation 
  ABBREV_PWAVE		, 	///< 24 P-wave peak 
  ABBREV_BBB			, 	///< 25 left or right bundle branch block 
  ABBREV_PACESP	  , 	///< 26 non-conducted pacer spike 
  ABBREV_TWAVE		, 	///< 27 T-wave peak 
  ABBREV_RHYTHM	  , 	///< 28 rhythm change 
  ABBREV_UWAVE		, 	///< 29 U-wave peak 
  ABBREV_LEARN		, 	///< 30 learning 
  ABBREV_FLWAV		, 	///< 31 ventricular flutter wave 
  ABBREV_VFON		  , 	///< 32 start of ventricular flutter/fibrillation 
  ABBREV_VFOFF		, 	///< 33 end of ventricular flutter/fibrillation 
  ABBREV_AESC		  , 	///< 34 atrial escape beat 
  ABBREV_SVESC		, 	///< 35 supraventricular escape beat 
  ABBREV_LINK     , 	///< 36 link to external data (aux contains URL) 
  ABBREV_NAPC		  , 	///< 37 non-conducted P-wave (blocked APB) 
  ABBREV_PFUS		  , 	///< 38 fusion of paced and normal beat 
  ABBREV_WFON		  , 	///< 39 waveform onset 
  ABBREV_WFOFF		,   ///< 40 waveform end 
  ABBREV_RONT		  ,   ///< 41 R-on-T premature ventricular contraction 
	ABBREV_ATR_42	  ,   ///< 42
	ABBREV_ATR_43	  ,   ///< 43
	ABBREV_ATR_44	  ,   ///< 44
	ABBREV_ATR_45	  ,   ///< 45
	ABBREV_ATR_46	  ,   ///< 46
	ABBREV_ATR_47	  ,   ///< 47
	ABBREV_ATR_48	  ,   ///< 48
	ABBREV_ATR_49	  ,   ///< 49

	ABBREV_ATR_50	  ,   ///< 50
	ABBREV_ATR_51	  ,   ///< 51
	ABBREV_ATR_52	  ,   ///< 52
	ABBREV_ATR_53	  ,   ///< 53 
	ABBREV_ATR_54	  ,   ///< 54
	ABBREV_ATR_55	  ,   ///< 55
	ABBREV_ATR_56	  ,   ///< 56
	ABBREV_ATR_57	  ,   ///< 57
	ABBREV_ATR_58	  ,   ///< 58

  ABBREV_ATR_SKIP ,   ///< 59 SKIP

	ABBREV_ATR_NUM  ,   ///< 60 NUM 
	ABBREV_ATR_SUB  ,   ///< 61 SUB
	ABBREV_ATR_CHN  ,   ///< 62 CHN
	ABBREV_ATR_AUX  ,   ///< 63 AUX
};

JBOOL JAtrFileBinSave(char * strFileName, JINT idx, JINT A, JFLOAT fTimeSec)
{
	JBOOL bRet = FALSE;
	FILE *fp = NULL;
	JBOOL bCreate = FALSE;
	//char msg[256];  

	if(UtilFileExisted(strFileName) == FALSE)
	{
		bCreate = TRUE;
	}
	if(bCreate == TRUE)
	{
		fp = fopen(strFileName, "wb+");
	}
	else
	{
		fp = fopen(strFileName, "ab+");
	}
	if(fp == NULL)
	{
    //sprintf(msg, "[ERROR][ATR][SAVE] Failed to open %s\r\n", strFileName);
	  //DBG_PRINTF(msg);
	}
	else
	{
		fwrite(&A, 4, 1, fp);
		fwrite(&fTimeSec, 4, 1, fp);
		fclose(fp);
	}			
	return bRet;
}

JBOOL JAtrFileCsvSave(char * strFileName, JINT idx, JDWORD A, JFLOAT fTimeSec, JFLOAT fDeltaSec)
{
  char msg[256];  
	char strTime[256];
	char strNote[256];
	FILE * fp = NULL;
	
	JBOOL bCreate = FALSE;

	if(UtilFileExisted(strFileName) == FALSE)
	{
		bCreate = TRUE;
	}
	if(bCreate == TRUE)
	{
		fp = fopen(strFileName, "w+");
		if(fp == NULL)
		{
      sprintf(msg, "\t\t  FAIL fp == null, %s\r\n", strFileName);
      DBG_PRINTF2(msg);
			return FALSE;
		}
		fprintf(fp, "%s", "#,TIME,TSEC,DT,A,LABEL,NOTE\n");
	}
	else
	{
		fp = fopen(strFileName, "a+");
	}

	if(fp == NULL)
	{
    sprintf(msg, "\t\t  FAIL fp == null, %s\r\n", strFileName);
    DBG_PRINTF2(msg);
	  return FALSE;
	}
	
	UtilTimeStringGet(fTimeSec, strTime),
	strcpy(strNote, "");
		
	fprintf(fp, "%d,%s,%0.3f,%0.3f,%d,%s,%s\n",	 idx, strTime, fTimeSec, fDeltaSec, A,  JAtrLabel[A], strNote);

	fclose(fp);

  return TRUE;
}

void JAtrDataGet(JAtrType *pAtr, JINT *piAtr, JFLOAT *pfTimeSec)
{
  *pfTimeSec = (JFLOAT)pAtr->dwMS / 1000;
  *piAtr     = (JINT)pAtr->bAtr;
}

void JAtrDataPrint(JAtrType *pAtr)
{
  char msg[256];  
  JFLOAT fTimeSec = 0;
  char strTime[256];
  
  fTimeSec = (JFLOAT)pAtr->dwMS / 1000;
  UtilTimeStringGet(fTimeSec, strTime);  
}

void JAtrDataClean(JAtrType *pAtr)
{
  pAtr->bAtr  = ATR_NONE;
  pAtr->dwMS  = 0;  
}
