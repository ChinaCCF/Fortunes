#include <UI/Login.h>
#include "../Resource/Resource.h"
#include <CL/Memory.h>
#include <WL/WindowControls/Button.h>
#include <WL/WindowControls/Image.h>
#include <WL/WindowControls/TextInput.h>

namespace Fortunes
{
	class _Login
	{
	public:
		WL::SubWindow::Button* btn_login;
		WL::SubWindow::Image* img_login;
		WL::SubWindow::TextInput* text_login;

		_Login()
		{
			btn_login = NULL;
		}
	};

	st Login::init()
	{
		if(!WL::Window::init()) return FALSE;
		self = cl_new(_Login);
		if(self == NULL) return FALSE;

		set_background_color(255, 0, 0);
		set_frame(0, 0, 800, 400);
		set_title("î£ÔÆ");

		/*	self->btn_login = cl_new(WL::SubWindow::Button);
		if(!self->btn_login->init()) return FALSE;

		add_window(self->btn_login);
		self->btn_login->set_background_color(0,0,255);
		self->btn_login->set_frame(20,20,50,50);
		self->btn_login->get_title()->set_text("ccf");
		self->btn_login->show();

		self->img_login = cl_new(WL::SubWindow::Image);
		if(!self->img_login->init()) return FALSE;

		add_window(self->img_login);
		self->img_login->set_frame(0, 100, 100, 100);
		self->img_login->set_file("C:\\1.png");
		self->img_login->show();*/

		self->text_login = cl_new(WL::SubWindow::TextInput);
		if(!self->text_login->init()) return FALSE;

		add_window(self->text_login);
		self->text_login->set_frame(100, 100, 100, 100);
		//self->text_login->set_is_password(TRUE);
		self->text_login->set_text("ccf");
		self->text_login->show();

		show();
		return TRUE;
	}
}