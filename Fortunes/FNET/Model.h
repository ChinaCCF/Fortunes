#include <CL/CL.h>

#pragma once

namespace Fortunes
{

	class VMInfo
	{
	public:
		enum VMStatus
		{
			Running,
			Stop,
			Disable
		};
		char* id;
		char* name;
		char* sys;
		char* cpu;
		char* mem;

		char* ip;
		st port;
		char* pwd;

		VMStatus status;
		VMInfo* next;
	};

	class NetUtil
	{
	public:
		static st login(const char* host, const char* user, const char* pwd);
		static VMInfo* get_list();
	};
}