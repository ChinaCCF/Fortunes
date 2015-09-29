#include <CL/IO/File.h>

#if !CL_IS_MAC
#include <Windows.h>
#endif

#include <stdio.h>

namespace cl
{
	class _File
	{
	public:
		FILE* file;
	};

	st File::init()
	{
		self = cl_alloc_type(_File);
		self->file = NULL;
		return TRUE;
	}

	File::~File()
	{
		if(self)
		{
			if(self->file) close();
		}
		cl_free(self);
	}

	st File::open(const char* file)
	{
#if CL_IS_MAC
		self->file = fopen(file, "a+");
#else
		fopen_s(&self->file, file, "rb+");
#endif
		if(self->file)
			return TRUE;
		return FALSE;
	}
	void File::close()
	{
		fclose(self->file);
		self->file = NULL;
	}

	st File::read(char* buf, st to_read)
	{
		return fread(buf, 1, to_read, self->file);
	}
	st File::write(const char* buf, st size)
	{
		return fwrite(buf, 1, size, self->file);
	}

	char* File::read_line()
	{
		long start = ftell(self->file);
		char c;
		st r = FALSE;
		st end = FALSE;
		do
		{
			c = fgetc(self->file);
			if(c == '\r')
			{
				char c2 = fgetc(self->file);
				if(c2 == '\n')
					r = TRUE;
				else
					fseek(self->file, -1, SEEK_CUR);
				break;
			}
			if(c == '\n')
				break;
			if(c == EOF)
			{
				end = TRUE;
				break;
			}
		} while(TRUE);

		long current = ftell(self->file);
		st len = current - start;
		if(!end)
		{
			if(r)
				len -= 2;
			else
				len -= 1;
		}

		if(len > 0)
		{
			char* buf = cl_alloc_type_with_count(char, len + 1);
			if(buf == NULL) return NULL;

			fseek(self->file, start, SEEK_SET);

			if(end)
				read(buf, len);
			else
				read(buf, len + 1);

			buf[len] = 0;
			if(r) fgetc(self->file);
			return buf;
		}
		else
			return NULL;
	}

	st File::write_line(const char* line)
	{
		st len = strlen(line);
		st w = write(line, len);
#if !CL_IS_MAC
		fputc('\r', self->file);
#endif
		fputc('\n', self->file);
		return w;
	}

	void File::seek(s64 position)
	{
		fseek(self->file, (long)position, SEEK_SET);
	}
	s64 File::size()
	{
		return 0;
	}


	File* File::open_file(const char* file)
	{
		File* f = cl_new_init(File);
		if(f->open(file))
			return f;
		else
		{
			cl_delete(f);
			return NULL;
		}
	}
	void File::close_file(File* f)
	{
		f->close();
		f->self->file = NULL;
		cl_delete(f);
	}
	st File::is_exist_file(const char* file)
	{
		//WIN32_FIND_DATA find_data;
		//HANDLE find_ret = FindFirstFileA(path_, &find_data);
		//if(find_ret == INVALID_HANDLE_VALUE)
		//{
		//	*ret = FALSE;
		//}
		//else
		//{
		//	if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		//		*ret = FALSE;
		//	else
		//		*ret = TRUE;
		//}
		return FALSE;
	}
	void File::delete_file(const char* file)
	{
		//DeleteFileA
	}
	st File::create_file(const char* file)
	{
		return FALSE;
	}
}