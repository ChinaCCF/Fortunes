#include <CL/CL.h>
#include <string.h>

#ifndef __CL_Object_String__
#define __CL_Object_String__

namespace cl
{
	class StringUtil
	{
	public:
#if CL_IS_DEBUG
		static char* alloc(const char*str, const char* file, st line);
		static char* alloc_from_memory(void* buf, st size, const char* file, st line);
		static void* memory_alloc_from(const char* file, st line, const char* str, st len = 0);
#else
		static char* alloc(const char*str);
		static char* alloc_from_memory(void* buf, st size);
		static void* memory_alloc_from(const char* str, st len = 0);
#endif

		static st length(const char* str);
		static st copy(char* buf, st buf_size, const char* str);
		static st compare(const char* str1, const char* str2);
		static st compare(const char* str1, const char* str2, st len);
		static st compare_without_case(const char* str1, const char* str2, st len = 9999);
		static void append(char* buf, st buf_size, const char* str);
		static char* search(char* str, char c);
		static const char* search(const char* str, char c);
		static st format(char* buf, st buf_size, const char* format_str, ...);

		static st char_to_wchar_count(const char* str);
		static st wchar_to_char_count(const wchar* str);

		static st char_to_wchar(wchar* buf, st buf_count, const char* str);
		static st wchar_to_char(char* buf, st buf_count, const wchar* str);

		//static st char_to_utf8_count(const char* str);
		//static st utf8_to_char_count(const char* str);

		//static st char_to_utf8(const char* str, char* buf, st buf_count);
		//static st utf8_to_char(const char* str, char* buf, st buf_count);

		//static st wchar_to_utf8_count(const wchar* str);
		//static st utf8_to_wchar_count(const char* str);

		//static st wchar_to_utf8(const char* str, wchar* buf, st buf_count);
		//static st utf8_to_wchar(const wchar* str, char* buf, st buf_count);

		static inline st is_alpha(char c) { if(c >= 'a' && c <= 'z') return TRUE; if(c >= 'A' && c <= 'Z') return TRUE; return FALSE; }
		static inline st is_number(char c) { if(c >= '0' && c <= '9') return TRUE; return FALSE; }
		static inline char upper(char c) { if(c >= 'a') c -= 'a' - 'A'; return c; }
		static inline char lower(char c) { if(c <= 'Z') c += 'a' - 'A'; return c; }
		static inline st skip_blank_char(const char* str) { const char* org = str; while(*str && *str <= 32) ++str; return str - org; }
		static st parse_hex(const char* str, st* val);
		static st parse_dec(const char* str, st* val);
		static st parse_bin(const char* str, st* val);
		static st parse_value(const char* str, st* val);
		static st parse_float(const char* str, ft* val);
		static st is_float_string(const char* str);
		static wchar parse_uchar(const char* str);// \u5c0f 'С'
		static st parse_uchar_string(const char* str, wchar** result);
	};

#if CL_IS_DEBUG
#define cl_string_alloc(x) cl::StringUtil::alloc(x, __FILE__, __LINE__)
#define cl_string_alloc_from_memory(p, size) cl::StringUtil::alloc_from_memory(p, size, __FILE__, __LINE__)
#define cl_memory_alloc_from_string(str, ...) cl::StringUtil::memory_alloc_from(__FILE__, __LINE__, str, ##__VA_ARGS__)
#else
#define cl_string_alloc(x) cl::StringUtil::alloc(x)
#define cl_string_alloc_from_memory(p, size) cl::StringUtil::alloc_from_memory(p, size)
#define cl_memory_alloc_from_string(str, ...) cl::StringUtil::memory_alloc_from(str, ##__VA_ARGS__)
#endif

	struct StringInfo
	{
		char* string;
		st length;
	};

	/*class _String;
	class String
	{
	_String* self;
	public:
	st init();
	void uninit();

	void set(const char* str);
	void set(const wchar* str);
	st length();
	const char* string();
	};*/
}

#endif