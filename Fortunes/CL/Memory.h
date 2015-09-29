#include <CL/CL.h>
#include <new> 

#ifndef __CL_Memory__
#define __CL_Memory__

namespace cl
{
	class MemoryUtil
	{
	public:
#if CL_IS_DEBUG
		static void* _alloc(st size, const char* file, st line);
		static void* _realloc(void* p, st size, const char* file, st line);
		static void _free(void* p);
		static void unmark(void* p);
		static void check();
#endif

		static void* alloc(st size);
		static void* realloc(void* p, st size);
		static void free(void* p);

		static void copy(void* dst, void* src, st size);
		static void zero(void* buf, st size);

		static inline void little_endian_put(u8* p, s16 val)
		{
			p[0] = (val >> 0) & 0xFF;
			p[1] = (val >> 8) & 0xFF;
		}
		static inline void little_endian_put(u8* p, s32 val)
		{
			p[0] = (val >> 0) & 0xFF;
			p[1] = (val >> 8) & 0xFF;
			p[2] = (val >> 16) & 0xFF;
			p[3] = (val >> 24) & 0xFF;
		}
		static inline void little_endian_put(u8* p, s64 val)
		{
			p[0] = (val >> 0) & 0xFF;
			p[1] = (val >> 8) & 0xFF;
			p[2] = (val >> 16) & 0xFF;
			p[3] = (val >> 24) & 0xFF;
			p[4] = (val >> 32) & 0xFF;
			p[5] = (val >> 40) & 0xFF;
			p[6] = (val >> 48) & 0xFF;
			p[7] = (val >> 56) & 0xFF;
		}

		static inline void big_endian_put(u8* p, s16 val)
		{
			p[1] = (val >> 0) & 0xFF;
			p[0] = (val >> 8) & 0xFF;
		}
		static inline void big_endian_put(u8* p, s32 val)
		{
			p[3] = (val >> 0) & 0xFF;
			p[2] = (val >> 8) & 0xFF;
			p[1] = (val >> 16) & 0xFF;
			p[0] = (val >> 24) & 0xFF;
		}
		static inline void big_endian_put(u8* p, s64 val)
		{
			p[7] = (val >> 0) & 0xFF;
			p[6] = (val >> 8) & 0xFF;
			p[5] = (val >> 16) & 0xFF;
			p[4] = (val >> 24) & 0xFF;
			p[3] = (val >> 32) & 0xFF;
			p[2] = (val >> 40) & 0xFF;
			p[1] = (val >> 48) & 0xFF;
			p[0] = (val >> 56) & 0xFF;
		}

		static inline void little_endian_get(u8* p, s16* val)
		{
			*val = p[0];
			*val |= ((s16)p[1]) << 8;
		}
		static inline void little_endian_get(u8* p, s32* val)
		{
			*val = p[0];
			*val |= ((s32)p[1]) << 8;
			*val |= ((s32)p[2]) << 16;
			*val |= ((s32)p[3]) << 24;
		}
		static inline void little_endian_get(u8* p, s64* val)
		{
			*val = p[0];
			*val |= ((s64)p[1]) << 8;
			*val |= ((s64)p[2]) << 16;
			*val |= ((s64)p[3]) << 24;
			*val |= ((s64)p[4]) << 32;
			*val |= ((s64)p[5]) << 40;
			*val |= ((s64)p[6]) << 48;
			*val |= ((s64)p[7]) << 56;
		}

		static inline void big_endian_get(u8* p, s16* val)
		{
			*val = p[1];
			*val |= ((s16)p[0]) << 8;
		}
		static inline void big_endian_get(u8* p, s32* val)
		{
			*val = p[3];
			*val |= ((s32)p[2]) << 8;
			*val |= ((s32)p[1]) << 16;
			*val |= ((s32)p[0]) << 24;
		}
		static inline void big_endian_get(u8* p, s64* val)
		{
			*val = p[7];
			*val |= ((s64)p[6]) << 8;
			*val |= ((s64)p[5]) << 16;
			*val |= ((s64)p[4]) << 24;
			*val |= ((s64)p[3]) << 32;
			*val |= ((s64)p[2]) << 40;
			*val |= ((s64)p[1]) << 48;
			*val |= ((s64)p[0]) << 56;
		}

		//if is_right, shift right
		static inline u8 rotate(u8 val, ut n, st is_right = TRUE) { if(is_right) return val >> n | val << (8 - n); return val << n | val >> (8 - n); }
		static inline u16 rotate(u16 val, ut n, st is_right = TRUE) { if(is_right) return val >> n | val << (16 - n); return val << n | val >> (16 - n); }
		static inline u32 rotate(u32 val, ut n, st is_right = TRUE) { if(is_right) return val >> n | val << (32 - n); return val << n | val >> (32 - n); }
		static inline u64 rotate(u64 val, ut n, st is_right = TRUE) { if(is_right) return val >> n | val << (64 - n); return val << n | val >> (64 - n); }
		static inline st is_contain_bits(st flags, st bits) { st val = flags & bits; if(val == bits) return TRUE; return FALSE; }
		static inline st is_float_equ(ft f1, ft f2, ft offset = 0.001) { ft val = abs(f1 - f2); if(val < offset) return TRUE; return FALSE; }
	};

#if CL_IS_DEBUG
#define cl_alloc(x)         cl::MemoryUtil::_alloc(x, __FILE__, __LINE__)
#define cl_realloc(p, s)    cl::MemoryUtil::_realloc(p, s, __FILE__, __LINE__)
#define cl_free(p)          do{if(p)cl::MemoryUtil::_free(p);}while(FALSE)
#define cl_delete(p)        do{if(p){cl::MemoryUtil::unmark(p); delete p;}}while(FALSE)
#else
#define cl_alloc(x)         cl::MemoryUtil::alloc(x)
#define cl_realloc(p, s)    cl::MemoryUtil::realloc(p, s)
#define cl_free(p)          do{if(p)cl::MemoryUtil::free(p);}while(FALSE)
#define cl_delete(p)        do{if(p)delete p;}while(FALSE)
#endif

#define cl_alloc_type(T)                ((T*)cl_alloc(sizeof(T)))
#define cl_alloc_type_with_count(T, n)  ((T*)cl_alloc(sizeof(T) * n))

	template<typename T>
	static inline T* _cl_new(T* obj)
	{
		if(obj) return new(obj)T;
		return NULL;
	}
#define cl_new(T) cl::_cl_new(cl_alloc_type(T))

	template<typename T>
	static inline T* _cl_init(T* obj)
	{
		if(obj)
		{
			new(obj)T;//for class member
			if(obj->init()) return obj;
			delete obj;
			return NULL;
		}
		return NULL;
	}
#define cl_new_init(T) cl::_cl_init(cl_alloc_type(T))


	template<typename T>
	class Auto
	{
		T* p;
	public:
		Auto(T* _p) { p = _p; }
		~Auto() { cl_delete(p); }

		T* operator->() { return p; }
		T& operator*() { return *p; }
		st operator==(st val)
		{
			st self = (st)p;
			if(self == val) return TRUE;
			return FALSE;
		}
		st operator!=(st val) { return operator==(val); }
		operator st() { return p != NULL; }
	};
#define cl_auto_new_init(T) cl::Auto<T>(cl_new_init(T))
}
#endif