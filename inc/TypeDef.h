/**
 * @file TypeDef.h 
 * @brief TypeDef.h define variable type 
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

///------------------------------------------------------------------------///
/// Ble Scan devices
///------------------------------------------------------------------------///
#define BLE_MAC_ADDR_SIZE						  (6)
#define BLE_NAME_SIZE							    (24)
#define BLE_SCAN_DEV_LIST_LEN			    (32)
typedef struct JBleScanItem
{
  JBYTE  bAddr[BLE_MAC_ADDR_SIZE];
  JINT   iRssi;
  JBYTE  name[BLE_NAME_SIZE];
} JBleScanItem;

///------------------------------------------------------------------------///               
/// Ecg Info Item
///------------------------------------------------------------------------///
typedef struct JEcgInfoItemST
{                                                                  
	JINT    iYear;							///< #00, 4     4                             
	JINT    iMonth;							///< #01, 4     8                             
	JINT    iDay;								///< #02, 4     12                               
	JINT    iHour;							///< #03, 4     16                               
	JINT    iMin;								///< #04, 4     20                              
	JINT    iSec;								///< #05, 4	    24                                   
	JFLOAT  fTemp;              ///< #06, 4     28                                      
  JINT    iBatLevel;          ///< #07, 4     32                                      
  JFLOAT  fPower;             ///< #08, 4     36                                      
  JINT    iGsenX;             ///< #09, 4     40                                     
  JINT    iGsenY;             ///< #10, 4     44                                    
  JINT    iGsenZ;             ///< #11, 4     48                                   
  JINT    iHR;                ///< #12, 4     52         
  JDWORD  dwRRTimeOffset;     ///< #13, 4     56                                  
  JFLOAT  fRRAvg;	            ///< #14, 4     60
  
  /// SDNN                                                               
  JFLOAT 	fSDNN;              ///< #15, 4     64                                          
  JINT	 	iNN50;              ///< #16, 4     68                                         
  JFLOAT  fRMSSD;             ///< #17, 4     72

  /// Freq                                                               
  JFLOAT  fVLF;               ///< #18, 8     76                                          
  JFLOAT  fLF;                ///< #19, 8     80                                     
  JFLOAT  fHF;	              ///< #20, 8     84                                    
  JFLOAT  fTP;		            ///< #21, 8     88                                     
  JFLOAT  fLFHF;			        ///< #22, 8     92                                     
  JFLOAT  fLFTP;              ///< #23, 8     96                                     
  JFLOAT  fHFTP;              ///< #24, 8     100 

  JFLOAT  fBar;               ///< #25, 4     104
  JFLOAT  fBarHeight;         ///< #26, 4     108
  JFLOAT  fBarTemp;          	///< #27, 4     112 

  JINT    iMaxGsenX;          ///< #28, 4     116                                    
  JINT    iMaxGsenY;          ///< #29, 4     120                                      
  JINT    iMaxGsenZ;          ///< #30, 4     124

  JFLOAT  fQTTime;            ///< #31, 4     128
  JFLOAT  fQRSTime;           ///< #32, 4     132
  JFLOAT  fPRTime;            ///< #33, 4     136
  JINT    iLeadOff;           ///< #34, 4     140 
  JINT    iMotionOn;          ///< #35, 4     144
	JINT 		iBatVolt;					  ///< #36, 4     148       
  JINT    iTimeZoneSec;       ///< #37, 4     152
  JDWORD 	dwUtc;              ///< #38, 4     156  ///< JTM time
  JUINT   iTimeSec;           ///< #39, 4     160
  JWORD   wConnected;         ///< #40, 4     162
  JWORD   wVersion;           ///< #40, 4     164
  JWORD   wChargerStatus;     ///< #41, 4     166
  JWORD   wDummy;             ///< #41, 4     168
} JEcgInfoItem;  ///< 168 bytes       

/// device info report list
#define ECG_ITEM_LIST_MAX_CNT                (1024)
#define ECG_ITEM_LIST_TOTAL_CNT              (20)
#define ECG_ITEM_LIST_FETCH_MAX_CNT          (6) ///< 224 * 6 + 16 = 1360 , 1360 < MTU 1460

#define SSN_SIZE                             (32) 

typedef struct DeviceEcgST
{ 
  JDWORD        dwDid;                    ///< 4 bytes      
	JWORD         wTm;                      ///< 2 bytes  
  JBYTE         bAddr[BLE_MAC_ADDR_SIZE]; ///< 6 bytes
  JINT          iRssi;                    ///< 4 byte         
  JBYTE         strSSN[SSN_SIZE];         ///< 32
  JEcgInfoItem  ecgInfoItem;              ///< 168 bytes
  JDWORD        dwAlarm;                  ///< 4 byte         
  JDWORD        dwDummy;                  ///< 4 byte         
} DeviceEcgType; ///< 224 bytes, name shall be the same as the firmware

///----------------------------------------------------------------///
/// Use "Object" instead of "Item" to distinguish that this data structure
//  originates from firmware rather than the cloud.
///----------------------------------------------------------------///
typedef struct JEcgObjListSt
{
  JDWORD          dwGatewayUtc;  ///< record the GW Time
	JINT            iEcgObjectCount;
	DeviceEcgType   ecgObjectArr[ECG_ITEM_LIST_MAX_CNT];
} JEcgObjListType; ///< name shall be the same as the firmware

///----------------------------------------------------------------///
/// Use "Object" instead of "Item" to distinguish that this data structure
//  originates from firmware rather than the cloud.
///----------------------------------------------------------------///
typedef struct JGatewayInfoST
{
	JINT    iYear;						            ///< #0
	JINT    iMonth;			   			          ///< #1
	JINT    iDay;						    	        ///< #2
	
	JINT    iHour;						            ///< #3
  JINT    iMin;					   		          ///< #4
	JINT    iSec;							            ///< #5	  

  JINT    iGatewayDid;		   	          ///< #6
	JFLOAT  fGatewayTemp;		              ///< #7
	JFLOAT  fGatewayVolt;			            ///< #8
	JINT    iGatewayBattLevel;		        ///< #9

	JUINT   dwGatewayUtc;                 ///< #10  
  JUINT   dwGatewayOnlineTime;          ///< #11

  JINT    iBtnEvent00;                  ///< #12
  JINT    iBtnEvent01;                  ///< #13
  
  JINT    iSosBtnLongPressEvt;          ///< #14
  JDWORD  dwGatewayAlarm;               ///< #15

  JDWORD  dwEventControl;               ///< #16
  JDWORD  dwGatewayStatus;              ///< #17
  JBYTE   strGatewayName[24];           ///< #18 ~ 23
  JINT    iEcgItemCount;                ///< #24 Ecg info
  JINT    iTempItemCount;               ///< #25 Device temp info
  JINT    iLongBtnEvent;                ///< #26  
  JINT    iGatewayOnline;               ///< #27
  JINT    iVersion;                     ///< #28
  JINT    iVscModeQueueCnt;             ///< #29
  JINT    iBleConnectStatus;            ///< #30 0: disconnected, 1: Connected
  JDWORD  dwMonitorFileUtc;             ///< #31
  JDWORD  dwMonitorFileSize;            ///< #32
  JINT    iBleOpenDid;                  ///< #33 0: no auto-connect, other: auto connected to the did
  JINT    iVscModeOn;                   ///< #34
  JDWORD  dwVscModeStartUtc;            ///< #35
  JDWORD  dwQueueHeaderUtc;             ///< #36
  JINT    iButtonDeviceCnt;             ///< #37
  JINT    iLocationDeviceCount;         ///< #38
  JINT    iOximeterCount;               ///< #39
  JINT    iWeighingScaleCount;          ///< #40
  JINT    iThermometerIRCount;          ///< #41
  JINT    iBloodPressureMeterCnt;       ///< #42
  JINT    iGlucoseMeterCount;           ///< #43
  
  // other settting
  JINT    iNotification;                ///< #44
  JINT    iGWInfoStatus;                ///< #45
  JDWORD  dwNotificationUtc;            ///< #46

  JDWORD  dwDummy0;                     ///< #47
  JDWORD  dwDummy1;                     ///< #48
  JDWORD  dwDummy2;                     ///< #49 : 8 + 200   = 208 = 16 * 13  
} JGatewayInfo;

#define DF_STR_VERSION_LEN							(32)

///----------------------------------------------------------------///
/// parameter
///----------------------------------------------------------------///
#define INVALID_MID					(0xFFFFFFFF)
#define INVALID_PID					(0xFFFFFFFF)
#define INVALID_DID					(0xFFFFFFFF)

#define CMD_BUF_SIZE				(25000)

#define DF_STR_MAX_LEN        (32)

typedef struct WifiItemSt
{
	char 	 strAPName[DF_STR_MAX_LEN];					
  char 	 strPassword[DF_STR_MAX_LEN];				
} WifiAPItemType;

///----------------------------------------------------------------///
/// Gateway Config
///----------------------------------------------------------------///
typedef struct GatewayConfigSt
{
  JDWORD dwDID;
  JINT   iLteIPAddr[4];
  JDWORD dwLtePort;
  JBYTE  strAPN[DF_STR_VERSION_LEN];
  JINT   iBtnEventTime;
	JINT   iBleMacAddr[2][BLE_MAC_ADDR_SIZE];
  JINT   iGsenPwTh;
  JINT   iGsenPwSec;
  JINT   iGatewayDeviceMode;
  WifiAPItemType wifiAp[4];
  JINT   iGatewayType; ///< 0:LTE , 1:WIFI
} GatewayConfigType;

///----------------------------------------------------------------///
/// Gateway scan information ot table "gateway_scan_info" in DB
///----------------------------------------------------------------///
typedef struct JGatewayScanInfoST
{
  /// device info
  JINT   iDeviceDID;
  JINT   iDeviceConnected;
	char 	 strDeviceSSN[32];		
  JINT   iDeviceRssi;
  JINT   iDeviceOnline;
  JINT   iDeviceHR;
  JFLOAT fDeviceTemp;
  JINT   iDeviceBatLevel;
  /// gateway info
  JINT   iGatewayDID;
  JINT   iGatewayConnectDID;
  JINT   iGatewayConnStatus;
  JINT   iGatewayVscModeStatus;
  JLONG  utc;
	JINT   iYear;
	JINT   iMonth;
	JINT   iDay;
	JINT   iHour;
	JINT   iMin;
	JINT   iSec;	
  JINT   iGatewayScanId;
} JGatewayScanInfoType;

#define GATEWAY_SCAN_LIST_NONE            (0)
#define GATEWAY_SCAN_LIST_ADD             (1)
#define GATEWAY_SCAN_LIST_UPDATE          (2)

typedef struct JGatewayDeviceInfoListSt
{
  JINT iListCnt;
  JGatewayScanInfoType devItem[128];
} JGatewayScanInfoListType;

///----------------------------------------------------------------///
/// measurement Structure
///----------------------------------------------------------------///
#define MEAS_DEVICE_ECG_COUNT   (4)
#define MEAS_ITEM_LIST_CNT              (16)
typedef struct MeasItemST
{	
  JLONG		iMeasId;  
  JFLOAT  fMeasType;  
	char 		strDeviceSSN[64];		
  JDWORD  dwGatewayID;
  JINT    iDeviceEcgDid[MEAS_DEVICE_ECG_COUNT];   ///< at most 4 devices
  JINT    iMeasPeriodSec[MEAS_DEVICE_ECG_COUNT];  ///< at most 4 devices
  JINT    iMeasWaitSec[MEAS_DEVICE_ECG_COUNT];    ///< at most 4 devices
  char    strTokens[MEAS_DEVICE_ECG_COUNT][128];  ///< at most 4 devices
  JINT    iDeviceRssi[MEAS_DEVICE_ECG_COUNT];     ///< at most 4 devices  
  JINT    iDeviceEcgCount;
  JDWORD 	dwTotalTime;
	JDWORD 	dwStartTime;
	JDWORD 	dwLastUpdateTime;
  JDWORD  dwEndTime;
	JINT 		iMeasState;

  JINT    iBleDidTarget;
  JBYTE   bBleMacTarget[BLE_MAC_ADDR_SIZE];

  /// wait for restart
  JINT    iWaitSec;
  JINT    iWaitSecNow;

  char 		strMeasToken[64];		
} MeasItemType;

typedef struct MeasItemListST
{
	JINT          iMeasurementCount;
	MeasItemType  measItemArr[MEAS_ITEM_LIST_CNT];
} MeasItemListType;

typedef struct JDeviceEcgST
{
	JINT 		iDid;				          		///<	00: device_did
	JINT 		iYear;          					///<	01: device_year
	JINT 		iMonth;	          				///<	02: device_month
	JINT 		iDay;	          					///<	03: device_day
	JINT 		iHour;          					///<	04: device_hour
	JINT 		iMin;	          					///<	05: device_min
	JINT 		iSec;		          				///<	06: device_sec
	JFLOAT 	fTemp0;	          				///<	07: device_temp
  JINT    iBatLevel;                ///<  08: device_bat_level
  JFLOAT  fGsenPower;               ///<  09: device_gsen_power
	JINT	 	iGSenX;		          			///<	10: device_gsen_x
	JINT	 	iGSenY;	          				///<	11: device_gsen_y
	JINT	 	iGSenZ;		          			///<	12: device_gsen_z
 	JINT    iMaxGSenX;                ///<  13: device_gsen_max_x
 	JINT    iMaxGSenY;                ///<  14: device_gsen_max_y
 	JINT    iMaxGSenZ;                ///<  15: device_gsen_max_z  
  JINT    iHR;                      ///<  16: device_hr
  JINT	 	dwUTC;	          				///<  17: device_tm   
  
  JFLOAT  fRRAvg;	                  ///<  18: device_rr_avg
  /// SDNN                                                               
  JFLOAT 	fSDNN;                    ///<  19: device_sdnn                                        
  JINT	 	iNN50;                    ///<  20: device_nn50                                      
  JFLOAT  fRMSSD;                   ///<  21: device_rmssd
							    
  /// Freq                                                   
  JFLOAT  fVLF;                     ///<  22: device_vlf                                        
  JFLOAT  fLF;                      ///<  23: device_lf                                    
  JFLOAT  fHF;	                    ///<  24: device_hf                                    
  JFLOAT  fTP;		                  ///<  25: device_tp                                   
  JFLOAT  fLFHF;		          	    ///<  26: device_plfhf                                  
  JFLOAT  fLFTP;                    ///<  27: device_lf_tp                                  
  JFLOAT  fHFTP;                    ///<  28: device_hf_tp

  JFLOAT  fBar;                     ///<  29: device_bar
  JFLOAT  fBarHeight;               ///<  30: device_bar_height
  JFLOAT  fBarTemp;                 ///<  31: device_bar_temp

  JFLOAT  fQTTime;                  ///<  32: device_qt_time
  JFLOAT  fQRSTime;                 ///<  33: device_qrs_time
  JFLOAT  fPRTime;                  ///<  34: device_pr_time

  JINT    iLeadOff;                 ///<  35: device_lead_off
	JINT	 	iRssi;		          			///<	36: device_rssi
  char    strSSN[SSN_SIZE];         ///<  37: device_ssn
  JBYTE   bAddr[BLE_MAC_ADDR_SIZE]; ///<  38: device_mac
	JINT 		iGatewayDID;		          ///<	39: gateway_did
  JINT    iTimeZone;                ///<  40: device_time_zone  
	JINT	 	iTimeDelta;		          	///<	41: delta time

  JUINT   dwConnected;              ///< #42: device is connected
  JUINT   dwAlarm;                  ///< #43: device alarm
	JINT 		iBatVolt;	          			///< #44: device_bat_volt   
  JINT    iVersion;                 ///< #45: device_version
  JUINT   dwChargerStatus;          ///< #46: device_charger_status  
	char    strToken[64];             ///< device_info_token
  
  ///----------------------------------------------------------------///
  /// Above : comes from the ecgObject.report
  /// The rest parts are fields for DB loading
  ///----------------------------------------------------------------///
  JINT    iPose;                    ///< #47: device_pose       : [JL], not modified
  JDWORD  dwFileUtc;                ///< #48: device_file_utc   : [JL], I don't know what is  file utc
  JBYTE   strDeviceName[128];       ///< #49: device_pname
  JINT    iOnline;                  ///< #50: device_online     
} JDeviceEcg;

#include "JAtr.h"
#include "VscMode.h"

typedef struct JDeviceST
{
	JINT    iDevSocket;
	JINT    iDevStatus;

	JDWORD  dwMID;
	JDWORD  dwPID;
	JDWORD  dwDID;

	JDWORD  dwDevEvent0;	

  ///----------------------------------------------------------------///
	/// Folder path
  ///----------------------------------------------------------------///
	char    strDataRoot[256];
	char    strDataRootEx[256];
	char    strInfoDataRoot[256];
	char    strGatewayDataRoot[256];

  ///----------------------------------------------------------------///
  /// Task wait time setting
  ///----------------------------------------------------------------///
  JINT    iWaitTaskSystemVersionGet;
  JINT    iWaitTaskGatewayInfoGet;
  JINT    iWaitTaskGatewayEcgObjectListGet;
  JINT    iWaitTaskGatewayScanDeviceInfoUpdate;
  JINT    iWaitTaskTaskLocationInfoGet;

  ///----------------------------------------------------------------///
	/// Timer for timout check	
  ///----------------------------------------------------------------///
	JINT    iSec;
	JINT    iSecPre;	
	
	time_t  tPre;
	time_t  tNow;	
	
	time_t  t0;
	time_t  t1;

	unsigned long dwTick;
	unsigned long dwTickPre;	

	JINT iTick100MS;
  JINT iTimeZoneOffset;

  ///----------------------------------------------------------------///
	/// System Version
  ///----------------------------------------------------------------///
  JINT    iSystemVersion;            ///< system version
  JINT    iOnlineType;               ///< Online type: 0 OFF, 1 LTE, 2 WIFI
  JINT    iOnlineTime;               ///< Online time in seconds
	
  ///----------------------------------------------------------------///
  /// gateway config
	///----------------------------------------------------------------///
  GatewayConfigType GatewayConfig;

  ///----------------------------------------------------------------///
	/// Packet command buffer
  ///----------------------------------------------------------------///
	JBYTE bCmdBuf[CMD_BUF_SIZE];

  ///----------------------------------------------------------------///
  ///  Program & Program Stream
  ///----------------------------------------------------------------/// 
 	JEcgInfoItem	  ecgInfoItem;    ///< report data for ecg device

  ///----------------------------------------------------------------///
  /// Device ECG
  ///----------------------------------------------------------------/// 
  JDeviceEcg	     deviceEcgRunning;    ///< for run-time device ecg information  
  JDeviceEcg	     deviceEcgInfo;       ///< for run-time device ecg information  

  ///----------------------------------------------------------------///
  ///  Gatewat Info File
  ///----------------------------------------------------------------///  
  JINT                          iGatewayInfoFileIdx;
  JINT                          iGatewayInfoFileIdxPre;      
  JINT                          iGatewayInfoFileSize;

  JGatewayInfo  	              gatewayInfo;  
  
  JGatewayScanInfoListType      DBGatewayScanInfoList;
  
  JEcgObjListType               ecgObjectList;	                    

	///----------------------------------------------------------------///
  /// Ble open device info
  ///----------------------------------------------------------------///
  JBYTE              strDeviceSSN[SSN_SIZE];  
  JINT               iUserId;
      
  JDWORD             dwBleOpenDid;
  JBYTE              bBleOpenAddr[BLE_MAC_ADDR_SIZE];
  JINT               iBleOpen;                           ///< 1 : is open , 0 : is close, 2:opening
  JDWORD             dwBleConnectingT0;                  ///< Connecting t0 
  JINT               iGatewayCmdTimeOut;

  ///----------------------------------------------------------------///
  /// Record token of report
  ///----------------------------------------------------------------///
  char               strReportRecordToken[128];     

	///----------------------------------------------------------------///
  /// VSC Mode
  ///----------------------------------------------------------------///
  JINT               iVscModeQueueHead;
  JINT               iVscModeQueueTail;
  JINT               iVscModeDataCount;       ///< VSC_MODE data count has been read
  VscModeControlType vscModeQueue[VSC_MODE_QUEUE_SIZE];  

  time_t             tVscModeStartTime;       ///< VSC_MODE start time
  JINT               iVscModeTotalSec;        ///< VSC_MODE total run sec
        
  JINT               iVscModeDataCountLeft;   ///< VSC_MODE data count that not read in Gateway

  JINT               iVscModeIdx;

  ///----------------------------------------------------------------///
  /// VSC File Mode
  ///----------------------------------------------------------------///
  JINT               iVscModeItemReadCount;   ///< 
  JDWORD             dwVscFileModeUtcStart;   ///< 
  JDWORD             dwVscFileModeUtcGet;     ///< 
  JINT               iVscFileModeMSGet;       ///< 
    
  JINT               iVscModeIdxLast;         ///< VSC_MODE data count has been read
  JINT               iNotFoundCount;      
       
	///----------------------------------------------------------------///
  /// VSC ATR
  ///----------------------------------------------------------------///
  JINT		          iVscAtrCnt;
  JINT		          iVscAtrCntPre;
  JAtrType          vscAtrNow;
  JAtrType          vscAtrPre;
  	
  ///----------------------------------------------------------------///
  /// Measurement Object
  ///----------------------------------------------------------------///  
  MeasItemType *    pMeasItemActive;  
  MeasItemListType  MeasItemList;
} JDevice;

#endif ///< __TYPEDEF_H__

