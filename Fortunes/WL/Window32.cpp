#include <WL/Window32.h>
#include <math.h>
#include <CL/Memory.h>
#include <CL/Object/String.h> 

namespace WL
{
	class _Window32
	{
	public:
		HWND hwnd_;
		char* text_;
		Rect frame_;
		st text_size_;
		st is_layer_;
		ft alpha_;
		st has_show_;
		IResponse* response_;

		_Window32()
		{
			hwnd_ = NULL;
			text_size_ = 64;
			text_ = NULL;
			frame_.set(0, 0, 100, 100);
			is_layer_ = FALSE;
			has_show_ = FALSE;
			alpha_ = 1.0;
			response_ = NULL;
		}
		~_Window32()
		{
			cl_free(text_);
		}
		st init()
		{
			text_ = cl_alloc_type_with_count(char, text_size_);
			return text_ ? TRUE : FALSE;
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
		if(self && self->hwnd_)
			DestroyWindow(self->hwnd_);
		cl_delete(self);
	}

	st Window32::init()
	{
		self = cl_new(_Window32);
		if(self == NULL) return FALSE;
		if(!self->init()) return FALSE;

		const char* class_name = "WLWindow";
		if(!register_window_class(class_name, Window_Process)) return FALSE;
		self->hwnd_ = create_window(class_name);
		if(self->hwnd_ != NULL)
		{
			SetWindowLongA(self->hwnd_, 0, (LONG)this);
			return TRUE;
		}
		return FALSE;
	}

	HWND Window32::get_handle() { return self->hwnd_; }
	void Window32::set_ico(st ico) { SendMessageA(self->hwnd_, WM_SETICON, ICON_SMALL, (LPARAM)LoadIconA(g_application_instance, MAKEINTRESOURCEA(ico))); }
	void Window32::set_title(const char* title)
	{
		CL::StringUtil::string_copy(self->text_, self->text_size_, title);
		SetWindowTextA(self->hwnd_, self->text_);
	}
	const char* Window32::get_title() { return self->text_; }
	void Window32::set_parent(HWND parent) { SetParent(self->hwnd_, parent); }
	HWND Window32::get_parent() { return GetParent(self->hwnd_); }
	void Window32::set_frame(Rect* r) { set_frame(r->x, r->y, r->w, r->h); }
	void Window32::set_frame(st x, st y, st w, st h)
	{
		st need_redraw = FALSE;
		if(w != self->frame_.w || h != self->frame_.h)
			need_redraw = TRUE;
		self->frame_.set(x, y, w, h);
		MoveWindow(self->hwnd_, x, y, w, h, need_redraw);
	}
	void Window32::get_frame(Rect* r) { *r = self->frame_; }
	void Window32::set_is_layer(st val)
	{
		if(val != self->is_layer_)
		{
			st style = GetWindowLongA(self->hwnd_, GWL_EXSTYLE);
			if(val)
				style |= WS_EX_LAYERED;
			else
				style ^= WS_EX_LAYERED;
			SetWindowLongA(self->hwnd_, GWL_EXSTYLE, style);
			self->is_layer_ = val;
		}
	}
	st Window32::get_is_layer() { return self->is_layer_; }
	void Window32::set_alpha(ft val)
	{
		if(self->is_layer_)
		{
			if(abs(self->alpha_ - val) > 0.01)
			{
				self->alpha_ = val;
				self->alpha_ = CL::MAX(0.0, CL::MIN(1.0, self->alpha_));
				SetLayeredWindowAttributes(self->hwnd_, NULL, (BYTE)(self->alpha_ * 254), LWA_ALPHA);
			}
		}
	}
	ft Window32::get_alpha() { return self->alpha_; }


	void Window32::set_response(IResponse* response) { self->response_ = response; }


	void Window32::show()
	{
		if(!self->has_show_)
		{
			if(get_parent() == NULL)
			{
				int w = GetSystemMetrics(SM_CXSCREEN);
				int h = GetSystemMetrics(SM_CYSCREEN);
				Rect screen;
				screen.set(0, 0, w, h);

				self->frame_.move_to_center_in(&screen);
				MoveWindow(self->hwnd_, self->frame_.x, self->frame_.y, self->frame_.w, self->frame_.h, FALSE);
				UpdateWindow(self->hwnd_);
			}
			else
			{
				MoveWindow(self->hwnd_, self->frame_.x, self->frame_.y, self->frame_.w, self->frame_.h, TRUE);
				InvalidateRect(self->hwnd_, NULL, FALSE);
			}
			self->has_show_ = TRUE;
		}
		ShowWindow(self->hwnd_, SW_SHOW);
	}
	void Window32::hide() { ShowWindow(self->hwnd_, SW_HIDE); }
	void Window32::close() { cl_delete(this); }

	/*********************************************************************************/
	void Window32::set_application(HINSTANCE ins) { g_application_instance = ins; }
	void Window32::loop() { MSG msg; while(GetMessage(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); } }
	void Window32::exit() { PostQuitMessage(0); }


	static LRESULT CALLBACK Window_Process(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(message == WM_NCCREATE)
			SetWindowLong(hWnd, 0, 0);

		Window32* win = (Window32*)GetWindowLong(hWnd, 0);
		if(win)
		{
			if(message == WM_PAINT)
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				st w = ps.rcPaint.right - ps.rcPaint.left;
				st h = ps.rcPaint.bottom - ps.rcPaint.top;
				WL::Rect r;
				r.set(ps.rcPaint.left, ps.rcPaint.top, w, h);

				/*WL::IRender* gdi = WL::IRender::gdi(hdc, r);
				win->window_->redraw(gdi, r);
				gdi->release();*/

				EndPaint(hWnd, &ps);
				return 0;
			}
			else
			{
				if(message == WM_LBUTTONDOWN)
				{

				}
				else
				{
					if(message == WM_LBUTTONUP)
					{

					}
					else
					{

					}
				}
			}
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}