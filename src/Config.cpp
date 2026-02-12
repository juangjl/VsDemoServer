/**
 * @file JConfig.cpp 
 * @brief JConfig function 
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology Co., Ltd., all rights reserved.
 * @note
*/
///------------------------------------------///
///  Include Area
///------------------------------------------///
#include "Global.h"

ConfigItemType ConfigItem[] = 
{
		///------------------------------------------------------------------------------------------------------------------///
		///  NAME       								VALUE																									TYPE
		///------------------------------------------------------------------------------------------------------------------///		
		{CONFIG_ITEM_SERVER_PORT, 			"7000", 																							JCONFIG_TYPE_INT},					
	  {CONFIG_ITEM_VSC_MODE_SAVE, 	 	"ON", 																								JCONFIG_TYPE_STR},		
		{CONFIG_ITEM_DATA_ROOT,   			"./data/", 																						JCONFIG_TYPE_STR},
		{CONFIG_ITEM_CMD_ROOT,   				"./data/CMD/", 																				JCONFIG_TYPE_STR},																
		///------------------------------------------------------------------------------------------------------------------///
		{"",                            "",                                                   JCONFIG_TYPE_STR}			
};

void ConfigPrint()
{
	JINT i = 0;
  ConfigItemType *pItem = NULL;
  char msg[256];
 	while(1)
 	{
 		pItem = (ConfigItemType *)&ConfigItem[i];
 		i = i + 1;
 		if(strlen(pItem->strName) == 0)
		{
			break;
		}
 		sprintf(msg, "\t%20s = %-40s (type = %02d) \n", pItem->strName, pItem->strValue, pItem->iType);
 		DBG_PRINTF2(msg);
 	}
}

void ConfigSet(char *strName, char *strValue)
{
  JINT i = 0;
  ConfigItemType *pItem = NULL;
 	while(1)
 	{
 		pItem = (ConfigItemType *)&ConfigItem[i];
 		i = i + 1;
 		if(strlen(pItem->strName) == 0)
		{
			break;
		}
 		if(strstr(pItem->strName, strName) == pItem->strName)
		{
			strcpy(pItem->strValue, strValue);
			break;
		}
 	}
}

void ConfigStrSet(char *strName, char *strValue)
{
  JINT i = 0;
  ConfigItemType *pItem = NULL;
 	while(1)
 	{
 		pItem = (ConfigItemType *)&ConfigItem[i];
 		i = i + 1;
 		if(strlen(pItem->strName) == 0)
		{
			break;
		}
 		if(strstr(pItem->strName, strName) == pItem->strName)
		{
			strcpy(pItem->strValue, strValue);
			break;
		}
 	}
}


void ConfigStrGet(char *strName, char *strValue)
{
  JINT i = 0;
  ConfigItemType *pItem = NULL;
 	while(1)
 	{
 		pItem = (ConfigItemType *)&ConfigItem[i];
 		i = i + 1;
 		if(strlen(pItem->strName) == 0)
		{
			break;
		}
 		if(strstr(pItem->strName, strName) == pItem->strName)
		{
			strcpy(strValue, pItem->strValue);
			break;
		}
 	}
}

void ConfigIntGet(char *strName, JINT *piVal)
{
  JINT i = 0;
  ConfigItemType *pItem = NULL;
 	while(1)
 	{
 		pItem = (ConfigItemType *)&ConfigItem[i];
 		i = i + 1;
 		if(strlen(pItem->strName) == 0)
		{
			break;
		}
		if(strstr(pItem->strName, strName) == pItem->strName)
		{
			sscanf(pItem->strValue, "%d", piVal);
			break;
		}
 	}
}

void ConfigFloatGet(char *strName, JFLOAT *pfVal)
{
  JINT i = 0;
  ConfigItemType *pItem = NULL;
 	while(1)
 	{
 		pItem = (ConfigItemType *)&ConfigItem[i];
 		i = i + 1;
 		if(strlen(pItem->strName) == 0)
		{
			break;
		}
 		if(strstr(pItem->strName, strName) == pItem->strName)
		{
			sscanf(pItem->strValue, "%f", pfVal);
			break;
		}
 	}
}

void ConfigFloatSet(char *strName, JFLOAT fVal)
{
  JINT i = 0;
  ConfigItemType *pItem = NULL;
 	while(1)
 	{
 		pItem = (ConfigItemType *)&ConfigItem[i];
 		i = i + 1;
 		if(strlen(pItem->strName) == 0)
		{
			break;
		}
 		if(strstr(pItem->strName, strName) == pItem->strName)
		{
			sprintf(pItem->strValue, "%f", fVal);
			break;
		}
 	}
}

void ConfigParse(char *strLine)
{
	char strName[128];
	char strValue[128];	
	JINT iCnt = strlen(strLine);
	JINT i = 0;
	JINT idx = 0;	
	char *ptr = NULL;
	
	ptr = &strName[0];	
	for(i = 0 ; i < iCnt; i = i + 1)
	{
		if(strLine[i] == '=')
		{
			ptr = &strValue[0];
			*ptr = 0;
			continue;
		}
		if(strLine[i] == '\r')
		{
			continue;
		}
		if(strLine[i] == '\n')
		{
			continue;
		}		
		if(strLine[i] == 0)
		{
			break;
		}
		*ptr = strLine[i];
		 ptr  =  ptr + 1;
		*ptr = 0;
	}
	ConfigStrSet(strName, strValue);
}

void ConfigLoad(void)
{
	FILE *fp = NULL;
	char strFileName[256];
	char line[256];
	JINT iRet = 0;

	#if OS_TYPE == OS_TYPE_UBUNTU
	sprintf(strFileName, "%s.txt", CONFIG_FILE_NAME);
	#elif OS_TYPE == OS_TYPE_MAC_OS
	sprintf(strFileName, "%s.mac.txt", CONFIG_FILE_NAME);
	#elif OS_TYPE == OS_TYPE_WINDOWS
	sprintf(strFileName, "%s.win.txt", CONFIG_FILE_NAME);
	#endif ///< for #if

	if(GlobalVar.iOSType == OS_TYPE_MAC_OS)
	{
		if(UtilFileExisted(strFileName) == FALSE)
		{			
			sprintf(strFileName, "/Users/Shared/VsEcg/cfg/config.mac.txt");
		}
		if(UtilFileExisted(strFileName) == FALSE)
		{
			sprintf(strFileName, "%s.mac.txt", CONFIG_FILE_NAME);			
		}
	}
	
	UtilWinPathNameSet(strFileName);
	fp  = fopen(strFileName, "r");
	if(fp == NULL)
	{
		ConfigSave();
		return;
	}
	
	while(1)
	{
		iRet = fscanf(fp, "%[^\n]\n", line);
		if(iRet <= 0)
		{
			break;
		}		
		ConfigParse(line);
	}
	
	fclose(fp);

	//ConfigPrint();
}

void ConfigSave(void)
{
	JINT i = 0;
  ConfigItemType *pItem = NULL;
  FILE * fp = NULL;
  char strFileName[256];  
	
	/// Log save
	#if OS_TYPE == OS_TYPE_UBUNTU
	sprintf(strFileName, "%s.txt", CONFIG_FILE_NAME);
	#elif OS_TYPE == OS_TYPE_MAC_OS
	sprintf(strFileName, "%s.mac.txt", CONFIG_FILE_NAME);
	#elif OS_TYPE == OS_TYPE_WINDOWS
	sprintf(strFileName, "%s.win.txt", CONFIG_FILE_NAME);
	#endif ///< for #if


	if(GlobalVar.iOSType == OS_TYPE_MAC_OS)
	{
		if(UtilFileExisted(strFileName) == FALSE)
		{
			sprintf(strFileName, "/Users/Shared/EcgServer/data/config.mac.txt");
		}
		if(UtilFileExisted(strFileName) == FALSE)
		{
			sprintf(strFileName, "%s.mac.txt", CONFIG_FILE_NAME);
		}
	}

	UtilWinPathNameSet(strFileName);
  fp = fopen(strFileName, "w+");
  if(fp == NULL)
	{
		DBG_PRINTF2("Failed to open the config file\r\n");
		return;
	}
  
	while(1)
 	{
 		pItem = (ConfigItemType *)&ConfigItem[i];
 		i = i + 1;
 		if(strlen(pItem->strName) == 0)
		{
			break;
		}
		fprintf(fp, "%s=%s\n", pItem->strName, pItem->strValue);
 	}
	fclose(fp);
	
	//ConfigPrint();
	DBG_PRINTF2("Config file saved\r\n");
}
