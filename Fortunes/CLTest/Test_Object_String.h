#include <CLTest/TestCommon.h>
#include <CL/Object/String.h>

#pragma once 

namespace test
{
	namespace Object
	{
		namespace String
		{
			
			void test()
			{
				st ret = cl::StringUtil::is_alpha('a');
				CL_ASSERT(ret);
				ret = cl::StringUtil::is_alpha('A');
				CL_ASSERT(ret);
				ret = cl::StringUtil::is_alpha('z');
				CL_ASSERT(ret);
				ret = cl::StringUtil::is_alpha('Z');
				CL_ASSERT(ret);
				ret = cl::StringUtil::is_alpha(64); //@
				CL_ASSERT(!ret);
				ret = cl::StringUtil::is_alpha(91);//'['
				CL_ASSERT(!ret);
				ret = cl::StringUtil::is_alpha(96);//`
				CL_ASSERT(!ret);
				ret = cl::StringUtil::is_alpha(123);//{
				CL_ASSERT(!ret);

				ret = cl::StringUtil::is_number('0');
				CL_ASSERT(ret);
				ret = cl::StringUtil::is_number('9');
				CL_ASSERT(ret);
				ret = cl::StringUtil::is_number(47);// /
				CL_ASSERT(!ret);
				ret = cl::StringUtil::is_number(58);//:
				CL_ASSERT(!ret);

				st val;
				ret = cl::StringUtil::parse_hex("0",&val); 
				CL_ASSERT(ret == 0);
				ret = cl::StringUtil::parse_hex("0x", &val);
				CL_ASSERT(ret == 0);
				ret = cl::StringUtil::parse_hex("0xo", &val);
				CL_ASSERT(ret == 0);
				ret = cl::StringUtil::parse_hex("0x0", &val);
				CL_ASSERT(ret == 3);
				CL_ASSERT(val == 0);
				ret = cl::StringUtil::parse_hex("0x1x", &val);
				CL_ASSERT(ret == 3);
				CL_ASSERT(val == 1);
				ret = cl::StringUtil::parse_hex("0x01", &val);
				CL_ASSERT(ret == 4);
				CL_ASSERT(val == 1);
				ret = cl::StringUtil::parse_hex("0x020", &val);
				CL_ASSERT(ret == 5);
				CL_ASSERT(val == 32);
				ret = cl::StringUtil::parse_hex("0xF", &val);
				CL_ASSERT(ret == 3);
				CL_ASSERT(val == 15);
				ret = cl::StringUtil::parse_hex("0x0F0", &val);
				CL_ASSERT(ret == 5);
				CL_ASSERT(val == 15 * 16); 

				ret = cl::StringUtil::parse_dec("a", &val);
				CL_ASSERT(ret == 0); 
				ret = cl::StringUtil::parse_dec("0", &val);
				CL_ASSERT(ret == 1);
				CL_ASSERT(val == 0);
				ret = cl::StringUtil::parse_dec("1", &val);
				CL_ASSERT(ret == 1);
				CL_ASSERT(val == 1);
				ret = cl::StringUtil::parse_dec("01", &val);
				CL_ASSERT(ret == 2);
				CL_ASSERT(val == 1);
				ret = cl::StringUtil::parse_dec("123d", &val);
				CL_ASSERT(ret == 3);
				CL_ASSERT(val == 123);
				ret = cl::StringUtil::parse_dec("0d", &val);
				CL_ASSERT(ret == 1);
				CL_ASSERT(val == 0);
				ret = cl::StringUtil::parse_dec("-", &val);
				CL_ASSERT(ret == 0);
				ret = cl::StringUtil::parse_dec("-a", &val);
				CL_ASSERT(ret == 0);
				ret = cl::StringUtil::parse_dec("-0", &val);
				CL_ASSERT(ret == 2);
				CL_ASSERT(val == 0);
				ret = cl::StringUtil::parse_dec("-1", &val);
				CL_ASSERT(ret == 2);
				CL_ASSERT(val == -1);

				ret = cl::StringUtil::parse_bin("2", &val);
				CL_ASSERT(ret == 0);
				ret = cl::StringUtil::parse_bin("0", &val);
				CL_ASSERT(ret == 1);
				CL_ASSERT(val == 0);
				ret = cl::StringUtil::parse_bin("1", &val);
				CL_ASSERT(ret == 1);
				CL_ASSERT(val == 1);
				ret = cl::StringUtil::parse_bin("01", &val);
				CL_ASSERT(ret == 2);
				CL_ASSERT(val == 1);
				ret = cl::StringUtil::parse_bin("010", &val);
				CL_ASSERT(ret == 3);
				CL_ASSERT(val == 2);

				
				ret = cl::StringUtil::parse_value("0", &val);
				CL_ASSERT(ret == 1);
				CL_ASSERT(val == 0);
				ret = cl::StringUtil::parse_value("0a", &val);
				CL_ASSERT(ret == 1); 
				CL_ASSERT(val == 0);
				ret = cl::StringUtil::parse_value("01", &val);
				CL_ASSERT(ret == 2);
				CL_ASSERT(val == 1);
				ret = cl::StringUtil::parse_value("010", &val);
				CL_ASSERT(ret == 3);
				CL_ASSERT(val == 10);
				ret = cl::StringUtil::parse_value("0x1", &val);
				CL_ASSERT(ret == 3);
				CL_ASSERT(val == 1);
				ret = cl::StringUtil::parse_value("011", &val);
				CL_ASSERT(ret == 3);
				CL_ASSERT(val == 11);
				ret = cl::StringUtil::parse_value("023", &val);
				CL_ASSERT(ret == 3);
				CL_ASSERT(val == 23);
				ret = cl::StringUtil::parse_value("1", &val);
				CL_ASSERT(ret == 1);
				CL_ASSERT(val == 1);
				ret = cl::StringUtil::parse_value("123", &val);
				CL_ASSERT(ret == 3);
				CL_ASSERT(val == 123);

				ft fval;
				ret = cl::StringUtil::parse_float("0.1", &fval);
				CL_ASSERT(ret == 3);
				CL_ASSERT(abs(fval - 0.1) < 0.00000000001);

				ret = cl::StringUtil::parse_float("12.34", &fval);
				CL_ASSERT(ret == 5);
				CL_ASSERT(abs(fval - 12.34) < 0.00000000001);

				ret = cl::StringUtil::parse_float("-10.001", &fval);
				CL_ASSERT(ret == 7);
				CL_ASSERT(abs(fval + 10.001) < 0.00000000001);

				ret = cl::StringUtil::parse_float("10.12e2", &fval);
				CL_ASSERT(ret == 7);
				CL_ASSERT(abs(fval - 1012) < 0.00000000001);

				ret = cl::StringUtil::parse_float("10.12e4", &fval);
				CL_ASSERT(ret == 7);
				CL_ASSERT(abs(fval - 101200) < 0.0000000001);

				ret = cl::StringUtil::parse_float("-10.12e4", &fval);
				CL_ASSERT(ret == 8);
				CL_ASSERT(abs(fval + 101200) < 0.0000000001);

				ret = cl::StringUtil::parse_float("-10.12e-1", &fval);
				CL_ASSERT(ret == 9);
				CL_ASSERT(abs(fval + 1.012) < 0.0000000001);

				ret = cl::StringUtil::parse_float("-10.12e-1f", &fval);
				CL_ASSERT(ret == 9);
				CL_ASSERT(abs(fval + 1.012) < 0.0000000001);


				wchar c = cl::StringUtil::parse_uchar("\\u5c0f");
				CL_ASSERT(c == L'小'); 

				wchar* src = L"小说很好看";
				wchar* cs;
				st len = cl::StringUtil::parse_uchar_string("\\u5c0f\\u8bf4\\u5f88\\u597d\\u770b", &cs);
				CL_ASSERT(memcmp(src, cs, 10) == 0);
				CL_ASSERT(len == 30);
			}
		}
	}
}