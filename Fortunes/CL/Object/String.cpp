#include <CL/Object/String.h>
#include <stdarg.h>
#include <stdio.h>
#if !CL_IS_MAC
#include <Windows.h>
#endif
#include <CL/Memory.h>

namespace cl
{
#if CL_IS_DEBUG
	char* StringUtil::alloc(const char* str, const char* file, st line)
#else
	char* StringUtil::alloc(const char* str)
#endif
	{
		st len = strlen(str) + 1;
#if CL_IS_DEBUG
		char* buf = (char*)cl::MemoryUtil::_alloc(len, file, line);
#else
		char* buf = (char*)cl::MemoryUtil::alloc(len);
#endif
		if(buf == NULL) return buf;
		memcpy(buf, str, len);
		return buf;
	}

	static inline char char_for(u8 val) { const char* str = "0123456789ABCDEF"; return str[val]; }
	static inline void to_hex(void* buf, st size, char* str)
	{
		u8* p = (u8*)buf;
		for(st i = 0; i < size; ++i)
		{
			str[2 * i + 1] = char_for(p[i] & 0xF);
			str[2 * i] = char_for((p[i] & 0xF0) >> 4);
		}
	}
#if CL_IS_DEBUG
	char* StringUtil::alloc_from_memory(void* buf, st size, const char* file, st line)
#else
	char* StringUtil::alloc_from_memory(void* buf, st size)
#endif
	{
#if CL_IS_DEBUG
		char* tmp = (char*)MemoryUtil::_alloc(size * 2 + 1, file, line);
#else
		char* tmp = (char*)MemoryUtil::alloc(size * 2 + 1);
#endif
		if(tmp)
		{
			to_hex(buf, size, tmp);
			tmp[size * 2] = 0;
		}
		return tmp;
	}
	static inline u8 hex_char_value(char c)
	{
		if(StringUtil::is_number(c))
			return c - '0';
		else
			return StringUtil::lower(c) - 'a' + 10;
	}
	static inline u8 to_memory(const char* str)
	{
		u8 val = hex_char_value(*str) << 4;
		++str;
		val += hex_char_value(*str);
		return val;
	}

#if CL_IS_DEBUG
	void* StringUtil::memory_alloc_from(const char* file, st line, const char* str, st len)
#else
	void* StringUtil::memory_alloc_from(const char* str, st len)
#endif
	{
		if(len == 0) len = length(str);
#if CL_IS_DEBUG
		void* buf = MemoryUtil::_alloc(len / 2, file, line);
#else
		void* buf = MemoryUtil::alloc(len / 2);
#endif
		if(buf)
		{
			u8* p = (u8*)buf;
			for(st i = 0; i < len; ++i)
			{
				p[i] = to_memory(str);
				str += 2;
			}
		}
		return buf;
	}


	st StringUtil::length(const char* str)
	{
		return strlen(str);
	}
	st StringUtil::copy(char* buf, st buf_size, const char* str)
	{
		st len = strlen(str);
		if(len >= buf_size) len = buf_size - 1;
		memcpy(buf, str, len);
		buf[len] = 0;
		return len + 1;
	}
	void StringUtil::append(char* buf, st buf_size, const char* str)
	{
		st buf_str_len = strlen(buf);
		st str_len = strlen(str);
		st len = buf_size - buf_str_len - 1;
		if(len > str_len) len = str_len;
		memcpy(buf + buf_str_len, str, len);
		buf[buf_str_len + len] = 0;
	}


	st StringUtil::compare(const char* str1, const char* str2)
	{
		return strcmp(str1, str2) == 0;
	}
	st StringUtil::compare(const char* str1, const char* str2, st len)
	{
		return strncmp(str1, str2, len) == 0;
	}

	st StringUtil::compare_without_case(const char* str1, const char* str2, st len)
	{
		char c1, c2;
	Label_Do:
		if(len <= 0) return FALSE;
		c1 = lower(*str1++);
		c2 = lower(*str2++);
		if(c1 == 0) { if(c2 == 0) return TRUE; return FALSE; }
		else { if(c2 == 0) return FALSE; }
		if(c1 != c2) return FALSE;
		--len;
		goto Label_Do;
	}

	char* StringUtil::search(char* str, char c)
	{
		return strchr(str, c);
	}
	const char* StringUtil::search(const char* str, char c)
	{
		return strchr(str, c);
	}
	st StringUtil::format(char* buf, st buf_size, const char* format_str, ...)
	{
		va_list list;
		va_start(list, format_str);
#if CL_IS_MAC
		st len = vsnprintf(buf, buf_size, format_str, list);
#else
		st len = vsnprintf_s(buf, buf_size, buf_size - 1, format_str, list);
#endif
		va_end(list);
		return len;
	}

	st StringUtil::char_to_wchar_count(const char* str)
	{
		return MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	}
	st StringUtil::wchar_to_char_count(const wchar* str)
	{
		return WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	}

	st StringUtil::char_to_wchar(wchar* buf, st buf_count, const char* str)
	{
		st count = char_to_wchar_count(str);
		if(count > buf_count) return FALSE;
		MultiByteToWideChar(CP_ACP, 0, str, -1, buf, buf_count);
		return TRUE;
	}
	st StringUtil::wchar_to_char(char* buf, st buf_count, const wchar* str)
	{
		st count = wchar_to_char_count(str);
		if(count > buf_count) return FALSE;
		WideCharToMultiByte(CP_ACP, 0, str, -1, buf, buf_count, NULL, NULL);
		return TRUE;
	}

#if CL_IS_DEBUG
	char* StringUtil::alloc_chars_for_wchars(const wchar* str, const char* file, st line)
#else
	char* StringUtil::alloc_chars_for_wchars(const wchar* str)
#endif
	{
		st len = wchar_to_char_count(str);
#if CL_IS_DEBUG
		char* buf = (char*)MemoryUtil::_alloc(len, file, line);
#else
		char* buf = (char*)MemoryUtil::alloc(len);
#endif
		if(!wchar_to_char(buf, len, str)) { cl_free(buf); return NULL; }
		return buf;
	}

#if CL_IS_DEBUG
	wchar* StringUtil::alloc_wchars_for_chars(const char* str, const char* file, st line)
#else
	wchar* StringUtil::alloc_wchars_for_chars(const char* str)
#endif
	{
		st len = char_to_wchar_count(str);
#if CL_IS_DEBUG
		wchar* buf = (wchar*)MemoryUtil::_alloc(len * sizeof(wchar), file, line);
#else
		wchar* buf = (wchar*)MemoryUtil::alloc(len * sizeof(wchar));
#endif
		if(!char_to_wchar(buf, len, str)) { cl_free(buf); return NULL; }
		return buf;
	}

	st StringUtil::parse_hex(const char* str, st* val)
	{
		if(*str == '0')
		{
			++str;
			if(*str == 'x' || *str == 'X')
			{
				++str;
				const char* org = str;
				*val = 0;
			Label_Do:
				if(is_number(*str))
				{
					*val <<= 4;
					*val += *str - '0';
					++str;
				}
				else
				{
					char c = *str;
					c = lower(c);
					if(c >= 'a' && c <= 'f')
					{
						*val <<= 4;
						*val += c - 'a' + 10;
						++str;
					}
					else
					{
						if(str == org) return 0;
						return str - org + 2;
					}
				}
				goto Label_Do;
			}
			else
				return 0;
		}
		else
			return 0;
	}
	st StringUtil::parse_dec(const char* str, st* val)
	{
		st negative = FALSE;
		if(*str == '-')
		{
			negative = TRUE;
			++str;
		}
		const char* org = str;
		*val = 0;
	Label_Do:
		if(is_number(*str))
		{
			*val *= 10;
			*val += *str - '0';
			++str;
			goto Label_Do;
		}
		else
		{
			if(str == org) return 0;
			if(negative)
			{
				*val = -*val;
				return str - org + 1;
			}
			else
				return str - org;
		}
	}
	st StringUtil::parse_bin(const char* str, st* val)
	{
		const char* org = str;
		*val = 0;
	Label_Do:
		if(*str == '0' || *str == '1')
		{
			*val <<= 1;
			*val += *str - '0';
			++str;
			goto Label_Do;
		}
		else
			return str - org;
	}
	st StringUtil::parse_value(const char* str, st* val)
	{
		st len = parse_hex(str, val);
		if(len == 0)
			return parse_dec(str, val);
		return len;
	}
	static inline ft pow(ft val, ft power, st time)
	{
		for(st i = 0; i < time; ++i)
			val *= power;
		return val;
	}
	st StringUtil::parse_float(const char* str, ft* _val)
	{
		st val;
		const char* org = str;
		st len = parse_dec(str, &val);
		if(len == 0) return 0;
		*_val = val;
		str += len;
		if(*str == '.')
		{
			++str;
			len = parse_dec(str, &val);
			if(len == 0) return 0;
			str += len;
			if(*_val >= 0)
				*_val += pow(val, 0.1, len);
			else
				*_val -= pow(val, 0.1, len);
		}

		if(*str == 'e' || *str == 'E')
		{
			++str;
			st negative = FALSE;
			if(*str == '+') ++str;
			if(*str == '-') { negative = TRUE; ++str; }
			len = parse_dec(str, &val);
			if(len == 0) return 0;
			str += len;
			if(negative)
				*_val = pow(*_val, 0.1, val);
			else
				*_val = pow(*_val, 10, val);
		}
		return str - org;
	}
	st StringUtil::is_float_string(const char* str)
	{
		if(str == NULL || *str == 0) return FALSE;
		if(*str == '+' || *str == '-') ++str;
		while(is_number(*str)) ++str;
		if(*str == '.')
		{
			++str;
			if(is_number(*str)) return TRUE;
			return FALSE;
		}
		if(*str == 'e' || *str == 'E')
		{
			++str;
			if(*str == '+' || *str == '-') ++str;
			if(is_number(*str)) return TRUE;
			return FALSE;
		}
		return FALSE;
	}
	static inline uchar parse_hex_c(char c)
	{
		if(StringUtil::is_number(c))
			return c - '0';
		else
			return StringUtil::lower(c) - 'a' + 10;
	}

	wchar StringUtil::parse_uchar(const char* str)
	{
		if(*str == '\\')
		{
			++str;
			if(*str == 'u' || *str == 'U')
			{
				++str;
				wchar c;
				uchar p[2];

				p[0] = parse_hex_c(*str++) << 4;
				p[0] += parse_hex_c(*str++);
				p[1] = parse_hex_c(*str++) << 4;
				p[1] += parse_hex_c(*str++);
				cl::MemoryUtil::big_endian_get(p, (s16*)&c);
				return c;
			}
		}
		return 0;
	}
	st StringUtil::parse_uchar_string(const char* str, wchar** result)
	{
		st len = 0;
		const char* org = str;
		do
		{
			if(*str != '\\') break;
			++str;
			if(*str != '\u') break;
			str += 5;
			len += 1;
		} while(TRUE);
		if(len == 0) return 0;

		wchar* r = cl_alloc_type_with_count(wchar, len + 1);
		str = org;
		for(st i = 0; i < len; ++i)
		{
			r[i] = parse_uchar(str);
			str += 6;
		}
		r[len] = 0;
		*result = r;
		return str - org;
	}

	/**************************************************************************************/
	//String
	/**************************************************************************************/
	/*class _String
	{
	public:
	st len;
	char* str;
	};

	st String::init()
	{
	self = cl_alloc_type(_String);
	if(self == NULL) return FALSE;
	self->str = NULL;
	return TRUE;
	}

	void String::uninit()
	{
	if(self) cl_free(self->str);
	cl_free(self);
	}*/

}