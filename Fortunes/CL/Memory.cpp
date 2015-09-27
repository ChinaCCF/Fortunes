#include <CL/Memory.h>
#include <stdlib.h>

namespace cl
{
	void* MemoryUtil::alloc(st size)
	{
		return ::malloc(size);
	}
	void* MemoryUtil::realloc(void* p, st size)
	{
		return ::realloc(p, size);
	}
	void MemoryUtil::free(void* p)
	{
		::free(p);
	}

	void MemoryUtil::copy(void* dst, void* src, st size)
	{
		::memcpy(dst, src, size);
	}
	void MemoryUtil::zero(void* buf, st size)
	{
		::memset(buf, 0, size);
	}

	
#if CL_IS_DEBUG
#include <stdio.h>
#include <string.h>
#if !CL_IS_MAC
#include <Windows.h>
#endif

	class MemoryNode
	{
	public:
		const char* file_;
		st line_;
		void* p_;
		st size_;
		MemoryNode* next_;
	public:
		MemoryNode()
		{
			file_ = NULL;
			line_ = 0;
			p_ = NULL;
			size_ = 0;
			next_ = NULL;
		}
	};

#define in_new(T) (new(MemoryUtil::alloc(sizeof(T))) T)

	class MemoryList
	{
	public:
		MemoryNode* head_;
	public:
		MemoryList()
		{
			head_ = NULL;
		}
		void push(void* p, const char* file, st line, st size)
		{
			MemoryNode* node = in_new(MemoryNode);
			node->p_ = p;
			node->file_ = file;
			node->line_ = line;
			node->size_ = size;
			if(head_)
				node->next_ = head_;
			head_ = node;
		}
		MemoryNode* search(void* p)
		{
			MemoryNode* node = head_;
			MemoryNode* pre = NULL;
			u64 p_val = (u64)p;
			while(node)
			{
				u64 node_p_val = (u64)node->p_;
				if(p_val == node_p_val)
				{
					if(pre)
						pre->next_ = node->next_;
					else
						head_ = node->next_;
					return node;
				}
				else
				{
					pre = node;
					node = node->next_;
				}
			}
			return NULL;
		}
	};

	/*****************************************************************/
	static MemoryList* list = NULL;

	inline void push(void* p, const char* file, st line, st size)
	{
		if(list == NULL)
			list = in_new(MemoryList);
		list->push(p, file, line, size);
	}

	void* MemoryUtil::_alloc(st size, const char* file, st line)
	{
		void* p = alloc(size);
		push(p, file, line, size);
		return p;
	}
	void* MemoryUtil::_realloc(void* p, st size, const char* file, st line)
	{
		unmark(p);
		void* np = realloc(p, size);
		push(np, file, line, size);
		return np;
	}
	void MemoryUtil::_free(void* p)
	{
		unmark(p);
		free(p);
	}
	void MemoryUtil::unmark(void *p)
	{
		if(list)
		{
			MemoryNode* node = list->search(p);
			delete node;
		}
	}
	void MemoryUtil::check()
	{
		if(list)
		{
			MemoryNode* node = list->head_;
			MemoryNode* tmp = NULL;
			list->head_ = NULL;
			while(node)
			{
#if CL_IS_MAC
				const char* file_name = strrchr(node->file_, '/');
#else
				const char* file_name = strrchr(node->file_, '\\');
#endif
				if(file_name == NULL)
					file_name = node->file_;
				else
					file_name += 1;
#if CL_IS_MAC
				printf("Memory leak in File: %s,   Line: %d,   Size: %d\n", file_name, node->line_, node->size_);
#else
				char buf[128];
				_snprintf_s(buf, 128, "Memory leak in File: %s,   Line: %d,   Size: %d\n", file_name, node->line_, node->size_);
				OutputDebugStringA(buf);
#endif
				tmp = node;
				node = node->next_;
				delete tmp;
			}
		}
	}
#endif
}