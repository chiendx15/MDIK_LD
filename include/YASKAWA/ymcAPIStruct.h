/************************************************************************

        ymcAPIStruct.h

        COPYRIGHT (C) YASKAWA ELECTRIC CORPORATION

*************************************************************************/


#ifndef _API_STRUCT_H
#define _API_STRUCT_H

/************************************************************************/
/*                                                                      */
/*              << Common motion API extern declaration >>              */
/*                                                                      */
/*      File name   :  ymcAPIStruct.h                                   */
/*                                                                      */
/*                                                                      */
/************************************************************************/

/* Extracted from windows.h  */
typedef unsigned char    UCHAR;
typedef unsigned short   USHORT;
typedef unsigned long    ULONG;
typedef unsigned __int64 QWORD;

typedef char             CHAR;
typedef short            SHORT;
typedef long             LONG;
typedef float            FLOAT;
typedef double           DOUBLE;
typedef unsigned char    BIT;
typedef unsigned char    BYTE;
typedef unsigned short   WORD;
typedef unsigned long    DWORD;
typedef void *           LPVOID;
#ifdef _UNICODE
//typedef unsigned short *          LPTSTR;
//typedef const unsigned short *    LPCTSTR;
#else
typedef char *			 LPTSTR;
typedef const char *	 LPCTSTR;
#endif
typedef unsigned char *  LPBYTE;
typedef unsigned short * LPWORD;
typedef unsigned long *  LPDWORD;
typedef float *          LPFLOAT;
typedef double *         LPDOUBLE;
typedef unsigned __int64 *  LPQWORD;

typedef void             (*FP)();
typedef void             (*VOID_FP)();
typedef WORD             (*WORD_FP)();
typedef DWORD            (*DWORD_FP)();
typedef DWORD_FP         API_FP;


typedef DWORD MP_STATUS;

typedef DWORD HAXIS, *LPHAXIS;
typedef DWORD HDEVICE, *LPHDEVICE;
typedef DWORD HCONTROLLER, *LPHCONTROLLER;
typedef DWORD HREGISTERDATA, *LPHREGISTERDATA;
typedef DWORD HALARM, *LPHALARM;
typedef DWORD HMOVE, *LPHMOVE;
typedef DWORD HACTION, *LPHACTION;
typedef DWORD HEVENT, *LPHEVENT;

typedef QWORD HREGISTERDATA_EX, *LPHREGISTERDATA_EX;

/************************************************************************/
/*    STARTUP_PARAM                                                     */
/************************************************************************/
typedef struct _startup_param
{
    ULONG      TaskNo;            /*  Task number                       */
    LPVOID*    pTable;            /*  System call table pointer         */
    API_FP*    pApiTable;         /*  Pointer to API function table     */
    ULONG      TaskId;            /*  Own task ID                       */
    ULONG      MboxId;            /*  Own mail box ID                   */
    ULONG      Dummy1;
    ULONG      Dummy2;
    ULONG      Dummy3;

} STARTUP_PARAM,*LPSTARTUP_PARAM;


/************************************************************************/
/*    MOTION_DATA                                                       */
/*    Structure to specify motion data                                  */
/************************************************************************/
typedef struct motion_data2_str
{
    SHORT      CoordinateSystem;  /*  Coordinate system specified                                        */
    SHORT      MoveType;          /*  Motion type                                                        */
    SHORT      VelocityType;      /*  Speed type                                                         */
    SHORT      AccDecType;        /*  Acceleration type                                                  */
    SHORT      FilterType;        /*  Filter type                                                        */
    SHORT      DataType;          /*  Data type (0: immediate, 1: indirect designation)                  */
    LONG       MaxVelocity;       /*  Maximum feeding speed [reference unit/s]                           */
    LONG       Acceleration;      /*  Acceleration [reference unit/s2], Acceleration time constant [ms]  */
    LONG       Deceleration;      /*  Deceleration [reference unit/s2], Deceleration time constant [ms]  */
    LONG       FilterTime;        /*  Filter time [ms]                                                   */
    LONG       Velocity;          /*  Feeding speed [reference unit/s], offset speed                     */
    LONG       ApproachVelocity;  /*  Approach speed [reference unit/s]                                  */
    LONG       CreepVelocity;     /*  Creep speed [reference unit/s]                                     */

} MOTION_DATA,*LPMOTION_DATA;


/************************************************************************/
/*    POSITION_DATA                                                     */
/*    Structure to specify position data                                */
/************************************************************************/
typedef struct position_data2_str
{
    LONG       DataType;          /*  Data type [0: immediate, 1: indirect designation]  */
    LONG       PositionData;      /*  Position data                                      */

} POSITION_DATA,*LPPOSITION_DATA;


/************************************************************************/
/*    IO_DATA                                                           */
/*    IoData structure                                                  */
/************************************************************************/
typedef struct _IoData {
    WORD       DeviceType;        /*  I/O, direct I/O, variable, timer, motion, etc.                        */
                                  /*  Applied to packet access type                                         */
    WORD       DataUnitSize;      /*  Unit data size                                                        */
                                  /*  BIT             1                                                     */
                                  /*  BYTE            8                                                     */
                                  /*  Word            16  etc.                                              */
    WORD       RackNo;            /*  Rack number(1, 2, ...)                                                */
    WORD       SlotNo;            /*  Slot number(0, 1, ...)                                                */
    WORD       SubslotNo;         /*  Sub-slot number(1, 2, ...)                                            */
    WORD       StationNo;         /*  Station number(1, 2, ...)                                             */
    DWORD      hData;             /*  Data handle                                                           */
                                  /*  Global data handle or register data handle can be specified.          */
    DWORD      DeviceWordOffset;  /*  I/O or variable offset address (in units of words)                    */
                                  /*  Motion         Motion parameter number                                */
    WORD       DeviceBitOffset;   /*  Bit offset                                                            */
                                  /*  BIT            0?F                                                   */
                                  /*  BYTE           0?1(Hi,Lo)                                            */
                                  /*  WORD,DWORD     0(reserved)                                            */
    WORD       Reserve;           /*  Reversed                                                              */
    DWORD      DataUnitCount;     /*  used for multiple bit quantities or multiple byte arrays, normally 1  */

} IO_DATA,*LPIO_DATA;


/************************************************************************/
/*    EVENT_LOG_BUF                                                     */
/*    Buffer for event log                                              */
/************************************************************************/
typedef struct _event_log_buf
{
    USHORT     ExecStatus;        /*  Execution status                  */
    USHORT     Reserve;           /*  Reserved                          */
    BYTE       Data[16];          /*  Log data                          */

} EVENT_LOG_BUF,*LPEVENT_LOG_BUF;


/************************************************************************/
/*    CALENDAR_DATA                                                     */
/*    Calendar data                                                     */
/************************************************************************/
typedef struct _calendar_data
{
    WORD       Year;              /*  Year         */
    WORD       Month;             /*  Month        */
    WORD       DayOfWeek;         /*  Day of week  */
    WORD       Day;               /*  Day          */
    WORD       Hour;              /*  Hour         */
    WORD       Minutes;           /*  Minute       */
    WORD       Second;            /*  Second       */
    WORD       Milliseconds;      /*  Millisecond  */

}    CALENDAR_DATA,*LPCALENDAR_DATA;


/************************************************************************/
/*    COM_DEVICE                                                        */
/*    ComDevice structure                                               */
/************************************************************************/
typedef struct _communication_device
{
    WORD       ComDeviceType;     /*  Communication type                */
    WORD       PortNumber;        /*  Port number                       */
    WORD       CpuNumber;         /*  CPU number                        */
    WORD       NetworkNumber;     /*  Network number                    */
    WORD       StationNumber;     /*  Station number                    */
    WORD       UnitNumber;        /*  Unit number                       */
    LPTSTR     IPAddress;         /*  IP address (Ethernet)             */
    DWORD      Timeout;           /*  Communication timeout time        */

}    COM_DEVICE,*LPCOM_DEVICE;


/************************************************************************/
/*    SELF_CONFIG_STATUS                                                */
/*    Automatic configuration status definition                         */
/************************************************************************/
typedef struct _self_config_status
{
    USHORT     Status;            /*  Status                            */
    USHORT     AllSlot;           /*  All slots                         */
    USHORT     FinishedSlot;      /*  Number of completed slots         */
    USHORT     ErrorCode;         /*  Error code                        */
    USHORT     ErrorRack;         /*  Rack number                       */
    USHORT     ErrorSlot;         /*  Slot number                       */
    USHORT     ErrorSubslot;      /*  Sub-slot number                   */
    USHORT     Reserved;          /*  Reserved                          */

} SELF_CONFIG_STATUS,*LPSELF_CONFIG_STATUS;


/************************************************************************/
/*    PHYSICAL_SLOT_INFO , SUB_SLOT_INFO                                */
/************************************************************************/
typedef struct _physical_slot_information
{
    UCHAR      RackNo;            /*  Rack number                                         */
    UCHAR      SlotNo;            /*  Slot number                                         */
    UCHAR      SubslotNo;         /*  Sub-slot number                                     */
                                  /*  Physical slot 겏 "0"                                */
    UCHAR      Reserved;          /*  Reserved                                            */
    USHORT     ModuleId;          /*  Module ID                                           */
    USHORT     ModuleNo;          /*  Module number                                       */
    USHORT     FunctionNum;       /*  Number of functions                                 */
                                  /*  Sub-slot 겏 "0"                                     */
                                  /*  "0" when physical slot does not have any sub-slot.  */
    USHORT     HardwareVersion;   /*  HW version                                          */
    USHORT     SoftwareVersion;   /*  SW version                                          */
    USHORT     Status;            /*  Status                                              */

} PHYSICAL_SLOT_INFO,*LPPHYSICAL_SLOT_INFO,SUB_SLOT_INFO,*LPSUB_SLOT_INFO;


/************************************************************************/
/*    MODULE_CONFIG_HEADER                                              */
/*    Module configuration header                                       */
/************************************************************************/
typedef struct _module_configuration_header
{
    USHORT     MaxSlot;           /*  Number of slots                   */
    USHORT     Reserved;          /*  Reserved                          */

} MODULE_CONFIG_HEADER,*LPMODULE_CONFIG_HEADER;


/************************************************************************/
/*    MODULE_CONFIG_TABLE                                               */
/*    Module configuration                                              */
/************************************************************************/
typedef struct _module_configuration_table
{
    MODULE_CONFIG_HEADER    Header;            /*  Header                     */
    PHYSICAL_SLOT_INFO      PhyslotInfo[4];    /*  Physical slot information  */
    SUB_SLOT_INFO           SubslotInfo[8];    /*  Sub-slot information       */

}    MODULE_CONFIG_TABLE,*LPMODULE_CONFIG_TABLE;


/************************************************************************/
/*    ALARM_INFO                                                        */
/*    Alarm information structure                                       */
/************************************************************************/
typedef    struct    _alarm_information
{
    DWORD            ErrorCode;        /*  Error code                   */
    DWORD            ErrorLocation;    /*  Occurring position           */
    DWORD            DetectTask;       /*  Detector                     */
    DWORD            hDevice;          /*  Device handle                */
    DWORD            TaskID;           /*  Task ID                      */
    BYTE             TaskName[8];      /*  Task name                    */
    DWORD            ObjectHandle;     /*  Object handle                */
    BYTE             ObjectName[8];    /*  Object name                  */
    CALENDAR_DATA    Calendar;         /*  Calendar                     */
    DWORD            hAxis;            /*  Axis handle                  */
    DWORD            DetailError;      /*  Detailed error               */
    BYTE             Comment[32];      /*  Comment                      */

} ALARM_INFO,*LPALARM_INFO;


/************************************************************************/
/*    MECHATROLINK_INFO                                                 */
/*    MECHATROLINK information structure                                */
/************************************************************************/
typedef struct _mechatrolink_information
{
    WORD       TypeCode;               /*  Type code                    */
    WORD       InputAddress;           /*  Input starting address       */
    BYTE       InputWord;              /*  Number of input words        */
    BYTE       InputDisable;           /*  Input disabled               */
    WORD       OutputAddress;          /*  Output address               */
    BYTE       OutputWord;             /*  Output word                  */
    BYTE       OutputDisable;          /*  Output disabled              */
    BYTE       ScanType;               /*  I/O scan                     */
    BYTE       Mode;                   /*  Transmission mode            */
    DWORD      Reserved;               /*  Reserved                     */
    DWORD      NameSize;               /*  Name size                    */
    BYTE       Name[32];               /*  Name                         */

} MECHATROLINK_INFO,*LPMECHATROLINK_INFO;


/************************************************************************/
/*    PROGRAM_TYPE                                                      */
/************************************************************************/
typedef struct  _program_type
{
    LPBYTE     pFileName ;             /*  Fine Name storage pointer          */
    DWORD      Reserve ;               /*  Reservation region for the future  */

} PROGRAM_TYPE,*LPPROGRAM_TYPE;



/************************************************************************/
/*    PLS_EVENT                                                         */
/*    PLSEvent condition structure                                      */
/************************************************************************/
typedef struct _pls_event
{
    DOUBLE     MaximumValue;           /*  Maximum value                      */
    DOUBLE     MinimumValue;           /*  Minimum value                      */
    ULONG      hOutputBit;             /*  Output bit data handle             */
    ULONG      ReactionTime;           /*  Operation delay compensation time  */

} PLS_EVENT,*LPPLS_EVENT;


/************************************************************************/
/*    SYNC_DISTANCE                                                     */
/*    Master moving distance from GEAR function invalidation to stopping of Slave, or from GEAR function validation to Slave's synchronization with Master  */
/************************************************************************/
typedef struct _SyncDistance
{
    WORD       SyncType;               /*  Synchronization type specified              */
                                       /*    0 : Distance synchronization              */
                                       /*    1 : Time synchronization                  */
    WORD       DataType;               /*  Master axis moving distance data type       */
                                       /*    0 : Direct designation                    */
                                       /*    1 : Indirect designation                  */
    DWORD      DistanceData;           /*  Direct designation or indirect designation  */

} SYNC_DISTANCE,*LPSYNC_DISTANCE;


/************************************************************************/
/*    GEAR_RATIO_DATA                                                   */
/*    Data used for GEAR ratio setting                                  */
/************************************************************************/
typedef struct _GearRatioData
{
    WORD       Master;                 /*  GEAR ratio (numerator) specified    */
    WORD       Slave;                  /*  GEAR ratio (denominator) specified  */

} GEAR_RATIO_DATA,*LPGEAR_RATIO_DATA;


/************************************************************************/
/*    SYNC_POSITION                                                     */
/*    Structure to specify the position for master axis which starts CAM control  */
/************************************************************************/
typedef struct _SyncPosition
{
    DWORD      DataType;               /*  Master axis moving distance data type                */
                                       /*    0 : Direct designation                             */
                                       /*    1 : Indirect designation                           */
    DWORD      PositionData;           /*  Master axis moving distance direct value or pointer  */

} SYNC_POSITION,*LPSYNC_POSITION;


/************************************************************************/
/*    CAM_SHIFT_DATA                                                    */
/************************************************************************/
typedef struct _CamShiftData
{
    DWORD      ShiftType;              /*  Shift type                                  */
                                       /*    0 : Shift by time                         */
                                       /*    1 : Shift by position                     */
    DOUBLE     Offset;                 /*  CAM phase offset value                      */
    DOUBLE     Duration;               /*  Compensation time or compensation distance  */

} CAM_SHIFT_DATA,*LPCAM_SHIFT_DATA;


/************************************************************************/
/*    POSITION_OFFSET_DATA                                              */
/************************************************************************/
typedef struct _PositionOffsetData
{
    DWORD      ShiftType;              /*  Shift type                                  */
                                       /*    0: Shift by time                          */
                                       /*    1: Shift by position                      */
    DOUBLE     Offset;                 /*  Position compensation offset value          */
    DOUBLE     Duration;               /*  Compensation time or compensation distance  */

} POSITION_OFFSET_DATA,*LPPOSITION_OFFSET_DATA;


/************************************************************************/
/*    ARC_DATA                                                          */
/************************************************************************/
typedef struct _arc_data
{
    DOUBLE     StartAngle;             /*  Starting angle                                */
    DOUBLE     AngleTraversed;         /*  Rotating angle                                */
    DOUBLE     Radius;                 /*  Radius                                        */
    DOUBLE     RatioX;                 /*  Setting of horizontal axis ratio for ellipse  */
    DOUBLE     RatioY;                 /*  Setting of vertical axis ratio for ellipse    */
    DOUBLE     Velocity;               /*  Speed                                         */
    DOUBLE     Dummy1;                 /*  Not used                                      */
    DOUBLE     Dummy2;                 /*  Not used                                      */
    DOUBLE     Dummy3;                 /*  Not used                                      */

} ARC_DATA,*LPARC_DATA;


/************************************************************************/
/*    HELIX                                                             */
/************************************************************************/
typedef struct _helix_data
{
    DOUBLE     StartAngle;             /*  Starting angle                 */
    DOUBLE     AngleTraversed;         /*  Rotating angle                 */
    DOUBLE     Radius;                 /*  Radius                         */
    DOUBLE     Pitch;                  /*  Moving amount of helical axis  */
    DOUBLE     Velocity;               /*  Speed                          */
    DOUBLE     Dummy1;                 /*  Not used                       */
    DOUBLE     Dummy2;                 /*  Not used                       */
    DOUBLE     Dummy3;                 /*  Not used                       */
    DOUBLE     Dummy4;                 /*  Not used                       */

} HELIX_DATA,*LPHELIX_DATA;


/************************************************************************/
/*    LINEAR                                                            */
/************************************************************************/
typedef struct _linear_data
{
    DOUBLE     xAxis;                  /*  First axis target position or relative moving amount    */
    DOUBLE     yAxis;                  /*  Second axis target position or relative moving amount   */
    DOUBLE     zAxis;                  /*  Third axis target position or relative moving amount    */
    DOUBLE     FourthAxis;             /*  Fourth axis target position or relative moving amount   */
    DOUBLE     FifthAxis;              /*  Fifth axis target position or relative moving amount    */
    DOUBLE     SixthAxis;              /*  Sixth axis target position or relative moving amount    */
    DOUBLE     SeventhAxis;            /*  Seventh axis target position or relative moving amount  */
    DOUBLE     EighthAxis;             /*  Eighth axis target position or relative moving amount   */
    DOUBLE     Velocity;               /*  Speed                                                   */

} LINEAR_DATA,*LPLINEAR_DATA;


/************************************************************************/
/*    CONTOUR                                                           */
/************************************************************************/
typedef struct _contour_data
{
    DOUBLE     xAxis;                  /*  First axis target position or relative moving amount    */
    DOUBLE     yAxis;                  /*  Second axis target position or relative moving amount   */
    DOUBLE     zAxis;                  /*  Third axis target position or relative moving amount    */
    DOUBLE     FourthAxis;             /*  Fourth axis target position or relative moving amount   */
    DOUBLE     FifthAxis;              /*  Fifth axis target position or relative moving amount    */
    DOUBLE     SixthAxis;              /*  Sixth axis target position or relative moving amount    */
    DOUBLE     SeventhAxis;            /*  Seventh axis target position or relative moving amount  */
    DOUBLE     EighthAxis;             /*  Eighth axis target position or relative moving amount   */
    DOUBLE     SegmentTime;            /*  Segment compensaton time                                */

} CONTOUR_DATA,*LPCONTOUR_DATA;

/************************************************************************/
/*    SEGMENT_DATA                                                      */
/************************************************************************/
typedef struct _segment_data
{
    LONG       SegmentType;            /*  Interpolation type 걁0:EMPTY?1:ARC?2:HELIX?3:LINEAR_ABS?4:LINEAR_INC?5:CONTOUR걂        */
    union
    {
        ARC_DATA        Arc;
        HELIX_DATA      Helix;
        LINEAR_DATA     Linear;
        CONTOUR_DATA    Contour;

    } UnionData;
    LONG       StopSegment;            /*  Conditions to transit to next segment (0: distribution completed, 1: positioning completed)  */

} SEGMENT_DATA,*LPSEGMENT_DATA;


/************************************************************************/
/*    TABLE_INFO                                                        */
/************************************************************************/
typedef struct _tableInfo
{
    LONG       InterporationTableType;  /*  StaticTable=1,DynamicTable=2  */
    LONG       Entries;
    LONG       Available;               /*  StaticTable=0                 */
    LONG       Index;                   /*  StaticTable=0                 */

} TABLE_INFO,*LPTABLE_INFO;


/************************************************************************/
/*    DWORD_DATA                                                        */
/*    Structure to specify 2-word data/*                                */
/************************************************************************/
typedef struct dword_data_str
{
    LONG       DataType;               /*  Data type (0: immediate, 1: indirect designation)  */
    LONG       Data;                   /*  Setting data                                       */

} DWORD_DATA,*LPDWORD_DATA;


/************************************************************************/
/*    ComuDevice                                                        */
/*    Structure to specify communication device                         */
/************************************************************************/
typedef struct _COMMUDEVICE
{
    WORD       RackNumber;             /*  Rack number                  */
    WORD       SlotNumber;             /*  Slot number                  */
    WORD       SubSlotNumber;          /*  Sub-slot number              */
    WORD       Reserved;               /*  Reserved area                */

} COMMUDEVICE,*LPCOMMUDEVICE;


/************************************************************************/
/*    COMMUCHANNEL_INFO                                                 */
/*    Communication module channel infomation structure                 */
/************************************************************************/
typedef struct _COMMUCHANNEL_INFO
{
	WORD	ChannelState;				// Communication Channel State
	WORD	Reserved;					// Reserved area
	DWORD	hChannel;					// Communication Channel handle
} COMMUCHANNEL_INFO,*LPCOMMUCHANNEL_INFO;

/************************************************************************/
/*    MSGDATA                                                           */
/*    Communication data structure                                      */
/************************************************************************/
typedef struct _MSGDATA
{    
    DWORD      Timeout;                /*  <IN>  Sending monitoring timer, unit: ms           */
    WORD       PeerAddr;               /*  <IN>  Sending destination specified                */
    WORD       PeerCPU;                /*  <IN>  CPU number of sending destination            */
    LPVOID     pData;                  /*  <IN>  Pointer to where sending data is stored      */
    WORD       DataSize;               /*  <IN>  Size of request-to-send data (bytes)         */
    WORD       SendLen;                /*  <OUT> Size of data that was actually sent (bytes)  */
    LPVOID     pBuffer;                /*  <IN>  Pointer to where received data is stored     */
    WORD       BufferSize;             /*  <IN>  Size of received data storage buffer         */
    WORD       RecvLen;                /*  <OUT> Received data size                           */
    WORD       sReserve[2];            /*  Reserved                                          */

} MSGDATA,*LPMSGDATA;


/************************************************************************/
/*    MESSAGEPARAM                                                      */
/*    MESSAGE communication data structure                              */
/************************************************************************/
typedef struct _MSGPARAM
{    
    DWORD      Timeout;                /*  <IN>  Receiving monitoring timer, unit: ms        */
    WORD       fStatus;                /*  <OUT> Function status                             */
    WORD       mStatus;                /*  <OUT> Module status                               */
    WORD       PeerAddr;               /*  <IN>/<OUT> Communication station specified        */
    WORD       Option;                 /*  <IN>/<OUT> Option                                      */
    WORD       FuncCode;               /*  <IN>/<OUT> function code                          */
    WORD       DataAddr;               /*  <IN>/<OUT> data address                           */
    WORD       DataSize;               /*  <IN>/<OUT> Data size (number of bits or words)    */
    WORD       PeerCPU;                /*  <IN>/<OUT> Mating CPU number                      */
    WORD       CoilOffset;             /*  <IN>  Coil offset                                 */
    WORD       RelOffset;              /*  <IN>  Input relay offset                          */
    WORD       IRegOffset;             /*  <IN>  Input register offset                       */
    WORD       HRegOffset;             /*  <IN>  Holding register offset                     */
    WORD       LLimit;                 /*  Reserved                                          */
    WORD       HLimit;                 /*  Reserved                                          */
    WORD       sReserve[2];            /*  Reserved                                          */

} MSGPARAM,*LPMSGPARAM;


/************************************************************************/
/*    FILE_HANDLE                                                       */
/*    Structure to file handle for FILE SYSTEM                          */
/************************************************************************/
typedef struct file_handle_str
{
    DWORD      FileDeviceNumber;		/* Logical filesystem (LFS) id. */
    LPVOID*    pFileHandle;				/* Pointer to the LFS dependent file context: used by LFM. */

} FILE_HANDLE,*LPFILE_HANDLE;


/************************************************************************/
/*    FILE_STATUS                                                       */
/*    Structure to file status for FILE SYSTEM                          */
/************************************************************************/
typedef struct file_status_str
{
	DWORD  	   Time; 					/* Last modification time 		*/
	DWORD	   Size;					/* file size 					*/
	DWORD	   Attr;					/* file attribute 				*/

} FILE_STATUS,*LPFILE_STATUS;


/************************************************************************/
/*	 Sub Slot Configuration Definition									*/
/*																		*/
/************************************************************************/
/************************************************************************/
	/* Sub Slot Device Specification Stractural Body	*/
/************************************************************************/
typedef	struct	_SUBSLOTDEVICE
{
	WORD	rackNumber;								/* Rack Number		:1?4	*/
	WORD	slotNumber;								/* Slot Number		:1?9	*/
	WORD	subSlotNumber;							/* Sub Slot Number	:1?2	*/
	WORD	reserved;								/* Reserved					*/
}	SUBSLOTDEVICE;

typedef	SUBSLOTDEVICE*			LPSUBSLOTDEVICE;


/************************************************************************/
/* For CP-217 Device Information										*/
/************************************************************************/
typedef	struct	_NDSI_CP217
{
	BYTE	protocolType;							/* Protocol Type 									*/
	BYTE	funcMode;								/* Function Mode									*/
	BYTE	myAddr;									/* Device Address	***Master갌괥갂Slave갌괦?괮괨	*/
	BYTE	portType;								/* Port Type										*/
	BYTE	codeType;								/* Protocol Code									*/
	BYTE	charSize;								/* character bit length								*/
	BYTE	parityBit;								/* Parity Bit										*/
	BYTE	stopBit;								/* Stop Bit											*/
	BYTE	baudRate;								/* Transmission Speed								*/
	BYTE	transDelay;								/* Transmission Delay	***괥갌None갂괦?괦괥괥[ms]	*/
	BYTE	reserved[4];							/* Reserved											*/
	BYTE	autoReceive;							/* Auto Receive Selection							*/
	BYTE	autoRecRegister;						/* I/F Register										*/
	BYTE	inputRelaySV;							/* System Reserved?Variable Type	:01H Fixation	*/
	BYTE	inputRelayTB;							/* Data Type?Bit Address			:10H Fixation	*/
	WORD	inputRelayAddress;						/* Input Relay Register Top Address	:0?32767		*/
	WORD	inputRelaySize;							/* Input Relay Register Word Size	:0?32768		*/
	BYTE	reserved1[2];							/* Reserved											*/
	BYTE	inputRegisterSV;						/* System Rserved?Variable Type	:01H Fixation	*/
	BYTE	inputRegisterTB;						/* Data Type?Bit Address			:10H Fixation	*/
	WORD	inputRegisterAddress;					/* Input Register Top Address		:0?32767		*/
	WORD	inputRegisterSize;						/* Input Register Word Size			:0?32768		*/
	BYTE	reserved2[2];							/* Reserved 										*/
	BYTE	coilSV;									/* System Rserved?Variable Type	:03H Fixation	*/
	BYTE	coilTB;									/* Data Type?Bit Address			:10H Fixation	*/
	WORD	coilAddress;							/* Coil Register Top Address		:0?65534		*/
	WORD	coilSize;								/* Coil Register Word Size			:0?65535		*/
	BYTE	reserved3[2];							/* Reserved											*/
	BYTE	holdingRegisterSV;						/* System Rserved?Variable Type	:03H Fixation	*/
	BYTE	holdingRegisterTB;						/* Data Type?Bit Address			:10H Fixation	*/
	WORD	holdingRegisterAddress;					/* Holding Register Top Address		:0?65534		*/
	WORD	holdingRegisterSize;					/* Holding Register Word Size		:0?65535		*/
	BYTE	reserved4[2];							/* Reserved											*/
	WORD	writeRangeLO;							/* Coil/Holding Register Write Range Low	:0?65534*/
	WORD	writeRangeHI;							/* Coil/Holding Register Write Range High	:0?65534*/
	BYTE	reserved5[12];							/* Reserved											*/

} NDSI_CP217;


/************************************************************************/
/* For CP-218 My Infomation												*/
/************************************************************************/
typedef	struct	_NCP218_MYADDR
{
	LONG	myIP;									/* IP Address										*/
													/* No.1,2,3 Field:0?255,No.4 Field:1?254 (However, No.1 Field excludes "127")*/
	LONG	subnetMask;								/* Subnetmask										*/
													/* All Field:0?255 (However, "255.255.255.255" cannot be set.)	*/
	LONG	gWIP;									/* Gateway IP Address								*/
													/* No.1,2,3 Field:0?255,No.4 Field:1?254 (However, No.1 Field excludes "127".)*/
	WORD	engPort;								/* Engineering Port			 	:256?65535			*/
	WORD	SendTimout;								/* Memobus Response Watch Time[sec] 	:0?255		*/
	WORD	SendRetry;								/* Memobus Retry Time			:0?255				*/
	WORD	reserved1;								/* Reserved											*/
	WORD	tCPZeroWin;								/* TCP Zero Window Timer 		:1?255				*/
	WORD	tCPRetry;								/* TCP Retry Timer Value		:50?32000			*/
	WORD	tCPFin;									/* TCP End Timer Value			:1?255				*/
	WORD	iPAssemble;								/* IP Assembly Timer Value		:1?255				*/
	WORD	maxPktLen;								/* Maximum Packet Length		:64?1500			*/
	BYTE	reserved2[34];							/* Reserved											*/
} NCP218_MYADDR;

/************************************************************************/
/* For CP-218 Connection Infomation										*/
/************************************************************************/
typedef	struct	_NCP218_PEERADDR
{
	WORD	MyPort;									/* My Port Number 				:0,256?65535		*/
	WORD	Reserved;								/* Reserved											*/
	LONG	dstIP;									/* Destination IP Address							*/
	WORD	dstPort;								/* Destination Port Number		:0,256?65535		*/
	WORD	trasnportProtocol;						/* Transport Protocol (TCP/UDP)						*/
	WORD 	applProtocol;							/* Application Protocol								*/
	WORD 	codeType;								/* Code Type (ASCII/ BINARY/ RTU)					*/
	BYTE 	dstMacAddr[6];							/* Destination MAC Address							*/
													/* All Field:00?FF									*/
	BYTE	Reserved1[4];							/* Reserved											*/
	BYTE	NameSize;								/* Size of Name (byte)			:0?32				*/
	BYTE	Name[32];								/* Name												*/
	BYTE	Reserved2[1];							/* Reserved											*/
} NCP218_PEERADDR;

/************************************************************************/
/* For CP-218 Device infomation											*/
/************************************************************************/
typedef	struct	_NDSI_CP218
{
	NCP218_MYADDR	myAddr;							/* My infomation									*/
	NCP218_PEERADDR	peerAddr[20];					/* Connection Infomation							*/
} NDSI_CP218;


//<API5025> >>>>>
/************************************************************************/
/* For 218IFA My Infomation												*/
/************************************************************************/
typedef	struct	_N218IFA_MYADDR
{
	LONG	myIP;									/* IP Address										*/
													/* No.1,2,3 Field:0?255,No.4 Field:1?254 (However, No.1 Field excludes "127")*/
	LONG	subnetMask;								/* Subnetmask										*/
													/* All Field:0?255 (However, "255.255.255.255" cannot be set.)	*/
	LONG	gWIP;									/* Gateway IP Address								*/
													/* No.1,2,3 Field:0?255,No.4 Field:1?254 (However, No.1 Field excludes "127".)*/
	WORD	engPort;								/* Engineering Port			 	:256?65535			*/
	WORD	SendTimout;								/* Memobus Response Watch Time[sec] 	:0?255		*/
	WORD	SendRetry;								/* Memobus Retry Time			:0?255				*/
	WORD	reserved1;								/* Reserved											*/
	WORD	reserved2;								/* Reserved											*/
	WORD	reserved3;								/* Reserved											*/
	WORD	reserved4;								/* Reserved											*/
	WORD	reserved5;								/* Reserved											*/
	WORD	maxPktLen;								/* Maximum Packet Length		:1500 FIX			*/
	WORD	reserved6;								/* Reserved											*/
	WORD	FuncOpt1;								/* Function Selection Option 1						*/
	WORD	reserved7;								/* Reserved											*/
	BYTE	Name[16];								/* Equipment Name	:ASCII 16byte					*/
	BYTE	reserved8[12];							/* Reserved											*/
} N218IFA_MYADDR;

/************************************************************************/
/* For 218IFA Connection Infomation										*/
/************************************************************************/
typedef	struct	_N218IFA_PEERADDR
{
	WORD	MyPort;									/* My Port Number 				:0,256?65535		*/
	WORD	Reserved;								/* Reserved											*/
	LONG	dstIP;									/* Destination IP Address							*/
	WORD	dstPort;								/* Destination Port Number		:0,256?65535		*/
	WORD	trasnportProtocol;						/* Transport Protocol (TCP/UDP)						*/
	WORD 	applProtocol;							/* Application Protocol								*/
	WORD 	codeType;								/* Code Type (ASCII/ BINARY/ RTU)					*/
	BYTE 	Reserved1[6];							/* Reserved											*/
	BYTE	OPTION1;								/* OMRON PROTOCOL My Node Address :01H?FEH			*/
	BYTE	Reserved2;								/* Reserved											*/
	WORD	AutoMbusDelay;							/* Auto Memobus Delay Time	:0?100					*/
	BYTE	AutoMbusSel;							/* Auto Memobus Select								*/
	BYTE	SlvlfReg;								/* Slave Interface Register Setting					*/
	WORD	RelayOffset[4];							/* Input Relay Offset								*/
	WORD	IRegOffset[4];							/* Input Register Offset							*/
	WORD	CoilOffset[4];							/* Coil Offset										*/
	WORD	HRegOffset[4];							/* Holding Register Offset							*/
	WORD	LowLimit;								/* Range of writing (Low) 							*/
	WORD	HighLimit;								/* Range of writing (High)							*/
	WORD	BufChNo;								/* Transmission buffer channel number				*/
	BYTE	NameSize;								/* Destination Equipment Name Size 	:0?32			*/
	BYTE	Name[32];								/* Destination Equipment Name						*/
	BYTE	Reserved3[1];							/* Reserved											*/
} N218IFA_PEERADDR;

/************************************************************************/
/* For 218IFA Connection Infomation	(I/O Message)						*/
/************************************************************************/
typedef	struct	_N218IFA_IOPEERADDR
{
	WORD	MyPort;									/* My Port Number 				:0,256?65535		*/
	WORD	Reserved;								/* Reserved											*/
	LONG	dstIP;									/* Destination IP Address							*/
	WORD	dstPort;								/* Destination Port Number		:0,256?65535		*/
	WORD	trasnportProtocol;						/* Transport Protocol (TCP/UDP)						*/
	WORD 	applProtocol;							/* Application Protocol								*/
	WORD 	codeType;								/* Code Type (ASCII/ BINARY/ RTU)					*/
	BYTE 	Reserved1[6];							/* Reserved											*/
	BYTE	OPTION1;								/* OMRON PROTOCOL My Node Address :01?FEH			*/
	BYTE	Reserved2[3];							/* Reserved											*/
	BYTE	IomSel;									/* I/O Message Select								*/
	BYTE	IomPrmType;								/* I/O Message Prameter Kind						*/
	WORD 	IomOffset;								/* My Register Offset 	:0?7FFFH					*/
	WORD 	IomSize;								/* Data Size										*/
	BYTE	IomDisable;								/* I/O Message Disable Setting						*/
	BYTE	IomScan;								/* I/O Update Timing Setting						*/
	WORD	IomRegType;								/* Your Register Prameter Kind						*/
	WORD	IomRegNum;								/* Your Register Offset								*/
	BYTE	IomOption1;								/* I/O Message Option 1								*/
	BYTE	IomOption2;								/* I/O Message Option 2								*/
	BYTE	Reserved3[10];							/* Reserved											*/
	BYTE	NameSize;								/* Destination Equipment Name		:0?32			*/
	BYTE	Name[32];								/* Destination Equipment Name						*/
	BYTE	Reserved4[1];							/* Reserved											*/
} N218IFA_IOPEERADDR;

/************************************************************************/
/* For 218IFA Device infomation											*/
/************************************************************************/
typedef	struct	_NDSI_218IFA
{
	N218IFA_MYADDR		myAddr;						/* My infomation									*/
	N218IFA_PEERADDR	peerAddr[4];				/* Connection Infomation							*/
	N218IFA_IOPEERADDR	iopeerAddr[2];				/* Connection Infomation (I/O Message)				*/
} NDSI_218IFA;

/************************************************************************/
/* For 218IFC Device infomation											*/
/************************************************************************/
typedef	struct	_NDSI_218IFC
{
	N218IFA_MYADDR		myAddr;						/* My infomation									*/
	N218IFA_PEERADDR	peerAddr[20];				/* Connection Infomation							*/
	N218IFA_IOPEERADDR	iopeerAddr[2];				/* Connection Infomation (I/O Message)				*/
} NDSI_218IFC;

/************************************************************************/
/* For 218IFB My Infomation												*/
/************************************************************************/
typedef	struct	_N218IFB_MYADDR
{
	LONG	myIP;									/* IP Address										*/
													/* No.1,2,3 Field:0?255,No.4 Field:1?254 (However, No.1 Field excludes "127")*/
	LONG	subnetMask;								/* Subnetmask										*/
													/* All Field:0?255 (However, "255.255.255.255" cannot be set.)	*/
	LONG	gWIP;									/* Gateway IP Address								*/
													/* No.1,2,3 Field:0?255,No.4 Field:1?254 (However, No.1 Field excludes "127".)*/
	WORD	engPort;								/* Engineering Port			 	:256?65535			*/
	WORD	SendTimout;								/* Memobus Response Watch Time[sec] 	:0?255		*/
	WORD	SendRetry;								/* Memobus Retry Time			:0?255				*/
	WORD	reserved1;								/* Reserved											*/
	WORD	reserved2;								/* Reserved											*/
	WORD	reserved3;								/* Reserved											*/
	WORD	reserved4;								/* Reserved											*/
	WORD	reserved5;								/* Reserved											*/
	WORD	maxPktLen;								/* Maximum Packet Length		:64?1500			*/
	WORD	reserved6;								/* Reserved											*/
	WORD	FuncOpt1;								/* Function Selection Option 1						*/
	WORD	reserved7;								/* Reserved											*/
	BYTE	Name[16];								/* Equipment Name	:ASCII 16byte					*/
	BYTE	reserved8[12];							/* Reserved											*/
} N218IFB_MYADDR;

/************************************************************************/
/* For 218IFB Connection Infomation										*/
/************************************************************************/
typedef	struct	_N218IFB_PEERADDR
{
	WORD	MyPort;									/* My Port Number 				:0,256?65535		*/
	WORD	Reserved;								/* Reserved											*/
	LONG	dstIP;									/* Destination IP Address							*/
	WORD	dstPort;								/* Destination Port Number		:0,256?65535		*/
	WORD	trasnportProtocol;						/* Transport Protocol (TCP/UDP)						*/
	WORD 	applProtocol;							/* Application Protocol								*/
	WORD 	codeType;								/* Code Type (ASCII/ BINARY/ RTU)					*/
	BYTE 	Reserved1[6];							/* Reserved											*/
	BYTE	OPTION1;								/* OMRON PROTOCOL My Node Address :01H?FEH			*/
	BYTE	Reserved2;								/* Reserved											*/
	WORD	Reserved3;								/* Reserved											*/
	BYTE	Reserved4;								/* Reserved											*/
	BYTE	Reserved5;								/* Reserved											*/
	WORD 	Reserved6[4];							/* Reserved											*/
	WORD 	Reserved7[4];							/* Reserved											*/
	WORD 	Reserved8[4];							/* Reserved											*/
	WORD 	Reserved9[4];							/* Reserved											*/
	WORD	Reserved10;								/* Reserved											*/
	WORD	Reserved11;								/* Reserved											*/
	WORD	Reserved12;								/* Reserved											*/
	BYTE	NameSize;								/* Destination Equipment Name Size 	:0?32			*/
	BYTE	Name[32];								/* Destination Equipment Name						*/
	BYTE	Reserved13[1];							/* Reserved											*/
} N218IFB_PEERADDR;

/************************************************************************/
/* For 218IFB Device infomation											*/
/************************************************************************/
typedef	struct	_NDSI_218IFB
{
	N218IFB_MYADDR		myAddr;						/* My infomation									*/
	N218IFB_PEERADDR	peerAddr[20];				/* Connection Infomation							*/
} NDSI_218IFB;
//<API5025> <<<<<

/************************************************************************/
/* REGISTER INFO                                                        */
/************************************************************************/
typedef struct _REGISTER_INFO
{    
	DWORD	hRegisterData;							/* handle to RegisterData 							*/
	DWORD	RegisterDataNumber;						/* Number of RegisterData 							*/
	LPVOID	pRegisterData;							/* pointer of Stored RegisterData 					*/
	DWORD	ReadDataNumber;							/* Stored RegisterData Number 						*/
} REGISTER_INFO;

typedef	REGISTER_INFO*		LPREGISTER_INFO;		/* pointer of REGISTER INFO 						*/


/************************************************************************/
/* 둮뮗REGISTER INFO                          */
/************************************************************************/
#pragma pack(push,4)
typedef struct _REGISTER_INFO_EX
{    
	QWORD	hRegisterData;							/* handle to RegisterData */
	DWORD	RegisterDataNumber;						/* Number of RegisterData */
	LPVOID	pRegisterData;							/* pointer of Stored RegisterData */
	DWORD	ReadDataNumber;							/* Stored RegisterData Number */
} REGISTER_INFO_EX;

typedef	REGISTER_INFO_EX*		LPREGISTER_INFO_EX;		/* pointer of REGISTER INFO */
#pragma pack(pop)

/************************************************************************/
/*	BUSIF INFO															*/
/************************************************************************/
typedef struct	_busif_info
{
	WORD	InputDataMaxSize;						/* InputDataMaxSize걁Word) 							*/
	WORD	InputDataAvailableSize;					/* InputDataAvailableSize걁Word걂					*/
	WORD	OutputDataMaxSize;						/* OutputDataMaxSize걁Word걂						*/
	WORD	OutputDataAvailableSize;				/* OutputDataAvailableSize걁Word걂					*/

} BUSIF_INFO, *LPBUSIF_INFO;

#endif /* _API_STRUCT_H */
