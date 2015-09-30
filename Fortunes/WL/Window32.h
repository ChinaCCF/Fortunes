#include <Windows.h> 
#include <WL/Graphics/Geometry.h>
#pragma once

namespace WL
{
	class _Window32;
	class Window32
	{
		_Window32* self;
	public:
		Window32();
		~Window32();
		st init(const char* class_name = NULL, st style = 0, st ex_style = 0);

		Window32(const Window32&) = delete;
		Window32& operator=(const Window32&) = delete;

		cl_define_property(Window32, HWND, parent);
		cl_define_property(Window32, const char*, title);
		cl_define_property(Window32, const Rect*, frame);
		void set_frame(ft x, ft y, ft w, ft h);
		cl_define_property(Window32, st, is_layer);
		cl_define_property(Window32, ft, alpha);
		cl_define_property(Window32, st, is_show);
		cl_define_property(Window32, st, ico);//resource id
		cl_define_property(Window32, st, is_focus);
		cl_define_property(Window32, void*, dispatcher);

		operator HWND();
		void update();
		void close();
	public:
		static HINSTANCE application = NULL;
		static void loop() { MSG msg; while(GetMessage(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); } }
		static void exit() { PostQuitMessage(0); }
	};
}