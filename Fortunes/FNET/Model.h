#include <CL/CL.h>
#include <CL/Memory.h>
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
		st init()
		{
			id = NULL;
			name = NULL;
			sys = NULL;
			cpu = NULL;
			mem = NULL;
			ip = NULL;
			pwd = NULL;
			next = NULL;
			return TRUE;
		}
		void uninit() {
			cl_free(id);
			cl_free(name);
			cl_free(sys);
			cl_free(cpu);
			cl_free(mem);
			cl_free(ip);
			cl_free(pwd);
			//cl_free(next);
		}
	};

	class NetUtil
	{
	public:
		static st login(const char* host, const char* user, const char* pwd);
		static VMInfo* get_list();
	};
}