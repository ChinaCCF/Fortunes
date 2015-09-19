#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")
#include <WL/Graphics/Render.h>
#include <CL/Memory.h>

namespace WL
{
	static inline Gdiplus::Rect convert(const Rect* r)
	{
		return Gdiplus::Rect(r->x, r->y, r->w, r->h);
	}
	/***********************************************************************************/
	/***********************************************************************************/
	class GDI : public IRender
	{
		ULONG_PTR gdiplus_token_;
		HDC hdc_;
		Gdiplus::Graphics* graphics_;
		Gdiplus::Color* color_;
		ft width_;
	public:
		GDI()
		{
			gdiplus_token_ = NULL;
			graphics_ = NULL;
			color_ = NULL;
			width_ = 1.0;
		}
		~GDI()
		{
			if(color_) cl_delete(color_);
			if(graphics_)
			{
				graphics_->ReleaseHDC(hdc_);
				cl_delete(graphics_);
			}
			if(gdiplus_token_) 	Gdiplus::GdiplusShutdown(gdiplus_token_);
		}
		st init(HDC hdc)
		{
			Gdiplus::GdiplusStartupInput gdiplus_input;
			if(Gdiplus::GdiplusStartup(&gdiplus_token_, &gdiplus_input, NULL) != Gdiplus::Status::Ok) return FALSE;

			hdc_ = hdc;

			graphics_ = new Gdiplus::Graphics(hdc_);
			if(graphics_ == NULL) return FALSE;
			graphics_->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

			color_ = cl_new(Gdiplus::Color);
			if(color_ == NULL)return FALSE;
			return TRUE;
		}
	public:
		void set_color(ft a, st r, st g, st b) { color_->SetValue(Gdiplus::Color::MakeARGB((BYTE)(a * 255), (BYTE)r, (BYTE)g, (BYTE)b)); }
		void set_color(Color* c) { color_->SetValue(Gdiplus::Color::MakeARGB((BYTE)(c->alpha * 255), (BYTE)c->red, (BYTE)c->green, (BYTE)c->blue)); }

		void set_pen_width(st width) { width_ = width; }

		void fill_rect(Rect* r)
		{
			Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(*color_);
			graphics_->FillRectangle(brush, convert(r));
			delete brush;
		}
		void draw_rect(Rect* r)
		{
			Gdiplus::Pen* pen = new Gdiplus::Pen(*color_, (Gdiplus::REAL)width_);
			graphics_->DrawRectangle(pen, r->x, r->y, r->w, r->h);
			delete pen;
		}

		void fill_rect_with_corner(Rect* r, st corner) {}
		void draw_rect_with_corner(Rect* r, st corner) {}

		void fill_ellipse(Rect* r) {}
		void draw_ellipse(Rect* r) {}

		void set_clip(Rect* r) {}
		void offset_x(st x) {}
		void offset_y(st y) {}
		void offset(st x, st y) {}

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