#include <windows.h> 
#include <SDKDDKVer.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <CL/Memory.h>
#include <WL/Window32.h>
#include <CL/Object/String.h>
#include <UI/Login.h>
#include <UI/Main.h>

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	WL::Window32::set_application(hInstance);

	Fortunes::Login* login = cl_new(Fortunes::Login);
	login->init();
	WL::Window32::loop();
	cl_delete(login);

	/*Fortunes::Main* main = cl_new(Fortunes::Main);
	main->init();
	WL::Window32::loop();
	cl_delete(main);*/

	CL::MemoryUtil::check();
	return 0;
}

