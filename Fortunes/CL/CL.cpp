#include <CL/CL.h>
#include <stdio.h>
#include <String.h>

#if !CL_IS_MAC
#include <Windows.h>
#endif

namespace cl
{
	void Logger::printf(const char *file, st line, const char *message)
	{
#if CL_IS_MAC
		const char* file_name  = strrchr(file, '/');
		if (file_name == NULL)
			file_name = file;
		else
			file_name += 1;
		::printf("File: %s   ", file_name);
		::printf("Line: %d   Message:   ", line);
		::printf("%s\n", message);
#else
		OutputDebugStringA("File: ");
		const char* file_name = strrchr(file, '\\');
		if(file_name == NULL)
			file_name = file;
		else
			file_name += 1;

		OutputDebugStringA(file_name);
		OutputDebugStringA("   ");
		OutputDebugStringA("Line: ");
		char buf[32];
		_snprintf_s(buf, 16, "%d   Message:  ", line);
		OutputDebugStringA(buf);
		OutputDebugStringA(message);
#endif
	}
}