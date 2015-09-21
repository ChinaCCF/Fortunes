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
		st init();

		virtual void set_frame(const Rect* r);
		virtual void set_frame(st x, st y, st w, st h);
		virtual void get_frame(Rect* r);

		virtual void set_background_color(const Color* c);
		virtual void set_background_color(st r, st g, st b);
		virtual void get_background_color(Color* c);

		virtual void set_alpha(ft val);
		virtual ft get_alpha();

		virtual void show();
		virtual void hide();
		virtual void update();
		virtual void redraw(IRender* render, Rect* r);

		virtual void remove_from_parent();
	};

	class Window : public BaseWindow
	{
	public:
		st init();

		virtual void update();

		virtual st event_for_touch(TouchEvent* e);

		void set_title(const char* title);
		const char* get_title();

		void add_window(const BaseWindow* win);
		void remove_window(const BaseWindow* win);
	};
}