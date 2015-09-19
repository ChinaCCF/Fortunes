#include <Windows.h> 
#include <WL/Graphics/Geometry.h>
#include <WL/Event.h>
#pragma once

namespace WL
{
	class _Window32;
	class Window32
	{
		_Window32* self;
		Window32(const Window32&) {}
		Window32& operator=(const Window32&) {}
	public:
		Window32();
		~Window32();
		st init();
	public:
		HWND get_handle();
		void set_ico(st ico);//resource id
		void set_title(const char* title);
		const char* get_title();
		void set_parent(HWND parent);
		HWND get_parent();
		void set_frame(Rect* r);
		void set_frame(st x, st y, st w, st h);
		void get_frame(Rect* r);
		void set_is_layer(st val);
		st get_is_layer();
		void set_alpha(ft val);
		ft get_alpha();

		void set_response(IResponse* response);

		void show();
		void hide();
		void close();
	public:
		static void set_application(HINSTANCE ins);
		static void loop();
		static void exit();
	};
}