/************************************************************************

        ymcAPIConstants.h

        COPYRIGHT (C) YASKAWA ELECTRIC CORPORATION

*************************************************************************/


#ifndef _API_CONSTANTS_H
#define _API_CONSTANTS_H

/************************************************************************/
/*                                                                      */
/*              << Constant definition header file >>                   */
/*                                                                      */
/*      File name   :  ymcAPIConstants.h                                */
/*                                                                      */
/*                                                                      */
/************************************************************************/


/************************************************************************/
/* AxisType                                                             */
/************************************************************************/
#define REAL_AXIS               1       /*  Real servo axis             */
#define VIRTUAL_AXIS            2       /*  Virtual servo axis          */
#define EXTERNAL_ENCODER        3       /*  External encoder            */


/************************************************************************/
/* Specify type Axis specification method                               */
/************************************************************************/
#define PHYSICALAXIS            1       /*  Physical axis specified     */
#define AXISNAME                2       /*  Axis name specified         */
#define LOGICALAXIS             3       /*  Logical axis specified      */

#define HMETHOD_DEC1_C          0       /*  0: DEC1 + phase-C pulse method        */
#define HMETHOD_ZERO            1       /*  1: ZERO signal method                 */
#define HMETHOD_DEC1_ZERO       2       /*  2: DEC1 + ZERO signal method          */
#define HMETHOD_C               3       /*  3: Phase-C pulse method               */
#define HMETHOD_DEC2_ZERO       4       /*  4: DEC2 + ZERO signal method          */
#define HMETHOD_DEC1_L_ZERO     5       /*  5: DEC1 + LMT + ZERO signal method    */
#define HMETHOD_DEC2_C          6       /*  6: DEC2 + phase-C pulse method        */
#define HMETHOD_DEC1_L_C        7       /*  7: DEC1 + LMT + phase-C pulse method  */

#define HMETHOD_C_ONLY          11      /*  11: C Pulse Only            */
#define HMETHOD_POT_C           12      /*  12: POT & C Pulse           */
#define HMETHOD_POT_ONLY        13      /*  13: POT Only                */
#define HMETHOD_HOMELS_C        14      /*  14: Home LS C Pulse         */
#define HMETHOD_HOMELS_ONLY     15      /*  15: Home LS Only            */
#define HMETHOD_NOT_C           16      /*  16: NOT & C Pulse           */
#define HMETHOD_NOT_ONLY        17      /*  17: NOT Only                */
#define HMETHOD_INPUT_C         18      /*  18: Input & C Pulse         */
#define HMETHOD_INPUT_ONLY      19      /*  19: Input Only              */


/************************************************************************/
/* Direction                                                            */
/************************************************************************/
#define DIRECTION_POSITIVE      0       /*  Positive direction          */
#define DIRECTION_NEGATIVE      1       /*  Negative direction          */


/************************************************************************/
/* Coordinate system specified                                          */
/************************************************************************/
#define WORK_SYSTEM             0       /*  Workpiece coordinate system     */
#define MACHINE_SYSTEM          1       /*  Machine coordinate system       */


/************************************************************************/
/* Motion type                                                          */
/************************************************************************/
#define MTYPE_RELATIVE          0       /*  0: Incremental value specified, common for linear and rotary axes                                         */
#define MTYPE_ABSOLUTE          1       /*  1: Absolute position speciifed, for linear axis                                                           */
#define MTYPE_R_SHORTEST        2       /*  2: Absolute position specified, for rotary axis (Rotates in the direction closer to the target position.) */
#define MTYPE_R_POSITIVE        3       /*  3: Absolute position specified, for rotary axis (forward run)                                             */
#define MTYPE_R_NEGATIVE        4       /*  4: Absolute position specified, for rotary axis (reverse run)                                             */


/************************************************************************/
/* Data type                                                            */
/************************************************************************/
#define DTYPE_IMMEDIATE         0       /*  Direct designation                      */
#define DTYPE_INDIRECT          1       /*  Indirect designation                    */
#define DTYPE_MAX_VELOCITY      0x0001  /*  bit0: Designation for Max. Velocity     */
#define DTYPE_ACCELERATION      0x0002  /*  bit1: Designation for Acceleration      */
#define DTYPE_DECELERATION      0x0004  /*  bit2: Designation for Deceleration      */
#define DTYPE_FILTER_TIME       0x0008  /*  bit3: Designation for FilterTime        */
#define DTYPE_VELOCITY          0x0010  /*  bit4: Designation for Velocity          */
#define DTYPE_APPROCH           0x0020  /*  bit5: Designation for ApproachVelocity  */
#define DTYPE_CREEP             0x0040  /*  bit6: Designation for CreepVelocity     */


/************************************************************************/
/* Feeding speed type                                                   */
/************************************************************************/
#define VTYPE_UNIT_PAR          0       /*  Speed [reference unit/s]              */
#define VTYPE_PERCENT           1       /*  Rated speed percentage (%) specified  */


/************************************************************************/
/* Acceleration/deceleration type                                       */
/************************************************************************/
#define ATYPE_UNIT_PAR          0       /*  Acceleration [reference unit/s2]  */
#define ATYPE_TIME              1       /*  Time constant [ms]                */
#define ATYPE_KEEP              2       /*  Current setting held              */


/************************************************************************/
/* Filter type                                                          */
/************************************************************************/
#define FTYPE_S_CURVE           0       /*  0: Move average filter (simple S-curve)  */
#define FTYPE_EXP               1       /*  1: Exponential filter                    */
#define FTYPE_NOTHING           2       /*  2: Without filter                        */
#define FTYPE_KEEP              3       /*  3: Current setting held                  */


/************************************************************************/
/* WaitForCompletion Constant definition                                */
/************************************************************************/
#define DISTRIBUTION_COMPLETED  0       /*  Distribution compled        */
#define POSITIONING_COMPLETED   1       /*  Positioning completed       */
#define COMMAND_STARTED         2       /*  Command completed           */
#define LATCH_COMMAND_STARTED   0       /*  Latch command started       */
#define LATCH_COMPLETED         1       /*  Latch completed             */


/************************************************************************/
/* SystemOption constant definition                                     */
/************************************************************************/
#define OP_BIT_ALARM_CONTINUE   0x00000001   /*  bit0: Normal axis operation continued at alarm occurrence  */


/************************************************************************/
/* Target for ChangeDynamics (1: Changed, 0: Not changed)               */
/************************************************************************/
#define SUBJECT_ACC             0x0001    /*  bit0: Acceleration        */
#define SUBJECT_DEC             0x0002    /*  bit1: Deceleration        */
#define SUBJECT_POS             0x0008    /*  bit3: Position            */
#define SUBJECT_VEL             0x0010    /*  bit4: Velocity            */


/************************************************************************/
/* ServoControlType                                                     */
/************************************************************************/
#define SERVO_OFF               0x00    /*  Servo OFF                   */
#define SERVO_ON                0x01    /*  Servo ON                    */


/************************************************************************/
/* Device type                                                          */
/************************************************************************/
#define DEVICETYPE_IO           1       /*  I/O                         */
#define DEVICETYPE_DIRECTIO     2       /*  Direct I/O                  */
#define DEVICETYPE_GLOBALDATA   3       /*  Global data                 */
#define DEVICETYPE_REGISTER     4       /*  Register                    */


/************************************************************************/
/* Unit data size (number of bits)                                      */
/************************************************************************/
#define DATAUNITSIZE_BIT        1       /*  1 bit                       */
#define DATAUNITSIZE_BYTE       8       /*  BYTE = 8 bits               */
#define DATAUNITSIZE_WORD       16      /*  WORD = 16 bits              */
#define DATAUNITSIZE_LONG       32      /*  LONG = 32 bits              */
#define DATAUNITSIZE_FLOAT      32      /*  FLOAT= 32 bits              */
#define DATAUNITSIZE_DOUBLE     64      /*  DOUBLE= 64 bits             */


/************************************************************************/
/* Semaphore type                                                       */
/************************************************************************/
#define SEMAPHORE_NOUSE         0       /*  Semaphore not used          */
#define SEMAPHORE_USE           1       /*  Semaphore used              */


/************************************************************************/
/* ComDevice type                                                       */
/************************************************************************/
#define RS232C_MODE             1       /*  RS232C                      */
#define MODEM_MODE              2       /*  Modem                       */
#define ETHERNET_MODE           3       /*  Ethernet                    */
#define PCI_MODE                4       /*  PCI bus (910)               */
#define CONTROLLER_MODE         5       /*  Interior of Controller      */
#define PCIe_MODE               6       /*  PCI bus (910)               */
#define MAX_CURRENT_ALARM       32


/************************************************************************/
/* BitEvent type                                                        */
/************************************************************************/
#define OFF_TO_ON               0       /*  Rising edge detection                                      */
#define ON_TO_OFF               1       /*  Falling edge detection                                     */
#define LEVEL_ON                3       /*  Level signal ON (Event only once at level ON detection)    */
#define LEVEL_OFF               4       /*  Level signal OFF (Event only once at level OFF detection)  */


/************************************************************************/
/* Gear                                                                 */
/************************************************************************/
#define MASTER_AXIS_FEEDBACK    0       /*  Feedback value              */
#define MASTER_AXIS_COMMAND     1       /*  Command value               */


/************************************************************************/
/* Gear synchronization type                                            */
/************************************************************************/
#define SYNCH_DISTANCE          0       /*  Distance synchronization    */
#define SYNCH_TIME              1       /*  Time synchroniztaion        */


/************************************************************************/
/* Attribute of Gear command                                            */
/************************************************************************/
#define GEAR_ENGAGE_COMPLETED   0       /*  Gear control started (Engage completed)  */
#define GEAR_COMMAND_STARTED    1       /*  Command started                          */


/************************************************************************/
/* Attribute of Gear status                                             */
/************************************************************************/
#define GEAR_NOT_ENGAGED           0    /*  During GEAR stop            */
#define GEAR_WAITING_ENGAGED       1    /*  Waiting for GEAR motion     */
#define GEAR_ENGAGED               2    /*  During GEAR motion          */
#define GEAR_WAITING_DISENGAGED    4    /*  Waiting for GEAR to stop    */


/************************************************************************/
/* Attribute of CAM command                                             */
/************************************************************************/
#define CAM_ENGAGE_COMPLETED       0    /*  CAM control started               */
#define CAM_SHIFT_COMPLETED        0    /*  CAM phase compensation completed  */

#define CAM_DISENGAGE_COMPLETED    0    /*  CAM control stopped               */
#define CAM_COMMAND_STARTED        1    /*  Command started                   */


/************************************************************************/
/* Attribute of Cam status                                              */
/************************************************************************/
#define CAM_NOT_ENGAGED            0    /*  During CAM stop             */
#define CAM_WAITING_ENGAGED        1    /*  Waiting for CAM motion      */
#define CAM_ENGAGED                2    /*  During CAM motion           */
#define CAM_WAITING_DISENGAGED     4    /*  Waiting for CAM to stop     */


/************************************************************************/
/* Shift type                                                           */
/************************************************************************/
#define SHIFT_TIME                 0    /*  Shift by time               */
#define SHIFT_POSITION             1    /*  Shift by position           */


/************************************************************************/
/* Attribute of POSITION command                                        */
/************************************************************************/
#define POSITION_OFFSET_COMPLETED          0    /*  Position compensation completed  */
#define POSITION_OFFSET_COMMAND_STARTED    1    /*  Command started                  */


/************************************************************************/
/* Table type                                                           */
/************************************************************************/
#define CAM_TABLE               2       /*  CAM table file name            */
#define INTERPOLATION_TABLE     3       /*  Interpolation table file name  */
#define REGISTERHANDLE          4       /*  Register handle                */
#define USER_FUNCTION           5       /*  User function                  */
#define IK_FUNCTION             6       /*  IK function                    */


/************************************************************************/
/* Motion parameter type                                                */
/************************************************************************/
#define SETTING_PARAMETER       0       /*  Setting parameter           */
#define MONITOR_PARAMETER       1       /*  Monitor parameter           */
#define FIXED_PARAMETER         2       /*  Fixed parameter             */


/************************************************************************/
/* Cyclic event                                                         */
/************************************************************************/
#define HIGH_SCAN               1       /*  High-speed scan             */
#define MIDDLE_SCAN             2       /*  Medium-speed scan           */
#define LOW_SCAN                3       /*  Low-speed scan              */

#define SCAN_OCCURED            1       /*  Minimum                     */


/************************************************************************/
/* Other program (task) action                                          */
/************************************************************************/
#define START_PROGRAM           1       /*  Other program (task) start      */


/************************************************************************/
/* Move control action                                                  */
/************************************************************************/
#define START_MOVE              1       /*  Start                       */
#define HOLD_MOVE               2       /*  Hold                        */
#define RELEASE_HOLD            3       /*  Hold released               */
#define ABORT_MOVE              4       /*  Abort                       */
#define SKIP_MOVE               5       /*  Skip                        */


/************************************************************************/
/* Attribute of move command                                            */
/************************************************************************/
#define DISCHARGING_COMPLETED   0       /*  Distribution completed      */
#define POSITIONING_COMPLETED   1       /*  Positioning completed       */


/************************************************************************/
/* Segment type                                                         */
/************************************************************************/
#define SEGMENT_TYPE_EMPTY         0    /*  Not used                    */
#define SEGMENT_TYPE_ARC           1
#define SEGMENT_TYPE_HELIX         2
#define SEGMENT_TYPE_LINEAR_ABS    3
#define SEGMENT_TYPE_LINEAR_INC    4
#define SEGMENT_TYPE_CONTOUR       5

#define MAX_DEVICE_AXIS_NUM        16   /*  Maximum number of device axes  */


/************************************************************************/
/* Circular arc type for circular arc and helical interpolation         */
/************************************************************************/
#define LESS_180DEGREE          0x01
#define GREATER_180DEGREE       0x02


/************************************************************************/
/* Coordinate system                                                    */
/************************************************************************/
#define COORDINATE_SYSTEM_DEFAULT    0
#define COORDINATE_SYSTEM_MACHINE    1    /*  Machine coordinate system  */


/************************************************************************/
/* Mode                                                                 */
/************************************************************************/
#define MODE_INCREMENTAL        0       /*    INC                       */
#define MODE_ABSOLUTE           1       /*    ABS                       */


/************************************************************************/
/* Feeding speed type                                                   */
/************************************************************************/
#define F_TYPE_COMMAND_UNIT     0       /*  Reference unit/min          */
#define F_TYPE_PARCENT          1       /*  % specified                 */


/************************************************************************/
/* Acceleration/deceleraton type                                        */
/************************************************************************/
#define ACCEL_TYPE_ACCERALATION     0   /*  Acceleration                */
#define ACCEL_TYPE_TIME_CONSTANT    1   /*  Time constant               */
#define ACCEL_TYPE_NO_SPECIFY       2   /*  Not specified               */


/************************************************************************/
/* Move event                                                           */
/************************************************************************/
#define MOVE_EVENT_DISTRIBUTION_START        0x01    /*  Distribution starting event       */
#define MOVE_EVENT_DISTRIBUTION_COMPLETED    0x02    /*  Distribution completion event     */
#define MOVE_EVENT_POSITION_COMPLETED        0x03    /*  Positioning completion event      */
#define MOVE_EVENT_POSITION_COINCIDED        0x04    /*  Specified position passing event  */
#define MOVE_EVENT_VELOCITY_COINCIDED        0x05    /*  Speed coincidence event           */
#define MOVE_EVENT_TORQUE_COINCIDED          0x06    /*  Torque coincidence event          */
#define MOVE_EVENT_ACCELERATION_COMPLETED    0x07    /*  Acceleration completion event     */
#define MOVE_EVENT_DECELERATION_START        0x08    /*  Deceleration starting event       */
#define MOVE_EVENT_LATCH_COMPLETED           0x09    /*  Latch completion event            */
#define MOVE_EVENT_ALARM_OCCURRED            0x0A    /*  Alarm occurrence event            */
#define MOVE_EVENT_ABORT_OCCURRED            0x0B    /*  Abort occurrence event            */

#define MOVE_EVENT_SPECIFIED_DISTRIBUTION_START        0x10    /*  Specified record distribution started    */
#define MOVE_EVENT_SPECIFIED_DISTRIBUTION_COMPLETED    0x11    /*  Specified record distribution completed  */


/************************************************************************/
/* Data comparison event                                                */
/************************************************************************/
#define EQUAL                   0x10    /*  Equal                       */
#define NOT_EQUAL               0x11    /*  Not equal                   */
#define GREATER                 0x12    /*  Greater than                */
#define LESS                    0x13    /*  Smaller than                */
#define GREATER_EQUAL           0x14    /*  Equal or greater than       */
#define LESS_EQUAL              0x15    /*  Equal or smaller than       */


/************************************************************************/
/* Event within the data range                                          */
/************************************************************************/
#define WITHIN                  0x20    /*  Within the range            */
#define WITHOUT                 0x21    /*  Out of the range            */


/************************************************************************/
/* Message event                                                        */
/************************************************************************/
#define MESSAGE_RECIEVED        1       /*  Message received            */


/************************************************************************/
/* Timer event                                                          */
/************************************************************************/
#define TIMEUP                  1       /*  Time up                     */


/************************************************************************/
/* Move action                                                          */
/************************************************************************/
#define MOVE_ACTION_START_MOVE         0x01    /*  Start action                     */
#define MOVE_ACTION_HOLD_MOVE          0x02    /*  Hold action                      */
#define MOVE_ACTION_RELESE_HOLD        0x03    /*  Hold release action              */
#define MOVE_ACTION_ABORT_MOVE         0x04    /*  Abort action                     */
#define MOVE_ACTION_SKIP_MOVE          0x05    /*  Skip action                      */
#define MOVE_ACTION_POSITION_VALUE     0x06    /*  Target position change action    */
#define MOVE_ACTION_SPEED_VALUE        0x07    /*  Reference speed change action    */
#define MOVE_ACTION_TORQUE_VALUE       0x08    /*  Reference torque change action   */
#define MOVE_ACTION_OVERRIDE           0x09    /*  Override change action           */
#define MOVE_ACTION_ACCELTIME_VALUE    0x0A    /*  Acceleration time change action  */
#define MOVE_ACTION_DECELTIME_VALUE    0x0B    /*  Deceleration time change action  */


/************************************************************************/
/* Servo control action                                                 */
/************************************************************************/
#define SERVO_OFF               0x00    /*  Servo OFF                   */
#define SERVO_ON                0x01    /*  Servo ON                    */


/************************************************************************/
/* Bit setting action                                                   */
/************************************************************************/
#define SET_BIT_OFF             0x00    /*  Bit OFF                     */
#define SET_BIT_ON              0x01    /*  Bit ON                      */


/************************************************************************/
/* Bit getting action                                                   */
/************************************************************************/
#define GET_IO                  1       /*  Get I/O.                    */


/************************************************************************/
/* Data setting action                                                  */
/************************************************************************/
#define SET_VALUE               0x10    /*  Data setting                */


/************************************************************************/
/* Data getting action                                                  */
/************************************************************************/
#define GET_VALUE               0x10    /*  Data getting                */


/************************************************************************/
/* Message action                                                       */
/************************************************************************/
#define SEND_MESSAGE            1       /*  Message sent                */
#define RECEIVE_MESSAGE         2       /*  Message received            */


/************************************************************************/
/* Timer action                                                         */
/************************************************************************/
#define START_TIMER             1       /*  Start                       */
#define STOP_TIMER              2       /*  Stop                        */
#define CONTINUE_TIMER          3       /*  Continuous start            */
#define RESET_TIMER             4       /*  Reset                       */


/************************************************************************/
/* User function action                                                 */
/************************************************************************/
#define START_USERFUNCTION      1       /*  Start                       */
#define ABORT_USERFUNCTION      2       /*  Abort                       */


/************************************************************************/
/* Log read-out                                                         */
/************************************************************************/
#define SEND_LOG                1       /*  Sending log                 */
#define RECV_LOG                2       /*  Receiving log               */


/************************************************************************/
/* Attribute of move                                                    */
/************************************************************************/
#define MOVE_TYPE_DISTRIBUTION_COMPLETED      0x00    /*  Distribution completed  */
#define MOVE_TYPE_POSITIONING_COMPLETED       0x01    /*  Positioning completed   */
#define MOVE_TYPE_POSITIONING_NEIGHBORHOOD    0x02    /*  Second INP completed    */


/************************************************************************/
/* Related to event log                                                 */
/************************************************************************/
#define EVENTLOG_BUF_TYPE_LINEAR        0    /*  Linear buffer          */
#define EVENTLOG_BUF_TYPE_RING          1    /*  Ring buffer            */

#define EVENTLOG_DATA_TYPE_LOGDATA      1    /*  Log data               */
#define EVENTLOG_DATA_TYPE_STARTTIME    2    /*  Starting time          */
#define EVENTLOG_DATA_TYPE_STOPTIME     3    /*  Stopping time          */


/************************************************************************/
/* Axis type                                                            */
/************************************************************************/
#define AXISTYPE_USE                 1    /*  Actual servo axis used    */
#define AXISTYPE_VIRTUAL             2    /*  Virtual servo axis used   */
#define AXISTYPE_EXTERNAL_ENCODER    3    /*  External encoder used     */


/************************************************************************/
/* Specified data type when getting axis handle                         */
/************************************************************************/
#define GETAXISHANDLE_PHYSICAL_NO_TYPE    1    /*  Physical axis specified  */
#define GETAXISHANDLE_NAME_TYPE           2    /*  Name specified           */


/************************************************************************/
/* UnitType definition                                                  */
/************************************************************************/
#define UNITTYPE_PULSE          0       /*  Pulse                       */
#define UNITTYPE_MM             1       /*  mm                          */
#define UNITTYPE_INCH           2       /*  inch                        */
#define UNITTYPE_DEGREE         3       /*  degree                      */


/************************************************************************/
/* Data type                                                            */
/************************************************************************/
#define DATATYPE_IMMEDIATE      0       /*  Direct designation          */
#define DATATYPE_INDIRECT       1       /*  Indirect designation        */


/************************************************************************/
/* ComDevice type                                                       */
/************************************************************************/
#define COMDEVICETYPE_RS232C_MODE         1    /*  RS232C                  */
#define COMDEVICETYPE_MODEM_MODE          2    /*  Modem                   */
#define COMDEVICETYPE_ETHERNET_MODE       3    /*  Ethernet                */
#define COMDEVICETYPE_PCI_MODE            4    /*  PCI bus (910)           */
#define COMDEVICETYPE_CONTROLLER_MODE     5    /*  Interior of Controller  */
#define	COMDEVICETYPE_PCIe_MODE			  6	   /* PCIe긫긚(Mp3100)		   */		


/************************************************************************/
/* Distribution synthesis action                                        */
/************************************************************************/
#define ENABLE_SUPERPOSE_ACTION     1    /*  Distribution synthesis Enabled   */
#define DISABLE_SUPERPOSE_ACTION    2    /*  Distribution synthesis Disabled  */


/************************************************************************/
/* CPU control type                                                     */
/************************************************************************/
#define CONTROLTYPE_RUN         1       /*    CPU Run                   */
#define CONTROLTYPE_STOP        2       /*    CPU Stop                  */


/************************************************************************/
/* File type definition                                                 */
/************************************************************************/
#define FILETYPE_DWG            1
#define FILETYPE_FUNC           2
#define FILETYPE_DEFTBL         3
#define FILETYPE_IOMAP          4
#define FILETYPE_TDEF           5
#define FILETYPE_HTBL           6
#define FILETYPE_CTBL           7
#define FILETYPE_MENU           8
#define FILETYPE_MOTION         9
#define FILETYPE_Cinclude       10


/************************************************************************/
/* Semaphore type definition                                            */
/************************************************************************/
#define SEMAPHORETYPE_NOUSE     0       /*  Not used                    */
#define SEMAPHORETYPE_USE       1       /*  Used                        */


/************************************************************************/
/* CAM execution status definition                                      */
/************************************************************************/
#define CAM_NOT_ENGAEGD           0     /*  During CAM control stop           */
#define CAM_WAITITNG_ENGAEGD      1     /*  Waiting for CAM control to start  */
#define CAM_ENGAEGD               2     /*  During CAM control                */
#define CAM_WAITING_DISENGAEGD    4     /*  Waiting for CAM control to stop   */


/************************************************************************/
/* Motion parameter type                                                */
/************************************************************************/
#define SETTING_PARAMETER       0       /*  Setting parameter           */
#define MONITOR_PARAMETER       1       /*  Monitor parameter           */
#define FIXED_PARAMETER         2       /*  Fixed parameter (reserved)  */


/************************************************************************/
/* CommuDiviceType Definition                                           */
/************************************************************************/
#define YMC_COM_DEV_217         0       /*  CP-217                            */
#define YMC_COM_DEV_218         1       /*  CP-218                            */
#define YMC_COM_DEV_260         2       /*  CP-260                            */
#define YMC_COM_DEV_261         3       /*  CP-261                            */
#define YMC_COM_DEV_PCI         4       /*  MP910X for PCI bus                */
#define YMC_COM_DEV_CPCI        5       /*  MP910X for CPCI bus               */
#define YMC_COM_DEV_ISA         6       /*  MP910X for ISA bus                */
#define YMC_COM_DEV_CONTROLLER  7       /*  Application in Controller         */


/************************************************************************/
/* File type definition                                                 */
/************************************************************************/
/* Path,File	*/
#define	MP_FILE_MAX_PATH_LEN			256			/* MAX Path-Name Length			*/
#define	MP_FILE_MAX_FILENAME_LEN		256			/* MAX File-Name Length			*/

/* Mount point	*/
#define	MP_MOUNT_POINT					"dev"		/* Mount  Mount Point Name		*/
#define	MP_MEDIA_DIR 					"card"		/* Media Top Directory			*/

/* File Open Mode */
#define	MP_FILE_O_READ					1			/* Read							*/
#define	MP_FILE_O_WRITE					2			/* Write						*/
#define	MP_FILE_O_READWRITE		(MP_FILE_O_READ|MP_FILE_O_WRITE)
													/* Read/Write					*/

/* File attributes */
#define	MP_FILE_S_IFMT					0xf000		/* Mask Data for File Type		*/
#define	MP_FILE_S_IFDIR					0x2000		/* Directory	 				*/
#define	MP_FILE_S_IFREG					0x1000 		/* Regular File					*/
#define	MP_FILE_S_IEXEC					4			/* Executable/chdir				*/
#define	MP_FILE_S_IWRITE				2 			/* Writable						*/
#define	MP_FILE_S_IREAD					1 			/* Readable		 				*/
#define	MP_FILE_S_IREADWRITE	(MP_FILE_S_IWRITE|MP_FILE_S_IREAD)
													/* Rewritable					*/

/* Seek originate positions	*/
#define	MP_FILE_SEEK_SET				0			/* Top 							*/
#define	MP_FILE_SEEK_CUR				1			/* Current 						*/
#define	MP_FILE_SEEK_END				2			/* End							*/


/************************************************************************/
/* Sub Slot Configuration Definition									*/
/************************************************************************/
/* SubSlotDiviceType Definition		*/
#define		YMC_SSLOT_DEV_217					1		/* CP-217					*/
#define		YMC_SSLOT_DEV_218					2		/* CP-218					*/
#define		YMC_SSLOT_DEV_260					3		/* CP-260					*/
#define		YMC_SSLOT_DEV_261					4		/* CP-261					*/
#define		YMC_SSLOT_DEV_215					5		/* CP-215					*/
#define		YMC_SSLOT_DEV_218A					6		/* 218IFA <API5025>			*/
#define		YMC_SSLOT_DEV_218B					7		/* 218IFB <API5025>			*/
#define		YMC_SSLOT_DEV_218C					8		/* 218IFC <API5025>			*/

/* ReadFileType	Definition			*/
#define		YMC_SSLOT_FILE_EXIST				0		/* Existing Definition		*/
#define		YMC_SSLOT_FILE_DEFAULT				1		/* Default Definition		*/

/* Protocol Type 					*/
#define		YMC_COM_SPROT_STDMSG				0		/* MEMOBUS					*/
#define		YMC_COM_SPROT_MELSEC				1		/* MELSEC					*/
#define		YMC_COM_SPROT_OMRON					2		/* OMRON					*/
#define		YMC_COM_SPROT_THROUGH				3		/* Non-Protocol				*/
/* Function Mode					*/
#define		YMC_COM_SFUNC_MASTER				0		/* Master					*/
#define		YMC_COM_SFUNC_SLAVE					1		/* Slave					*/
/* Device Address	***Master갌괥갂Slave갌괦?괮괨	*/
#define		YMC_COM_SMASTER_ADDR				0		/* Master					*/
#define		YMC_COM_SSLAVE_ADDR_MIN				1		/* Slave Min				*/
#define		YMC_COM_SSLAVE_ADDR_MAX				63		/* Slave Max				*/
/* Port Type						*/
#define		YMC_COM_SPORT_232					0		/* RS-232C					*/
#define		YMC_COM_SPORT_485					1		/* RS-485					*/
#define		YMC_COM_SPORT_422					2		/* RS-422					*/
/* Protocol Code					*/
#define		YMC_COM_SCODE_NONE					0		/* NONE						*/
#define		YMC_COM_SCODE_RTU					1		/* RTU						*/
#define		YMC_COM_SCODE_ASC					2		/* ASCII					*/
/* character bit length				*/
#define		YMC_COM_SCHAR_7BIT					7		/* 7bit						*/
#define		YMC_COM_SCHAR_8BIT					8		/* 8bit						*/
/* Parity Bit						*/
#define		YMC_COM_SPARI_NONE					0		/* None						*/
#define		YMC_COM_SPARI_ODD					1		/* Odd						*/
#define		YMC_COM_SPARI_EVEN					2		/* Even						*/
/* Stop Bit							*/
#define		YMC_COM_SSTOP_1BIT					1		/* 1bit						*/
#define		YMC_COM_SSTOP_2BIT					2		/* 2bit						*/
/* Transmission Speed				*/
#define		YMC_COM_SBPS_4800					6		/* 4800bps					*/
#define		YMC_COM_SBPS_9600					7		/* 9600bps					*/
#define		YMC_COM_SBPS_14400					8	 	/* 14400bps					*/
#define		YMC_COM_SBPS_19200					9		/* 19200bps					*/
#define		YMC_COM_SBPS_28800					10		/* 28800bps					*/
#define		YMC_COM_SBPS_38400					11		/* 38400bps					*/
#define		YMC_COM_SBPS_48000					12		/* 48000bps					*/
#define		YMC_COM_SBPS_57600					13		/* 57600bps					*/
#define		YMC_COM_SBPS_76800					15		/* 76800bps					*/
#define		YMC_COM_SBPS_115200					16		/* 115200bps	<API5025>	*/
/* Transmission Delay	***괥갌None갂괦?괦괥괥[ms]	*/
#define		YMC_COM_SSDLY_MIN					0		/* 0						*/
#define		YMC_COM_SSDLY_MAX					100		/* 100						*/
/* Auto Receive Selection			*/
#define		YMC_COM_SAUTO_REC_INVALID			0		/* Invalid					*/
#define		YMC_COM_SAUTO_REC_VALID				1		/* Valid
/* I/F Register						*/
#define		YMC_COM_SAUTO_REC_REG_INVALID 		0		/* Invalid					*/
#define		YMC_COM_SAUTO_REC_REG_VALID   		1		/* Valid					*/
/* System Reserved?Variable Type	:01H Fixation	*/
#define		YMC_COM_SINPUTREL_SV				01		/* 01						*/
/* Data Type?Bit Address			:10H Fixation	*/
#define		YMC_COM_SINPUTREL_TB				0x10	/* 10H						*/
/* Input Relay Register Top Address	:0?32767	*/
#define		YMC_COM_SINPUTREL_ADD_MIN			0		/* 0						*/
#define		YMC_COM_SINPUTREL_ADD_MAX			32767	/* 32767					*/
/* Input Relay Register Word Size	:0?32768	*/
#define		YMC_COM_SINPUTREL_SIZ_MIN			0		/* 0						*/
#define		YMC_COM_SINPUTREL_SIZ_MAX			32768	/* 32768					*/
/* System Rserved?Variable Type	:01H Fixation	*/
#define		YMC_COM_SINPUTREG_SV				01		/* 01						*/
/* Data Type?Bit Address			:10H Fixation	*/
#define		YMC_COM_SINPUTREG_TB				0x10	/* 10H						*/
/* Input Register Top Address		:0?32767	*/
#define		YMC_COM_SINPUTREG_ADD_MIN			0		/* 0						*/
#define		YMC_COM_SINPUTREG_ADD_MAX			32767	/* 32767					*/
/* Input Register Word Size			:0?32768	*/
#define		YMC_COM_SINPUTREG_SIZ_MIN			0		/* 0						*/
#define		YMC_COM_SINPUTREG_SIZ_MAX			32768	/* 32768					*/
/* System Rserved?Variable Type	:03H Fixation	*/
#define		YMC_COM_SCOIL_SV					03		/* 03						*/
/* Data Type?Bit Address			:10H Fixation	*/
#define		YMC_COM_SCOIL_TB					0x10	/* 10H						*/
/* Coil Register Top Address		:0?65534	*/
#define		YMC_COM_SCOIL_ADD_MIN				0		/* 0						*/
#define		YMC_COM_SCOIL_ADD_MAX				65534	/* 65534					*/
/* Coil Register Word Size			:0?65535	*/
#define		YMC_COM_SCOIL_SIZ_MIN				0		/* 0						*/
#define		YMC_COM_SCOIL_SIZ_MAX				65535	/* 65535					*/
/* System Rserved?Variable Type	:03H Fixation	*/
#define		YMC_COM_SHREG_SV					03		/* 03						*/
/* Data Type?Bit Address			:10H Fixation	*/
#define		YMC_COM_SHREG_TB					0x10	/* 10H						*/
/* Holding Register Top Address		:0?65534	*/
#define		YMC_COM_SHREG_ADD_MIN				0		/* 0						*/
#define		YMC_COM_SHREG_ADD_MAX				65534	/* 65534					*/
/* Holding Register Word Size		:0?65535	*/
#define		YMC_COM_SHREG_SIZ_MIN				0		/* 0						*/
#define		YMC_COM_SHREG_SIZ_MAX				65535	/* 65535					*/
/* Coil/Holding Register Write Range Low	:0?65534	*/
#define		YMC_COM_SWR_RNG_LO_MIN				0		/* 0						*/
#define		YMC_COM_SWR_RNG_LO_MAX				65534	/* 65534					*/
/* Coil/Holding Register Write Range High	:0?65534	*/
#define		YMC_COM_SWR_RNG_HI_MIN				0		/* 0						*/
#define		YMC_COM_SWR_RNG_HI_MAX				65534	/* 65534					*/

/* IP Address						*/
#define		YMC_COM_EIP_NO4_MIN					1		/* 1						*/
#define		YMC_COM_EIP_NO4_MAX					254		/* 254						*/
/* Engineering Port			 	:256?65535	*/
#define		YMC_COM_EENGP_MIN					256		/* 256						*/
#define		YMC_COM_EENGP_MAX					65535	/* 65535					*/
/* Memobus Response Watch Time[sec] 	:0?255	*/
#define		YMC_COM_ESTOUT_MIN					0		/* 0	sec					*/
#define		YMC_COM_ESTOUT_MAX					255		/* 255	sec					*/
/* Memobus Retry Time			:0?255	*/
#define		YMC_COM_ESRTY_MIN					0		/* 0						*/
#define		YMC_COM_ESRTY_MAX					255		/* 255						*/
/* TCP Zero Window Timer 		:1?255	*/
#define		YMC_COM_ETZW_MIN					1		/* 1	sec					*/
#define		YMC_COM_ETZW_MAX					255		/* 255	sec					*/
/* TCP Retry Timer Value		:50?32000	*/
#define		YMC_COM_ETRT_MIN					50		/* 50	sec					*/
#define		YMC_COM_ETRT_MAX					32000	/* 32000sec					*/
/* TCP End Timer Value			:1?255	*/
#define		YMC_COM_ETFN_MIN					1		/* 1	sec					*/
#define		YMC_COM_ETFN_MAX					255		/* 255	sec					*/
/* IP Assembly Timer Value		:1?255	*/
#define		YMC_COM_EIPASM_MIN					1		/* 1	sec					*/
#define		YMC_COM_EIPASM_MAX					255		/* 255	sec					*/
/* Maximum Packet Length		:64?1500	*/
#define		YMC_COM_EPKT_MIN					64		/* 64	byte				*/
#define		YMC_COM_EPKT_MAX					1500	/* 1500	byte				*/

/* Transport Protocol (TCP/UDP)		*/
#define		YMC_COM_ETPROT_TCP					0		/* TCP						*/
#define		YMC_COM_ETPROT_UDP					1		/* UDP						*/
/* Function Selection Option 1		*/
#define		YMC_COM_FOPT_SINGLEBUF				0		/* Single Buffer	<API5025>	*/
#define		YMC_COM_FOPT_MULTIBUF				1		/* Multi Buffer		<API5025>	*/
/* Application Protocol				*/
#define		YMC_COM_EAPROT_MEMOEX				0		/* Ex-MEMOBUS				*/
#define		YMC_COM_EAPROT_MEMOBUS				1		/* MEMOBUS					*/
#define		YMC_COM_EAPROT_MELSEC_A				2		/* MELSEC					*/
#define		YMC_COM_EAPROT_THROUGH				3		/* Non-Protocol				*/
#define		YMC_COM_EAPROT_MODTCP				4		/* MODBUS/TCP				*/
#define		YMC_COM_EAPROT_OMRON				5		/* OMRON(FINS)			<API5025>	*/
#define		YMC_COM_EAPROT_MELSEC_Q3E			6		/* MELSEC QnA 뚚듂 3E	<API5025>	*/
/* Code Type (ASCII/ BINARY/ RTU)	*/
#define		YMC_COM_ECODE_RTU					1		/* RTU						*/
#define		YMC_COM_ECODE_ASC					2		/* ASCII					*/
#define		YMC_COM_ECODE_BIN					3		/* BINARY					*/
/* Auto Memobus Selection			*/
#define		YMC_COM_AUTOMBUS_OFF				0		/* OFF			<API5025>	*/
#define		YMC_COM_AUTOMBUS_ON					1		/* ON			<API5025> 	*/
/* Slave Interface Register			*/
#define		YMC_COM_SLVIF_ON					1		/* ON			<API5025>	*/
/* Input Relay Offset				*/
#define YMC_COM_RELOFS_SVTB						0x1001	/* 1001 Fix		<API5025>	*/
#define YMC_COM_RELOFS_DEFADR					0		/* 0			<API5025>	*/
#define YMC_COM_RELOFS_DEFSIZE					65535	/* 65535 byte	<API5025>	*/
/* Input Register Offset			*/
#define YMC_COM_IREGOFS_SVTB					0x1001	/* 1001 Fix		<API5025>	*/
#define YMC_COM_IREGOFS_DEFADR					0		/* 0			<API5025>	*/
#define YMC_COM_IREGOFS_DEFSIZE					65535	/* 65535 byte	<API5025>	*/
/* Coil Offset						*/
#define YMC_COM_COILOFS_SVTB					0x1003	/* 1001 Fix		<API5025>	*/
#define YMC_COM_COILOFS_DEFADR					0		/* 0			<API5025>	*/
#define YMC_COM_COILOFS_DEFSIZE					65535	/* 65535 byte	<API5025>	*/
/* Hold Register					*/
#define YMC_COM_HREGOFS_SVTB					0x1003	/* 1003 Fix		<API5025>	*/
#define YMC_COM_HREGOFS_DEFADR					0		/* 0			<API5025>	*/
#define YMC_COM_HREGOFS_DEFSIZE					65535	/* 65535 byte	<API5025>	*/
/* Coil/Hold Register Write Area Limit Low	*/
#define YMC_COM_LLIMIT_DEF						0		/* 0			<API5025>	*/
/* Coil/Hold Register Write Area Limit High	*/
#define YMC_COM_HLIMIT_DEF						65534	/* 65534		<API5025>	*/
/* Transmission Buffer Channel Number		*/
#define YMC_COM_BUFCHNO_DEF						1		/* 1			<API5025>	*/
/* IO Message Selection				*/
#define YMC_COM_IOMSG_OFF						0		/* OFF			<API5025>	*/
#define YMC_COM_IOMSG_ON						1		/* ON			<API5025>	*/
/* IO Message Parameter Type		*/
#define YMC_COM_IOMSG_READ						0		/* Read Parameter	<API5025>	*/
#define YMC_COM_IOMSG_WRITE						1		/* Write Parameter	<API5025>	*/
/* IO Message Disable				*/
#define YMC_COM_IOMSG_ENABLE					0		/* Enable		<API5025>	*/
#define YMC_COM_IOMSG_DISABLE					1		/* Disable		<API5025>	*/
/* IO Message Update Selection		*/
#define YMC_COM_IOMSG_SCAN_H					'H'		/* H Scan		<API5025>	*/
#define YMC_COM_IOMSG_SCAN_L					'L'		/* L Scan		<API5025>	*/
/* IO Message Register Type			*/
#define YMC_COM_IOMSG_MEMB_MW					0		/* MEMOBUS		MW	<API5025>	*/
#define YMC_COM_IOMSG_MEL_X						10		/* MELSEC		X	<API5025>	*/
#define YMC_COM_IOMSG_MEL_Y						11		/* 				Y	<API5025>	*/
#define YMC_COM_IOMSG_MEL_M						12		/* 				M	<API5025>	*/
#define YMC_COM_IOMSG_MEL_B						13		/* 				B	<API5025>	*/
#define YMC_COM_IOMSG_MEL_L						14		/* 				L	<API5025>	*/
#define YMC_COM_IOMSG_MEL_S						15		/* 				S	<API5025>	*/
#define YMC_COM_IOMSG_MEL_SM					16		/* 				SM	<API5025>	*/
#define YMC_COM_IOMSG_MEL_D						20		/* 				D	<API5025>	*/
#define YMC_COM_IOMSG_MEL_W						21		/* 				W	<API5025>	*/
#define YMC_COM_IOMSG_MEL_R						22		/* 				R	<API5025>	*/
#define YMC_COM_IOMSG_MEL_SD					23		/* 				SD	<API5025>	*/
#define YMC_COM_IOMSG_MEL_ZR					24		/* 				ZR	<API5025>	*/
#define YMC_COM_IOMSG_MODB_4X					30		/* MODBUS/TCP	4X	<API5025>	*/
#define YMC_COM_IOMSG_OMR_CIO					40		/* OMRON		CIO	<API5025>	*/
#define YMC_COM_IOMSG_OMR_WR					41		/* 				WR	<API5025>	*/
#define YMC_COM_IOMSG_OMR_HR					42		/* 				HR	<API5025>	*/
#define YMC_COM_IOMSG_OMR_AR					43		/* 				AR	<API5025>	*/
#define YMC_COM_IOMSG_OMR_DM					50		/* 				DM	<API5025>	*/
/*	IO Message Option Setting 2		*/
#define YMC_COM_IOMSG_MEL_3FF					0		/* Local system CPU		<API5025>	*/
#define YMC_COM_IOMSG_MEL_3D0					1		/* Control system CPU	<API5025>	*/
#define YMC_COM_IOMSG_MEL_3D1					2		/* Standby system CPU	<API5025>	*/
#define YMC_COM_IOMSG_MEL_3D2					3		/* A system CPU			<API5025>	*/
#define YMC_COM_IOMSG_MEL_3D3					4		/* B system CPU			<API5025>	*/
#define YMC_COM_IOMSG_MEL_3E0					5		/* Multi CPU1			<API5025>	*/
#define YMC_COM_IOMSG_MEL_3E1					6		/* Multi CPU2			<API5025>	*/
#define YMC_COM_IOMSG_MEL_3E2					7		/* Multi CPU3			<API5025>	*/
#define YMC_COM_IOMSG_MEL_3E3					8		/* Multi CPU4			<API5025>	*/

#define		YMC_COM_EPEERADDRMAX				20		/* Maximum Connection Number	*/
#define		YMC_COM_EIFAPEERADDRMAX				4		/* Maximum Connection Number for 218IFA		<API5025> */
#define		YMC_COM_EIOPEERADDRMAX				2		/* Maximum Connection Number for IO Message	<API5025> */

/************************************************************************/
/* Communication Channel State of array                                 */
/************************************************************************/
#define		YMC_COM_FREE_CHANNEL				0		/* Free						*/
#define		YMC_COM_USED_CHANNEL				1		/* Used						*/

/************************************************************************/
/* Message Communication Definition                                     */
/************************************************************************/
/* Function code    */
#define		YMC_SFC_RD_COIL							0x01		// Read coil status
#define		YMC_SFC_RD_INPUT_RELAY					0x02		// Read input relay status
#define		YMC_SFC_RD_HOLDING_REG					0x03		// Read holding register contents
#define		YMC_SFC_RD_INPUT_REG					0x04		// Read input register contents
#define		YMC_SFC_WR_SINGLE_COIL					0x05		// Modify status of a single coil
#define		YMC_SFC_WR_SINGLE_HOLDING_REG			0x06		// Write to a single holding register
#define		YMC_SFC_LOOPBACK_TEST					0x08		// LoopBack test
#define		YMC_SFC_RD_HOLDING_REG_EX				0x09		// Read holding register contents(extended)
#define		YMC_SFC_RD_INPUT_REG_EX					0x0a		// Read input register contents(extended)
#define		YMC_SFC_WR_HOLDING_REG_EX				0x0b		// Write to holding register(extended)
#define		YMC_SFC_DESC_RD_MULTI_HOLDING_REG_EX	0x0d		// Read discoutinuos multiple holding registers contents(extended)
#define		YMC_SFC_DESC_WR_MULTI_HOLDING_REG_EX	0x0e		// Write to discontinuous multiple holding registers(extended)
#define		YMC_SFC_WR_MULTI_COIL					0x0f		// Modify status of multiple coils
#define		YMC_SFC_WR_MULTI_HOLDING_REG			0x10		// Write to multiple holding registers
#define		YMC_SFC_MEL_WR_FIX_BUF					0x31		// Fixed buffer communication(MELSEC)
#define		YMC_SFC_MEL_RD_RDM_BUF					0x32		// Read random-access buffer(MELSEC)
#define		YMC_SFC_MEL_WR_RDM_BUF					0x33		// Write to random-access buffer(MELSEC)

#endif /* #ifndef _API_CONSTANTS_H */
