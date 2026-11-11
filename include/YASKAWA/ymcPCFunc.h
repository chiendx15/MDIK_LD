/************************************************************************

		ymcPCFunc.h

		COPYRIGHT (C) YASKAWA ELECTRIC CORPORATION

*************************************************************************/


#ifndef	_PC_FUNC_H
#define	_PC_FUNC_H

/************************************************************************/
/*																		*/
/*				<< Declaration of common PCAPI function >>				*/
/*																		*/
/*		File name	:  ymcPCFunc.h										*/
/*																		*/
/*																		*/
/************************************************************************/


#ifndef	EXPORT_FUNC
	#ifdef __cplusplus
		#define	EXPORT_FUNC	extern "C"
	#else
		#define	EXPORT_FUNC
	#endif
#endif
#define	WINAPI __stdcall

#include "PCApiFunc.h"



#endif	/* #ifndef _PC_FUNC_H */
