/**
 * @file Packet.h 
 *  @brief Packet.h define command packet format 
 * @version $Revision$
 * @author JLJuang <jl_juang@vsigntek.com>
 * @note Copyright (c) 2021, VitalSigns Technology all rights reserved.
 * @note
*/
#ifndef __PACKET_H__
#define __PACKET_H__

#include "Global.h"

///------------------------------------------------------------------------------------------------///
///
/// Packet Command definition
///
///------------------------------------------------------------------------------------------------///

///-------------------------------------------------///
/// UI function Macro
///-------------------------------------------------///
#define CMD_ACK										'A'
#define CMD_NACK									'N'

#define PACKET_SEND_RETRY_COUNT 	(1)

///-------------------------------------------------------------------------------///
/// Command Group
///-------------------------------------------------------------------------------///
#define CMD_GROUP_ID_I2C										(0x20)
#define CMD_GROUP_ID_SMB										(0x30)
#define CMD_GROUP_ID_SPI										(0x40)
#define CMD_GROUP_ID_UART										(0x50)
#define CMD_GROUP_ID_GPIO										(0x60)
#define CMD_GROUP_ID_MOTOR									(0x70)
#define CMD_GROUP_ID_ADC										(0x80)
#define CMD_GROUP_ID_I2C2										(0x99)
#define CMD_GROUP_ID_DAC										(0xA0)
#define CMD_GROUP_ID_PWM										(0xA1)
#define CMD_GROUP_ID_IOE										(0xA2)
#define CMD_GROUP_ID_TMP										(0xA3)
#define CMD_GROUP_ID_HMD										(0xA4)
#define CMD_GROUP_ID_SREG										(0xA5)
#define CMD_GROUP_ID_BAR										(0xA6)
#define CMD_GROUP_ID_GG  										(0xA7)
#define CMD_GROUP_ID_LTE  									(0xA8)
#define CMD_GROUP_ID_PROG  									(0xA9)
#define CMD_GROUP_ID_DF	  									(0xAA)
#define CMD_GROUP_ID_JDS	  								(0xAB)
#define CMD_GROUP_ID_JFILE	  							(0xAC)

#define CMD_GROUP_ID_RELAY									(0xB0)
#define CMD_GROUP_ID_SYSTEM									(0xC0)
#define CMD_GROUP_ID_BLE                    (0xC2)
#define CMD_GROUP_ID_LOC                    (0xC8) ///< location

#define CMD_GROUP_ID_SOCK										(0xD0)

#define CMD_GROUP_ID_GATEWAY	  						(0xD8)

///-------------------------------------------------------------------------------///
/// SREG
///-------------------------------------------------------------------------------///
#define CMD_SREG_READ														(0x00)
#define CMD_SREG_WRITE													(0x01)

///-------------------------------------------------------------------------------///
/// BLE
///-------------------------------------------------------------------------------///
#define CMD_BLE_OPEN                            (0x20)

#define CMD_BLE_CLOSE                           (0x2F)

#define CMD_BLE_MONITOR_START		                (0x44)
#define CMD_BLE_MONITOR_STOP		                (0x45)

#define CMD_BLE_PACKET_WRITE                    (0x61)
#define CMD_BLE_PACKET_READ                     (0x62)

#define CMD_BLE_VSC_MODE_START					      	(0x64)
#define CMD_BLE_VSC_MODE_STOP					      		(0x65)

#define CMD_BLE_CONNECT_STATUS									(0x66)  

#define CMD_BLE_VSC_MODE_QUEUE_READ             (0x6C)

#define CMD_BLE_VSC_FILE_MODE_START             (0x71)
#define CMD_BLE_VSC_FILE_MODE_STOP              (0x72)

#define CMD_BLE_VSC_FILE_MODE_QUEUE_READ     	  (0x7C)
#define CMD_BLE_VSC_FILE_MODE_QUEUE_CLEAR       (0x7D)

#define CMD_BLE_VSC_FILE_MODE_UPLOAD_START      (0x7E)  

#define CMD_BLE_TIME_SET									     	(0xA0)
#define CMD_BLE_TIME_GET									     	(0xA1)

///-------------------------------------------------------------------------------///
/// LOCATION
///-------------------------------------------------------------------------------///
#define CMD_LOC_LOCATION_GET                    (0x01) ///< location get

///-------------------------------------------------------------------------------///
/// JFile
///-------------------------------------------------------------------------------///
#define CMD_JFILE_FILE_READ    									(0x01) ///< set file read event
#define CMD_JFILE_FILE_DATASET_READ_AUTO  	    (0x04) ///< dataset of file read
#define CMD_JFILE_FILE_READ_EX                  (0x05)
   
#define CMD_JFILE_DUMP_READ                     (0x60)

#define CMD_JFILE_LIST_CLEAR										(0x70)  
#define CMD_JFILE_LIST_READ  										(0x71)  
#define CMD_JFILE_READ  												(0x72) 

#define CMD_VSC_FILE_LIST_READ								  (0x80)

///-------------------------------------------------------------------------------///
/// SOCK
///-------------------------------------------------------------------------------///
#define CMD_SOCK_DEVICE_INFO										(0x04)

///-------------------------------------------------------------------------------///
/// SYSTEM
///-------------------------------------------------------------------------------///
#define CMD_SYS_VER	  												  (0x00)

#define CMD_SYS_TIME_SET                        (0x40)

#define CMD_SYS_TRANSMIT_TEST                   (0x50)

#define CMD_SYS_VER	  												  (0x00)

#define CMD_SYS_RESET					                  (0x03)

#define CMD_SYS_GATEWAY_BLE_SCAN                (0x06)

#define CMD_SYS_LIGHT							              (0x70)	
#define CMD_SYS_SOUND					               		(0x80)	
#define CMD_SYS_GW_EVT_TYPE  	               		(0x90)	
  
#define CMD_SYS_GW_MODE_SET                     (0x93)
#define CMD_SYS_GW_MODE_GET                     (0x94)
#define CMD_SYS_GW_SSID_GET                     (0x95)
#define CMD_SYS_GW_SSID_SET                     (0x96)
#define CMD_SYS_GW_NET_SET                      (0x97)
#define CMD_SYS_GW_NET_GET                      (0x98)
#define CMD_SYS_GW_DID_SET                      (0x99)
#define CMD_SYS_GW_DID_GET                      (0x9A)
#define CMD_SYS_GW_BLEMAC_SET                   (0x9B)
#define CMD_SYS_GW_BLEMAC_GET                   (0x9C)
#define CMD_SYS_GW_DEV_ALARM_CLEAR              (0x9D)
#define CMD_SYS_INFO_GET                        (0x9E)  
  #define SYS_INFO_TYPE_TEMP_DEV_GET               	(0x02)
	#define SYS_INFO_TYPE_EVENT_COUNT_DEV_GET         (0x04)
	#define SYS_INFO_TYPE_LOCATION_SENSOR_DEV_GET     (0x05)
	#define SYS_INFO_TYPE_OXIMETER_DEV_GET            (0x06)
	#define SYS_INFO_TYPE_GLUCOSE_DEV_GET             (0x07)
	#define SYS_INFO_TYPE_BLOODPRESSURE_DEV_GET       (0x08)
 	#define SYS_INFO_TYPE_PERIPHERAL_DEV_GET				  (0x10)
	  #define INFO_TYPE_HEADER_SIZE                     (9)
		#define INFO_TYPE_DEVICE_SCALE_SIZE               (51)
		#define INFO_TYPE_DEVICE_CORE_TEMP_SIZE           (50)
		#define INFO_TYPE_DEVICE_OXIMETER_SIZE            (64)
		#define INFO_TYPE_DEVICE_GLUCOSE_SIZE             (46)
		#define INFO_TYPE_DEVICE_BLOODPRESS_SIZE          (54)

///-------------------------------------------------------------------------------///
/// GATEWAY
///-------------------------------------------------------------------------------///
#define CMD_GW_GATEWAY_INFO_GET       		      (0x10)
#define CMD_GW_GATEWAY_BLE_CONNECT_STATUS_GET   (0x11)
#define CMD_GW_GATEWAY_VSC_MODE_STATUS_GET   		(0x12)

#define CMD_GW_OXIMETER_LIST_GET								(0x20)
#define CMD_GW_BLOOD_PRESSURE_LIST_GET					(0x30)
#define CMD_GW_TEMP_IR_LIST_GET									(0x40)
#define CMD_GW_GLUCOSE_LIST_GET									(0x50)

#define CMD_GW_DEVICE_TEMP_LIST_GET     				(0x91)

#define CMD_GW_DEVICE_ECG_LIST_GET     					(0xA0)
#define CMD_GW_DEVICE_ECG_LIST_COUNT_GET       	(0xA1)

/// previous 
#define CMD_GW_DEVICE_ECG_INFO_GET    	   	    (0x01)
#define CMD_GW_DEVICE_TEMP_INFO_GET    	   	    (0x02)
#define CMD_GW_DEVICE_EVENT_COUNTER_INFO_GET    (0x04)
#define CMD_GW_DEVICE_LOCATION_INFO_GET			    (0x05)
#define CMD_GW_DEVICE_OXIMETER_INFO_GET			    (0x06)
#define CMD_GW_DEVICE_GLUCOSE_INFO_GET			    (0x07)
#define CMD_GW_DEVICE_BLOOD_PRESSURE_INFO_GET		(0x08)

#define CMD_GW_DEVICE_IR_TEMP_COUNT_GET					(0x50)
#define CMD_GW_DEVICE_SCALE_COUNT_GET						(0x51)
#define CMD_GW_DEVICE_OXIMETER_COUNT_GET				(0x64)

#define CMD_SYS_GATEWAY_ECG_ITEM_LIST_GET       (0xA0)

///----------------------------------------------------------------///
///  2. Packet definition
///----------------------------------------------------------------///
#define PACKET_CMD_DATA_LEN									(20000)
#define PACKET_ACK_DATA_LEN									(20000)

///----------------------------------------------------------------///  
///  Packet header size
///----------------------------------------------------------------///
#define PACKET_CMD_HEADER_SIZE              (8)
#define PACKET_CMD_BDATA_SIZE               (16)

//#pragma pack(push)  /* push current alignment to stack */
//#pragma pack(1)     /* set alignment to 1 byte boundary */

#define PACKET_PARA_HEADER_SIZE                  (8)    
#define PACKET_PARA_BDATA_SIZE                   (16)
typedef struct PacketStruct
{
	///----------------------------------------------------///
	/// In data packet 65 bytes
	///----------------------------------------------------///
	JBYTE bPCode;				  								///< Command Group
	JBYTE bGroupId;												///< Command Group
	JBYTE bCmd;												  	///< Command Command Byte						
	JBYTE bCheckSum; 											///< Checksum		
	JWORD wMOSILen;												///< Slave: Data in length /  Master Data out length
	JWORD wMISOLen;												///< Slave: Data out length /  Master Data in length
	JBYTE bData[16]; 			                ///< Data Buffer						
	JBYTE bCmdData[PACKET_CMD_DATA_LEN]; 	///< Dummy bytes
} PacketType;

#define PACKET_ACK_HEADER_SIZE                   (8)    
typedef struct PacketAckStruct
{
	///----------------------------------------------------///
	/// In data packet 64 bytes
	///----------------------------------------------------///
	JBYTE bPCode;				  								///< Command Group
	JBYTE bGroupId;												///< Command Group
	JBYTE bAck;														///< Command Command Byte
	JBYTE bChkSum;							 					///< Checksum				
	JWORD wDataIdx;												///< Data in length
	JWORD wDataLen;												///< Data out length
	JBYTE bData[PACKET_ACK_DATA_LEN]; 		///< In  Data Buffer					
} PacketAckType;

#define SREG_ACT_READ				  (0)
#define SREG_ACT_WRITE			  (1)

///----------------------------------------------------------------------------------///
/// SREG TYPE
///----------------------------------------------------------------------------------///
#define SREG_NAME_SIZE 			(32)
#define SREG_DATA_SIZE 			(64)
#define SREG_SIZE     			(SREG_NAME_SIZE + SREG_DATA_SIZE)
typedef struct SRegSt
{ 	
  char  strName[SREG_NAME_SIZE];
	JBYTE bData[SREG_DATA_SIZE];  
} SRegType;

#define SREG_DATA_TYPE_STR       (1)
#define SREG_DATA_TYPE_JINT      (2)
#define SREG_DATA_TYPE_JFLOAT    (3)
#define SREG_DATA_TYPE_BLE_MAC   (4)
#define SREG_DATA_TYPE_NET_IP    (5)

#define SREG_TYPE_STR            ("STR")
#define SREG_TYPE_JINT           ("JINT")
#define SREG_TYPE_JFLOAT         ("JFLOAT")
#define SREG_TYPE_BLE_MAC        ("BLE_MAC")
#define SREG_TYPE_NET_IP         ("NET_IP")

extern void 	PacketInit(PacketType *pPacket);
extern void 	PacketAckInit(PacketAckType *pPacketAck);
extern void 	PacketAckCheckSumSet(PacketType *pPacketAck, JINT wLen);
extern JBYTE 	PacketAckCheckSumCheck(JDevice *pDevice, PacketAckType *pPacketAck, JINT size, JBYTE bChkSum);
extern JBOOL 	PacketSend(JDevice *pDevice, PacketType * pPacketOut, PacketAckType * pPacketIn, JINT iTimeout);

extern JBOOL  SBlePacketSendEx(JDevice *pDevice, PacketType * pPacketOut, PacketAckType * pPacketIn, JINT iTimeout);

#endif ///< __PACKET_H__

