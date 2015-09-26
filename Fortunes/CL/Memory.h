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
	};

#if CL_IS_DEBUG
#define cl_alloc(x)         cl::MemoryUtil::_alloc(x, __FILE__, __LINE__)
#define cl_realloc(p, s)    cl::MemoryUtil::_realloc(p, s, __FILE__, __LINE__)
#define cl_free(p)          do{if(p)cl::MemoryUtil::_free(p);}while(0)
#else
#define cl_alloc(x)         cl::MemoryUtil::alloc(x)
#define cl_realloc(p, s)    cl::MemoryUtil::realloc(p, s)
#define cl_free(p)          do{if(p)cl::MemoryUtil::free(p);}while(0)
#endif

#define cl_alloc_type(T)                ((T*)cl_alloc(sizeof(T)))
#define cl_alloc_type_with_count(T, n)  ((T*)cl_alloc(sizeof(T) * n))

#define cl_new(T)                       (new(cl_alloc_type(T)) T)

#if CL_IS_DEBUG
#define cl_delete(p)                    do{if(p){cl::MemoryUtil::unmark(p); delete p;}}while(0)
#else
#define cl_delete(p)                    do{if(p)delete p;}while(0)
#endif

	template<typename T>
	class Auto
	{
		T* p_;
	public:
		Auto(T* p) { p_ = p; }
		~Auto() { cl_delete(p_); }

		T* operator->() { return p_; }
		T& operator*() { return *p_; }
	};
#define cl_auto_new(T, var) cl::Auto<T> var(cl_new(T))

#if CL_IS_DEBUG
	template<typename T>
	T* _cl_alloc_init(st size, const char* file, st line)
	{
		T* obj = (T*)cl::MemoryUtil::_alloc(size, file, line);
		if(obj)
		{
			if(obj->init())
				return obj;
			else
			{
			cl_free(obj);
				return NULL;
			}
		}
		return NULL;
	}
#define cl_alloc_init(T) _cl_alloc_init<T>(sizeof(T), __FILE__, __LINE__)
#else
	template<typename T>
	T* _cl_alloc_init(st size)
	{
		T* obj = cl_alloc(size);
		if(obj)
		{
			if(obj->init())
				return obj;
			else
			{
				cl_free(obj);
				return NULL;
			}
		}
		return NULL;
	}
#define cl_alloc_init(T) _cl_alloc_init<T>(sizeof(T))
#endif
#define cl_uninit_free(p) do{if(p){p->uninit(); cl_free(p);}}while(0)

}
#endif