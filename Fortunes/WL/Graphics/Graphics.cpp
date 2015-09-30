#include <WL/Graphics/Graphics.h>
#include <CL/Memory.h>
#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")
#include <CL/Object/String.h>

namespace WL
{
	Font::Font() :cl_init_property(Font, name) {}
	Font::~Font()
	{
		cl_free(name_);
		if(font_) DeleteObject(font_);
	}
	st Font::init() { name_ = cl_string_alloc("新宋体"); if(name == NULL) return FALSE; return TRUE; }
	Font::operator HFONT()
	{
		if(font_) DeleteObject(font_);
		font_ = CreateFontA(
			size,						//字体的逻辑高度  
			0,					        //宽度  
			0,							//与水平线的角度  
			0,							//基线方位角度  
			FW_REGULAR,					//字形：常规  
			FALSE,						//字形：斜体  
			FALSE,						//字形：下划线  
			is_bold,					//字形：粗体  
			GB2312_CHARSET,				//字符集  
			OUT_DEFAULT_PRECIS,			//输出精度  
			CLIP_DEFAULT_PRECIS,		//剪截精度  
			PROOF_QUALITY,				//输出品质  
			DEFAULT_PITCH | FF_MODERN,	//倾斜度  
			name_); 				    //字体  
		return font_;
	}
	void Font::set_name(const char* _name)
	{
		cl_free(name_);
		name_ = cl_string_alloc(_name);
	}
	const char* Font::get_name(){ return name_; }
	/**********************************************************************************************/
	//#############################################################################################
	/**********************************************************************************************/
	ImageData::ImageData() :
		cl_init_property(ImageData, file),
		cl_init_readonly_property(ImageData, width),
		cl_init_readonly_property(ImageData, height),
		cl_init_readonly_property(ImageData, image)
	{}

	ImageData::~ImageData()
	{
		cl_free(file_);
		if(data_) { Gdiplus::Image* img = (Gdiplus::Image*)data_; delete img; }
	} 
	void ImageData::set_file(const char* file)
	{
		cl_free(file_);
		has_chage_ = TRUE;
		file_ = cl_string_alloc(file);
	}
	const char* ImageData::get_file() { return file_; }

	st ImageData::get_width()
	{
		get_image();
		if(data_) {
			Gdiplus::Image* img = (Gdiplus::Image*)data_;
			return img->GetWidth();
		}
		return 0;
	}
	st ImageData::get_height()
	{
		get_image();
		if(data_)
		{
			Gdiplus::Image* img = (Gdiplus::Image*)data_;
			return img->GetHeight();
		}
		return 0;
	}

	void* ImageData::get_image()
	{  
		if(has_chage_)
		{
			if(file_)
			{ 
				wchar* file = cl_alloc_wchars_for_chars(file_);
				data_ = new Gdiplus::Image(file);
				cl_free(file); 
			}
		}
		return data_;
	}

}