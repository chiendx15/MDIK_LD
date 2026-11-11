#pragma once

#pragma pack(push, 8)

#ifdef _IP_DATA_INTERNAL_
typedef int                 long;
typedef int                 INT;
typedef unsigned int        DWORD;
typedef unsigned int        UINT;
typedef int                 BOOL;
typedef unsigned char		bool;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef double              DOUBLE;
typedef DWORD				COLORREF;
typedef unsigned __int64	ULONGLONG;
#define TRUE                1
#define FALSE               0
#define MAX_PATH			260

typedef char						__int8;
typedef short						__int16;
typedef unsigned char				__uint8;
typedef unsigned short				__uint16;
typedef unsigned __int32			__uint32;
typedef unsigned __int64			__uint64;

// POINT, SIZE, RECT -- default.defines.h
//typedef struct { double x, y; }			POINT2D;
//typedef struct { double x, y, z; }		POINT3D;
//typedef struct { double cx, cy; }		SIZE2D;
//typedef struct { double cx, cy, cz; }	SIZE3D;
//typedef struct { __int32 x, y; }		POINT2L;
//typedef struct { __int32 x, y, z; }		POINT3L;
//typedef struct { __int32 cx, cy; }		SIZE2L;
//typedef struct { __int32 cx, cy, cz; }	SIZE3L;
//typedef struct { double left, top, right, bottom; }					RECT2D;
//typedef struct { double left, top, front, right, bottom, end; }		RECT3D;
//typedef struct { __int32 left, top, right, bottom; }				RECT2L;
//typedef struct { __int32 left, top, front, right, bottom, end; }	RECT3L;
typedef union { double  val[2]; struct { double x, y; }; }																		POINT2D;
typedef union { double  val[3]; struct { double x, y, z; }; }																	POINT3D;
typedef union { double  val[2]; struct { double cx, cy; }; }																	SIZE2D;
typedef union { double  val[3]; struct { double cx, cy, cz; }; }																SIZE3D;
typedef union { __int32 val[2]; struct { __int32 x, y; }; }																		POINT2L;
typedef union { __int32 val[3]; struct { __int32 x, y, z; }; }																	POINT3L;
typedef union { __int32 val[2]; struct { __int32 cx, cy; }; }																	SIZE2L;
typedef union { __int32 val[3]; struct { __int32 cx, cy, cz; }; }																SIZE3L;
typedef union { double  val[4]; struct { POINT2D pt0, pt1; }; struct { double left, top, right, bottom; }; }					RECT2D;
typedef union { double  val[6]; struct { POINT3D pt0, pt1; }; struct { double left, top, front, right, bottom, end; }; }		RECT3D;
typedef union { __int32 val[4]; struct { POINT2L pt0, pt1; }; struct { __int32 left, top, right, bottom; }; }					RECT2L;
typedef union { __int32 val[6]; struct { POINT3L pt0, pt1; }; struct { __int32 left, top, front, right, bottom, end; }; }		RECT3L;
#endif


//typedef struct T_P1 {
//	//<< Add by KMS
//#ifndef _IP_DATA_INTERNAL_
//	BOOL operator != (const T_P1& B) const { return !(*this == B); }
//	BOOL operator == (const T_P1& B) const {
//		return dPos		== B.dPos
//			&& dSpeed	== B.dSpeed
//			;
//	}
//	template <class ARCHIVE> friend ARCHIVE& operator << (ARCHIVE& ar, const T_P1& B) { return B.StoreTo(ar); }
//	template <class ARCHIVE> friend ARCHIVE& operator >> (ARCHIVE& ar, T_P1& B) { return B.LoadFrom(ar); }
//	template <class ARCHIVE > ARCHIVE& StoreTo(ARCHIVE& ar) const {
//		ar << dPos;
//		ar << dSpeed;
//		return ar;
//	}
//	template <class ARCHIVE > ARCHIVE& LoadFrom(ARCHIVE& ar) {
//		ar >> dPos;
//		ar >> dSpeed;
//		return ar;
//	}
//#endif
//	//>>
//
//	double dPos = 0.; double dSpeed = 0.;
//} T_P1;
//
//typedef struct T_P2 {
//	//<< Add by KMS
//#ifndef _IP_DATA_INTERNAL_
//	BOOL operator != (const T_P2& B) const { return !(*this == B); }
//	BOOL operator == (const T_P2& B) const {
//		return dPosX	== B.dPosX
//			&& dPosY	== B.dPosY
//			&& dSpeedX	== B.dSpeedX
//			&& dSpeedY	== B.dSpeedY
//			;
//	}
//	template <class ARCHIVE> friend ARCHIVE& operator << (ARCHIVE& ar, const T_P2& B) { return B.StoreTo(ar); }
//	template <class ARCHIVE> friend ARCHIVE& operator >> (ARCHIVE& ar, T_P2& B) { return B.LoadFrom(ar); }
//	template <class ARCHIVE > ARCHIVE& StoreTo(ARCHIVE& ar) const {
//		ar << dPosX;
//		ar << dPosY;
//		ar << dSpeedX;
//		ar << dSpeedY;
//		return ar;
//	}
//	template <class ARCHIVE > ARCHIVE& LoadFrom(ARCHIVE& ar) {
//		ar >> dPosX;
//		ar >> dPosY;
//		ar >> dSpeedX;
//		ar >> dSpeedY;
//		return ar;
//	}
//#endif
//	//>>
//
//	double dPosX = 0.; double dPosY = 0.; double dSpeedX = 0.; double dSpeedY= 0.;
//} T_P2;
//
//
//typedef struct T_P3 {
//	//<< Add by KMS
//#ifndef _IP_DATA_INTERNAL_
//	BOOL operator != (const T_P3& B) const { return !(*this == B); }
//	BOOL operator == (const T_P3& B) const {
//		return dPosX	== B.dPosX
//			&& dPosY	== B.dPosY
//			&& dPosZ	== B.dPosZ
//			&& dSpeedX	== B.dSpeedX
//			&& dSpeedY	== B.dSpeedY
//			&& dSpeedZ	== B.dSpeedZ
//			;
//	}
//	template <class ARCHIVE> friend ARCHIVE& operator << (ARCHIVE& ar, const T_P3& B) { return B.StoreTo(ar); }
//	template <class ARCHIVE> friend ARCHIVE& operator >> (ARCHIVE& ar, T_P3& B) { return B.LoadFrom(ar); }
//	template <class ARCHIVE > ARCHIVE& StoreTo(ARCHIVE& ar) const {
//		ar << dPosX;
//		ar << dPosY;
//		ar << dPosZ;
//		ar << dSpeedX;
//		ar << dSpeedY;
//		ar << dSpeedZ;
//		return ar;
//	}
//	template <class ARCHIVE > ARCHIVE& LoadFrom(ARCHIVE& ar) {
//		ar >> dPosX;
//		ar >> dPosY;
//		ar >> dPosZ;
//		ar >> dSpeedX;
//		ar >> dSpeedY;
//		ar >> dSpeedZ;
//		return ar;
//	}
//#endif
//	//>>
//
//	double dPosX = 0.; double dPosY = 0.; double dPosZ = 0.; double dSpeedX = 0.; double dSpeedY = 0.; double dSpeedZ = 0.;
//} T_P3;
//>>

#ifndef _IP_DATA_INTERNAL_
#include "MessageToolWnd\MessageTool.hpp"
#endif

#pragma pack(pop)
