#include <CL/Object/String.h>
#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>
#include <CL/Memory.h>

namespace CL
{
	void StringUtil::string_copy(char* buf, st buf_size, const char* str)
	{
		st len = strlen(str);
		if(len >= buf_size) len = buf_size - 1;
		memcpy(buf, str, len + 1);
	}
#if CL_IS_DEBUG
	char* StringUtil::string_alloc(const char* str, const char* file, st line)
	{
		st len = strlen(str) + 1;
		char* buf = (char*)CL::MemoryUtil::_alloc(len, file, line);
		if(buf == NULL) return buf;
		memcpy(buf, str, len);
		return buf;
	}
#else
	char* StringUtil::string_alloc(const char* str)
	{
		st len = strlen(str) + 1;
		char* buf = (char*)CL::MemoryUtil::alloc(len);
		if(buf == NULL) return buf;
		memcpy(buf, str, len);
		return buf;
	}
#endif

	void StringUtil::format(char* buf, st buf_size, const char* format_str, ...)
	{
		va_list list;
		va_start(list, format_str);
#if CL_IS_MAC
		vsnprintf(buf, buf_size, format_str, list);
#else
		vsnprintf_s(buf, buf_size, buf_size - 1, format_str, list);
#endif
		va_end(list);
	}

	st StringUtil::char_to_wchar_count(const char* str)
	{
		return MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	}
	st StringUtil::wchar_to_char_count(const wchar* str)
	{
		return WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	}

	st StringUtil::char_to_wchar(const char* str, wchar* buf, st buf_count)
	{
		st count = char_to_wchar_count(str);
		if(count > buf_count) return FALSE;
		MultiByteToWideChar(CP_ACP, 0, str, -1, buf, buf_count);
		return TRUE;
	}
	st StringUtil::wchar_to_char(const wchar* str, char* buf, st buf_count)
	{
		st count = wchar_to_char_count(str);
		if(count > buf_count) return FALSE;
		WideCharToMultiByte(CP_ACP, 0, str, -1, buf, buf_count, NULL, NULL);
		return TRUE;
	}
}