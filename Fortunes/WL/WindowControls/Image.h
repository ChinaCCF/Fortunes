#include <WL/Window.h>
#include <WL/Graphics/Graphics.h>
#pragma once

namespace WL
{
	namespace SubWindow
	{
		class _Image;
		class Image :public BaseWindow
		{
			_Image* self;
		public:
			Image() { self = NULL; }
			~Image();
			st init();

			void set_file(const char* file);
			const char* get_file();

			//void set_image(const WL::ImageData* img);
			//const WL::ImageData* get_image();

			void set_scale(st val);
			st get_scale();

			void set_fit(st val);
			st get_fit();

			virtual void redraw(IRender* render, Rect* r);
		};
	}
}