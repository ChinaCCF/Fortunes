#include <Windows.h> 
#include <WL/Graphics/Geometry.h>
#pragma once

namespace WL
{
	class _Window32;
	class Window32
	{
		_Window32* self;
	private:
		Window32(const Window32&) {}//forbidden
		Window32& operator=(const Window32&) {}//forbidden
	public:
		Window32() {}
		~Window32();
		st init();
	public:
		st create(const char* class_name, st style, st ex_style);
		HWND get_handle();
		void set_ico(st ico_resource_id);//resource id
		void set_title(const char* title);
		const char* get_title();
		void set_parent(HWND parent);
		HWND get_parent();
		void set_frame(const Rect* r);
		void set_frame(ft x, ft y, ft w, ft h);
		void get_frame(Rect* r);
		ft get_width();
		ft get_height();
		void set_is_layer(st val);
		st get_is_layer();
		void set_alpha(ft val);
		ft get_alpha();

		void update();
		void show();
		void hide();
		void close();
		void set_focus();

		void set_dispatcher(void* dispatcher);
	public:
		static void set_application(HINSTANCE ins);
		static void loop();
		static void exit();
	};
}