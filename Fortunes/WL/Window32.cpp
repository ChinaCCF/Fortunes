#include <WL/Window32.h>
#include <math.h>
#include <CL/Memory.h>
#include <CL/Object/String.h> 
#include <WL/Window.h>

namespace WL
{
	class _Window32
	{
	public:
		HWND hwnd;
		char* text;
		Rect frame;
		st text_size;
		st is_layer;
		st is_child;
		ft alpha;
		st has_show;
		BaseWindow* dispatcher;
		st is_mouse_track;
		_Window32()
		{
			hwnd = NULL;
			text_size = 64;
			text = NULL;
			frame.set(0, 0, 100, 100);
			is_layer = FALSE;
			is_child = FALSE;
			has_show = FALSE;
			alpha = 1.0;
			dispatcher = NULL;
			is_mouse_track = FALSE;
		}
	};

	/************************************************************************************************/
	/************************************************************************************************/
	static LRESULT CALLBACK Window_Process(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static HINSTANCE g_application_instance = NULL;

	static st register_window_class(const char* class_name, WNDPROC fun)
	{
		WNDCLASSEXA wcex;
		if(!GetClassInfoExA(NULL, class_name, &wcex))
		{
			if(!GetClassInfoExA(g_application_instance, class_name, &wcex))
			{
				if(g_application_instance == NULL)
				{
					cl_printf("g_application_instance is NULL!");
					return FALSE;
				}

				memset(&wcex, 0, sizeof(WNDCLASSEXA));
				wcex.cbSize = sizeof(WNDCLASSEXA);
				wcex.style = CS_HREDRAW | CS_VREDRAW;
				wcex.cbWndExtra = sizeof(void*);
				wcex.lpfnWndProc = fun;
				wcex.hInstance = g_application_instance;
				wcex.hCursor = LoadCursorA(NULL, IDC_ARROW);
				wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
				wcex.lpszClassName = class_name;
				return RegisterClassExA(&wcex) != NULL;
			}
		}
		return TRUE;
	}

	static HWND create_window(const char* class_name)
	{
		DWORD flags = WS_POPUP;
		DWORD ex_flags = WS_EX_ACCEPTFILES | WS_EX_APPWINDOW | WS_EX_NOPARENTNOTIFY | WS_EX_TRANSPARENT;
		return CreateWindowExA(ex_flags, class_name, ""/*title*/, flags, 0, 0, 1, 1, NULL/*parent*/, NULL/*menu*/, g_application_instance, NULL/*extra*/);
	}
	/************************************************************************************************/
	/************************************************************************************************/
	Window32::Window32() { self = NULL; }
	Window32::~Window32()
	{
		if(self)
		{
			cl_free(self->text);
			if(self->hwnd)
				DestroyWindow(self->hwnd);
		}
		cl_delete(self);
	}

	st Window32::init()
	{
		self = cl_new(_Window32);
		if(self == NULL) return FALSE;
		self->text = cl_alloc_type_with_count(char, self->text_size);
		if(self->text == NULL) return FALSE;

		const char* class_name = "WLWindow";
		if(!register_window_class(class_name, Window_Process)) return FALSE;
		self->hwnd = create_window(class_name);
		if(self->hwnd != NULL)
		{
			SetWindowLongA(self->hwnd, 0, (LONG)self);
			return TRUE;
		}
		return FALSE;
	}

	HWND Window32::get_handle() { return self->hwnd; }
	void Window32::set_ico(st ico) { SendMessageA(self->hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIconA(g_application_instance, MAKEINTRESOURCEA(ico))); }
	void Window32::set_title(const char* title)
	{
		CL::StringUtil::string_copy(self->text, self->text_size, title);
		SetWindowTextA(self->hwnd, self->text);
	}
	const char* Window32::get_title() { return self->text; }
	void Window32::set_parent(HWND parent)
	{
		if(parent)
			self->is_child = TRUE;
		else
			self->is_child = FALSE;
		SetParent(self->hwnd, parent);
	}
	HWND Window32::get_parent() { return GetParent(self->hwnd); }
	void Window32::set_frame(const Rect* r) { set_frame(r->x, r->y, r->w, r->h); }
	void Window32::set_frame(st x, st y, st w, st h)
	{
		st need_redraw = FALSE;
		if(w != self->frame.w || h != self->frame.h)
			need_redraw = TRUE;
		self->frame.set(x, y, w, h);
		MoveWindow(self->hwnd, x, y, w, h, need_redraw);
	}
	void Window32::get_frame(Rect* r) { *r = self->frame; }
	void Window32::set_is_layer(st val)
	{
		if(val != self->is_layer)
		{
			st style = GetWindowLongA(self->hwnd, GWL_EXSTYLE);
			if(val)
				style |= WS_EX_LAYERED;
			else
				style ^= WS_EX_LAYERED;
			SetWindowLongA(self->hwnd, GWL_EXSTYLE, style);
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
				self->alpha = CL::MAX(0.0, CL::MIN(1.0, self->alpha));
				SetLayeredWindowAttributes(self->hwnd, NULL, (BYTE)(self->alpha * 254), LWA_ALPHA);
			}
		}
	}
	ft Window32::get_alpha() { return self->alpha; }


	void Window32::update()
	{
		if(self->is_child)
			InvalidateRect(self->hwnd, NULL, FALSE);
		else
			UpdateWindow(self->hwnd);
	}
	void Window32::show()
	{
		if(!self->has_show)
		{
			if(!self->is_child)
			{
				int w = GetSystemMetrics(SM_CXSCREEN);
				int h = GetSystemMetrics(SM_CYSCREEN);
				Rect screen;
				screen.set(0, 0, w, h);

				self->frame.move_to_center_in(&screen);
				MoveWindow(self->hwnd, self->frame.x, self->frame.y, self->frame.w, self->frame.h, FALSE);
			}
			else
				MoveWindow(self->hwnd, self->frame.x, self->frame.y, self->frame.w, self->frame.h, TRUE);
			self->has_show = TRUE;
			update();
		}
		ShowWindow(self->hwnd, SW_SHOW);
	}
	void Window32::hide() { ShowWindow(self->hwnd, SW_HIDE); }
	void Window32::close() { cl_delete(this); }


	void Window32::set_dispatcher(void* dispatcher) { self->dispatcher = (BaseWindow*)dispatcher; }
	/*********************************************************************************/
	void Window32::set_application(HINSTANCE ins) { g_application_instance = ins; }
	void Window32::loop() { MSG msg; while(GetMessage(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); } }
	void Window32::exit() { PostQuitMessage(0); }

	/*********************************************************************************/
	/*********************************************************************************/
	static st paint(_Window32* win, WPARAM w, LPARAM l)
	{
		if(win->dispatcher)
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(win->hwnd, &ps);

			st w = ps.rcPaint.right - ps.rcPaint.left;
			st h = ps.rcPaint.bottom - ps.rcPaint.top;
			WL::Rect r;
			r.set(ps.rcPaint.left, ps.rcPaint.top, w, h);

			WL::IRender* gdi = WL::IRender::create_gdi_render(hdc);
			if(gdi == NULL) return FALSE;
			win->dispatcher->redraw(gdi, &r);
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
			Point p;
			p.set(LOWORD(l), HIWORD(l));
			WL::TouchEvent e;
			if(!e.init(type, 1, &p)) return FALSE;
			win->dispatcher->event_for_touch(&e);
			return TRUE;
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
	static LRESULT CALLBACK Window_Process(HWND hWnd, UINT message, WPARAM w, LPARAM l)
	{
		if(message == WM_NCCREATE)
			SetWindowLong(hWnd, 0, 0);

		_Window32* win = (_Window32*)GetWindowLong(hWnd, 0);
		if(win)
		{
			if(message == WM_PAINT) { if(paint(win, w, l)) return 0; }
			else
			{
				if(message == WM_LBUTTONDOWN) { if(touch_event(win, WL::TouchEvent::Down, l)) return 0; }
				else
				{
					if(message == WM_LBUTTONUP) { if(touch_event(win, WL::TouchEvent::Up, l)) return 0; }
					else
					{
						if(message == WM_MOUSEMOVE)
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
						}
						else
						{
							if(message == WM_MOUSEHOVER) { if(touch_event(win, WL::TouchEvent::Enter, l)) return 0; }
							else
							{
								if(message == WM_MOUSELEAVE)
								{
									win->is_mouse_track = FALSE;
									if(touch_event(win, WL::TouchEvent::Leave, l))
										return 0;
								}
								else
								{
									if(message == WM_EXITSIZEMOVE) { update_frame(win); return 0; }
									else
									{

									}
								}
							}
						}
					}
				}
			}
		}

		return DefWindowProc(hWnd, message, w, l);
	}
}