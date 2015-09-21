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
		st need_redraw_bg;
		Window* parent;
		_BaseWindow() { win32 = NULL;  parent = NULL; }
	};

	BaseWindow::BaseWindow() { self = NULL; }
	BaseWindow::~BaseWindow() { if(self) cl_delete(self->win32); cl_delete(self); }

	st BaseWindow::init()
	{
		self = cl_new(_BaseWindow);
		if(self == NULL) return FALSE;

		self->win32 = cl_new(Window32);
		if(self->win32 == NULL) return FALSE;
		if(!self->win32->init()) return FALSE;
		self->win32->set_dispatcher(this);
		self->need_redraw_bg = TRUE;
		return TRUE;
	}

	void BaseWindow::set_frame(const Rect* r) { self->win32->set_frame(r); update(); }
	void BaseWindow::set_frame(st x, st y, st w, st h) { self->win32->set_frame(x, y, w, h); update(); }
	void BaseWindow::get_frame(Rect* r) { self->win32->get_frame(r); }

	void BaseWindow::set_background_color(const Color* c)
	{
		if(self->background != *c)
		{
			self->background = *c;
			self->need_redraw_bg = TRUE;
			update();
		}
	}
	void BaseWindow::set_background_color(st r, st g, st b)
	{
		if(self->background != Color(r, g, b))
		{
			self->background.set(r, g, b);
			self->need_redraw_bg = TRUE;
			update();
		}
	}
	void BaseWindow::get_background_color(Color* c) { *c = self->background; }

	void BaseWindow::set_alpha(ft val) { self->win32->set_alpha(val); update(); }
	ft BaseWindow::get_alpha() { return self->win32->get_alpha(); }

	void BaseWindow::show() { self->win32->show(); }
	void BaseWindow::hide() { self->win32->hide(); }
	void BaseWindow::update() { self->win32->update(); }

	void BaseWindow::redraw(IRender* render, Rect* r)
	{
		if(self->need_redraw_bg)
		{
			self->need_redraw_bg = FALSE;
			Rect sr;
			self->win32->get_frame(&sr);
			render->set_color(&self->background);
			sr.x = 0;
			sr.y = 0;
			if(*r == sr)
				render->fill_rect(-1, -1, sr.w + 2, sr.h + 2);
			else
				render->fill_rect(r);
		}
	}
	void BaseWindow::remove_from_parent() { if(self->parent) self->parent->remove_window(this); }
	/**********************************************************************************/
	/**********************************************************************************/

	st Window::init()
	{
		if(!BaseWindow::init()) return FALSE;
		return TRUE;
	}
	void Window::set_title(const char* title) { BaseWindow::self->win32->set_title(title); }
	const char* Window::get_title() { return BaseWindow::self->win32->get_title(); }

	void Window::update() { BaseWindow::self->win32->update(); }

	st Window::event_for_touch(TouchEvent* e)
	{
		if(e->get_type() != WL::TouchEvent::Move)
		{
			char buf[32];
			const char* action = "down";
			if(e->get_type() == WL::TouchEvent::Up) action = "up";
			else
			{
				if(e->get_type() == WL::TouchEvent::Enter) action = "enter";
				else action = "leave";
			}
			CL::StringUtil::format(buf, 32, "%d, %d   %s\n", e->get_points()[0].x, e->get_points()[0].y, action);
			cl_printf(buf);
		}

		if(e->get_type() == WL::TouchEvent::Down)
			SendMessage(BaseWindow::self->win32->get_handle(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		return TRUE;
	}

	void Window::add_window(const BaseWindow* win)
	{
		win->self->win32->set_parent(BaseWindow::self->win32->get_handle());
		win->self->parent = this;
	}
	void Window::remove_window(const BaseWindow* win)
	{
		win->self->win32->set_parent(NULL);
		win->self->parent = NULL;
	}
}