#include <WL/Window.h>
#include <WL/Window32.h>
#include <CL/Memory.h>
#include <CL/Object/String.h>

namespace WL
{

	/**********************************************************************************/
	/**********************************************************************************/
	class _BaseWindow
	{
	public:
		Window32* win32;
		Color background;
		BaseWindow* parent;
		st horizontal;
		st vertical;
	};

	st BaseWindow::init()
	{
		self = cl_alloc_type(_BaseWindow);
		if(self == NULL) return FALSE;

		self->win32 = cl_new_init(Window32);
		if(self->win32 == NULL) return FALSE;

		self->win32->set_dispatcher(this);
		return TRUE;
	}

	BaseWindow::~BaseWindow()
	{
		if(self)
			cl_delete(self->win32);
		cl_free(self);
	}
	st BaseWindow::create(const char* class_name, st style, st ex_style)
	{
		return self->win32->create(class_name, style, ex_style);
	}

	//IView
	void BaseWindow::set_frame(const Rect* r) { self->win32->set_frame(r); update(); }
	void BaseWindow::set_frame(ft x, ft y, ft w, ft h) { self->win32->set_frame(x, y, w, h); update(); }
	void BaseWindow::get_frame(Rect* r) { self->win32->get_frame(r); }
	ft BaseWindow::get_width() { return self->win32->get_width(); }
	ft BaseWindow::get_height() { return self->win32->get_height(); }

	void BaseWindow::set_background_color(const Color* c)
	{
		if(self->background != *c)
		{
			self->background = *c;
			update();
		}
	}
	void BaseWindow::set_background_color(st r, st g, st b)
	{
		if(self->background != Color(r, g, b))
		{
			self->background.set(r, g, b);
			update();
		}
	}
	void BaseWindow::set_background_color(ft a, st r, st g, st b)
	{
		if(self->background != Color(a, r, g, b))
		{
			self->background.set(a, r, g, b);
			update();
		}
	}
	void BaseWindow::get_background_color(Color* c) { *c = self->background; }

	void BaseWindow::set_alpha(ft val) { self->win32->set_alpha(val); update(); }
	ft BaseWindow::get_alpha() { return self->win32->get_alpha(); }

	void BaseWindow::show() { self->win32->show(); }
	void BaseWindow::hide() { self->win32->hide(); }
	void BaseWindow::update() { self->win32->update(); }

	void BaseWindow::redraw(IRender* render)
	{
		render->set_color(&self->background);
		render->fill_rect(-2, -2, get_width() + 4, get_height() + 4);
	}
	void BaseWindow::remove_from_parent() { if(self->parent) self->parent->remove_window(this); }
	//end of IView

	//IResponse
	st BaseWindow::event_for_touch(TouchEvent* e)
	{
		if(self->parent) return self->parent->event_for_touch(e);
		return FALSE;
	}
	st BaseWindow::event_for_wheel(st direction)
	{
		if(self->parent) return self->parent->event_for_wheel(direction);
		return FALSE;
	}
	//end of IResponse

	HWND BaseWindow::get_handle() { return self->win32->get_handle(); }
	void BaseWindow::set_focus() { self->win32->set_focus(); }

	void BaseWindow::set_horizontal_align(st val) { self->horizontal = val; }
	void BaseWindow::set_vertical_align(st val) { self->vertical = val; }

	st BaseWindow::get_horizontal_align() { return self->horizontal; }
	st BaseWindow::get_vertical_align() { return self->vertical; }

	void BaseWindow::add_window(const BaseWindow* win)
	{
		win->self->win32->set_parent(BaseWindow::self->win32->get_handle());
		win->self->parent = this;
	}
	void BaseWindow::remove_window(const BaseWindow* win)
	{
		win->self->win32->set_parent(NULL);
		win->self->parent = NULL;
	}
	/**********************************************************************************/
	/**********************************************************************************/

	void Window::set_title(const char* title) { BaseWindow::self->win32->set_title(title); }
	const char* Window::get_title() { return BaseWindow::self->win32->get_title(); }

	st Window::event_for_touch(TouchEvent* e)
	{
		if(e->get_type() == WL::TouchEvent::Down)
			SendMessage(BaseWindow::self->win32->get_handle(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		return TRUE;
	}
}