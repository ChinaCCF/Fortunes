#include <FNET/Model.h>
#include <FNET/cJSON.h>
#include <FNET/http.h>
#include <CL/Object/String.h>
#include <CL/Memory.h>

namespace Fortunes
{
	static char g_token[64];
	static char g_host[256];

	cJSON* do_login(const char* _interface, const char* _content)
	{
		const wchar_t* headers[] = {L"Content-Type:application/json\r\n"};

		char buf[128];
		wchar url[128];

		cl::StringUtil::string_format(buf, 128, "http://%s/v2/logon", g_host);
		cl::StringUtil::char_to_wchar(url, 128, buf);

		char* response;
		int len;
		unsigned long code;
		int ret = urlRequest(url, L"POST", headers, 1, _content, 0, &code, &response, &len);

		if(ret)
		{
			if(code == 200)
			{
				cJSON* root = cJSON_Parse(response);
				free(response);
				return root;
			}
			else
			{
				free(response);
				return NULL;
			}
		}
		return NULL;
	}
	st NetUtil::login(const char* host, const char* user, const char* pwd)
	{
		cJSON* root = cJSON_CreateObject();
		cJSON_AddItemToObject(root, "username", cJSON_CreateString(user));
		cJSON_AddItemToObject(root, "password", cJSON_CreateString(pwd));
		char* str = cJSON_PrintUnformatted(root);

		cl::StringUtil::string_copy(g_host, 256, host);

		cJSON* ret = do_login("/v2/logon", str);

		cJSON_Delete(root);
		free(str);

		if(ret == NULL)	return FALSE;

		cJSON* tokenItem = cJSON_GetObjectItem(ret, "token");
		cl::StringUtil::string_copy(g_token, 64, tokenItem->valuestring);
		cJSON_Delete(ret);
		return TRUE;
	}


	cJSON* do_get_list()
	{
		wchar header[128];
		const wchar_t* arr[1];
		arr[0] = header;

		wchar url[128];
		char buf[128];

		cl::StringUtil::string_format(buf, 128, "Authorization: %s\r\n", g_token);
		cl::StringUtil::char_to_wchar(header, 128, buf);

		cl::StringUtil::string_format(buf, 128, "http://%s/v2/instances", g_host);
		cl::StringUtil::char_to_wchar(url, 128, buf);

		char* response;
		int len;
		unsigned long code;
		int ret = urlRequest(url, L"GET", arr, 1, NULL, 0, &code, &response, &len);

		if(ret)
		{
			if(code == 200)
			{
				cJSON* root = cJSON_Parse(response);
				cl_free(response);
				return root;
			}
			else
			{
				cl_free(response);
				return NULL;
			}
		}
		return NULL;
	}

	VMInfo* NetUtil::get_list()
	{
		cJSON* root = do_get_list();
		if(root == NULL) return NULL;

		st size = cJSON_GetArraySize(root);
		if(size == 0) goto Label_Err;

		VMInfo* infos = cl_alloc_type_with_count(VMInfo, size);
		if(infos == NULL) goto Label_Err;
		memset(infos, 0, sizeof(infos) * size);
		for(st i = 0; i < size - 1; ++i)
			infos[i].next = infos + i + 1;
		infos[size - 1].next = NULL;

		for(st i = 0; i < size; ++i)
		{
			cJSON* item = cJSON_GetArrayItem(root, i);

			infos[i].id = cl_string_alloc(cJSON_GetObjectItem(item, "id")->valuestring);
			infos[i].name = cl_string_alloc(cJSON_GetObjectItem(item, "name")->valuestring); //may be utf8

			const char* str = cJSON_GetObjectItem(item, "status")->valuestring;
			if(cl::StringUtil::string_compare(str, "stop"))
				infos[i].status = VMInfo::Stop;
			else
			{
				if(cl::StringUtil::string_compare(str, "running"))
					infos[i].status = VMInfo::Running;
				else
					infos[i].status = VMInfo::Disable;
			}

			char buf[64];

			cJSON* rc = cJSON_GetObjectItem(item, "resource");
			cJSON* vcpu = cJSON_GetObjectItem(rc, "vcpu");
			st cpu_v = cJSON_GetObjectItem(vcpu, "total")->valueint;

			cJSON* mem = cJSON_GetObjectItem(rc, "mem");
			st mem_v = cJSON_GetObjectItem(mem, "total")->valueint;

			cl::StringUtil::string_format(buf, 64, "%d 核", cpu_v);
			infos[i].cpu = cl_string_alloc(buf);

			cl::StringUtil::string_format(buf, 64, "%d MB", mem_v);
			infos[i].mem = cl_string_alloc(buf);

			cJSON* img = cJSON_GetObjectItem(item, "image");
			infos[i].sys = cl_string_alloc(cJSON_GetObjectItem(img, "name")->valuestring);

			if(infos[i].status == VMInfo::Running)
			{
				cJSON* gra = cJSON_GetObjectItem(item, "graphics");

				size = cJSON_GetArraySize(gra);
				for(st j = 0; j < size; ++j)
				{
					item = cJSON_GetArrayItem(gra, j);
					if(cl::StringUtil::string_compare(cJSON_GetObjectItem(item, "type")->valuestring, "spice"))
						break;
				}

				infos[i].ip = cl_string_alloc(cJSON_GetObjectItem(item, "host")->valuestring);
				infos[i].pwd = cl_string_alloc(cJSON_GetObjectItem(item, "password")->valuestring);
				infos[i].port = cJSON_GetObjectItem(item, "port")->valueint;
			}
		}
		cJSON_Delete(root);
		return infos;
Label_Err:
		cl_free(infos);
		cJSON_Delete(root);
		return NULL;
	}

	st action(const char* _id, st type)
	{
		wchar_t header1[128];
		wchar_t header2[128];
		const wchar_t* arr[2];
		arr[0] = header1;
		arr[1] = header2;

		wchar url[128];
		char buf[128];

		cl::StringUtil::string_format(buf, 128, "Authorization: %s\r\n", g_token);
		cl::StringUtil::char_to_wchar(header1, 128, buf);

		cl::StringUtil::string_copy(buf, 128, "Content-Type: application/json\r\n");
		cl::StringUtil::char_to_wchar(header2, 128, buf);

		cl::StringUtil::string_format(buf, 128, "http://%s/v2/instances/%s/action", g_host, _id);
		cl::StringUtil::char_to_wchar(url, 128, buf);


		cJSON* root = cJSON_CreateObject();
		const char* action_str;
		switch(type)
		{
			case -1:
				action_str = "poweroff";
				break;
			case 0:
				action_str = "shutdown";
				break;
			case 1:
				action_str = "start";
				break;
		}
		cJSON_AddItemToObject(root, "action", cJSON_CreateString(action_str));
		char* str = cJSON_PrintUnformatted(root);
		cJSON_Delete(root);

		char* response;
		int len;
		unsigned long code;
		int ret = urlRequest(url, L"POST", arr, 2, str, 0, &code, &response, &len);
		free(str);

		if(cl::StringUtil::string_compare(response, "null"))
			ret = TRUE;
		else
			ret = FALSE;
		free(response);
		return ret;
	}

	//isize remoteStopVM(const char* _id, isize is_force)
	//{
	//	if(is_force)
	//		return action(_id, 2);
	//	else
	//		return action(_id, 1);
	//}
	//isize remoteStartVM(const char* _id)
	//{
	//	return action(_id, 0);
	//}
}