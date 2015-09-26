#include <CL/Object/Object.h>

#ifndef __CL_Object_Array__
#define __CL_Object_Array__

namespace cl
{
	template<typename T>
	class Array :public virtual Object
	{
		T* items_;
		ut count_;
		ut size_;

		void expand()
		{
			if(count_ >= size_)
			{
				size_ *= 2;
				items_ = cl_realloc_ntype(items_, T, size_);
			}
		}
	public:
		Array(ut size = 8) { size_ = size; count_ = 0; items_ = cl_alloc_ntype(T, size); }
		~Array() { free(items_); }

		ut count() { return count_; }
		void clear() { count_ = 0; }
		void add(T val)
		{
			expand();
			items_[count_++] = val;
		}
		void insert(T val, ut index)
		{
			expand();
			for(ut i = count_; i > index; --i)
				items_[i] = items_[i - 1];
			items_[index] = val;
			count_++;
		}

		void remove_last() { count_--; }
		void remove_at(ut index)
		{
			for(ut i = index; i < count_ - 1; i++)
				items_[i] = items_[i + 1];
			count_--;
		}
		void remove(T val)
		{
			for(ut i = 0; i < count_; i++)
				if(items_[i] == val)
					this->remove_at(i);
		}

		T operator[](ut index)
		{
			return items_[index];
		}
	};
}

#endif