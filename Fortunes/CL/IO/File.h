#include <CL/Memory.h>

#ifndef __CL_IO_File__
#define __CL_IO_File__

namespace cl
{
	class _File;
	class File
	{
		_File* self;
	public:
		st init();
		void uninit();

		st open(const char* file);
		void close();

		st read(char* buf, st to_read);
		st write(const char* buf, st size);

		char* read_line();
		st write_line(const char* line);

		void seek(s64 position);
		s64 size();

		static File* open_file(const char* file);
		static void close_file(File* f);
		static st is_exist_file(const char* file);
		static void delete_file(const char* file);
		static st create_file(const char* file);
	};
}
#endif