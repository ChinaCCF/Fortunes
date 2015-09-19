#ifndef __CL_CL__
#define __CL_CL__

#define CL_IS_MAC 0
#define CL_IS_64 0
#define CL_IS_DEBUG 1

/*#########################################################*/
// type
/*#########################################################*/
#if CL_IS_MAC
#include <stdint.h>
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef __int8 s8;
typedef __int16 s16;
typedef __int32 s32;
typedef __int64 s64;

typedef unsigned __int8 u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;
#endif

typedef float f32;
typedef double f64;
typedef unsigned char uchar;
typedef wchar_t wchar;

#define MINS8 ((s8)0x80)
#define MAXS8 ((s8)0x7F)
#define MAXU8 ((u8)0xFF)

#define MINS16 ((s16)0x8000)
#define MAXS16 ((s16)0x7FFF)
#define MAXU16 ((u16)0xFFFF)

#define MINS32 ((s32)0x80000000)
#define MAXS32 ((s32)0x7FFFFFFF)
#define MAXU32 ((u32)0xFFFFFFFF)

#define MINS64 ((s64)0x8000000000000000)
#define MAXS64 ((s64)0x7FFFFFFFFFFFFFFF)
#define MAXU64 ((u64)0xFFFFFFFFFFFFFFFF)

#define F32_MAX_MEM 0x7F7FFFFF
#define F32_MAX 3.4028234e+38
#define F64_MAX_MEM 0x7FEFFFFFFFFFFFFF
#define F64_MAX 1.7976931348623157e+308
#define F32_MIN_MEM 0x800000
#define F32_MIN 1.1754943e-38
#define F64_MIN_MEM 0x10000000000000
#define F64_MIN 2.2250738585072014e-308

#if CL_IS_64
typedef s64 st;
typedef u64 ut;
#define MAXT MAXS64
#define MAXU MAXU64
#else
typedef s32 st;
typedef u32 ut;
#define MAXT MAXS32
#define MAXU MAXU32
#endif
typedef f64 ft;

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

namespace CL
{
	template <typename T> inline T MAX(T a, T b) { return a > b ? a : b; }
	template <typename T> inline T MIN(T a, T b) { return a > b ? b : a; }
	template <typename T> inline T DIF(T a, T b) { if(a > b) return a - b; else return b - a; }

	class Logger
	{
	public:
		static void printf(const char* file, st line, const char* message);
	};
#define cl_printf(msg) CL::Logger::printf(__FILE__, __LINE__, msg)

}
#endif