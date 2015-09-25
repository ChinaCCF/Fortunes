#include <WL/WindowControls/Image.h>
#include <CL/Memory.h>

namespace WL
{
	namespace SubWindow
	{
		class _Image
		{
		public:
			WL::ImageData* image;
			st scale;
			st fit;
			_Image() { image = NULL;  scale = TRUE; fit = TRUE; }
		};


		Image::~Image() {
			if(self) cl_delete(self->image);
			cl_delete(self);
		}

		st Image::init() {
			if(!BaseWindow::init()) return FALSE;
			self = cl_new(_Image);
			if(self == NULL) return FALSE;
			self->image = cl_new(WL::ImageData);
			if(self->image == NULL) return FALSE;
			if(!self->image->init()) return FALSE;
			return TRUE; 
		}

		void Image::set_file(const char* file) {
			self->image->set_file(file);
		}
		const char* Image::get_file() { return self->image->get_file(); }
 
		//void set_image(const WL::ImageData* img);
		//const WL::ImageData* get_image();

		void Image::set_scale(st val) { self->scale = val; }
		st Image::get_scale() { return self->scale; }

		void Image::set_fit(st val) { self->fit = val; }
		st Image::get_fit() { return self->fit; }

		void Image::redraw(IRender* render) {
			if(self->image->get_image() == NULL) return;
			Rect target;
			target.w = self->image->get_width();
			target.h = self->image->get_height();
			if(self->scale)
			{
				Rect r;
				get_frame(&r);
				r.x = 0;
				r.y = 0;

				if(self->fit)
				{
					target.fit_in(&r);
					 
					st align = get_horizontal_align();
					if(align == 0)
						target.move_to_horizontal_center_in(&r);
					else
					{
						if(align == 1)
							target.move_to_right_in(&r);
					}
					align = get_vertical_align();
					if(align == 0)
						target.move_to_vertical_center_in(&r);
					else
					{
						if(align == 1)
							target.move_to_bottom_in(&r);
					}
				}
				else
				{
					target.w = r.w;
					target.h = r.h;
				}
			}
			render->draw_image(self->image, &target);
		}
	}
}