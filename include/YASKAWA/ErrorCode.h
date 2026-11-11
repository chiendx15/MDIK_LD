/************************************************************************

        ErrorCode.h

        COPYRIGHT (C) YASKAWA ELECTRIC CORPORATION

*************************************************************************/


#ifndef _ERROR_CODE_H
#define _ERROR_CODE_H

/************************************************************************/
/*                                                                      */
/*              << Error code definition >>                             */
/*                                                                      */
/*      File name   :  ErrorCode.h                                      */
/*                                                                      */
/*                                                                      */
/************************************************************************/


#define	MP_SUCCESS							0x00000000		/*	API関数 正常終了									*/
															/*	API function normal completion						*/
#define	MP_FAIL								0x4000ffff		/*	API関数 異常終了									*/
															/*	API function erroneous completion					*/
#define	WDT_OVER_ERR						0x81000001		/*														*/
															/*														*/
#define	MANUAL_RESET_ERR					0x82000020		/*	Manual reset error									*/
															/*														*/
#define	TLB_MLTHIT_ERR						0x82000140		/*	TLB multi hit error									*/
															/*														*/
#define	UBRK_ERR							0x820001E0		/*	User break execution error							*/
															/*														*/
#define	ADR_RD_ERR							0x820000E0		/*	Address read error error							*/
															/*														*/
#define	TLB_MIS_RD_ERR						0x82000040		/*	TLB read mis error									*/
															/*														*/
#define	TLB_PROTECTION_RD_ERR				0x820000A0		/*	TLB protection read vaiolation error				*/
															/*														*/
#define	GENERAL_INVALID_INS_ERR				0x82000180		/*	General invalid instruction error					*/
															/*														*/
#define	SLOT_ERR							0x820001A0		/*	Slot invalid instruction error						*/
															/*														*/
#define	GENERAL_FPU_DISABLE_ERR				0x82000800		/*	General FPU disable error							*/
															/*														*/
#define	SLOT_FPU_ERR						0x82000820		/*	Slot FPU exception error							*/
															/*														*/
#define	ADR_WR_ERR							0x82000100		/*	Data address write error error						*/
															/*														*/
#define	TLB_MIS_WR_ERR						0x82000060		/*	TLB write mis error									*/
															/*														*/
#define	TLB_PROTECTION_WR_ERR				0x820000C0		/*	TLB protection write vaiolation error				*/
															/*														*/
#define	FPU_EXP_ERR							0x82000120		/*	FPU exception error									*/
															/*														*/
#define	INITIAL_PAGE_EXP_ERR				0x82000080		/*	Initial page write exception error					*/
															/*														*/
#define	ROM_ERR								0x81000041		/*	ROM  error											*/
															/*														*/
#define	RAM_ERR								0x81000042		/*	RAM  error											*/
															/*														*/
#define	MPU_ERR								0x81000043		/*	CPU  error											*/
															/*														*/
#define	FPU_ERR								0x81000044		/*	FPU  error											*/
															/*														*/
#define	CERF_ERR							0x81000049		/*	CERF error											*/
															/*														*/
#define	EXIO_ERR							0x81000050		/*	EXIO error											*/
															/*														*/
#define	BUSIF_ERR							0x8100005F		/*	Common RAM error for OEM							*/
															/*														*/
#define	ALM_NO_ALM							0x00000000		/*	アラ??なし状態																					*/
															/*	No alarm																							*/
#define	ALM_MK_DEBUG						0x67050300		/*	軽故障	デバッグ用アラ??コ?ド																	*/
															/*	Minor failure: Alarm code for debugging																*/
#define	ALM_MK_ROUND_ERR					0x67050301		/*	軽故障	半径指定で1周?エラ?																		*/
															/*	Minor failure: Improper specification of one cycle at radius specification							*/
#define	ALM_MK_FSPEED_OVER					0x67050302		/*	軽故障	送り速度オ?バ?																			*/
															/*	Minor failure: Feeding speed exceeded																*/
#define	ALM_MK_FSPEED_NOSPEC				0x67050303		/*	軽故障	送り速度指定無し																			*/
															/*	Minor failure: Feeding speed not specified															*/
#define	ALM_MK_PRM_OVER						0x67050304		/*	軽故障	加減速パラメ??の変換後の値が範囲外														*/
															/*	Minor failure: Value after conversion of acceleration or deceleration parameter is out of range.	*/
#define	ALM_MK_ARCLEN_OVER					0x67050305		/*	軽故障	弧の長さがLONG_MAXを超えた																	*/
															/*	Minor failure: Arc length exceeds LONG_MAX.															*/
#define	ALM_MK_VERT_NOSPEC					0x67050306		/*	軽故障	平面指定の縦軸指定無し																		*/
															/*	Minor failure: Vertical axis by plane specification not specified									*/
#define	ALM_MK_HORZ_NOSPEC					0x67050307		/*	軽故障	平面指定の横軸指定無し																		*/
															/*	Minor failure: Horizontal axis by plane specification not specified									*/
#define	ALM_MK_TURN_OVER					0x67050308		/*	軽故障	??ン数指定オ?バ?																		*/
															/*	Minor failure: Specified number of turns exceeded													*/
#define	ALM_MK_RADIUS_OVER					0x67050309		/*	軽故障	半径がLONG_MAXを超えた																		*/
															/*	Minor failure: Radius exceeds LONG_MAX.																*/
#define	ALM_MK_CENTER_ERR					0x6705030A		/*	軽故障	中心?指定エラ?																			*/
															/*	Minor failure: Illegal center point specification													*/
#define	ALM_MK_BLOCK_OVER					0x6705030B		/*	軽故障	直線補間block移動量オ?バ?																	*/
															/*	Minor failure: Linear interpolation block moving amount exceeded									*/
#define	ALM_MK_MAXF_NOSPEC					0x6705030C		/*	軽故障	maxf未定?																					*/
															/*	Minor failure: maxf not defined																		*/
#define	ALM_MK_TDATA_ERR					0x6705030D		/*	軽故障	アドレスTデ??エラ?																		*/
															/*	Minor failure: Address T data error																	*/
#define	ALM_MK_REG_ERR						0x6705030E		/*	軽故障	REGデ??エラ? PP異常																		*/
															/*	Minor failure: REG data error and PP fault															*/
#define	ALM_MK_COMMAND_CODE_ERR				0x6705030F		/*	軽故障	コ?ンド範囲外																				*/
															/*	Minor failure: Out-of-range command																	*/
#define	ALM_MK_AXIS_CONFLICT				0x67050310		/*	軽故障	?理軸使用禁?中																			*/
															/*	Minor failure: Use of logical axis being prohibited													*/
#define	ALM_MK_POSMAX_OVER					0x67050311		/*	軽故障	無限長軸、MVMまたはABSの座標指定がPOSMAXを超えた											*/
															/*	Minor failure: Infinite length axis, MVM or ABS coordinate designation exceeds POSMAX.				*/
#define	ALM_MK_DIST_OVER					0x67050312		/*	軽故障	軸の移動距離が(LONG_MIN,LONG_MAX)以外														*/
															/*	Minor failure: Axis moving distance is other than (LONG_MIN, LONG_MAX).								*/
#define	ALM_MK_MODE_ERR						0x67050313		/*	軽故障	制御モ?ド不正																				*/
															/*	Minor failure: Illegal control mode																	*/
#define	ALM_MK_CMD_CONFLICT					0x67050314		/*	軽故障	モ?ションコ?ンド重複指令																	*/
															/*	Minor failure: Motion command overlapping instruction												*/
#define	ALM_MK_RCMD_CONFLICT				0x67050315		/*	軽故障	モ?ションレス?ンスコ?ンド重複指令														*/
															/*	Minor failure: Motion response command overlapping instruction										*/
#define	ALM_MK_CMD_MODE_ERR					0x67050316		/*	軽故障	モ?ションコ?ンドモ?ド不正																*/
															/*	Minor failure: Illegal motion command mode															*/
#define	ALM_MK_CMD_NOT_ALLOW				0x67050317		/*	軽故障	コ?ンドはこのモジュ?ルでは実行できない													*/
															/*	Minor failure: Command cannot be executed ih this Module.											*/
#define	ALM_MK_CMD_DEN_FAIL					0x67050318		/*	軽故障	払い出し完了状態でない																		*/
															/*	Minor failure: Distribution is not completed.														*/
#define	ALM_MK_H_MOVE_ERR					0x67050319		/*	軽故障	hMove不正																					*/
															/*	Minor failure: Illegal hMove																		*/
#define	ALM_MK_MOVE_NOT_SUPPORT				0x6705031A		/*	軽故障	未サ??トのMoveの定?																		*/
															/*	Minor failure: Non-supported Move defined															*/
#define	ALM_MK_EVENT_NOT_SUPPORT			0x6705031B		/*	軽故障	未サ??トのMove Eventの定?																*/
															/*	Minor failure: Non-supported Move Event defined														*/
#define	ALM_MK_ACTION_NOT_SUPPORT			0x6705031C		/*	軽故障	未サ??トのMove Actionの定?																*/
															/*	Minor failure: Non-supported Move Action defined													*/
#define	ALM_MK_MOVE_TYPE_ERR				0x6705031D		/*	軽故障	MoveType指定エラ?																			*/
															/*	Minor failure: MoveType specification error															*/
#define	ALM_MK_VTYPE_ERR					0x6705031E		/*	軽故障	VelocityType指定エラ?																		*/
															/*	Minor failure: VelocityType specification error														*/
#define	ALM_MK_ATYPE_ERR					0x6705031F		/*	軽故障	AccelerationType指定エラ?																	*/
															/*	Minor failure: AccelerationType specification error													*/
#define	ALM_MK_HOMING_METHOD_ERR			0x67050320		/*	軽故障	homing_method指定エラ?																		*/
															/*	Minor failure: Homing_method specification error													*/
#define	ALM_MK_ACC_ERR						0x67050321		/*	軽故障	加速度設定エラ?																			*/
															/*	Minor failure: Acceleration setting error															*/
#define	ALM_MK_DEC_ERR						0x67050322		/*	軽故障	減速度設定エラ?																			*/
															/*	Minor failure: Deceleration setting error															*/
#define	ALM_MK_POS_TYPE_ERR					0x67050323		/*	軽故障	位置指令?イプエラ?																		*/
															/*	Minor failure: Position reference type error														*/
#define	ALM_MK_INVALID_EVENT_ERR			0x67050324		/*	軽故障	不正EVENT?イプエラ?																		*/
															/*	Minor failure: Illegal EVENT type																	*/
#define	ALM_MK_INVALID_ACTION_ERR			0x67050325		/*	軽故障	不正ACTION?イプエラ?																		*/
															/*	Minor failure: Illegal ACTION type																	*/
#define	ALM_MK_MOVE_NOT_ACTIVE				0x67050326		/*	軽故障	未実行のMoveに対するAction																	*/
															/*	Minor failure: Action for Move that has not been executed											*/
#define	ALM_MK_MOVELIST_NOT_ACTIVE			0x67050327		/*	軽故障	未実行のMoveListに対するAction																*/
															/*	Minor failure: Action for MoveList that has not been executed										*/
#define	ALM_MK_TBL_INVALID_DATA				0x67050328		/*	軽故障	テ?ブルデ??不正エラ?																	*/
															/*	Minor failure: Illegal table data																	*/
#define	ALM_MK_TBL_INVALID_SEG_NUM			0x67050329		/*	軽故障	テ?ブル補間 実行セグメント数不正															*/
															/*	Minor failure: Illegal number of table interpolation execution segments								*/
#define	ALM_MK_TBL_INVALID_AXIS_NUM			0x6705032A		/*	軽故障	テ?ブル補間 軸数指定エラ?																	*/
															/*	Minor failure: Illegal number of table interpolation axes specified									*/
#define	ALM_MK_TBL_INVALID_ST_SEG			0x6705032B		/*	軽故障	テ?ブル補間 開始セグメントNo不正															*/
															/*	Minor failure: Illegal table interpolation starting segment number									*/
#define	ALM_MK_STBL_INVALID_EXE				0x6705032C		/*	軽故障	Static tableテ?ブル フ?イル書き込み中の実行エラ?											*/
															/*	Minor failure: Execution error during Static table file being written								*/
#define	ALM_MK_DTBL_DUPLICATE_EXE			0x6705032D		/*	軽故障	Dynamic tableテ?ブル ２重実行エラ?														*/
															/*	Minor failure: Dynamic table duplicated execution error												*/
#define	ALM_MK_LATCH_CONFLICT				0x6705032E		/*	軽故障	LATCH要求重複指令エラ?																		*/
															/*	Minor failure: LATCH request overlapping instruction error											*/
#define	ALM_MK_INVALID_AXISTYPE				0x6705032F		/*	軽故障	軸?イプ（有限長軸/無限長軸）不正															*/
															/*	Minor failure: Illegal axis type (finite length axis/inifinite length axis)							*/
#define	ALM_MK_NOT_SVCRDY					0x67050330		/*	軽故障	モ?ションドライバ???備未完状態でのMoveオブジェクト実行									*/
															/*	Minor failure: Move object executed when Motion Driver operation is not ready						*/
#define	ALM_MK_NOT_SVCRUN					0x67050331		/*	軽故障	サ??電源未完状態でのMoveオブジェクト実行													*/
															/*	Minor failure: Move object executed at servo OFF													*/
#define	ALM_MK_MDALARM						0x67050332		/*	軽故障	モ?ションドライバアラ??状態でのMoveオブジェクト実行										*/
															/*	Minor failure: Move object executed at occurrence of Motion Driver alarm							*/
#define	ALM_MK_SUPERPOSE_MASTER_ERR			0x67050333		/*	軽故障	払い出し合成オブジェクト?ス?軸条件エラ?													*/
															/*	Minor failure: Distribution synthetic object master axis condition error							*/
#define	ALM_MK_SUPERPOSE_SLAVE_ERR			0x67050334		/*	軽故障	払い出し合成オブジェクトスレ?ブ軸条件エラ?												*/
															/*	Minor failure: Distribution synthetic object slave axis condition error								*/
#define	ALM_MK_MDWARNING					0x57050335		/*	ﾜｰﾆﾝｸﾞ	モ?ションドライバワ?ニング																*/
															/*	Warning: Motion Driver warning																		*/
#define	ALM_MK_MDWARNING_POSERR				0x57050336		/*	ﾜｰﾆﾝｸﾞ	モ?ションドライバ偏差ワ?ニング															*/
															/*	Warning: Motion Driver deviation warning															*/
#define	ALM_MK_NOT_INFINITE_ABS				0x67050337		/*	軽故障	指定された軸はABS無限長軸として使用可?ではない												*/
															/*	Minor failure: Specified axis cannot be used as ABS infinite length axis.							*/
#define	ALM_MK_INVALID_LOGICAL_NUM			0x67050338		/*	軽故障	不正な?理軸番号が指定された																*/
															/*	Minor failure: Illegal logical axis number has been specified.										*/
#define	ALM_MK_MAX_VELOCITY_ERR				0x67050339		/*	軽故障	最高速度設定エラ?																			*/
															/*	Minor failure: Maximum speed setting error															*/
#define	ALM_MK_VELOCITY_ERR					0x6705033A		/*	軽故障	速度設定エラ?																				*/
															/*	Minor failure: Speed setting error																	*/
#define	ALM_MK_APPROACH_ERR					0x6705033B		/*	軽故障	アプロ??速度設定エラ?																	*/
															/*	Minor failure: Approach speed setting error															*/
#define	ALM_MK_CREEP_ERR					0x6705033C		/*	軽故障	クリ?プ速度設定エラ?																		*/
															/*	Minor failure: Creep speed setting error															*/
#define	ALM_MK_OS_ERR_MBOX1					0x83050340		/*	重故障	メイル?ックス生成エラ?（モ?ションカ?ネル Moveオブジェクト実行要求用メイル?ックス）		*/
															/*	Major failure: Mail box creation error (mail box for request for Motion Kernel Move object execution)*/
#define	ALM_MK_OS_ERR_MBOX2					0x83050341		/*	重故障	メイル?ックス生成エラ?（モ?ションカ?ネル アクション実行要求用メイル?ックス）			*/
															/*	Major failure: Mail box creation error (mail box for request for Motion Kernel action execution)	*/
#define	ALM_MK_OS_ERR_SEND_MSG1				0x83050342		/*	重故障	OSレベルでのメッセ?ジ送信異常（MK→EM：イベント検出通知）									*/
															/*	Major failure: Message sending error at OS level (MK to EM: Notification of event detection)		*/
#define	ALM_MK_OS_ERR_SEND_MSG2				0x83050343		/*	重故障	OSレベルでのメッセ?ジ送信異常（MK→MM：Move完了メッセ?ジ）								*/
															/*	Major failure: Message sending error at OS level (MK to MM: Move completion message)				*/
#define	ALM_MK_OS_ERR_SEND_MSG3				0x83050344		/*	重故障	OSレベルでのメッセ?ジ送信異常（EM→MM：Aciton通知）										*/
															/*	Major failure: Message sending error at OS level (EM to MM: Notification of Action)					*/
#define	ALM_MK_OS_ERR_SEND_MSG4				0x83050345		/*	重故障	OSレベルでのメッセ?ジ送信異常（その他）													*/
															/*	Major failure: Message sending error at OS level (Others)											*/
#define	ALM_MK_ACTION_ERR1					0x53050346		/*	ﾜｰﾆﾝｸﾞ	不正応答メッセ?ジ受信（アクション実行完了通知の応答待ち状態へのアクション実行要求）		*/
															/*	Warning: Illegal response message received (action execution request to waiting status for response of notification of action execution completion)	*/
#define	ALM_MK_ACTION_ERR2					0x53050347		/*	ﾜｰﾆﾝｸﾞ	不正応答メッセ?ジ受信（現在実行中のMoveオブジェクトへのアクションではない）				*/
															/*	Warning: Illegal response message received(not an action for Move object currently executed)		*/
#define	ALM_MK_ACTION_ERR3					0x53050348		/*	ﾜｰﾆﾝｸﾞ	不正応答メッセ?ジ受信（現在実行中のMoveLISTオブジェクトへのアクションではない）			*/
															/*	Warning: Illegal response message received (not an action for MoveLIST object currently executed)	*/
#define	ALM_MK_RCV_INV_MSG1					0x53050349		/*	ﾜｰﾆﾝｸﾞ	不正指令メッセ?ジ受信（MOVEオブジェクトハンドルではない）									*/
															/*	Warning: Illegal command message received (not a MOVE object handle)								*/
#define	ALM_MK_RCV_INV_MSG2					0x5305034A		/*	ﾜｰﾆﾝｸﾞ	不正指令メッセ?ジ受信（モ?ション?ネ?ジャ以外からの指令）								*/
															/*	Warning: Illegal command message received (command from other than Motion Manager)					*/
#define	ALM_MK_RCV_INV_MSG3					0x5305034B		/*	ﾜｰﾆﾝｸﾞ	不正指令メッセ?ジ受信（指令メッセ?ジではない）											*/
															/*	Warning: Illegal command message received (not a command message)									*/
#define	ALM_MK_RCV_INV_MSG4					0x5305034C		/*	ﾜｰﾆﾝｸﾞ	不正指令メッセ?ジ受信（指令?応答以外のメッセ?ジ）										*/
															/*	Warning: Illegal command message received (message other than command/response)						*/
#define	ALM_MK_RCV_INV_MSG5					0x5305034D		/*	ﾜｰﾆﾝｸﾞ	不正指令メッセ?ジ受信（イベント?ネ?ジャ以外からの指令メッセ?ジ）						*/
															/*	Warning: Illegal command message received (command message from other than Event Manager)			*/
#define	ALM_MK_RCV_INV_MSG6					0x5305034E		/*	ﾜｰﾆﾝｸﾞ	不正指令メッセ?ジ受信（アクション実行要求ではない指令メッセ?ジ）							*/
															/*	Warning: Illegal command message received (command message other than request for action execution)	*/
#define	ALM_MK_RCV_INV_MSG7					0x5305034F		/*	ﾜｰﾆﾝｸﾞ	不正応答メッセ?ジ受信（イベント?ネ?ジャ以外からの応答メッセ?ジ）						*/
															/*	Warning: Illegal response message received (response message from other than Event Manager)			*/
#define	ALM_MK_RCV_INV_MSG8					0x53050350		/*	ﾜｰﾆﾝｸﾞ	不正応答メッセ?ジ受信（イベント通知完了応答/アクション完了通知応答以外の応答メッセ?ジ）	*/
															/*	Warning: Illegal response message received (response message other than event notification completion/action completion notification)	*/
#define	ALM_MK_AXIS_HANDLE_ERROR			0x67050360		/*	軽故障	軸ハンドル番号が異常																		*/
															/*	Minor failure: Axis handle number is incorrect.														*/
#define	ALM_MK_SLAVE_USED_AS_MASTER			0x67050361		/*	軽故障	スレ?ブ軸を?ス?軸として使用しようとした													*/
															/*	Minor failure: An attempt was made to use a slave axis as a master axis.							*/
#define	ALM_MK_MASTER_USED_AS_SLAVE			0x67050362		/*	軽故障	?ス?軸をスレ?ブ軸として使用しようとした													*/
															/*	Minor failure: An attempt was made to use a master axis as a slave axis.							*/
#define	ALM_MK_SLAVE_HAS_2_MASTERS			0x67050363		/*	軽故障	同じスレ?ブ軸に２つ以上の?ス?軸を指定した												*/
															/*	Minor failure: More than two master axes were specified for the same slave axis.					*/
#define	ALM_MK_SLAVE_HAS_NOT_WORK			0x67050364		/*	軽故障	スレ?ブ軸のワ?クメモリが確保できない														*/
															/*	Minor failure: Work axis cannot be assured for a slave axis.										*/
#define	ALM_MK_PARAM_OUT_OF_RANGE			0x67050365		/*	軽故障	パラメ??は範囲外																			*/
															/*	Minor failure: Parameter is out of range.															*/
#define	ALM_MK_NNUM_MAX_OVER				0x67050366		/*	軽故障	平均化回数最大値オ?バ?																	*/
															/*	Minor failure: Maximum number of averaging times exceeded											*/
#define	ALM_MK_FGNTBL_INVALID				0x67050367		/*	軽故障	FGN テ?ブルの内容が不正																	*/
															/*	Minor failure: Contents of the FGN table are illegal.												*/
#define	ALM_MK_PARAM_OVERFLOW				0x67050368		/*	軽故障	設定値はオ?バ?フロ?																		*/
															/*	Minor failure: Set value overflows.																	*/
#define	ALM_MK_ALREADY_COMMANDED			0x67050369		/*	軽故障	CAM または GEAR はすでに実行中																*/
															/*	Minor failure: CAM or GEAR has already been under execution.										*/
#define	ALM_MK_MULTIPLE_SHIFTS				0x6705036A		/*	軽故障	Position Offset/Cam Shift 実行中に Position Offset/Cam Shift を実行した						*/
															/*	Minor failure: Position Offset/Cam Shift was executed during execution of Position Offset/Cam Shift.*/
#define	ALM_MK_CAMTBL_INVALID				0x6705036B		/*	軽故障	CAM テ?ブルが不正（アドレス、内容など）													*/
															/*	Minor failure: CAM table is illegal (address, contents, etc.).										*/
#define	ALM_MK_ABORTED_BY_STOPMTN			0x6705036C		/*	軽故障	STOP MOTION 等によって CAM/GEAR 動作がア??トされた										*/
															/*	Minor failure: CAM/GEAR is aborted by STOP MOTION, etc.												*/
#define	ALM_MK_HMETHOD_INVALID				0x6705036D		/*	軽故障	未サ??トの原?復帰方式																	*/
															/*	Minor failure: Non-supported zero point return method												*/
#define	ALM_MK_MASTER_INVALID				0x6705036E		/*	軽故障	?ス?軸がモニ?用として指定されていない													*/
															/*	Minor failure: Master axis is not specified for monitoring.											*/
#define	ALM_MK_DATA_HANDLE_INVALID			0x6705036F		/*	軽故障	レジス??グロ?バルデ??ハンドルが不正													*/
															/*	Minor failure: Register/global data handle is illegal.												*/
#define	ALM_MK_UNKNOWN_CAM_GEAR_ERR			0x67050370		/*	軽故障	CAM/GEAR 関連の不明なエラ?																	*/
															/*	Minor failure: Unclear error related to CAM/GEAR													*/
#define	ALM_MK_REG_SIZE_INVALID				0x67050371		/*	軽故障	レジス?ハンドルのサイズが不正																*/
															/*	Minor failure: Register handle size is illegal.														*/
#define	ALM_MK_ACTION_HANDLE_ERROR			0x67050372		/*	軽故障	アクションハンドルが不正																	*/
															/*	Minor failure: Action handle is illegal.															*/
#define	ALM_MM_OS_ERR_MBOX1					0x83040380		/*	重故障	メイル?ックス生成エラ?（モ?ション?ネ?ジャ 起動用メイル?ックス）						*/
															/*	Major failure: Mail box creation error (mail box to start up Motion Manager)						*/
#define	ALM_MM_OS_ERR_SEND_MSG1				0x83040381		/*	重故障	OSレベルでのメッセ?ジ送信異常（モ?ション?ネ?ジャ→モ?ションカ?ネル）					*/
															/*	Major failure: Message sending error at OS level (Motion Manager to Motion Kernel)					*/
#define	ALM_MM_OS_ERR_SEND_MSG2				0x83040382		/*	重故障	OSレベルでのメッセ?ジ送信異常（モ?ション?ネ?ジャ→イベント?ネ?ジャ）					*/
															/*	Major failure: Message sending error at OS level (Motion Manager to Event Manager)					*/
#define	ALM_MM_OS_ERR_RCV_MSG1				0x83040383		/*	重故障	OSレベルでのメッセ?ジ受信異常																*/
															/*	Major failure: Message receiving error at OS level													*/
#define	ALM_MM_MK_BUSY						0x67040384		/*	軽故障	モ?ションカ?ネルが全て使用中																*/
															/*	Minor failure: All Motion Kernels are in use.														*/
#define	ALM_MM_RCV_INV_MSG1					0x53040385		/*	ﾜｰﾆﾝｸﾞ	不正指令メッセ?ジ受信（ハンドル不正１：hMOVEではない）										*/
															/*	Warning: Illegal command message received (illegal handle 1: Not hMOVE.)							*/
#define	ALM_MM_RCV_INV_MSG2					0x53040386		/*	ﾜｰﾆﾝｸﾞ	不正指令メッセ?ジ受信（ハンドル不正２：hMOVEが存在しない）									*/
															/*	Warning: Illegal command message received (illegal handle 2: hMOVE does not exist.)					*/
#define	ALM_MM_RCV_INV_MSG3					0x53040387		/*	ﾜｰﾆﾝｸﾞ	不正指令メッセ?ジ受信（ス??トアクション実行要求ではない）								*/
															/*	Warning: Illegal command message received (Not request for start action execution)					*/
#define	ALM_MM_RCV_INV_MSG4					0x53040388		/*	ﾜｰﾆﾝｸﾞ	不正応答メッセ?ジ受信（イベント通知完了応答以外の応答メッセ?ジ）							*/
															/*	Warning: Illegal response message received (response message other than event notification completion)	*/
#define	ALM_MM_RCV_INV_MSG5					0x53040389		/*	ﾜｰﾆﾝｸﾞ	不正応答メッセ?ジ受信（アクション完了応答メッセ?ジでその他のメッセ?ジを受信）			*/
															/*	Warning: Illegal response message received (Other messages received with action completion response message)	*/
#define	ALM_IM_DEVICEID_ERR					0x53060480		/*	DeviceID異常またはそのDeviceが未サ??ト			*/
															/*	DeviceID error or non-supported Device				*/
#define	ALM_IM_REGHANDLE_ERR				0x53060481		/*	レジス?ハンドル異常								*/
															/*	Register handle error								*/
#define	ALM_IM_GLOBALHANDLE_ERR				0x53060482		/*	グロ?バルデ??ハンドル異常						*/
															/*	Global data handle error							*/
#define	ALM_IM_DEVICETYPE_ERR				0x53060483		/*	デ???イプが未サ??ト							*/
															/*	Non-supported data type								*/
#define	ALM_IM_OFFSET_ERR					0x53060484		/*	オフセット値異常									*/
															/*	Incorrect offset value								*/
#define	AM_ER_UNDEF_COMMAND					0x57020501		/*	コ?ンドコ?ド不正									*/
															/*	Illegal command code								*/
#define	AM_ER_UNDEF_CMNDTYPE				0x57020502		/*	コ?ンド種別不正									*/
															/*	Illegal command type								*/
#define	AM_ER_UNDEF_OBJTYPE					0x57020503		/*	オブジェクト?イプ不正								*/
															/*	Illegal object type									*/
#define	AM_ER_UNDEF_HANDLETYPE				0x57020504		/*	ハンドル種別不正									*/
															/*	Illegal handle type									*/
#define	AM_ER_UNDEF_PKTDAT					0x57020505		/*	パケットデ??不正									*/
															/*	Illegal packet data									*/
#define	AM_ER_UNDEF_AXIS					0x57020506		/*	軸未定?											*/
															/*	axis not defined									*/
#define	AM_ER_MSGBUF_GET_FAULT				0x57020510		/*	メッセ?ジバッフ?管理テ?ブル取得失敗				*/
															/*	Acquisition failure of  message buffer managed table*/
#define	AM_ER_ACTSIZE_GET_FAULT				0x57020511		/*	ACTサイズ取得失敗									*/
															/*	Acquisition failure of ACT size						*/
#define	AM_ER_APIBUF_GET_FAULT				0x57020512		/*	APIバッフ?管理テ?ブル取得失敗						*/
															/*	Acquisition failure of API buffer managed table		*/
#define	AM_ER_MOVEOBJ_GET_FAULT				0x57020513		/*	MOVEオブジェクト管理テ?ブル取得失敗				*/
															/*	Acquisition failure of MOVE object managed table	*/
#define	AM_ER_EVTTBL_GET_FAULT				0x57020514		/*	イベント管理テ?ブル取得失敗						*/
															/*	Acquisition failure of event managed table			*/
#define	AM_ER_ACTTBL_GET_FAULT				0x57020515		/*	アクション管理テ?ブル取得失敗						*/
															/*	Acquisition failure of Action managed table			*/
#define	AM_ER_1BY1APIBUF_GET_FAULT			0x57020516		/*	?次実行?API管理テ?ブル取得失敗					*/
															/*	Acquisition failure of Sequence managed table		*/
#define	AM_ER_AXSTBL_GET_FAULT				0x57020517		/*	AXISハンドル管理テ?ブル獲得失敗					*/
															/*	Acquisition failure of AXIS handle managed table	*/
#define	AM_ER_SUPERPOSEOBJ_GET_FAULT		0x57020518		/*	払い出し合成オブジェクト管理テ?ブル獲得失敗		*/
															/*	Acquisition failure of Distribution synthetic object managed table	*/
#define	AM_ER_SUPERPOSEOBJ_CLEAR_FAULT		0x57020519		/*	払い出し合成オブジェクト削除失敗					*/
															/*	Deletion failure of Distribution synthetic object	*/
#define	AM_ER_AXIS_IN_USE					0x5702051A		/*	軸使用中											*/
															/*	axis in use											*/
#define	AM_ER_HASHTBL_GET_FAULT				0x5702051B		/*	軸名称管理用ハッシュテ?ブル取得失敗				*/
															/*	Hash table acquisition failure for axial name management	*/
#define	AM_ER_UNMATCH_OBJHNDL				0x57020530		/*	MOVEオブジェクトハンドル不一致						*/
															/*	MOVE object handle mismatched 						*/
#define	AM_ER_UNMATCH_OBJECT				0x57020531		/*	オブジェクト不一致									*/
															/*	Object mismatched									*/
#define	AM_ER_UNMATCH_APIBUF				0x57020532		/*	APIバッフ?不一致									*/
															/*	API buffer mismatched								*/
#define	AM_ER_UNMATCH_MSGBUF				0x57020533		/*	メッセ?ジバッフ?不一致							*/
															/*	Message buffer mismatched							*/
#define	AM_ER_UNMATCH_ACTBUF				0x57020534		/*	アクション実行管理バッフ?不一致					*/
															/*	Action execution management buffer mismatched		*/
#define	AM_ER_UNMATH_SEQUENCE				0x57020535		/*	シ?ケンス番号不一致								*/
															/*	Sequence number mismatched							*/
#define	AM_ER_UNMATCH_1BY1APIBUF			0x57020536		/*	?次実行?API管理テ?ブル不一致						*/
															/*	Sequential API management table mismatched			*/
#define	AM_ER_UNMATCH_MOVEOBJTABLE			0x57020537		/*	MOVEオブジェクト管理テ?ブル不一致					*/
															/*	MOVE object management table mismatched				*/
#define	AM_ER_UNMATCH_MOVELISTTABLE			0x57020538		/*	MOVE LIST管理テ?ブル不一致							*/
															/*	MOVE LIST management table mismatched				*/
#define	AM_ER_UNMATCH_MOVELIST_OBJECT		0x57020539		/*	MOVE LISTオブジェクト不一致							*/
															/*	MOVE LIST object mismatched							*/
#define	AM_ER_UNMATCH_MOVELIST_OBJHNDL		0x5702053A		/*	MOVE LISTオブジェクトハンドル不一致					*/
															/*	MOVE LIST object handle mismatched					*/
#define	AM_ER_UNGET_MOVEOBJTABLE			0x57020550		/*	MOVEオブジェクト管理テ?ブル未確保					*/
															/*	MOVE object management table not assured			*/
#define	AM_ER_UNGET_MOVELISTTABLE			0x57020551		/*	MOVE LISTオブジェクト管理テ?ブル未確保				*/
															/*	MOVE LIST object management table not assured		*/
#define	AM_ER_UNGET_1BY1APIBUFTABLE			0x57020552		/*	?次実行?API管理テ?ブル未確保						*/
															/*	Sequential API management table not assured			*/
#define	AM_ER_NOEMPTYTBL_ERROR				0x57020560		/*	補間テ?ブルに空きテ?ブルなし						*/
															/*	No unused table among interpolation tables			*/
#define	AM_ER_NOTGETSEM_ERROR				0x57020561		/*	AM-MK セ?フォ取得失敗 (Dynamic)					*/
															/*	Failure to get AM-MK semaphore  (Dynamic)			*/
#define	AM_ER_NOTGETTBLADD_ERROR			0x57020562		/*	補間テ?ブルアドレス取得失敗						*/
															/*	Failure to get interpolation table address			*/
#define	AM_ER_NOTWRTTBL_ERROR				0x57020563		/*	実行時テ?ブル書き込み失敗 (Static)					*/
															/*	Failure to write in table at execution (Static)		*/
#define	AM_ER_TBLINDEX_ERROR				0x57020564		/*	Indexの設定異常 (Static)							*/
															/*	Index setting error (Static)						*/
#define	AM_ER_ILLTBLTYPE_ERROR				0x57020565		/*	無効なテ?ブル?イプの指定							*/
															/*	Invalid table type specified						*/
#define	AM_ER_UNSUPORTED_EVENT				0x57020570		/*	イベント非サ??ト or 引数エラ?					*/
															/*	Event not supported or argument error				*/
#define	AM_ER_WRONG_SEQUENCE				0x57020571		/*	シ?ケンス異常										*/
															/*	Sequence error										*/
#define	AM_ER_MOVEOBJ_BUSY					0x57020572		/*	MOVEオブジェクト実行中								*/
															/*	MOVE object under execution							*/
#define	AM_ER_MOVELIST_BUSY					0x57020573		/*	MOVE LIST実行中										*/
															/*	MOVE LIST under execution							*/
#define	AM_ER_MOVELIST_ADD_FAULT			0x57020574		/*	MOVE OBJ登?不?									*/
															/*	MOVE OBJ cannot be registered.						*/
#define	AM_ER_CONFLICT_PHI_AXS				0x57020575		/*	物理軸重複											*/
															/*	Physical axes overlapped							*/
#define	AM_ER_CONFLICT_LOG_AXS				0x57020576		/*	?理軸重複											*/
															/*	Logic axes overlapped								*/
#define	AM_ER_PKTSTS_ERROR					0x57020577		/*	受信パケットのステ??ス異常						*/
															/*	Receiving packet status error						*/
#define	AM_ER_CONFLICT_NAME					0x57020578		/*	軸名称重複											*/
															/*	Axis name overlapped								*/
#define	AM_ER_ILLEGAL_NAME					0x57020579		/*	軸名称不正											*/
															/*	Incorrect axis name									*/
#define	AM_ER_SEMAPHORE_ERROR				0x5702057A		/*	ホストPC割り込み時 セ?フォ異常						*/
															/*	Incorrect semaphore at host PC interruption			*/
#define	AM_ER_LOG_AXS_OVER					0x5702057B		/*	?理軸番号範囲オ?バ								*/
															/*	Logical axis number exceeded						*/
#define	AM_ER_EX_ENCD_AXS_OVER				0x5702057C		/*	外部エンコ?? 軸番号範囲オ?バ						*/
															/*	Exteranal encoder axis number exceeded				*/
#define	IM_STATION_ERR						0x55060B00		/*	ﾜｰﾆﾝｸﾞ	リンク?送ステ?ションエラ?				*/
															/*	Warning: Link communication station error			*/
#define	IM_IO_ERR							0x55060B01		/*	ﾜｰﾆﾝｸﾞ	ＩＯエラ?									*/
															/*	Warning: I/O error									*/
#define CC_ER_CHANNELOVER					0x47070680		/*	範囲外の?ャネル番号								*/
															/*	Out-of-range channel number.						*/
#define CC_ER_OPEN_CHANNEL_ERR				0x47070681		/*	既にオ?プンされている?ャネル 						*/
															/*	The same channel has already been opened.			*/
#define CC_ER_NOTCHANNELHANDLE				0x47070682		/*	無効な?ャネルハンドルが指定された					*/
															/*	Undefined channnnel handle							*/
#define CC_ER_BUSY_CHANNEL_ERR				0x47070683		/*	使用中の?ャネルが指定された						*/
															/*	Channel is in use. 									*/
#define CC_ER_ADDR_ERR						0x47070684		/*	ステ?ションアドレスエラ?							*/
															/*	Station address error								*/
#define CC_ER_DATASIZEOVER					0x47070685		/*	デ??サイズエラ?									*/
															/*	Data size error										*/
#define CC_ER_REG_ADR_ERR					0x47070686		/*	アドレス、オフセットエラ?							*/
															/*	Address setting error								*/
#define CC_ER_MSG_FC_ERR					0x47070687		/*	フ?ンクションコ?ドエラ?							*/
															/*	Function code error									*/
#define CC_ER_DEVICE_SELECT_ERR				0x47070689		/*	デバイス選択エラ?									*/
															/*	Device Selection error								*/
#define CC_ER_COM_TIMEOUT					0x4707068A		/*	?イ?アウト										*/
															/*	Time out											*/
#define CC_ER_CHNLHNDL_OVER					0x4707068B		/*	?ャネルハンドル登?数オ?バ?						*/
															/*	Number of channel handles exceeds the maximum value.*/
#define CC_ER_BUFSIZE_OVER					0x4707068C		/*	受信バッフ?サイズオ?バ?							*/
															/*	Receive buffer size error							*/
#define CC_ER_INTERNAL_COMM_FMT_NG			0x4707068D		/*	?送部エラ?(パラメ??フォ??ットエラ?)			*/
															/*	Communication device error(parameter format error)	*/
#define CC_ER_INTERNAL_COMM_SEQ_NG			0x4707068E		/*	?送部エラ?(コ?ンドシ?ケンスエラ?)				*/
															/*	Communication device error(command sequence error)	*/
#define CC_ER_INTERNAL_COMM_RST_NG			0x4707068F		/*	?送部エラ?(リセット状態)							*/
															/*	Communication device error(reset status)			*/
#define CC_ER_INTERNAL_COMM_REC_NG			0x47070690		/*	?送部エラ?(デ??受信エラ?)						*/
															/*	Communication device error(data receive error)		*/
#define CC_ER_INTERNAL_COMM_ERR				0x47070691		/*	?送部エラ?(その他)								*/
															/*	Communication device error(other error)				*/
#define CC_ER_ILLEGAL_TASK					0x47070692		/*	不正な?スク(ymcOpenChannel実行?スク以外)			*/
															/*	Illegal Task(other than ymcOpenChannel execute task).*/
#define	MP_CNF_NOCOMMUDEVICETYPE			0x47081600		/*	通信デバイス?イプの指定エラ?						*/
															/*														*/
#define	MP_CNF_NOTPROTOCOLTYPE				0x47081601		/*	無効なプロトコル?イプ								*/
															/*														*/
#define	MP_CNF_NOTFUNCMODE					0x47081602		/*	無効な??モ?ド									*/
															/*														*/
#define	MP_CNF_MYADDROVER					0x47081603		/*	範囲外の自局アドレスが設定された					*/
															/*														*/
#define	MP_CNF_NOTPORTTYPE					0x47081604		/*	無効な??ト?イプ									*/
															/*														*/
#define	MP_CNF_NOTPROTMODE					0x47081605		/*	無効なキャラク?ビット長							*/
															/*														*/
#define	MP_CNF_NOTCHARSIZE					0x47081606		/*	無効なプロトコルモ?ド無効なキャラク?ビット長		*/
															/*														*/
#define	MP_CNF_NOTPARITYBIT					0x47081607		/*	無効なパリティビット								*/
															/*														*/
#define	MP_CNF_NOTSTOPBIT					0x47081608		/*	無効なストップビット								*/
															/*														*/
#define	MP_CNF_NOTBAUDRAT					0x47081609		/*	無効な?送速度										*/
															/*														*/
#define	MP_CNF_TRANSDELAYOVER				0x47081610		/*	範囲外の送信遅延が指定された						*/
															/*														*/
#define	MP_CNF_PEERADDROVER					0x47081611		/*	範囲外の相手先アドレスが指定された					*/
															/*														*/
#define	MP_CNF_SUBNETMASKOVER				0x47081612		/*	範囲外のサブネット?スクが指定された				*/
															/*														*/
#define	MP_CNF_GWADDROVER					0x47081613		/*	範囲外のGWアドレスが指定された						*/
															/*														*/
#define	MP_CNF_DIAGPORTOVER					0x47081614		/*	範囲外の診断??トが指定された						*/
															/*														*/
#define	MP_CNF_PROCRETRYOVER				0x47081615		/*	範囲外の有手順リトライ回数が指定された				*/
															/*														*/
#define	MP_CNF_TCPZEROWINOVER				0x47081616		/*	範囲外のTCP?ロウィンドウ?イ?						*/
															/*														*/
#define	MP_CNF_TCPRETRYOVER					0x47081617		/*	範囲外のTCP再送?イ?値								*/
															/*														*/
#define	MP_CNF_TCPFINOVER					0x47081618		/*	範囲外の終了?イ?値								*/
															/*														*/
#define	MP_CNF_IPASSEMBLEOVER				0x47081619		/*	範囲外のIP組み込み?イ?値							*/
															/*														*/
#define	MP_CNF_MAXPKTLENOVER				0x47081620		/*	範囲外の最大パケット長								*/
															/*														*/
#define	MP_CNF_PEERPORTOVER					0x47081621		/*	範囲外の相手先??ト								*/
															/*														*/
#define	MP_CNF_MYPORTOVER					0x47081622		/*	範囲外の自局??ト									*/
															/*														*/
#define	MP_CNF_NOTTRANSPROT					0x47081623		/*	無効なトランス??ト層プロトコル					*/
															/*														*/
#define	MP_CNF_NOTAPPROT					0x47081624		/*	無効なアプリケ?ション層プロトコル					*/
															/*														*/
#define	MP_CNF_NOTCODETYPE					0x47081625		/*	無効なコ?ド?イプ									*/
															/*														*/
#define	MP_CNF_CYCTIMOVER					0x47081626		/*	範囲外の通信周期が指定された						*/
															/*														*/
#define	MP_CNF_NOTIOMAPCOM					0x47081627		/*	無効な入出力コ?ンド								*/
															/*														*/
#define	MP_CNF_NOTIOTYPE					0x47081628		/*	無効なI/O?イプ指定									*/
															/*														*/
#define	MP_CNF_IOOFFSETOVER					0x47081629		/*	範囲外のI/Oオフセットが割り付けられた				*/
															/*														*/
#define	MP_CNF_IOSIZEOVER					0x47081630		/*	範囲外のI/Oサイズが割り付けられた(個別)				*/
															/*														*/
#define	MP_CNF_TIOSIZEOVER					0x47081631		/*	範囲外のI/Oサイズが割り付けられた(合計)				*/
															/*														*/
#define	MP_CNF_MEMORY_ERR					0x47081632		/*	コントロ?ラメモリ不足								*/
															/*														*/
#define	MP_CNF_NOTPTR						0x47081633		/*	無効な?イン? (通信デバイス指定?造体/デバイス固有情報/対象通信デバイスハンドルへの?イン?異常)	*/
															/*														*/
#define	MP_CNF_NOTAUTORECOVER				0x47081634		/*	無効な自動受信選択									*/
															/*														*/
#define	MP_CNF_NOTINPUTRELADD				0x47081635		/*	範囲外の入力リレ?レジス?アドレス					*/
															/*														*/
#define	MP_CNF_NOTINPUTRELSIZ				0x47081636		/*	範囲外の入力リレ?レジス?ワ?ド数					*/
															/*														*/
#define	MP_CNF_NOTINPUTREGADD				0x47081637		/*	範囲外の入力レジス?アドレス						*/
															/*														*/
#define	MP_CNF_NOTINPUTREGSIZ				0x47081638		/*	範囲外の入力レジス?ワ?ド数						*/
															/*														*/
#define	MP_CNF_NOTCOILADD					0x47081639		/*	範囲外のコイルレジス?アドレス						*/
															/*														*/
#define	MP_CNF_NOTCOILSIZ					0x47081640		/*	範囲外のコイルレジス?ワ?ド数						*/
															/*														*/
#define	MP_CNF_NOTHOLDINGREGADD				0x47081641		/*	範囲外の保持レジス?アドレス						*/
															/*														*/
#define	MP_CNF_NOTHOLDINGREGSIZ				0x47081642		/*	範囲外の保持レジス?ワ?ド数						*/
															/*														*/
#define	MP_CNF_NOTWRITERANGELO				0x47081643		/*	範囲外のコイル?保持レジス?書込範囲ＬＯ			*/
															/*														*/
#define	MP_CNF_NOTWRITERANGEHI				0x47081644		/*	範囲外のコイル?保持レジス?書込範囲ＨＩ			*/
															/*														*/
#define	MP_CNF_MYIPADDROVER					0x47081645		/*	範囲外の自局IPアドレス								*/
															/*														*/
#define	MP_CNF_SENDTIMOUTOVER				0x47081646		/*	範囲外のメモバス再送?イ?							*/
															/*														*/
#define	MP_CNF_SENDRETRYOVER				0x47081647		/*	範囲外のメモバス再送回数							*/
															/*														*/
#define	MP_CNF_NOTNAMESIZEOVER				0x47081648		/*	範囲外のコメントサイズ								*/
															/*														*/
#define	MP_CNF_DUPMYPORT					0x47081649		/*	自局??トの重複									*/
															/*														*/
#define	MP_CNF_DUPDSTIP						0x47081650		/*	相手局IP,PORT,トランス層プロトコルが他と重複		*/
															/*														*/
#define	MP_CNF_NOTETHERADDR					0x47081651		/*	無効なイ?サネットアドレス							*/
															/*														*/
#define	MP_CNF_DUPMYDSTPORT					0x47081652		/*	自局と相手局のPORTの重複							*/
															/*														*/
#define	MP_CNF_NOTDSTCONNPAR				0x47081653		/*	トランス??ト層プロトコル、アプリケ?ション層プロトコル、コ?ド?イプが他と異なる	*/
															/*														*/
#define	MP_CNF_DUPDSTCONN					0x47081654		/*	コネクションの重複									*/
															/*														*/
#define	MP_CNF_NOTDSTCONN					0x47081655		/*	相手先コネクションがない							*/
															/*														*/
#define	MP_CNF_UNDEFINEER					0x47081656		/*	モジュ?ルの定?がない、モジュ?ル不一致			*/
															/*														*/
#define	MP_CNF_NOTSSLOTDEVTYPE				0x47081657		/*	未定?のデバイス?イプ								*/
															/*														*/
#define	MP_CNF_PANELCMDERR					0x47081658		/*	PPIFのパネルコ?ンドに対する異常応答				*/
															/*														*/
#define MP_CNF_AUTOMEMDELAYOVER				0x47081659		/*	範囲外の自動受信処理ディレイ時間					*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_AUTOMEMBUFCHNOOVER			0x47081660		/*	無効な自動受信?送バッフ??ャネル番号				*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_AUTOMEMSLVIFSELOVER			0x47081661		/*	無効な自動受信スレ?ブ側I/Fレジス?設定				*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_AUTOMEMSVTBOVER				0x47081662		/*	無効な自動受信S/V T/B 設定							*/	//<API5025>
															/*														*/	//<API5025>
#define MP_CNF_MYNODEADDROVER				0x47081663		/*	範囲外の自局ノ?ドアドレス番号						*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGSELOVER					0x47081664		/*	無効なIOメッセ?ジ通信選択（有効/無効）				*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGPRMTYPEOVER				0x47081665		/*	無効なIOメッセ?ジパラメ??種別（読出/書込)		*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGMYOFFSETOVER			0x47081666		/*	範囲外のIOメッセ?ジ先頭レジス?番号				*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGDATASIZEOVER			0x47081667		/*	範囲外のIOメッセ?ジデ??サイズ					*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGUPDATESELOVER			0x47081668		/*	無効なIOメッセ?ジデ??更新?イ?ング選択			*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGPEERREGTYPEOVER			0x47081669		/*	無効なIOメッセ?ジ相手?器レジス?種別				*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGPEEROFFSETOVER			0x47081670		/*	範囲外のIOメッセ?ジ相手?器レジス?先頭番号		*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGPEERNODEADDROVER		0x47081671		/*	範囲外のIOメッセ?ジ相手?器ノ?ドアドレス番号		*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGPEERUNITNOOVER			0x47081672		/*	範囲外のIOメッセ?ジ相手?器号?番号				*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGPEERUNITIONOOVER		0x47081673		/*	無効なIOメッセ?ジ相手?器要求先ユニットI/O番号		*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGDISABLESELOVER			0x47081674		/*	無効なIOメッセ?ジディスエ?ブル設定				*/	//<API5025>
															/*														*/	//<API5025>
#define MP_FILE_ERR_GENERAL 				0x53168001		/*	一般的なエラ?										*/
															/*	General error. 										*/
#define MP_FILE_ERR_NOT_SUPPORTED 			0x53168002		/*	指定した??がサ??トされていない					*/
															/*	Feature not supported. 								*/
#define MP_FILE_ERR_INVALID_ARGUMENT		0x53168003		/*	引数が不正											*/
															/*	Invalid argument 									*/
#define MP_FILE_ERR_INVALID_HANDLE			0x53168004		/*	ハンドルが不正										*/
															/*	Invalid handle 										*/
#define MP_FILE_ERR_NO_FILE 				0x53168064		/*	指定したフ?イルまたはディレクトリが存在しない		*/
															/*	No such file (or directory). 						*/
#define MP_FILE_ERR_INVALID_PATH 			0x53168065		/*	パスが異常											*/
															/*	Invalid path. 										*/
#define MP_FILE_ERR_EOF 					0x53168066		/*	フ?イルの最後に達した(EOF)							*/
															/*	End of file detected. 								*/
#define MP_FILE_ERR_PERMISSION_DENIED 		0x53168067		/*	フ?イル保護違反									*/
															/*	Not arrowed to access the file.						*/
#define MP_FILE_ERR_TOO_MANY_FILES 			0x53168068		/*	同時に開けるフ?イル数の限界に達した				*/
															/*	Too many files opened. 								*/
#define MP_FILE_ERR_FILE_BUSY 				0x53168069		/*	フ?イルは使用中									*/
															/*	File is in use. 									*/
#define MP_FILE_ERR_TIMEOUT 				0x5316806A		/*	?イ?アウトが生じた								*/
															/*	Timeout occured. 									*/
#define MP_FILE_ERR_BAD_FS 					0x531680C8		/*	?理フ?イルシステ?が異常							*/
															/*	Invalid or unexepected logical filesystem in the medium. */
#define MP_FILE_ERR_FILESYSTEM_FULL 		0x531680C9		/*	?理フ?イルシステ?がフル							*/
															/*	LFS (ie the media) is full.							*/
#define MP_FILE_ERR_INVALID_LFM 			0x531680CA		/*	指定した?理フ?イルシステ?モジュ?ルが不正		*/
															/*	Invalid LFM specified. 								*/
#define MP_FILE_ERR_TOO_MANY_LFM 			0x531680CB		/*	登?可?な?理フ?イルシステ?モジュ?ル数の限界	*/
															/*	LFM table is full. 									*/
#define MP_FILE_ERR_INVALID_PDM 			0x5316812C		/*	指定した物理記憶デバイスドラバモジュ?ルが不正		*/
															/*	Invalid PDM specified. 								*/
#define MP_FILE_ERR_INVALID_MEDIA 			0x5316812D		/*	指定したメディアが不正								*/
															/*	Invalid media specified. 							*/
#define MP_FILE_ERR_TOO_MANY_PDM 			0x5316812E		/*	登?可?な物理記憶デバイスドラバモジュ?ル数の限界	*/
															/*	Too many PDM. 										*/
#define MP_FILE_ERR_TOO_MANY_MEDIA 			0x5316812F		/*	登?可?なメディア数の限界に達した					*/
															/*	Too many media. 									*/
#define MP_FILE_ERR_WRITE_PROTECTED 		0x53168130		/*	メディアが書き込み禁?である						*/
															/*	Write protected media. 								*/
#define MP_FILE_ERR_INVALID_DEVICE 			0x53168190		/*	指定したデバイスが不正								*/
															/*	Invalid device specified. 							*/
#define MP_FILE_ERR_DEVICE_IO 				0x53168191		/*	デバイスI/Oエラ?									*/
															/*	Error occured in accessing the device. 				*/
#define MP_FILE_ERR_DEVICE_BUSY 			0x53168192		/*	デバイスが使用中									*/
															/*	Device is in use. 									*/
#define MP_FILE_ERR_NO_CARD 				0x5316A711		/*	CFカ?ド未装着										*/
															/*	CF CARD not mounted.								*/
#define MP_FILE_ERR_CARD_POWER 				0x5316A712		/*	CFカ?ドの電源がOFF状態								*/
															/*	CF CARD Power-OFF.									*/
#define	MP_CARD_SYSTEM_ERR					0x53178FFF		/*	カ?ドシステ?エラ?								*/
															/*	Card System Error.									*/
#define	ERROR_CODE_GET_DIREC_OFFSET			0x83001a01		/*	ディレクトリ領域のオフセットが獲得できない			*/
															/*	Directory area offset cannot be got.				*/
#define	ERROR_CODE_GET_DIREC_INFO			0x83001a02		/*	ディレクトリ情報の取得エラ?						*/
															/*	Directory area offset cannot be got.				*/
#define	ERROR_CODE_FUNC_TABLE				0x83001a03		/*	システ?コ?ル関数テ?ブルの取得エラ?				*/
															/*	Failure to get directory information				*/
#define	ERROR_CODE_SLEEP_TASK				0x83001a04		/*	スリ?プエラ?										*/
															/*	Failure to get system call function table			*/
#define	ERROR_CODE_DEVICE_HANDLE_FULL		0x43001a41		/*	デバイスハンドルが最大数を超えた					*/
															/*	Sleep error											*/
#define	ERROR_CODE_ALLOC_MEMORY				0x43001a42		/*	領域確保失敗										*/
															/*	Number of device handles exceeds the maximum value.	*/
#define	ERROR_CODE_BUFCOPY					0x43001a43		/*	MemoryCopy(),name_copy()のエラ?					*/
															/*	Failure to get the area.							*/
#define	ERROR_CODE_GET_COMMEM_OFFSET		0x43001a44		/*	共有メモリオフセット値の取得エラ?					*/
															/*	MemoryCopy(),name_copy() error						*/
#define	ERROR_CODE_CREATE_SEMPH				0x43001a45		/*	セ?フォ作成エラ?									*/
															/*	Failure to get common memory offset value			*/
#define	ERROR_CODE_DELETE_SEMPH				0x43001a46		/*	セ?フォ削除エラ?									*/
															/*	Semaphore creation error							*/
#define	ERROR_CODE_LOCK_SEMPH				0x43001a47		/*	セ?フォロック時のエラ?							*/
															/*	Semaphore deletion error							*/
#define	ERROR_CODE_UNLOCK_SEMPH				0x43001a48		/*	セ?フォロック解除時のエラ?						*/
															/*	Error at semaphore lock								*/
#define	ERROR_CODE_PACKETSIZE_OVER			0x43001a49		/*	パケットサイズのオ?バ?							*/
															/*	Error at semaphore release							*/
#define	ERROR_CODE_UNREADY					0x43001a4a		/*	コントロ?ラ初期化中エラ?							*/
															/*	Error when controller is being initialized			*/
#define	ERROR_CODE_CPUSTOP					0x43001a4b		/*	ＣＰＵストップ中エラ?								*/
															/*	Error when CPU is stopping							*/
#define	ERROR_LOG_NAME						0x43001a4c		/*	ログフ?イル(デ??)がない							*/
															/*	Log File or Data cannot be found					*/
#define	ERROR_LOG_WRITE_OPEN_ERROR			0x43001a4d		/*	PC側フ?イルOPENエラ?								*/
															/*	File in PC open error 								*/
#define	ERROR_LOG_SEQ						0x43001a4e		/*	ログシ?ケンスエラ?								*/
															/*	Log sequence error									*/
#define	ERROR_LOG_WRITE_FILE				0x43001a4f		/*	PC側フ?イル書込みエラ?							*/
															/*  File in PC write error								*/
#define	ERROR_LOG_TIME_OUT					0x43001a50		/*	ログ?イ?アウト									*/
															/*	Log Timeout											*/
#define	ERROR_LOG_OTHER						0x43001a51		/*	ログその他エラ?									*/
															/*	Log other error										*/
#define	ERROR_LOG_MPFILE_OPEN_ERROR			0x43001a52		/*	MP側フ?イルOPENエラ?								*/
															/*	File in MP open error								*/
#define	ERROR_CODE_CNTRNO					0x470B1a81		/*	CPU番号不正											*/
															/*	CPU number is illegal								*/
#define	ERROR_CODE_SELECTION				0x470B1a82		/*	選択値(0or1)が不正									*/
															/*	Device number										*/
#define	ERROR_CODE_LENGTH					0x470B1a83		/*	デ??長											*/
															/*	Illegal selected value (0 or 1)						*/
#define	ERROR_CODE_OFFSET					0x470B1a84		/*	オフセット値										*/
															/*	Data length											*/
#define	ERROR_CODE_DATACOUNT				0x470B1a85		/*	デ??数											*/
															/*	Offset value										*/
#define	ERROR_CODE_DATREAD					0x46001a86		/*	共有メモリからの読み出し失敗						*/
															/*	Number of data items								*/
#define	ERROR_CODE_DATWRITE					0x46001a87		/*	共有メモリへの書き込み失敗							*/
															/*	Failure to read out from common memory				*/
#define	ERROR_CODE_BITWRITE					0x46001a88		/*	共有メモリへのビットデ??書き込み失敗				*/
															/*	Failure to write in to common memory				*/
#define	ERROR_CODE_DEVCNTR					0x46001a89		/*	DeviceIoControl()がエラ?終了						*/
															/*	Failure to write in bit data to common memory		*/
#define	ERROR_CODE_NOTINIT					0x460F1a8a		/*	ドライバ初期化エラ?								*/
															/*	DeviceIoControl() completed erroneously.			*/
#define	ERROR_CODE_SEMPHLOCK				0x41001a8b		/*	パケット送信セ?フォロック							*/
															/*	Driver initialization error							*/
#define	ERROR_CODE_SEMPHUNLOCK				0x41001a8c		/*	パケット受信セ?フォアンロック						*/
															/*	Packet sending semaphore locked						*/
#define	ERROR_CODE_DRV_PROC					0x460F1a8d		/*	ドライバの処理がエラ?終了							*/
															/*	Packet receiving semaphore not locked				*/
#define	ERROR_CODE_GET_DRIVER_HANDLE		0x460F1a8e		/*	ドライバフ?イルのハンドル取得エラ?				*/
															/*	Driver processing completed erroneously.			*/
#define	ERROR_CODE_SEND_MSG					0x450E1ac1		/*	メッセ?ジ送信エラ?								*/
															/*	Failure to get driver file handle					*/
#define	ERROR_CODE_RECV_MSG					0x450E1ac2		/*	メッセ?ジ受信エラ?								*/
															/*	Message sending error								*/
#define	ERROR_CODE_INVALID_RESPONSE			0x450E1ac3		/*	受信パケットが不正									*/
															/*	Message receiving error								*/
#define	ERROR_CODE_INVALID_ID				0x450E1ac4		/*	受信パケットのIDが不正								*/
															/*	Receiving packet illegal							*/
#define	ERROR_CODE_INVALID_STATUS			0x450E1ac5		/*	受信パケットのステ??スが不正						*/
															/*	Receiving packet ID illegal							*/
#define	ERROR_CODE_INVALID_CMDCODE			0x450E1ac6		/*	受信パケットのコ?ンドコ?ドが不正					*/
															/*	Receiving packet status illegal						*/
#define	ERROR_CODE_INVALID_SEQNO			0x450E1ac7		/*	受信パケットのシ?ケンス番号が不正					*/
															/*	Receiving packet command code illegal				*/
#define	ERROR_CODE_SEND_RETRY_OVER			0x450E1ac8		/*	リトライ回数を超えた（パケット送信）				*/
															/*	Receiving packet sequence number illegal			*/
#define	ERROR_CODE_RECV_RETRY_OVER			0x450E1ac9		/*	リトライ回数を超えた（パケット受信）				*/
															/*	Number of retries exceeded (packet sending)			*/
#define	ERROR_CODE_RESPONSE_TIMEOUT			0x450E1aca		/*	応答待ち?イ?アウトエラ?							*/
															/*	Number of retries exceeded (packet receiving)		*/
#define	ERROR_CODE_WAIT_FOR_EVENT			0x450E1acb		/*	イベント待ちエラ?									*/
															/*	Response waiting timeout error						*/
#define	ERROR_CODE_EVENT_OPEN				0x450E1acc		/*	イベントオ?プン失敗								*/
															/*	Event waiting error									*/
#define	ERROR_CODE_EVENT_RESET				0x450E1acd		/*	イベントリセット失敗								*/
															/*	Failure to open event								*/
#define	ERROR_CODE_EVENT_READY				0x450E1ace		/*	イベント待ち?備失敗								*/
															/*	Failure to reset event								*/
#define	ERROR_CODE_PROCESSNUM				0x43001b01		/*	プロセス数 超過										*/
															/*	Failure to prepare for waiting for event			*/
#define	ERROR_CODE_GET_PROC_INFO			0x43001b02		/*	プロセス情報の取得									*/
															/*	Number of processes exceeded						*/
#define	ERROR_CODE_THREADNUM				0x43001b03		/*	スレッド数 超過										*/
															/*	Process information getting error					*/
#define	ERROR_CODE_GET_THRD_INFO			0x43001b04		/*	スレッド情報の取得									*/
															/*	Number of threads exceeded							*/
#define	ERROR_CODE_CREATE_MBOX				0x43001b05		/*	メ?ル?ックスの生成エラ?							*/
															/*	Thread information getting error					*/
#define	ERROR_CODE_DELETE_MBOX				0x43001b06		/*	メ?ル?ックスの削除エラ?							*/
															/*	Mail box creation error								*/
#define	ERROR_CODE_GET_TASKID				0x83001b07		/*	?スクID取得エラ?									*/
															/*	Mail box deletion error								*/
#define	ERROR_CODE_NO_THREADINFO			0x43001b08		/*	指定されたスレッド情報が存在しない					*/
															/*	Failure to get task ID								*/
#define	ERROR_CODE_COM_INITIALIZE			0x43001b09		/*	COMの初期化エラ?									*/
															/*	Specified thread information does not exist.		*/
#define	ERROR_CODE_COMDEVICENUM				0x430F1b41		/*	ComDevice数 超過									*/
															/*	COM initialization error							*/
#define	ERROR_CODE_GET_COM_DEVICE_HANDLE	0x430F1b42		/*	ComDevice情報?造体の取得エラ?						*/
															/*	Number of ComDevice exceeded						*/
#define	ERROR_CODE_COM_DEVICE_FULL			0x430F1b43		/*	ComDeviceが最大数を超えた							*/
															/*	Failure to get ComDevice information structure		*/
#define	ERROR_CODE_CREATE_PANELOBJ			0x430F1b44		/*	パネルコ?ンドオブジェクト生成失敗					*/
															/*	ComDevice exceeds the maximum number.				*/
#define	ERROR_CODE_OPEN_PANELOBJ			0x430F1b45		/*	パネルコ?ンドオブジェクトオ?プン失敗				*/
															/*	Failure to create panel command object				*/
#define	ERROR_CODE_CLOSE_PANELOBJ			0x430F1b46		/*	パネルコ?ンドオブジェクトクロ?ズ失敗				*/
															/*	Failure to open panel command object				*/
#define	ERROR_CODE_PROC_PANELOBJ			0x430F1b47		/*	パネルコ?ンドオブジェクト処理失敗					*/
															/*	Failure to close panel command object				*/
#define	ERROR_CODE_CREATE_CNTROBJ			0x430F1b48		/*	パネルコ?ンドオブジェクト生成失敗					*/
															/*	Failure to process panel command object				*/
#define	ERROR_CODE_OPEN_CNTROBJ				0x430F1b49		/*	パネルコ?ンドオブジェクトオ?プン失敗				*/
															/*	Failure to create panel command object				*/
#define	ERROR_CODE_CLOSE_CNTROBJ			0x430F1b4a		/*	パネルコ?ンドオブジェクトクロ?ズ失敗				*/
															/*	Failure to open panel command object				*/
#define	ERROR_CODE_PROC_CNTROBJ				0x430F1b4b		/*	パネルコ?ンドオブジェクト処理失敗					*/
															/*	Failure to close panel command object				*/
#define	ERROR_CODE_CREATE_COMDEV_MUTEX		0x430F1b4c		/*	ComDeviceテ?ブルのMutex作成失敗					*/
															/*	Failure to process panel command object				*/
#define	ERROR_CODE_CREATE_COMDEV_MAPFILE	0x430F1b4d		/*	ComDeviceテ?ブル用のMapFile作成失敗				*/
															/*	Failure to create Mutex for ComDevice table			*/
#define	ERROR_CODE_OPEN_COMDEV_MAPFILE		0x430F1b4e		/*	ComDeviceテ?ブル用のMapFileオ?プン失敗			*/
															/*	Failure to create MapFile for ComDevice table		*/
#define	ERROR_CODE_NOT_OBJECT_TYPE			0x430F1b4f		/*	オブジェクト種別のエラ?							*/
															/*	Failure to open MapFile for ComDevice table			*/
#define	ERROR_CODE_COM_NOT_OPENED			0x430F1b50		/*	未オ?プン											*/
															/*	Object type error									*/
#define	ERROR_CODE_PNLCMD_CPU_CONTROL		0x43081b80		/*	CPUコントロ?ルのエラ?								*/
															/*	Not opened											*/
#define	ERROR_CODE_PNLCMD_SFILE_READ		0x43081b81		/*	??スフ?イル読み出し失敗							*/
															/*	CPU control error									*/
#define	ERROR_CODE_PNLCMD_SFILE_WRITE		0x43081b82		/*	??スフ?イル書き込み失敗							*/
															/*	Failure to read out source file						*/
#define	ERROR_CODE_PNLCMD_REGISTER_READ		0x43081b83		/*	レジス?読み出し失敗								*/
															/*	Failure to write in source file						*/
#define	ERROR_CODE_PNLCMD_REGISTER_WRITE	0x43081b84		/*	レジス?書き込み失敗								*/
															/*	Failure to read out register						*/
#define	ERROR_CODE_PNLCMD_API_SEND			0x43081b85		/*	API Sendコ?ンド失敗								*/
															/*	Failure to write in register						*/
#define	ERROR_CODE_PNLCMD_API_RECV			0x43081b86		/*	API Recvコ?ンド失敗								*/
															/*	API Send command error								*/
#define	ERROR_CODE_PNLCMD_NO_RESPONSE		0x43081b87		/*	API Recvで応答パケットがまだない					*/
															/*	API Recv command error								*/
#define	ERROR_CODE_PNLCMD_PACKET_READ		0x43081b88		/*	パケット・リ?ド失敗								*/
															/*	No response packet is received at API Recv.			*/
#define	ERROR_CODE_PNLCMD_PACKET_WRITE		0x43081b89		/*	パケット・ライト失敗								*/
															/*	Failure to read packet								*/
#define	ERROR_CODE_PNLCMD_STATUS_READ		0x43081b8a		/*	ステ??スリ?ド失敗								*/
															/*	Failure to write packet								*/
#define	ERROR_CODE_NOT_CONTROLLER_RDY		0x440D1ba0		/*	コントロ?ラ???備未完							*/
															/*														*/
#define	ERROR_CODE_DOUBLE_CMD				0x440D1ba1		/*	モ?ションコ?ンド重複指令							*/
															/*														*/
#define	ERROR_CODE_DOUBLE_RCMD				0x440D1ba2		/*	モ?ションレス?ンスコ?ンド重複指令				*/
															/*														*/
#define	ERROR_CODE_FILE_NOT_OPENED			0x43001bc1		/*	フ?イルがオ?プンされていない						*/
															/*	Failure to read status								*/
#define	ERROR_CODE_OPENFILE_NUM				0x43001bc2		/*	オ?プンフ?イル数 超過								*/
															/*	File is not opened.									*/
#define	MP_CTRL_SYS_ERROR					0x4308106f		/*	システ?エラ?										*/
															/*														*/
#define	MP_CTRL_PARAM_ERR					0x43081092		/*	不正なパラメ??指定								*/
															/*														*/
#define	MP_CTRL_FILE_NOT_FOUND				0x43081094		/*	存在しないプログラ?が指定された					*/
															/*														*/
#define	MP_NOTMOVEHANDLE					0x470B1100		/*	未定?のMoveハンドル								*/
															/*	Undefined Move handle								*/
#define	MP_NOTTIMERHANDLE					0x470B1101		/*	未定?の?イ?ハンドル								*/
															/*	Undefined timer handle								*/
#define	MP_NOTINTERRUPT						0x470B1102		/*	未定?の仮想割り込み番号							*/
															/*	Undefined virtual interruption number				*/
#define	MP_NOTEVENTHANDLE					0x470B1103		/*	未定?のイベントハンドル							*/
															/*	Undefined event handle								*/
#define	MP_NOTMESSAGEHANDLE					0x470B1104		/*	未定?のメッセ?ジハンドル							*/
															/*	Undefined message handle							*/
#define	MP_NOTUSERFUNCTIONHANDLE			0x470B1105		/*	未定?のユ?ザ関数のハンドル						*/
															/*	Undefined user function handle						*/
#define	MP_NOTACTIONHANDLE					0x470B1106		/*	未定?のアクションハンドル							*/
															/*	Undefined action handle								*/
#define	MP_NOTSUBSCRIBEHANDLE				0x470B1107		/*	未定?のSubscribeハンドル							*/
															/*	Undefined Subscribe handle							*/
#define	MP_NOTDEVICEHANDLE					0x470B1108		/*	未定?のデバイスハンドル							*/
															/*	Undefined device handle								*/
#define	MP_NOTAXISHANDLE					0x470B1109		/*	未定?の軸ハンドル									*/
															/*	Undefined axis handle								*/
#define	MP_NOTMOVELISTHANDLE				0x470B110a		/*	未定?のMoveListハンドル							*/
															/*	Undefined MoveList handle							*/
#define	MP_NOTTRACEHANDLE					0x470B110b		/*	未定?のTraceハンドル								*/
															/*	Undefined Trace handle								*/
#define	MP_NOTGLOBALDATAHANDLE				0x470B110c		/*	未定?のグロ?バルデ??ハンドル					*/
															/*	Undefined global data handle						*/
#define	MP_NOTSUPERPOSEHANDLE				0x470B110d		/*	未定?のグロ?バルデ??ハンドル					*/
															/*	Undefined Superpose handle							*/
#define	MP_NOTCONTROLLERHANDLE				0x470B110e		/*	未定?のコントロ?ラハンドル						*/
															/*	Undefined Controller handle							*/
#define	MP_NOTFILEHANDLE					0x470B110f		/*	未定?のフ?イルハンドル							*/
															/*	Undefined file handle								*/
#define	MP_NOTREGISTERDATAHANDLE			0x470B1110		/*	レジス?ハンドルの未定?							*/
															/*	Undefined register handle							*/
#define	MP_NOTALARMHANDLE					0x470B1111		/*	アラ??ハンドルの未定?							*/
															/*	Undefined alarm handle								*/
#define	MP_NOTCAMHANDLE						0x470B1112		/*	CAMハンドルの未定?									*/
															/*	Undefined CAM handle								*/
#define	MP_NOTHANDLE						0x470B11ff		/*	未定?のハンドル									*/
															/*	Undefined handle									*/
#define	MP_NOTEVENTTYPE						0x470B1200		/*	未定?のイベント?イプ								*/
															/*	Undefined event type								*/
#define	MP_NOTDEVICETYPE					0x470B1201		/*	未定?のデバイス?イプ								*/
															/*	Undefined device type								*/
#define	MP_NOTDATAUNITSIZE					0x4B0B1202		/*	未定?の単位デ??サイズ							*/
															/*	Undefined unit data size							*/
#define	MP_NOTDEVICE						0x470B1203		/*	未定?のデバイス									*/
															/*	Undefined device									*/
#define	MP_NOTACTIONTYPE					0x470B1204		/*	未定?のアクション?イプ							*/
															/*	Undefined action type								*/
#define	MP_NOTPARAMNAME						0x4B0B1205		/*	未定?のパラメ??名								*/
															/*	Undefined parameter name							*/
#define	MP_NOTDATATYPE						0x470B1206		/*	未定?のデ???イプ								*/
															/*	Undefined data type									*/
#define	MP_NOTBUFFERTYPE					0x470B1207		/*	未定?のバッフ??イプ								*/
															/*	Undefined buffer type								*/
#define	MP_NOTMOVETYPE						0x4B0B1208		/*	未定?のMove?イプ									*/
															/*	Undefined Move type									*/
#define	MP_NOTANGLETYPE						0x470B1209		/*	未定?のAngle?イプ									*/
															/*	Undefined Angle type								*/
#define	MP_NOTDIRECTION						0x4B0B120a		/*	未定?の回?方向									*/
															/*	Undefined rotating direction						*/
#define	MP_NOTAXISTYPE						0x4B0B120b		/*	未定?の軸?イプ									*/
															/*	Undefined axis type									*/
#define	MP_NOTUNITTYPE						0x4B0B120c		/*	未定?の単位?イプ									*/
															/*	Undefined unit type									*/
#define	MP_NOTCOMDEVICETYPE					0x470B120d		/*	未定?のComDevice種別								*/
															/*	Undefined ComDevice type							*/
#define	MP_NOTCONTROLTYPE					0x470B120e		/*	未定?のControl種別									*/
															/*	Undefined Control type								*/
#define	MP_NOTFILETYPE						0x4B0B120f		/*	未定?のフ?イル種別								*/
															/*	Undefined file type									*/
#define	MP_NOTSEMAPHORETYPE					0x470B1210		/*	未定?のセ?フォ?イプ								*/
															/*	Undefined semaphore type							*/
#define	MP_NOTSYSTEMOPTION					0x470B1211		/*	未定?のSystemOption								*/
															/*	Undefined system option								*/
#define	MP_TIMEOUT_ERR						0x470B1212		/*	?イ?アウト エラ?									*/
															/*	Timeout error										*/
#define	MP_TIMEOUT							0x470B1213		/*	?イ?アウト										*/
															/*	Timeout												*/
#define	MP_NOTSUBSCRIBETYPE					0x470B1214		/*	未定?のSubscribe?イプ								*/
															/*	Undefined scan type									*/
#define	MP_NOTSCANTYPE						0x4B0B1214		/*	未定?のスキャン?イプ								*/
															/*	Undefined scan type									*/
#define	MP_DEVICEOFFSETOVER					0x470B1300		/*	範囲外のデバイスオフセット							*/
															/*	Out-of-range device offset							*/
#define	MP_DEVICEBITOFFSETOVER				0x470B1301		/*	範囲外のビットオフセット							*/
															/*	Out-of-range bit offset								*/
#define	MP_UNITCOUNTOVER					0x4B0B1302		/*	範囲外の個数										*/
															/*	Out-of-range quantity								*/
#define	MP_COMPAREVALUEOVER					0x4B0B1303		/*	範囲外の比較値										*/
															/*	Out-of-range compared value							*/
#define	MP_FCOMPAREVALUEOVER				0x4B0B1304		/*	範囲外の浮動小数?比較値							*/
															/*	Out-of-range floating-point compared value			*/
#define	MP_EVENTCOUNTOVER					0x470B1305		/*	範囲外の仮想割り込み番号							*/
															/*	Out-of-range virtual interruption number			*/
#define	MP_VALUEOVER						0x470B1306		/*	範囲外の値											*/
															/*	Out-of-range value									*/
#define	MP_FVALUEOVER						0x470B1307		/*	範囲外の浮動小数値									*/
															/*	Out-of-range floating point							*/
#define	MP_PSTOREDVALUEOVER					0x470B1308		/*	範囲外の格?場所の?イン?							*/
															/*	Out-of-range storage position pointer				*/
#define	MP_PSTOREDFVALUEOVER				0x470B1309		/*	範囲外の格?場所の?イン?(浮動小数?値)			*/
															/*	Out-of-range storage position pointer (floating decimal point value)	*/
#define	MP_SIZEOVER							0x470B130a		/*	範囲外のサイズ										*/
															/*	Out-of-range size									*/
#define	MP_RECEIVETIMEROVER					0x470B1310		/*	範囲外の受信待ち時間値								*/
															/*	Out-of-range waiting time value for receiving		*/
#define	MP_RECNUMOVER						0x470B1311		/*	範囲外のレコ?ド数(MoveList)						*/
															/*	Out-of-range number of records (MoveList)			*/
#define	MP_PARAMOVER						0x4B0B1312		/*	範囲外のパラメ??									*/
															/*	Out-of-range parameter								*/
#define	MP_FRAMEOVER						0x470B1313		/*	範囲外のパラメ??									*/
															/*	Out-of-range number of frames						*/
#define	MP_RADIUSOVER						0x4B0B1314		/*	範囲外の半径										*/
															/*	Out-of-range radius									*/
#define	MP_CONTROLLERNOOVER					0x470B1315		/*	範囲外の?器番号									*/
															/*	Out-of-range device number							*/
#define	MP_AXISNUMOVER						0x4B0B1316		/*	範囲外の軸数										*/
															/*	Out-of-range number of axes							*/
#define	MP_DIGITOVER						0x4B0B1317		/*	範囲外の桁数										*/
															/*	Out-of-range number of digits						*/
#define	MP_CALENDARVALUEOVER				0x4B0B1318		/*	範囲外のカレン?デ??								*/
															/*	Out-of-range calendar data							*/
#define	MP_OFFSETOVER						0x470B1319		/*	範囲外のオフセット									*/
															/*	Out-of-range offset									*/
#define	MP_NUMBEROVER						0x470B131a		/*	範囲外のレジス?の設定個数が指定された				*/
															/*	Out-of-range number of registers has been specified.*/
#define	MP_RACKOVER							0x470B131b		/*	範囲外のラック番号が指定された						*/
															/*	Out-of-range rack number has been specified.		*/
#define	MP_SLOTOVER							0x470B131c		/*	範囲外のラック番号が指定された						*/
															/*	Out-of-range slot number has been specified.		*/
#define	MP_FIXVALUEOVER						0x470B131d		/*	固定小数??のデ??が範囲外となった				*/
															/*	Fixed decimal point type data has been out of range.*/
#define	MP_REGISTERINFOROVER				0x470B131e		/*	範囲外のレジス?情報の個数が指定された				*/
															/*	Out-of-range number of register infomation has been specified.*/
#define	PC_MEMORY_ERR						0x430B1400		/*	パ?コンメモリ不足									*/
															/*	PC memory shortage									*/
#define	MP_NOCOMMUDEVICETYPE				0x470B1500		/*	通信デバイス?イプの指定エラ?						*/
															/*	Communication device type specification error		*/
#define	MP_NOTPROTOCOLTYPE					0x470B1501		/*	無効なプロトコル?イプ								*/
															/*	Invalid protocol type								*/
#define	MP_NOTFUNCMODE						0x470B1502		/*	無効な??モ?ド									*/
															/*	Invalid function mode								*/
#define	MP_MYADDROVER						0x470B1503		/*	範囲外の自局アドレスが設定された					*/
															/*	Out-of-range local station address has been set.	*/
#define	MP_NOTPORTTYPE						0x470B1504		/*	無効な??ト?イプ									*/
															/*	Invalid port type									*/
#define	MP_NOTPROTMODE						0x470B1505		/*	無効なプロトコルモ?ド								*/
															/*	Invalid protocol mode								*/
#define	MP_NOTCHARSIZE						0x470B1506		/*	無効なキャラク?ビット長							*/
															/*	Invalid character bit length						*/
#define	MP_NOTPARITYBIT						0x470B1507		/*	無効なパリティビット								*/
															/*	Invalid parity bit									*/
#define	MP_NOTSTOPBIT						0x470B1508		/*	無効なストップビット								*/
															/*	Invalid stop bit									*/
#define	MP_NOTBAUDRAT						0x470B1509		/*	無効な?送速度										*/
															/*	Invalid transmission speed							*/
#define	MP_TRANSDELAYOVER					0x470B1510		/*	範囲外の送信遅延が指定された						*/
															/*	Out-of-range sending delay has been specified.		*/
#define	MP_PEERADDROVER						0x470B1511		/*	範囲外の相手先アドレスが指定された					*/
															/*	Out-of-range remote station address has been specified.	*/
#define	MP_SUBNETMASKOVER					0x470B1512		/*	範囲外のサブネット?スクが指定された				*/
															/*	Out-of-range subnet mask has been specified.		*/
#define	MP_GWADDROVER						0x470B1513		/*	範囲外のGWアドレスが指定された						*/
															/*	Out-of-range GW address has been specified.			*/
#define	MP_DIAGPORTOVER						0x470B1514		/*	範囲外の診断??トが指定された						*/
															/*	Out-of-range diagnostic port has been specified.	*/
#define	MP_PROCRETRYOVER					0x470B1515		/*	範囲外の有手順リトライ回数が指定された				*/
															/*	Out-of-range number of retries has been specified.	*/
#define	MP_TCPZEROWINOVER					0x470B1516		/*	範囲外のTCP?ロウィンドウ?イ?						*/
															/*	Out-of-range TCP zero window timer					*/
#define	MP_TCPRETRYOVER						0x470B1517		/*	範囲外のTCP再送?イ?値								*/
															/*	Out-of-range TCP resending timer value				*/
#define	MP_TCPFINOVER						0x470B1518		/*	範囲外の終了?イ?値								*/
															/*	Out-of-range completion timer value					*/
#define	MP_IPASSEMBLEOVER					0x470B1519		/*	範囲外のIP組み込み?イ?値							*/
															/*	Out-of-range IP incorporating timer value			*/
#define	MP_MAXPKTLENOVER					0x470B1520		/*	範囲外の最大パケット長								*/
															/*	Out-of-range maximum packet length					*/
#define	MP_PEERPORTOVER						0x470B1521		/*	範囲外の相手先??ト								*/
															/*	Out-of-range remote station port					*/
#define	MP_MYPORTOVER						0x470B1522		/*	範囲外の自局??ト									*/
															/*	Out-of-range local station port						*/
#define	MP_NOTTRANSPROT						0x470B1523		/*	無効なトランス??ト層プロトコル					*/
															/*	Invalid transport layer protocol					*/
#define	MP_NOTAPPROT						0x470B1524		/*	無効なアプリケ?ション層プロトコル					*/
															/*	Invalid application layer protocol					*/
#define	MP_NOTCODETYPE						0x470B1525		/*	無効なコ?ド?イプ									*/
															/*	Invalid code type									*/
#define	MP_CYCTIMOVER						0x470B1526		/*	範囲外の通信周期が指定された						*/
															/*	Out-of-range communication cycle has been specified.*/
#define	MP_NOTIOMAPCOM						0x470B1527		/*	無効な入出力コ?ンド								*/
															/*	Invalid I/O commands								*/
#define	MP_NOTIOTYPE						0x470B1528		/*	無効なI/O?イプ指定									*/
															/*	Invalid I/O type specification						*/
#define	MP_IOOFFSETOVER						0x470B1529		/*	範囲外のI/Oオフセットが割り付けられた				*/
															/*	Out-of-range I/O offset has been allocated.			*/
#define	MP_IOSIZEOVER						0x470B1530		/*	範囲外のI/Oサイズが割り付けられた(個別)				*/
															/*	Out-of-range I/O size has been allocated (individualy).	*/
#define	MP_TIOSIZEOVER						0x470B1531		/*	範囲外のI/Oサイズが割り付けられた(合計)				*/
															/*	Out-of-range I/O size has been allocated (total).	*/
#define	MP_MEMORY_ERR						0x470B1532		/*	コントロ?ラメモリ不足								*/
															/*	Controller memory shortage							*/
#define	MP_NOTPTR							0x470B1533		/*	無効な?イン? (通信デバイス指定?造体/デバイス固有情報/対象通信デバイスハンドルへの?イン?異常)													*/
															/*	Invalid pointer (communication device specification structure/device inherent information/pointer error to objective communication device handle)	*/
#define	MP_NOTTIMEOUTVALUE					0x470B1534		/*	無効な?イ?アウト値								*/
															/*	Invalid time-out value								*/
#define	MP_TABLEOVER						0x43001800		/*	イベント検出テ?ブルリ??スが取得できない			*/
															/*	Event detection table resource cannot be got.		*/
#define	MP_ALARM							0x43001801		/*	アラ??発生										*/
															/*	Alarm has occurred.									*/
#define	MP_MEMORYOVER						0x43001802		/*	メモリリ??スが獲得できない						*/
															/*	Memory resource cannot be got.						*/
#define	MP_EXEC_ERR							0x470B1803		/*	アラ??発生										*/
															/*	System execution error								*/
#define	MP_NOTLOGICALAXIS					0x470B1804		/*	アラ??発生										*/
															/*	Logical axis number error							*/
#define	MP_NOTSUPPORTED						0x470B1805		/*	未サ??ト											*/
															/*	Not supported										*/
#define	MP_ILLTEXT							0x470B1806		/*	有効範囲外の長さの文字列入力						*/
															/*	Out-of-range length of character string was input.	*/
#define	MP_NOFILENAME						0x470B1807		/*	フ?イル名が未設定									*/
															/*	File name has not been set.							*/
#define	MP_NOTREGSTERNAME					0x470B1808		/*	指定されたレジス??デ??名がない					*/
															/*	Specified register data name cannot be found.		*/
#define	MP_FILEALREADYOPEN					0x4B0B1809		/*	同じフ?イルが既にオ?プンされている				*/
															/*	The same file has already been opened.				*/
#define	MP_NOFILEPACKET						0x470B180a		/*	指定された??スフ?イルパケットがない				*/
															/*	Specified source file packet cannot be found.		*/
#define	MP_NOTFILEPACKETSIZE				0x470B180b		/*	??スフ?イルパケットのサイズが違う				*/
															/*	Source file packet size is incorrect.				*/
#define	MP_NOTUSERFUNCTION					0x4B0B180c		/*	指定されたユ?ザ関数が存在しない					*/
															/*	Specified user funtion does not exist.				*/
#define	MP_NOTPARAMETERTYPE					0x4B0B180d		/*	指定されたユ?ザ関数が存在しない					*/
															/*	Specified parameter type does not exist.			*/
#define	MP_ILLREGHANDLETYPE					0x470B180e		/*	異常なレジス?ハンドル種別の指定					*/
															/*	Erroneous register handle type specified.			*/
#define	MP_ILLREGTYPE						0x470B1810		/*	異常なレジス?種別の指定							*/
															/*	Erroneous register type specified.					*/
#define	MP_ILLREGSIZE						0x470B1811		/*	異常なレジス?サイズの指定(WORD以外)				*/
															/*	Erroneous register size specified.(other than WORD)	*/
#define	MP_REGNUMOVER						0x470B1812		/*	レジス?有効範囲外									*/
															/*	Out-of-range register								*/
#define	MP_RELEASEWAIT						0x470B1813		/*	待ち状態解除										*/
															/*	Waiting status released								*/
#define	MP_PRIORITYOVER						0x470B1814		/*	設定できないPriority								*/
															/*	Priority that can not be set						*/
#define	MP_NOTCHANGEPRIORITY				0x470B1815		/*	待ち状態解除										*/
															/*	Priority that cannot be changed						*/
#define	MP_SEMAPHOREOVER					0x470B1816		/*	セ?ホ定?オ?バ?									*/
															/*	Semaphore definition over							*/
#define	MP_NOTSEMAPHOREHANDLE				0x470B1817		/*	未定?のセ?ホハンドル指定							*/
															/*	Undefined semaphore handle specification			*/
#define	MP_SEMAPHORELOCKED					0x470B1818		/*	指定セ?ホハンドルロック中							*/
															/*	Specified semaphore handle being locked				*/
#define	MP_CONTINUE_RELEASEWAIT				0x470B1819		/*	ymcContinueWaitForCompletion中の待ち状態解除		*/
															/*	Waiting status released during ymcContinueWaitForCompletion	*/
#define	MP_NOTTABLENAME						0x4B0B1820		/*	未定?のTable名称									*/
															/*	Undefined Table name								*/
#define	MP_ILLTABLETYPE						0x470B1821		/*	不正なTableType										*/
															/*	Illegal Table Type									*/
#define	MP_TIMEOUTVALUEOVER					0x470B1822		/*	範囲外の?イ?アウト値が指定された					*/
															/*	Out-of-range timeout value has been specified		*/
#define	MP_SYSTASK_CALL_ERR					0x470B1823		/*	call不可なシステ??スクからcallされた				*/
															/*														*/
#define	MP_CANNOTUSE_APICALL				0x470B19f0		/*	未対応APIがcallされた								*/
															/*														*/
#define	MP_OTHER_ERR						0x470B19ff		/*	その他のエラ?										*/
															/*	Other errors										*/

#endif	/*	#ifndef	_ERROR_CODE_H	*/
