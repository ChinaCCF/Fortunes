#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")
#include <WL/Graphics/Render.h>
#include <CL/Memory.h>

namespace WL
{
#define TO_RECT(r) Gdiplus::Rect((INT)r->x, (INT)r->y, (INT)r->w, (INT)r->h)
#define TO_POINT(p) Gdiplus::Point((INT)p->x, (INT)p->y)
#define TO_POINTF(p) Gdiplus::PointF((Gdiplus::REAL)p->x, (Gdiplus::REAL)p->y)
#define TO_COLOR(c) Gdiplus::Color((BYTE)(c->alpha * 255), c->red, c->green, c->blue)
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
			delete color;
			delete font;
			if(graphics)
			{
				graphics->ReleaseHDC(hdc);
				delete graphics;
			}
			if(gdiplus_token) Gdiplus::GdiplusShutdown(gdiplus_token);
		}
		st init(HDC hdc)
		{
			Gdiplus::GdiplusStartupInput gdiplus_input;
			if(Gdiplus::GdiplusStartup(&gdiplus_token, &gdiplus_input, NULL) != Gdiplus::Status::Ok) return FALSE;

			hdc = hdc;

			graphics = new Gdiplus::Graphics(hdc);
			if(graphics == NULL) return FALSE;
			graphics->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

			color = new Gdiplus::Color();
			if(color == NULL)return FALSE;
			font = new Gdiplus::Font(L"ÐÂËÎÌå", 15, Gdiplus::FontStyle::FontStyleRegular);
			if(font == NULL) return FALSE;
			return TRUE;
		}
	public:
		void set_color(st r, st g, st b) { color->SetValue(Gdiplus::Color::MakeARGB((BYTE)(255), (BYTE)r, (BYTE)g, (BYTE)b)); }
		void set_color(ft a, st r, st g, st b) { color->SetValue(Gdiplus::Color::MakeARGB((BYTE)(a * 255), (BYTE)r, (BYTE)g, (BYTE)b)); }
		void set_color(const Color* c) { color->SetValue(Gdiplus::Color::MakeARGB((BYTE)(c->alpha * 255), (BYTE)c->red, (BYTE)c->green, (BYTE)c->blue)); }

		void set_pen_width(st _width) { width = _width; }

		void draw_line(Point* p1, Point* p2)
		{
			Gdiplus::Pen* pen = new Gdiplus::Pen(*color, (Gdiplus::REAL)width);
			graphics->DrawLine(pen, TO_POINT(p1), TO_POINT(p2));
			delete pen;
		}

		virtual void fill_rect(ft x, ft y, ft w, ft h)
		{
			Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(*color);
			graphics->FillRectangle(brush, Gdiplus::Rect((st)x, (st)y, (st)w, (st)h));
			delete brush;
		}
		void fill_rect(const Rect* r) { fill_rect(r->x, r->y, r->w, r->h); }
		void draw_rect(const Rect* r)
		{
			Gdiplus::Pen* pen = new Gdiplus::Pen(*color, (Gdiplus::REAL)width);
			graphics->DrawRectangle(pen, (Gdiplus::REAL)r->x, (Gdiplus::REAL)r->y, (Gdiplus::REAL)r->w, (Gdiplus::REAL)r->h);
			delete pen;
		}

		void fill_rect_with_corner(const Rect* r, st corner) {}
		void draw_rect_with_corner(const Rect* r, st corner) {}

		void fill_ellipse(const Rect* r) { fill_ellipse(r->x, r->y, r->w, r->h); }
		void fill_ellipse(ft x, ft y, ft w, ft h)
		{
			Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(*color);
			graphics->FillEllipse(brush, Gdiplus::Rect((st)x, (st)y, (st)w, (st)h));
			delete brush;
		}
		void draw_ellipse(const Rect* r) {}

		void set_clip(const Rect* r) {}
		void offset_x(st x) {}
		void offset_y(st y) {}
		void offset(st x, st y) {}

		void set_font(Font* f)
		{
			Gdiplus::FontStyle style;
			if(f->get_is_bold())
				style = Gdiplus::FontStyle::FontStyleBold;
			else
				style = Gdiplus::FontStyle::FontStyleRegular;

			wchar font_name[32];
			CL::StringUtil::char_to_wchar(f->get_name(), font_name, 32);
			Gdiplus::Font* tmp = new Gdiplus::Font(font_name, (Gdiplus::REAL)f->get_size(), style);
			if(tmp) { delete font; font = tmp; }
		}
		ft text_height(const char* str)
		{
			Size size;
			text_size(str, &size);
			return size.h;
		}
		ft text_width(const char* str)
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

			Gdiplus::Region new_val(TO_RECT(r));
			graphics->SetClip(&new_val);

			graphics->DrawString(wstr, -1, font, TO_POINTF(((Point*)r)), brush);
			graphics->SetClip(&old_val);
			cl_free(wstr);
			delete brush;
		}
		void draw_image(ImageData* img, const Rect* r)
		{
			if(img->get_image() == NULL) return;
			graphics->DrawImage((Gdiplus::Image*)img->get_image(), TO_RECT(r));
		}

		void fill_gradient_rect(const Rect* r, const Color* c1, const Color* c2, const Point* p1, const Point* p2)
		{ 
			Gdiplus::LinearGradientBrush* brush = new Gdiplus::LinearGradientBrush(TO_POINT(p1), TO_POINT(p2), TO_COLOR(c1), TO_COLOR(c2));
			graphics->FillRectangle(brush, TO_RECT(r));
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