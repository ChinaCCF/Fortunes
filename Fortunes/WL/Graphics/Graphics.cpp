#include <WL/Graphics/Graphics.h>
#include <CL/Memory.h>
#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

namespace WL
{
	class _Font
	{
	public:
		HFONT font;
		char* name;
		st size;
		st is_bold;

		_Font()
		{
			font = NULL;
			name = NULL;
			size = 15;
			is_bold = FALSE;
		}
	};
	Font::~Font()
	{
		if(self)
		{
			cl_free(self->name);
			if(self->font) DeleteObject(self->font);
		}

		cl_delete(self);
	}

	st Font::init()
	{
		self = cl_new(_Font);
		if(self == NULL) return FALSE;
		self->name = cl_string_alloc("新宋体");
		if(self->name == NULL) return FALSE;
		return TRUE;
	}

	void Font::set_size(st val) { self->size = val; }
	st Font::get_size()const { return self->size; }

	void Font::set_is_bold(st val) { self->is_bold = val; }
	st Font::get_is_bold()const { return self->is_bold; }

	void Font::set_name(const char* name)
	{
		cl_free(self->name);
		self->name = cl_string_alloc(name);
	}
	const char* Font::get_name()const { return self->name; }

	Font* Font::copy()
	{
		Font* f = cl_new(Font);
		if(f)
		{
			if(!f->init())
			{
				cl_delete(f);
				return NULL;
			}
			f->set_is_bold(self->is_bold);
			f->set_name(self->name);
			f->set_size(self->size);
			return f;
		}
		return NULL;
	}

	Font& Font::operator = (const Font& f)
	{
		cl_free(self->name);
		self->name = cl_string_alloc(f.get_name());
		self->size = f.get_size();
		self->is_bold = f.get_is_bold();
		return *this;
	}

	void* Font::get_HFONT()
	{
		if(self->font) DeleteObject(self->font);
		self->font = CreateFontA(
			self->size,						//字体的逻辑高度  
			0,					        //宽度  
			0,							//与水平线的角度  
			0,							//基线方位角度  
			FW_REGULAR,					//字形：常规  
			FALSE,						//字形：斜体  
			FALSE,						//字形：下划线  
			self->is_bold,					//字形：粗体  
			GB2312_CHARSET,				//字符集  
			OUT_DEFAULT_PRECIS,			//输出精度  
			CLIP_DEFAULT_PRECIS,		//剪截精度  
			PROOF_QUALITY,				//输出品质  
			DEFAULT_PITCH | FF_MODERN,	//倾斜度  
			self->name						//字体  
			);
		return self->font;
	}

	class _ImageData
	{
	public:
		char* file;
		Gdiplus::Image* image;
		_ImageData() { file = NULL; image = NULL; }
	};

	ImageData::~ImageData()
	{
		if(self)
		{
			cl_free(self->file);
			if(self->image) delete self->image;
		}
		cl_delete(self);
	}

	st ImageData::init()
	{
		self = cl_new(_ImageData);
		if(self == NULL) return FALSE;
		return TRUE;
	}

	void ImageData::set_file(const char* file)
	{
		if(self->image) { delete self->image; self->image = NULL; }
		cl_free(self->file);
		self->file = cl_string_alloc(file);
	}
	const char* ImageData::get_file() { return self->file; }

	st ImageData::get_width() { if(self->image) return self->image->GetWidth(); return 0; }
	st ImageData::get_height() { if(self->image) return self->image->GetHeight(); return 0; }

	void* ImageData::get_image()
	{
		if(self->image)
			return self->image;

		if(self->file)
		{
			st count = CL::StringUtil::char_to_wchar_count(self->file);
			wchar* file = cl_alloc_type_with_count(wchar, count);
			CL::StringUtil::char_to_wchar(self->file, file, count);
			self->image = new Gdiplus::Image(file);
			cl_free(file);
			return self->image;
		}
		return NULL;
	}

}