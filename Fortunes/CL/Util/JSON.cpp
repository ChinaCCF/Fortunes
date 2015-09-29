#include <CL/Util/JSON.h>
#include <CL/Object/String.h>
#include <CL/Memory.h>

namespace cl
{
	class _JSON
	{
	public:
		JSON::Type type;
		st val;
		StringInfo str_val;
		ft ft_val;
	};

	st parse_string(const char* str, StringInfo* info)
	{
		const char* org = str;
		++str; //skip "
	Label_Search:
		const char* end = StringUtil::search(str, '"');
		if(end == NULL) return 0;
		if(*(end - 1) == '\\')
		{
			str = end + 1;
			goto Label_Search;
		}
		info->length = end - org - 1;
		info->string = cl_alloc_type_with_count(char, info->length + 1);

		if(info->string == NULL) return 0;
		st index = 0;
		str = org + 1;
	Label_Copy:
		if(*str == '\\')
		{
			++str;
			switch(*str)
			{
				case '"':{
						info->string[index++] = '"'; ++str;
						goto Label_Copy;
					}
				case '\\':{
						info->string[index++] = '\\'; ++str;
						goto Label_Copy;
					}
				case '/':{
						info->string[index++] = '/'; ++str;
						goto Label_Copy;
					}
				case 'b':{
						info->string[index++] = '\b'; ++str;
						goto Label_Copy;
					}
				case 'f':{
						info->string[index++] = '\f'; ++str;
						goto Label_Copy;
					}
				case 'n':{
						info->string[index++] = '\n'; ++str;
						goto Label_Copy;
					}
				case 'r':{
						info->string[index++] = '\r'; ++str;
						goto Label_Copy;
					}
				case 't':{
						info->string[index++] = '\t'; ++str;
						goto Label_Copy;
					}
				case 'u':{
						wchar* wustr;
						str -= 1;
						str += StringUtil::parse_uchar_string(str, &wustr);
						st str_cnt = StringUtil::wchar_to_char_count(wustr);
						char* ustr = cl_alloc_type_with_count(char, str_cnt);
						StringUtil::wchar_to_char(ustr, str_cnt, wustr);
						cl_free(wustr);
						index += StringUtil::copy(info->string + index, info->length - index, ustr) - 1;
						cl_free(ustr);
					}
				default:
					cl_printf("error \\ convert char");
					cl_free(info->string);
					return 0;
					break;
			}
			goto Label_Copy;
		}
		else
		{
			if(*str != '"' && index < info->length)
			{
				info->string[index++] = *str++;
				goto Label_Copy;
			}
		}
		return end - org + 1;
	}

	st parse_object(const char* str, _JSON* obj)
	{
		return 0;
	}
	st parse_array(const char* str, _JSON* js)
	{
		return 0;
	}
	st parse_number(const char* str, _JSON* js)
	{
		if(StringUtil::is_number(*str))
		{
			st len;
			if(StringUtil::is_float_string(str))
			{
				len = StringUtil::parse_float(str, &js->ft_val);
				if(len > 0)	js->type = JSON::Float;
			}
			else
			{
				len = StringUtil::parse_dec(str, &js->val);
				if(len > 0)	js->type = JSON::Int;
			}
			return len;
		}
		return 0;
	}
	st parse_value(const char* str, _JSON* js)
	{
		if(str == NULL || *str == 0) return 0;
		const char* org = str;
		str += StringUtil::skip_blank_char(str);
		st dif = str - org;

		if(cl::StringUtil::compare_without_case(str, "null", 4))
		{
			js->type = JSON::Null;
			return 4 + dif;
		}
		if(cl::StringUtil::compare_without_case(str, "false", 5))
		{
			js->type = JSON::Bool;
			js->val = FALSE;
			return 5 + dif;
		}
		if(cl::StringUtil::compare_without_case(str, "true", 4))
		{
			js->type = JSON::Bool;
			js->val = TRUE;
			return 4 + dif;
		}
		if(*str == '"')
		{
			st len = parse_string(str, &js->str_val);
			if(len > 0)
			{
				js->type = JSON::String;
				return len + dif;
			}
			return 0;
		}
		if(*str == '-')
		{
			++str;
			st len = parse_number(str, js);
			if(len > 0)
			{
				if(js->type == JSON::Float) js->ft_val = -js->ft_val;
				else js->val = -js->val;
				return len + 1 + dif;
			}
			return 0;
		}
		if(StringUtil::is_number(*str))
			return parse_number(str, js) + dif;
		if(*str == '[')
			return parse_array(str, js) + dif;
		if(*str == '{')
			return parse_object(str, js) + dif;
		return 0;
	}

	st JSON::init()
	{
		self = cl_alloc_type(_JSON);
		if(self == NULL) return FALSE;
		return 0;
	}
	void JSON::uninit()
	{
		switch(self->type)
		{
			case JSON::Array:
			case JSON::Object:
			case JSON::String:
			default:
				break;
		}
	}

	st JSON::parse(const char* str)
	{
		return 0;
	}
	const char* JSON::to_string()
	{
		return NULL;
	}

	const char* JSON::value_of_string()
	{
		return NULL;
	}
	st JSON::value_of_int()
	{
		return 0;
	}
	ft JSON::value_of_float()
	{
		return 0;
	}
	st JSON::value_of_bool()
	{
		return 0;
	}

	st JSON::size()
	{
		return 0;
	}
	JSON* JSON::operator[](st index)
	{
		return NULL;
	}
	JSON* JSON::operator[](const char* name)
	{
		return NULL;
	}
}