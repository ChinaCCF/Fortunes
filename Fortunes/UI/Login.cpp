#include <UI/Login.h>
#include "../Resource/Resource.h"
#include <CL/Memory.h>
#include <WL/WindowControls/Button.h>
#include <WL/WindowControls/Image.h>
#include <WL/WindowControls/TextInput.h>
#include <WL/Graphics/Render.h>

namespace Fortunes
{
	class Input :public WL::BaseWindow
	{
	public:
		WL::SubWindow::TextInput* input;
		WL::SubWindow::Image* img;

		Input() { input = NULL; }
		~Input() { 
			cl_delete(img);
			cl_delete(input);
		}

		st init(const char* file, st is_pwd)
		{
			if(!BaseWindow::init())return FALSE;
			img = cl_new(WL::SubWindow::Image);
			if(!img->init()) return FALSE;
			add_window(img);
			img->set_file(file);
			img->show();

			input = cl_new(WL::SubWindow::TextInput);
			if(!input->init()) return FALSE;
			if(is_pwd) input->set_is_password(TRUE);
			add_window(input);
			WL::Font font;
			font.init();
			font.set_is_bold(FALSE);
			font.set_size(17);
			input->set_font(&font);

			input->show();
			return TRUE;
		}

		void set_frame(ft x, ft y, ft w, ft h)
		{
			BaseWindow::set_frame(x, y, w, h);
			img->set_frame(h * 0.3, h * 0.15, h * 0.7, h * 0.7);
			input->set_frame(h * 1.3, h * 0.15, w - h * 1.6, h * 0.7);
		}
		void redraw(WL::IRender* render, WL::Rect* r)
		{
			BaseWindow::redraw(render, r);
		}
	};

	class SelectBtn : public WL::SubWindow::BaseButton
	{
		st is_select;
	public:
		SelectBtn() { is_select = NULL; }
		void set_select(st val) { is_select = val; }
		virtual void redraw(WL::IRender* render, WL::Rect* r)
		{
			WL::Color c;
			get_background_color(&c);
			render->set_color(&c);
			render->set_pen_width(1);
			WL::Rect t = *r;
			t.shrink(1);
			render->draw_rect(&t);
			if(is_select)
			{
				t.shrink(2);
				render->fill_rect(&t);
			}
		}
	};

	class CloseButton : public WL::SubWindow::BaseButton
	{
	public:
		virtual void redraw(WL::IRender* render, WL::Rect* r)
		{
			WL::Color c;
			get_background_color(&c);
			render->set_color(&c);
			WL::Rect re;
			re = *r;
			re.shrink(1);
			render->fill_ellipse(&re);

			if(get_status() != BaseButton::Normal)
			{
				WL::Color c;
				c.set(250, 250, 250);
				render->set_color(&c);
				render->set_pen_width(3);

				WL::Point center;
				center.set(r->w / 2, r->h / 2);
				ft len = CL::MIN(re.w, re.h);
				len /= 4;

				WL::Point p1, p2;
				p1 = center;
				p2 = center;

				p1.x -= len;
				p1.y -= len;

				p2.x += len;
				p2.y += len;

				render->draw_line(&p1, &p2);

				p1 = center;
				p2 = center;
				p1.x -= len;
				p1.y += len;

				p2.x += len;
				p2.y -= len;
				render->draw_line(&p1, &p2);
			}
		}
	};

	class _Login
	{
	public:
		WL::SubWindow::Image* img;
		Input* user;
		Input* pwd;
		Input* host;
		SelectBtn* remember;
		WL::SubWindow::Label* lbl;
		WL::SubWindow::Button* btn_login;
		CloseButton* close;
		_Login()
		{
			img = NULL;
			btn_login = NULL;
			close = NULL;
			lbl = NULL;
			remember = NULL;
			host = NULL;
			pwd = NULL;
			user = NULL;
		}
	};
	Login::~Login()
	{
		if(self)
		{
			cl_delete(self->img);
			cl_delete(self->user);
			cl_delete(self->pwd);
			cl_delete(self->host);
			cl_delete(self->remember);
			cl_delete(self->lbl);
			cl_delete(self->btn_login);
			cl_delete(self->close);
		}
		cl_delete(self);
	}
	void login_click(WL::SubWindow::BaseButton* btn, void* extra)
	{
		cl_printf("login\n");
	}

	void remember_click(WL::SubWindow::BaseButton* btn, void* extra)
	{
		cl_printf("login\n");
		//SelectBtn* 
	}
	void close_click(WL::SubWindow::BaseButton* btn, void* extra)
	{
		PostQuitMessage(0);
	}
	st Login::init()
	{
		if(!WL::Window::init()) return FALSE;
		self = cl_new(_Login);
		if(self == NULL) return FALSE;

		set_background_color(239, 236, 255);
		set_frame(0, 0, 700, 400);
		set_title("î£ÔÆ");

		self->img = cl_new(WL::SubWindow::Image);
		if(!self->img->init()) return FALSE;
		add_window(self->img);
		self->img->set_frame(50, 70, 200, 260);
		self->img->set_file("Resource\\login_side.png");
		self->img->show();

		self->user = cl_new(Input);
		if(!self->user->init("Resource\\user.png", FALSE)) return FALSE;
		add_window(self->user);
		self->user->set_frame(400, 80, 200, 27);
		self->user->show();

		self->pwd = cl_new(Input);
		if(!self->pwd->init("Resource\\pwd.png", TRUE)) return FALSE;
		add_window(self->pwd);
		self->pwd->set_frame(400, 130, 200, 27);
		self->pwd->show();

		self->host = cl_new(Input);
		if(!self->host->init("Resource\\host.png", FALSE)) return FALSE;
		add_window(self->host);
		self->host->set_frame(400, 180, 200, 27);
		self->host->show();

		self->lbl = cl_new(WL::SubWindow::Label);
		if(!self->lbl->init()) return FALSE;
		add_window(self->lbl);
		WL::Color c;
		c.set(0, 0, 0);
		self->lbl->set_text_color(&c);
		WL::Font f;
		f.init();
		f.set_size(10);
		self->lbl->set_font(&f);
		self->lbl->set_text("¼Ç×¡ÃÜÂë");
		self->lbl->set_frame(400, 222, 200, 27);
		self->lbl->set_horizontal_align(1);
		self->lbl->show();

		self->remember = cl_new(SelectBtn);
		if(!self->remember->init()) return FALSE;
		add_window(self->remember);
		self->remember->set_background_color(0, 111, 222);
		self->remember->set_frame(525, 228, 15, 15);
		self->remember->show();
		self->remember->set_click(remember_click, this);

		self->btn_login = cl_new(WL::SubWindow::Button);
		if(!self->btn_login->init()) return FALSE;
		add_window(self->btn_login);
		self->btn_login->set_background_color(0, 111, 222);
		self->btn_login->set_frame(400, 260, 200, 30);
		self->btn_login->get_title()->set_text("µÇÂ¼");
		WL::Font font;
		font.init();
		font.set_size(13);
		self->btn_login->get_title()->set_font(&font);
		self->btn_login->show();
		self->btn_login->set_click(login_click, this);


		self->close = cl_new(CloseButton);
		if(!self->close->init()) return FALSE;
		add_window(self->close);
		self->close->set_background_color(200, 50, 50);
		self->close->set_frame(700 - 26, 1, 25, 25);
		self->close->show();
		self->close->set_click(close_click, this);

		show();
		return TRUE;
	}



	void Login::redraw(WL::IRender* render, WL::Rect* r)
	{
		BaseWindow::redraw(render, r);
		render->set_color(186, 186, 186);
		WL::Rect line;
		line.set(300, 50, 0.2, 300);
		render->draw_rect(&line);
	}
}