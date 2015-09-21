#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")
#include <WL/Graphics/Render.h>
#include <CL/Memory.h>

namespace WL
{
	/***********************************************************************************/
	/***********************************************************************************/
	class GDI : public IRender
	{
		ULONG_PTR gdiplus_token;
		HDC hdc;
		Gdiplus::Graphics* graphics;
		Gdiplus::Color* color;
		ft width;
		Gdiplus::Font* font;
	public:
		GDI()
		{
			gdiplus_token = NULL;
			graphics = NULL;
			color = NULL;
			width = 1.0;
		}
		~GDI()
		{
			if(color) cl_delete(color);
			if(font) delete font;
			if(graphics)
			{
				graphics->ReleaseHDC(hdc);
				cl_delete(graphics);
			}
			if(gdiplus_token) 	Gdiplus::GdiplusShutdown(gdiplus_token);
		}
		st init(HDC hdc)
		{
			Gdiplus::GdiplusStartupInput gdiplus_input;
			if(Gdiplus::GdiplusStartup(&gdiplus_token, &gdiplus_input, NULL) != Gdiplus::Status::Ok) return FALSE;

			hdc = hdc;

			graphics = new Gdiplus::Graphics(hdc);
			if(graphics == NULL) return FALSE;
			graphics->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

			color = cl_new(Gdiplus::Color);
			if(color == NULL)return FALSE;
			font = new Gdiplus::Font(L"ÐÂËÎÌå", 15, Gdiplus::FontStyle::FontStyleRegular);
			if(font == NULL) return FALSE;
			return TRUE;
		}
	public:
		void set_color(st r, st g, st b) { color->SetValue(Gdiplus::Color::MakeARGB((BYTE)(255), (BYTE)r, (BYTE)g, (BYTE)b)); }
		void set_color(ft a, st r, st g, st b) { color->SetValue(Gdiplus::Color::MakeARGB((BYTE)(a * 255), (BYTE)r, (BYTE)g, (BYTE)b)); }
		void set_color(const Color* c) { color->SetValue(Gdiplus::Color::MakeARGB((BYTE)(c->alpha * 255), (BYTE)c->red, (BYTE)c->green, (BYTE)c->blue)); }

		void set_pen_width(st width) { width = width; }

		virtual void fill_rect(st x, st y, st w, st h)
		{
			Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(*color);
			graphics->FillRectangle(brush, Gdiplus::Rect(x, y, w, h));
			delete brush;
		}
		void fill_rect(const Rect* r) { fill_rect(r->x, r->y, r->w, r->h); }
		void draw_rect(const Rect* r)
		{
			Gdiplus::Pen* pen = new Gdiplus::Pen(*color, (Gdiplus::REAL)width);
			graphics->DrawRectangle(pen, r->x, r->y, r->w, r->h);
			delete pen;
		}

		void fill_rect_with_corner(const Rect* r, st corner) {}
		void draw_rect_with_corner(const Rect* r, st corner) {}

		void fill_ellipse(const Rect* r) {}
		void draw_ellipse(const Rect* r) {}

		void set_clip(const Rect* r) {}
		void offset_x(st x) {}
		void offset_y(st y) {}
		void offset(st x, st y) {}

		void set_font(const Font* f)
		{
			Gdiplus::FontStyle style;
			if(f->is_bold)
				style = Gdiplus::FontStyle::FontStyleBold;
			else
				style = Gdiplus::FontStyle::FontStyleRegular;

			wchar font_name[32];
			CL::StringUtil::char_to_wchar(f->name, font_name, 32);
			Gdiplus::Font* tmp = new Gdiplus::Font(font_name, (Gdiplus::REAL)f->size, style);
			if(tmp) { delete font; font = tmp; }
		}
		st text_height(const char* str)
		{
			Size size;
			text_size(str, &size);
			return size.h;
		}
		st text_width(const char* str)
		{
			Size size;
			text_size(str, &size);
			return size.w;
		}
		void text_size(const char* str, Size* size)
		{
			size->w = 0;
			size->h = 0;

			st count = CL::StringUtil::char_to_wchar_count(str);
			wchar* wstr = cl_alloc_type_with_count(wchar, count);
			if(wstr == NULL) return;
			CL::StringUtil::char_to_wchar(str, wstr, count);

			Gdiplus::RectF limit(0, 0, 9999, 9999);
			Gdiplus::RectF result;
			graphics->MeasureString(wstr, -1, font, limit, &result);
			cl_free(wstr);
			size->set((st)result.Width, (st)result.Height);
		}
		void draw_text(const Rect* r, const char* str)
		{
			Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(*color);
			if(brush == NULL) return;

			st count = CL::StringUtil::char_to_wchar_count(str);
			wchar* wstr = cl_alloc_type_with_count(wchar, count);
			if(wstr == NULL) { delete brush;  return; }
			CL::StringUtil::char_to_wchar(str, wstr, count);

			Gdiplus::Region old_val;
			graphics->GetClip(&old_val);

			Gdiplus::Region new_val(Gdiplus::Rect(r->x, r->y, r->w, r->h));
			graphics->SetClip(&new_val);

			graphics->DrawString(wstr, -1, font, Gdiplus::PointF((Gdiplus::REAL)r->x, (Gdiplus::REAL)r->y), brush);
			graphics->SetClip(&old_val);
			cl_free(wstr);
			delete brush;
		}
	};
	/*****************************************************************/
	/*****************************************************************/
	IRender* IRender::create_gdi_render(HDC hdc)
	{
		GDI* gdi = cl_new(GDI);
		if(gdi)
		{
			if(!gdi->init(hdc))
			{
				cl_delete(gdi);
				gdi = NULL;
			}
		}
		return gdi;
	}
}