#include <windows.h> 
#include <SDKDDKVer.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <UI/Login.h>
#include <CL/Memory.h>

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{

	WL::Window32::set_application(hInstance);
	cl_auto(Fortunes::Login, login);
	//login->close();
	WL::Window32::loop();
	CL::MemoryUtil::check();
	return 0;
}

