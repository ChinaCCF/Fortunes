#include <CL/Object/String.h>
#include <stdarg.h>
#include <stdio.h>

namespace CL {
	void  StringUtil::string_copy(char* buf, st buf_size, const char* str)
	{
		st len = strlen(str);
		if(len >= buf_size) len = buf_size - 1;
		memcpy(buf, str, len + 1);
	}

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
}