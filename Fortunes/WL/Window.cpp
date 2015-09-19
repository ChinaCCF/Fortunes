#include <WL/Window.h>
#include <WL/Window32.h>
#include <CL/Memory.h>
#include <CL/Object/String.h>

namespace WL
{

	class _Window
	{
	public:
		Window32* win32_;

		_Window()
		{
			win32_ = NULL;
		}
	};

	/**********************************************************************************/
	/**********************************************************************************/
	Window::Window()
	{
		self = NULL;
	}
	Window::~Window()
	{
		if(self)
		{
			cl_delete(self->win32_);
		}

		cl_delete(self);
	}
	st Window::init()
	{
		self = cl_new(_Window);
		if(self == NULL) return FALSE;

		self->win32_ = cl_new(Window32);
		if(self->win32_ == NULL) return FALSE;
		if(!self->win32_->init()) return FALSE;
		self->win32_->set_title("title");
		self->win32_->show();
		self->win32_->set_response(this);
		return TRUE;
	}


	st Window::event_for_keyboard(KeyBoardEvent* e)
	{
		return TRUE;
	}

	st Window::event_for_touch(TouchEvent* e)
	{
		if(e->get_type() != WL::TouchEvent::Move)
		{
			char buf[32];
			const char* action = "down";
			if(e->get_type() == WL::TouchEvent::Up) action = "up";
			CL::StringUtil::format(buf, 32, "%d, %d   %s\n", e->get_points()[0].x, e->get_points()[0].y, action);
			cl_printf(buf);
		}
		
		if(e->get_type() == WL::TouchEvent::Down)
			SendMessage(self->win32_->get_handle(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		return TRUE;
	}
}