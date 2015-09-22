#include <CL/CL.h>
#include <string.h>

#ifndef __CL_Object_String__
#define __CL_Object_String__

namespace CL
{
	class StringUtil
	{
	public:
#if CL_IS_DEBUG
		static char* string_alloc(const char*str, const char* file, st line);
#else
		static char* string_alloc(const char*str);
#endif
		static void string_copy(char* buf, st buf_size, const char* str);
		static void format(char* buf, st buf_size, const char* format_str, ...);

		static st char_to_wchar_count(const char* str);
		static st wchar_to_char_count(const wchar* str);

		static st char_to_wchar(const char* str, wchar* buf, st buf_count);
		static st wchar_to_char(const wchar* str, char* buf, st buf_count);

		//static st char_to_utf8_count(const char* str);
		//static st utf8_to_char_count(const char* str);

		//static st char_to_utf8(const char* str, char* buf, st buf_count);
		//static st utf8_to_char(const char* str, char* buf, st buf_count);

		//static st wchar_to_utf8_count(const wchar* str);
		//static st utf8_to_wchar_count(const char* str);

		//static st wchar_to_utf8(const char* str, wchar* buf, st buf_count);
		//static st utf8_to_wchar(const wchar* str, char* buf, st buf_count);
	};

#if CL_IS_DEBUG
#define cl_string_alloc(x) CL::StringUtil::string_alloc(x, __FILE__, __LINE__)
#else
#define cl_string_alloc(x) CL::StringUtil::string_alloc(x)
#endif

	/*class String : public virtual Object
	{
	char* buffer_;
	char* string_;
	ut size_;
	ut length_;
	public:
	String()
	{
	buffer_ = NULL;
	string_ = NULL;
	size_ = 0;
	length_ = 0;
	}
	~String()
	{

	}
	ut length() { return length_; }
	const char* string() { return string_; }
	};*/
}

#endif