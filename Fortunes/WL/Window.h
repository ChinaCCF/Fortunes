#include <CL/CL.h>
#include <WL/Event.h> 
#include <WL/View.h>

#pragma once

namespace WL
{
	class Window;
	class _BaseWindow;
	class BaseWindow : public IResponse, public IView
	{
	protected:
		friend class Window;
		_BaseWindow* self;
	public:
		BaseWindow();
		~BaseWindow();
		st init(const char* class_name = NULL, st style = 0);

		virtual void set_frame(const Rect* r);
		virtual void set_frame(ft x, ft y, ft w, ft h);
		virtual void get_frame(Rect* r);

		virtual void set_background_color(const Color* c);
		virtual void set_background_color(st r, st g, st b);
		virtual void get_background_color(Color* c);

		virtual void set_alpha(ft val);
		virtual ft get_alpha();

		void set_horizontal_align(st val);
		void set_vertical_align(st val);

		st get_horizontal_align();
		st get_vertical_align();

		virtual void set_focus();
		virtual void show();
		virtual void hide();
		virtual void update();
		virtual void redraw(IRender* render, Rect* r);

		virtual void remove_from_parent();
		virtual st event_for_touch(TouchEvent* e);

		HWND get_handle();
		void add_window(const BaseWindow* win);
		void remove_window(const BaseWindow* win);
	};

	class Window : public BaseWindow
	{
	public:
		st init();

		virtual void update();

		virtual st event_for_touch(TouchEvent* e);

		void set_title(const char* title);
		const char* get_title();
	};
}