#include <CL/Object/String.h>
#include <stdarg.h>
#include <stdio.h>
#if !CL_IS_MAC
#include <Windows.h>
#endif
#include <CL/Memory.h>

namespace cl
{
	void StringUtil::string_copy(char* buf, st buf_size, const char* str)
	{
		st len = strlen(str);
		if(len >= buf_size) len = buf_size - 1;
		memcpy(buf, str, len);
		buf[len] = 0;
	}
	void StringUtil::string_append(char* buf, st buf_size, const char* str)
	{
		st buf_str_len = strlen(buf);
		st str_len = strlen(str);
		st len = buf_size - buf_str_len - 1;
		if(len > str_len) len = str_len;
		memcpy(buf + buf_str_len, str, len);
		buf[buf_str_len + len] = 0;
	}
#if CL_IS_DEBUG
	char* StringUtil::string_alloc(const char* str, const char* file, st line)
	{
		st len = strlen(str) + 1;
		char* buf = (char*)cl::MemoryUtil::_alloc(len, file, line);
		if(buf == NULL) return buf;
		memcpy(buf, str, len);
		return buf;
	}
#else
	char* StringUtil::string_alloc(const char* str)
	{
		st len = strlen(str) + 1;
		char* buf = (char*)cl::MemoryUtil::alloc(len);
		if(buf == NULL) return buf;
		memcpy(buf, str, len);
		return buf;
	}
#endif

	st StringUtil::string_compare(const char* str1, const char* str2)
	{
		return strcmp(str1, str2) == 0;
	}
	void StringUtil::string_format(char* buf, st buf_size, const char* format_str, ...)
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

	st StringUtil::char_to_wchar(wchar* buf, st buf_count, const char* str)
	{
		st count = char_to_wchar_count(str);
		if(count > buf_count) return FALSE;
		MultiByteToWideChar(CP_ACP, 0, str, -1, buf, buf_count);
		return TRUE;
	}
	st StringUtil::wchar_to_char(char* buf, st buf_count, const wchar* str)
	{
		st count = wchar_to_char_count(str);
		if(count > buf_count) return FALSE;
		WideCharToMultiByte(CP_ACP, 0, str, -1, buf, buf_count, NULL, NULL);
		return TRUE;
	}
	}