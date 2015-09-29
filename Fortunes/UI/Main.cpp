#include <UI/Main.h>
#include "../Resource/Resource.h"
#include <CL/Memory.h>
#include <WL/WindowControls/Button.h>
#include <WL/WindowControls/Image.h>
#include <WL/WindowControls/TextInput.h>
#include <WL/Graphics/Render.h>
#include <UI/Common.h>
#include <FNET/Model.h>
#include <WL/WindowControls/ScrollView.h>

namespace Fortunes
{
	WL::SubWindow::Label* create_item_label(WL::BaseWindow* win)
	{
		WL::Font f;
		f.init();
		f.set_size(11);
		WL::Color c;
		c.set(0, 0, 0);

		WL::SubWindow::Label* lbl = cl_new(WL::SubWindow::Label);
		if(lbl == NULL) return NULL;
		if(!lbl->init()) return NULL;

		lbl->set_text_color(&c);
		lbl->set_font(&f);
		win->add_window(lbl);
		lbl->show();
		return lbl;
	}

	static void item_select_click(WL::SubWindow::BaseButton* btn, void* extra);
	class ListItem : public WL::SubWindow::BaseButton
	{
	public:
		st is_select;
		WL::SubWindow::Label* name;
		WL::SubWindow::Label* status;
		WL::SubWindow::Label* cpu;
		WL::SubWindow::Label* memory;
		WL::SubWindow::Label* system;
		ListItem* next;
		VMInfo* info;

		st init()
		{
			if(!BaseButton::init())return FALSE;
			name = create_item_label(this);
			status = create_item_label(this);
			cpu = create_item_label(this);
			memory = create_item_label(this);
			system = create_item_label(this);
			next = NULL;
			is_select = FALSE;
			show();
			return TRUE;
		}
		void uninit()
		{
			cl_delete(name);
			cl_delete(status);
			cl_delete(cpu);
			cl_delete(memory);
			cl_delete(system);
		}
		void set_frame(ft x, ft y, ft _w, ft h)
		{
			ft w = _w / 5;
			name->set_frame(0, 0, w, h);
			status->set_frame(w, 0, w, h);
			cpu->set_frame(w * 2, 0, w, h);
			memory->set_frame(w * 3, 0, w, h);
			system->set_frame(w * 4, 0, w, h);

			BaseWindow::set_frame(x, y, _w, h);
		}
		void set_Data(VMInfo* _info)
		{
			info = _info;
			name->set_text(info->name);
			status->set_text(info->status == VMInfo::Running ? "运行中" : (info->status == VMInfo::Stop ? "已关机" : "操作中"));
			cpu->set_text(info->cpu);
			memory->set_text(info->mem);
			system->set_text(info->sys);
		}
		st get_is_select() { return is_select; }
		void set_is_select(st val)
		{
			is_select = val;
			WL::Color c;
			if(is_select)
				c.set(255, 255, 255);
			else
				c.set(0, 0, 0);

			name->set_text_color(&c);
			status->set_text_color(&c);
			cpu->set_text_color(&c);
			memory->set_text_color(&c);
			system->set_text_color(&c);
			update();
		}
		void redraw(WL::IRender* render)
		{
			WL::Color c;
			if(is_select)
				c.set(0, 111, 222);
			else
				c.set(239, 236, 255);

			set_background_color(&c);
			BaseWindow::redraw(render);
		}
	};
	static ListItem* g_item = NULL;
	static void item_select_click(WL::SubWindow::BaseButton* btn, void* extra)
	{
		ListItem* item = (ListItem*)extra;
		if(item != g_item)
		{
			if(g_item)
				g_item->set_is_select(FALSE);
			
			item->set_is_select(TRUE);
			g_item = item;
		}
		else
			item->set_is_select(!item->get_is_select());
	}

	class _Main
	{
	public:
		WL::SubWindow::Label* name;
		WL::SubWindow::Label* status;
		WL::SubWindow::Label* cpu;
		WL::SubWindow::Label* memory;
		WL::SubWindow::Label* system;

		WL::SubWindow::Button* connect;
		WL::SubWindow::Button* close;
		WL::SubWindow::Button* force_close;
		WL::SubWindow::Button* refresh;

		WL::SubWindow::ScrollView* list;
		WL::SubWindow::Label* err;
		VMInfo* infos;
		st loading;
		ListItem* items;
		CloseButton* quit;
		_Main()
		{
			name = NULL;
			status = NULL;
			cpu = NULL;
			memory = NULL;
			system = NULL;

			connect = NULL;
			close = NULL;
			force_close = NULL;
			refresh = NULL;
			list = NULL;
			err = NULL;
			items = NULL;
			infos = NULL;

		}
	};

	Main::~Main()
	{
		if(self)
		{
			cl_delete(self->name);
			cl_delete(self->status);
			cl_delete(self->cpu);
			cl_delete(self->memory);
			cl_delete(self->system);


			cl_delete(self->connect);
			cl_delete(self->close);
			cl_delete(self->force_close);
			cl_delete(self->refresh);
			cl_delete(self->quit);
			self->list->uninit();
			cl_delete(self->list);
			cl_delete(self->err);

			ListItem* item = self->items;
			while(item)
			{
				ListItem* i = item;
				item = item->next;
				i->uninit();
				cl_delete(i);
			}
			VMInfo* info = self->infos;
			while(info)
			{
				info->uninit();
				info = info->next;
			}
			cl_free(self->infos);
		}
		cl_delete(self);
	}
	static void connect_click(WL::SubWindow::BaseButton* btn, void* extra)
	{}

	static void close_click(WL::SubWindow::BaseButton* btn, void* extra)
	{}
	static void force_close_click(WL::SubWindow::BaseButton* btn, void* extra)
	{}
	static void quit_click(WL::SubWindow::BaseButton* btn, void* extra)
	{
		Main* m = (Main*)extra;
		cl_delete(m);
		PostQuitMessage(0);
	}
	static void refresh_click(WL::SubWindow::BaseButton* btn, void* extra)
	{
		_Main* self = (_Main*)extra;
		g_item = NULL;
		self->loading = TRUE;
		self->err->show();

		{//create thread
			VMInfo* info = self->infos;
			while(info)
			{
				info->uninit();
				info = info->next;
			}
			cl_free(self->infos);

			ListItem* item = self->items;
			while(item)
			{
				ListItem* i = item;
				item = item->next;
				i->uninit();
				cl_delete(i);
			}
			self->items = NULL;

			self->infos = NetUtil::get_list();
			ft h = 40;
			st index = 0;
			info = self->infos;
			while(info)
			{
				item = cl_alloc_init(ListItem);
				item->set_click(item_select_click, item);
				item->set_frame(0, index * h, 700, h);
				item->set_Data(info);
				self->list->add_window(item);
				item->next = self->items;
				self->items = item;
				item->show();
				info = info->next;
				++index;
			}
			WL::Size size;
			size.set(0, index * h);
			self->list->set_content_size(&size);
		}

		self->loading = FALSE;
		self->err->hide();
	}
	WL::SubWindow::Label* create_label(const char* txt, ft x, ft y, ft w, ft h)
	{
		WL::Color c;
		c.set(0, 0, 0);
		WL::Font f;
		f.init();
		//f.set_is_bold(TRUE);
		f.set_size(14);

		WL::SubWindow::Label* lbl = cl_new(WL::SubWindow::Label);
		if(lbl == NULL) return NULL;
		if(!lbl->init()) return NULL;

		lbl->set_text_color(&c);
		lbl->set_font(&f);
		lbl->set_text(txt);
		lbl->set_frame(x, y, w, h);
		return lbl;
	}
	st Main::init()
	{
		if(!WL::Window::init()) return FALSE;
		self = cl_new(_Main);
		if(self == NULL) return FALSE;

		set_background_color(239, 236, 255);
		set_frame(0, 0, 700, 400);
		set_title("睿云");

		ft w = 700.0 / 5;
		self->name = create_label("名称", 0, 0, w, 50);
		add_window(self->name);
		self->name->show();

		self->status = create_label("状态", w, 0, w, 50);
		add_window(self->status);
		self->status->show();

		self->cpu = create_label("处理器", w * 2, 0, w, 50);
		add_window(self->cpu);
		self->cpu->show();

		self->memory = create_label("内存", w * 3, 0, w, 50);
		add_window(self->memory);
		self->memory->show();

		self->system = create_label("系统", w * 4, 0, w, 50);
		add_window(self->system);
		self->system->show();

		WL::Font font;
		font.init();
		font.set_size(13);
		ft space = 50;
		w = (700 - 4 * 50) / 4;

		self->connect = cl_new(WL::SubWindow::Button);
		if(!self->connect->init()) return FALSE;
		add_window(self->connect);
		self->connect->set_background_color(0, 111, 222);
		self->connect->set_frame(space / 2, 400 - 40, w, 30);
		self->connect->get_title()->set_text("连接");
		self->connect->get_title()->set_font(&font);
		self->connect->show();
		//self->connect->set_click(login_click, this);

		self->close = cl_new(WL::SubWindow::Button);
		if(!self->close->init()) return FALSE;
		add_window(self->close);
		self->close->set_background_color(0, 111, 222);
		self->close->set_frame(space / 2 + w + space, 400 - 40, w, 30);
		self->close->get_title()->set_text("关闭");
		self->close->get_title()->set_font(&font);
		self->close->show();

		self->force_close = cl_new(WL::SubWindow::Button);
		if(!self->force_close->init()) return FALSE;
		add_window(self->force_close);
		self->force_close->set_background_color(0, 111, 222);
		self->force_close->set_frame(space / 2 + w * 2 + space * 2, 400 - 40, w, 30);
		self->force_close->get_title()->set_text("强关");
		self->force_close->get_title()->set_font(&font);
		self->force_close->show();

		self->refresh = cl_new(WL::SubWindow::Button);
		if(!self->refresh->init()) return FALSE;
		add_window(self->refresh);
		self->refresh->set_background_color(0, 111, 222);
		self->refresh->set_frame(space / 2 + w * 3 + space * 3, 400 - 40, w, 30);
		self->refresh->get_title()->set_text("刷新");
		self->refresh->get_title()->set_font(&font);
		self->refresh->set_click(refresh_click, self);
		self->refresh->show();


		self->quit = cl_new(CloseButton);
		if(!self->quit->init()) return FALSE;
		add_window(self->quit);
		self->quit->set_background_color(200, 50, 50);
		self->quit->set_frame(700 - 21, 1, 20, 20);
		self->quit->show();
		self->quit->set_click(quit_click, this);

		self->list = cl_alloc_init(WL::SubWindow::ScrollView);
		if(self->list == NULL) return FALSE;
		add_window(self->list);
		self->list->set_frame(0, 50, 700, 400 - 50 - 50);
		WL::Color c;
		c.set(239, 236, 255);
		self->list->set_background_color(&c);
		self->list->show();

		c.set(0, 0, 0);
		WL::Font f;
		f.init();
		f.set_size(11);

		self->err = cl_new(WL::SubWindow::Label);
		if(self->err == NULL) return NULL;
		if(!self->err->init()) return NULL;
		self->err->set_text_color(&c);
		self->err->set_font(&f);
		self->err->set_text("加载中");
		self->err->set_frame(0, 0, 700, 300);
		self->list->add_window(self->err);
		self->err->show();

		refresh_click(NULL, self);

		show();
		return TRUE;
	}

	void Main::redraw(WL::IRender* render)
	{
		BaseWindow::redraw(render);
		render->set_color(255, 255, 255);
		WL::Rect tmp;
		tmp.set(0, 0, 700, 50);
		render->fill_rect(&tmp);

		//WL::Color c;
		//c.set(0,0,0);
		//render->set_color(&c);
		//render->fill_rect(0,49,700, 1);

		WL::Color cb;
		WL::Color ct;
		cb.set(0.2, 0, 0, 0);
		ct.alpha = 0;

		ft w = 700.0 / 5;

		WL::Point p1;
		WL::Point p2;

		p1.set(0, 0);
		p2.set(0, 50);
		tmp.set(0, 0, 1, 50);

		for(st i = 0; i < 4; ++i)
		{
			p1.x += w;
			p2.x += w;
			tmp.x += w;
			render->fill_gradient_rect(&tmp, &ct, &cb, &p1, &p2);
		}

		render->set_color(189, 202, 213);
		tmp.set(0, 400 - 50, 700, 50);
		render->fill_rect(&tmp);
	}
}