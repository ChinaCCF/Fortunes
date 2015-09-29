#include <UI/Login.h>
#include "../Resource/Resource.h"
#include <CL/Memory.h>
#include <WL/WindowControls/Button.h>
#include <WL/WindowControls/Image.h>
#include <WL/WindowControls/TextInput.h>
#include <WL/Graphics/Render.h>
#include <UI/Common.h>
#include <CL/IO/File.h>
#include <FNET/Model.h>
#include <UI/Main.h>
#include <WL/WindowControls/ScrollView.h>

namespace Fortunes
{
	static st g_is_loging = 0;
	static st text_filter(WL::SubWindow::TextInput* txt, WL::KeyBoardEvent* e, void* extra);

	class Input :public WL::BaseWindow
	{
	public:
		WL::SubWindow::TextInput* input;
		WL::SubWindow::Image* img;

		Input() { input = NULL; }
		~Input()
		{
			cl_delete(img);
			cl_delete(input);
		}

		st init(const char* file, st is_pwd, _Login* l)
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
			input->set_filter(text_filter, l);
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
		void set_focus() { input->set_focus(); }
		void set_text(const char* text) { input->set_text(text); }
		void redraw(WL::IRender* render)
		{
			BaseWindow::redraw(render);
		}
	};

	class SelectBtn : public WL::SubWindow::BaseButton
	{
		st is_select;
	public:
		SelectBtn() { is_select = NULL; }
		void set_select(st val) { is_select = val; update(); }
		st get_select() { return is_select; }

		virtual void redraw(WL::IRender* render)
		{
			WL::Color c;
			WL::Rect r;

			get_frame(&r);
			r.x = 0;
			r.y = 0;

			c.set(239, 236, 255);
			render->set_color(&c);
			render->fill_rect(&r);

			get_background_color(&c);
			render->set_color(&c);
			render->set_pen_width(1);

			r.shrink(1);
			render->draw_rect(&r);
			if(is_select)
			{
				r.shrink(2);
				render->fill_rect(&r);
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
		WL::SubWindow::Button* btn_logout;
		WL::SubWindow::Label* err;
		CloseButton* close;
		cl::File* file;
		Login* self;
		_Login()
		{
			img = NULL;
			btn_login = NULL;
			btn_logout = NULL;
			close = NULL;
			lbl = NULL;
			remember = NULL;
			host = NULL;
			pwd = NULL;
			user = NULL;
			file = NULL;
			err = NULL;
			self = NULL;
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
			cl_delete(self->btn_logout);
			cl_delete(self->err);
			cl::File::close_file(self->file);
		}
		cl_delete(self);
	}
	static void login_click(WL::SubWindow::BaseButton* btn, void* extra)
	{
		_Login* self = (_Login*)extra;
		g_is_loging = TRUE;
		st ret = NetUtil::login(self->host->input->get_text(), self->user->input->get_text(), self->pwd->input->get_text());
		g_is_loging = FALSE;
		if(ret)
		{
			self->err->hide();

			Fortunes::Main* main = cl_new(Fortunes::Main);
			main->init();

			cl_delete(self->self);
		}
		else
		{
			self->err->show();
		}
	}

	static void remember_click(WL::SubWindow::BaseButton* btn, void* extra)
	{
		if(!g_is_loging)
		{
			SelectBtn* sel = (SelectBtn*)btn;
			sel->set_select(!sel->get_select());
		}
	}
	static void close_click(WL::SubWindow::BaseButton* btn, void* extra)
	{
		PostQuitMessage(0);
	}
	static st text_filter(WL::SubWindow::TextInput* txt, WL::KeyBoardEvent* e, void* extra)
	{
		if(g_is_loging) return TRUE;
		_Login* l = (_Login*)extra;
		if(e->get_char() == '\t')
		{
			if(txt == l->user->input)
				l->pwd->set_focus();
			else
			{
				if(txt == l->pwd->input)
					l->host->set_focus();
				else
				{
					l->user->set_focus();
				}
			}
		}
		if(e->get_char() == '\n' || e->get_char() == '\r')
		{
			login_click(NULL, l);
		}
		return FALSE;
	}


	st Login::init()
	{
		if(!WL::Window::init()) return FALSE;
		self = cl_new(_Login);
		if(self == NULL) return FALSE;
		self->self = this;

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
		if(!self->user->init("Resource\\user.png", FALSE, self)) return FALSE;
		add_window(self->user);
		self->user->set_frame(400, 80, 200, 27);
		self->user->show();

		self->pwd = cl_new(Input);
		if(!self->pwd->init("Resource\\pwd.png", TRUE, self)) return FALSE;
		add_window(self->pwd);
		self->pwd->set_frame(400, 130, 200, 27);
		self->pwd->show();

		self->host = cl_new(Input);
		if(!self->host->init("Resource\\host.png", FALSE, self)) return FALSE;
		add_window(self->host);
		self->host->set_frame(400, 180, 200, 27);
		self->host->show();

		WL::Font f;
		f.init();
		f.set_size(10);

		WL::Color c;
		c.set(0, 0, 0);

		self->lbl = cl_new(WL::SubWindow::Label);
		if(!self->lbl->init()) return FALSE;
		add_window(self->lbl);
		self->lbl->set_text_color(&c);
		self->lbl->set_font(&f);
		self->lbl->set_text("¼Ç×¡ÃÜÂë");
		self->lbl->set_frame(600 - 60, 222, 60, 27);
		self->lbl->set_horizontal_align(1);
		self->lbl->show();

		self->remember = cl_new(SelectBtn);
		if(!self->remember->init()) return FALSE;
		add_window(self->remember);
		self->remember->set_background_color(0, 111, 222);
		self->remember->set_frame(525, 228, 15, 15);
		self->remember->show();
		self->remember->set_click(remember_click, self);

		WL::Font font;
		font.init();
		font.set_size(13);

		self->btn_login = cl_new(WL::SubWindow::Button);
		if(!self->btn_login->init()) return FALSE;
		add_window(self->btn_login);
		self->btn_login->set_background_color(0, 111, 222);
		self->btn_login->set_frame(400, 260, 200, 30);
		self->btn_login->get_title()->set_text("µÇÂ¼");
		self->btn_login->get_title()->set_font(&font);
		self->btn_login->show();
		self->btn_login->set_click(login_click, self);

		//self->btn_logout = cl_new(WL::SubWindow::Button);
		//if(!self->btn_logout->init()) return FALSE;
		//self->btn_login->add_window(self->btn_logout);
		//self->btn_logout->set_background_color(100, 0, 222);
		//self->btn_logout->set_frame(50, 0, 200, 30);
		//self->btn_logout->get_title()->set_text("ÍË³ö");
		//self->btn_logout->get_title()->set_font(&font);
		//self->btn_logout->show();
		//self->btn_logout->set_click(close_click, self);

		self->close = cl_new(CloseButton);
		if(!self->close->init()) return FALSE;
		add_window(self->close);
		self->close->set_background_color(200, 50, 50);
		self->close->set_frame(700 - 21, 1, 20, 20);
		self->close->show();
		self->close->set_click(close_click, NULL);

		c.set(255, 0, 0);
		self->err = cl_new(WL::SubWindow::Label);
		if(!self->err->init()) return FALSE;
		add_window(self->err);
		self->err->set_text_color(&c);
		self->err->set_font(&f);
		self->err->set_text("µÇÂ¼Ê§°Ü");
		self->err->set_frame(600 - 60, 350, 60, 27);
		self->err->set_horizontal_align(1);
		//self->err->show();

		self->file = cl::File::open_file("info");
		if(self->file == NULL) return FALSE;

		char* str_remember = self->file->read_line();
		char* str_user = self->file->read_line();
		char* str_pwd = self->file->read_line();
		char* str_host = self->file->read_line();

		if(cl::StringUtil::compare(str_remember, "true"))
		{
			self->remember->set_select(TRUE);
			self->user->set_text(str_user);
			self->pwd->set_text(str_pwd);
			self->host->set_text(str_host);
		}
		cl_free(str_user);
		cl_free(str_pwd);
		cl_free(str_host);
		cl_free(str_remember);

		show();
		return TRUE;
	}

	void Login::redraw(WL::IRender* render)
	{
		BaseWindow::redraw(render);
		render->set_color(186, 186, 186);
		WL::Rect line;
		line.set(300, 50, 1, 300);
		render->fill_rect(&line);
	}
}