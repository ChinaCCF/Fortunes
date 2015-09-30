#include <WL/Window32.h>
#include <math.h>
#include <CL/Memory.h>
#include <CL/Object/String.h> 
#include <WL/Window.h>

namespace WL
{
	/************************************************************************************************/
	/************************************************************************************************/
	static LRESULT CALLBACK Window_Process(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Text_Process(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static st register_window_class(const char* class_name, WNDPROC fun)
	{
		WNDCLASSEXA wcex;
		if(!GetClassInfoExA(NULL, class_name, &wcex))
		{
			if(!GetClassInfoExA(Window32::application, class_name, &wcex))
			{
				if(Window32::application == NULL)
				{
					cl_printf("Window32 application is NULL!");
					return FALSE;
				}

				memset(&wcex, 0, sizeof(WNDCLASSEXA));
				wcex.cbSize = sizeof(WNDCLASSEXA);
				wcex.style = CS_HREDRAW | CS_VREDRAW | CS_SAVEBITS;
				//wcex.cbWndExtra = sizeof(void*);
				wcex.lpfnWndProc = fun;
				wcex.hInstance = Window32::application;
				wcex.hCursor = LoadCursorA(NULL, IDC_ARROW);
				//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
				wcex.lpszClassName = class_name;
				return RegisterClassExA(&wcex) != NULL;
			}
		}
		return TRUE;
	}

	static HWND create_window(const char* class_name, st style, st exstyle, Rect* r)
	{
		DWORD flags = WS_POPUP;
		DWORD ex_flags = WS_EX_ACCEPTFILES | WS_EX_APPWINDOW | WS_EX_NOPARENTNOTIFY | WS_EX_TRANSPARENT;
		if(style) flags |= style;
		if(exstyle) ex_flags |= exstyle;
		return CreateWindowExA(ex_flags, class_name, ""/*title*/, flags,
							   (st)r->x, (st)r->y, (st)r->w, (st)r->h,
							   NULL/*parent*/, NULL/*menu*/, Window32::application, NULL/*extra*/);
	}
	/************************************************************************************************/
	/************************************************************************************************/
	class _Window32
	{
	public:
		HWND hwnd;//window handle
		char* title;//window title
		Rect frame;
		st is_layer;//has exstyle WS_EX_LAYERED
		ft alpha;
		st has_show;
		st ico;
		BaseWindow* dispatcher;//event handle
		st is_mouse_track;
		WNDPROC ctrl_proc;
	};

	Window32::~Window32()
	{
		if(self)
		{
			cl_free(self->text);
			if(self->hwnd)
			{
				SetWindowLongPtrA(self->hwnd, GWLP_USERDATA, 0);
				DestroyWindow(self->hwnd);
			}
		}
		cl_delete(self);
	}
	st Window32::init()
	{
		self = cl_alloc_type(_Window32);
		if(self == NULL) return FALSE;

		self->hwnd = NULL;
		self->text = NULL;
		self->frame.set(0, 0, 100, 100);
		self->is_layer = FALSE;
		self->alpha = 1.0;
		self->has_show = FALSE;
		self->dispatcher = NULL;
		self->is_mouse_track = FALSE;
		self->ctrl_proc = NULL;
		return TRUE;
	}

	st Window32::create(const char* class_name, st style, st ex_style)
	{
		if(class_name == NULL)
		{
			class_name = "WLWindow";
			if(!register_window_class(class_name, Window_Process)) return FALSE;
		}
		if(cl::MemoryUtil::is_contain_bits(ex_style, WS_EX_LAYERED))
			self->is_layer = TRUE;
		self->hwnd = create_window(class_name, style, ex_style, &self->frame);
		if(self->hwnd)
			SetWindowLongPtrA(self->hwnd, GWLP_USERDATA, (LONG)self);
		else
			return FALSE;

		if(strcmp(class_name, "edit") == 0)
			self->ctrl_proc = (WNDPROC)SetWindowLongPtrA(self->hwnd, GWL_WNDPROC, (LONG)Text_Process);
		return TRUE;
	}

	HWND Window32::get_handle() { return self->hwnd; }
	void Window32::set_ico(st ico) { SendMessageA(self->hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIconA(g_application_instance, MAKEINTRESOURCEA(ico))); }
	void Window32::set_title(const char* title)
	{
		cl_free(self->text);
		self->text = cl_string_alloc(title);
		if(self->text)
			SetWindowTextA(self->hwnd, self->text);
	}
	const char* Window32::get_title() { return self->text; }
	void Window32::set_parent(HWND parent) { SetParent(self->hwnd, parent); }
	HWND Window32::get_parent() { return GetParent(self->hwnd); }
	void Window32::set_frame(const Rect* r) { set_frame(r->x, r->y, r->w, r->h); }
	void Window32::set_frame(ft x, ft y, ft w, ft h)
	{
		st need_redraw = FALSE;
		if(!cl::MemoryUtil::is_float_equ(w, self->frame.w)
		   || !cl::MemoryUtil::is_float_equ(h, self->frame.h))
		   need_redraw = TRUE;
		self->frame.set(x, y, w, h);
		MoveWindow(self->hwnd, (st)x, (st)y, (st)w, (st)h, need_redraw);
	}
	void Window32::get_frame(Rect* r) { *r = self->frame; }
	ft Window32::get_width() { return self->frame.w; }
	ft Window32::get_height() { return self->frame.h; }
	void Window32::set_is_layer(st val)
	{
		if(val != self->is_layer)
		{
			st style = GetWindowLongPtrA(self->hwnd, GWL_EXSTYLE);
			if(val)
				style |= WS_EX_LAYERED;
			else
				style ^= WS_EX_LAYERED;
			SetWindowLongPtrA(self->hwnd, GWL_EXSTYLE, style);
			self->is_layer = val;
		}
	}
	st Window32::get_is_layer() { return self->is_layer; }
	void Window32::set_alpha(ft val)
	{
		if(self->is_layer)
		{
			if(abs(self->alpha - val) > 0.01)
			{
				self->alpha = val;
				self->alpha = cl::MAX(0.0, cl::MIN(1.0, self->alpha));
				SetLayeredWindowAttributes(self->hwnd, NULL, (BYTE)(self->alpha * 254), LWA_ALPHA);
			}
		}
	}
	ft Window32::get_alpha() { return self->alpha; }

	void Window32::update()
	{
		HWND parent = get_parent();
		if(parent)
			InvalidateRect(self->hwnd, NULL, FALSE);
		else
			UpdateWindow(self->hwnd);
	}
	void Window32::show()
	{
		if(!self->has_show)
		{
			HWND parent = get_parent();
			if(!parent)
			{
				Rect screen(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
				self->frame.move_to_center_in(&screen);
			}

			MoveWindow(self->hwnd, (st)self->frame.x, (st)self->frame.y, (st)self->frame.w, (st)self->frame.h, FALSE);
			self->has_show = TRUE;
			update();
		}
		ShowWindow(self->hwnd, SW_SHOW);
	}
	void Window32::hide() { ShowWindow(self->hwnd, SW_HIDE); }
	void Window32::close() { cl_delete(this); }
	void Window32::set_focus() { SetFocus(self->hwnd); }

	void Window32::set_dispatcher(void* dispatcher) { self->dispatcher = (BaseWindow*)dispatcher; }

	/*********************************************************************************/
	/*********************************************************************************/
	static st paint(_Window32* win, WPARAM w, LPARAM l)
	{
		if(win->dispatcher)
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(win->hwnd, &ps);
			WL::Rect r(0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);

			WL::IRender* gdi = WL::IRender::create_gdi_render(hdc);
			if(gdi == NULL) return FALSE;
			win->dispatcher->redraw(gdi);
			cl_delete(gdi);

			EndPaint(win->hwnd, &ps);
			return TRUE;
		}
		return FALSE;
	}

	static st touch_event(_Window32* win, WL::TouchEvent::TouchType type, LPARAM l)
	{
		if(win->dispatcher)
		{

			WL::TouchEvent e;
			if(e.init())
			{
				Point p(LOWORD(l), HIWORD(l));
				if(e.set(type, 1, &p))
				{
					win->dispatcher->event_for_touch(&e);
					return TRUE;
				}
			}
		}
		return FALSE;
	}

	static void update_frame(_Window32* win)
	{
		RECT r;
		GetWindowRect(win->hwnd, &r);
		win->frame.x = r.left;
		win->frame.y = r.top;
	}

	static st wheel(_Window32* win, WPARAM w)
	{
		if(win->dispatcher)
		{
			s16 direction = HIWORD(w);
			direction /= 120;
			win->dispatcher->event_for_wheel(direction);
			return TRUE;
		}
		return FALSE;
	}
	static LRESULT CALLBACK Window_Process(HWND hWnd, UINT message, WPARAM w, LPARAM l)
	{
		if(message == WM_NCCREATE)
			SetWindowLongPtrA(hWnd, GWLP_USERDATA, 0);

		_Window32* win = (_Window32*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
		if(win)
		{
			switch(message)
			{
				case WM_PAINT:{
						if(paint(win, w, l))
							return 0;
						break;
					}
				case WM_LBUTTONDOWN: {
						if(touch_event(win, WL::TouchEvent::Down, l))
							return 0;
						break;
					}
				case WM_LBUTTONUP: {
						if(touch_event(win, WL::TouchEvent::Up, l))
							return 0;
						break;
					}
				case WM_MOUSEMOVE:
					{
						if(!win->is_mouse_track)
						{
							TRACKMOUSEEVENT track = {0};
							track.cbSize = sizeof(TRACKMOUSEEVENT);
							track.dwFlags = TME_LEAVE | TME_HOVER;
							track.hwndTrack = hWnd;
							track.dwHoverTime = 10;
							TrackMouseEvent(&track);
							win->is_mouse_track = TRUE;
						}
						if(touch_event(win, WL::TouchEvent::Move, l))
							return 0;
						break;
					}
				case WM_MOUSEHOVER: {
						if(touch_event(win, WL::TouchEvent::Enter, l))
							return 0;
						break;
					}
				case WM_MOUSELEAVE:
					{
						win->is_mouse_track = FALSE;
						if(touch_event(win, WL::TouchEvent::Leave, l))
							return 0;
						break;
					}
				case WM_EXITSIZEMOVE: {
						update_frame(win);
						return 0;
					}
				case WM_MOUSEWHEEL:{
						if(wheel(win, w))
							return 0;
						break;
					}
				default:
					break;
			}
		}
		return DefWindowProc(hWnd, message, w, l);
	}

	static LRESULT CALLBACK Text_Process(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		_Window32* win = (_Window32*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
		switch(message)
		{
			case WM_CHAR:
				{
					WL::KeyBoardEvent e;
					if(e.init())
					{
						e.set(WL::KeyBoardEvent::Char, wParam);
						if(win->dispatcher->event_for_keyboard(&e))
							return 0;
					}
				}
			default:
				return win->ctrl_proc(hWnd, message, wParam, lParam);
		}
	}
}