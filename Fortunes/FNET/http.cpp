#include "http.h"
#include <windows.h>
#include <winhttp.h> 
#pragma comment(lib, "Winhttp.lib") 

int initURL(const wchar_t* str, wchar_t** host, wchar_t** path, int* port, int* is_ssl)
{
	URL_COMPONENTS url;
	memset(&url, 0, sizeof(URL_COMPONENTS));

	url.dwExtraInfoLength = 1;
	url.dwHostNameLength = 1;
	url.dwPasswordLength = 1;
	url.dwSchemeLength = 1;
	url.dwStructSize = sizeof(URL_COMPONENTS);
	url.dwUrlPathLength = 1;
	url.dwUserNameLength = 1;

	BOOL ret = WinHttpCrackUrl(str, 0, NULL, &url);
	if(ret)
	{
		int host_len = sizeof(wchar_t) * url.dwHostNameLength;
		int path_len = sizeof(wchar_t) * url.dwUrlPathLength;

		*host = (wchar_t*)malloc(sizeof(wchar_t) + host_len);
		*path = (wchar_t*)malloc(sizeof(wchar_t) + path_len);
		*port = url.nPort;

		memcpy(*host, url.lpszHostName, host_len);
		memcpy(*path, url.lpszUrlPath, path_len);

		(*host)[url.dwHostNameLength] = 0;
		(*path)[url.dwUrlPathLength] = 0;

		if(is_ssl)
			*is_ssl = (url.nScheme == INTERNET_SCHEME_HTTPS);
	}
	return ret;
}

int urlRequest(const wchar_t* url,
			   const wchar_t* method,
			   const wchar_t** headers,
			   unsigned long cnt,
			   const char* content,
			   unsigned long len,
			   unsigned long* code,
			   char** rep,
			   int* rep_len)
{
	wchar_t* host = NULL;
	wchar_t* path = NULL;
	HINTERNET session = NULL;
	HINTERNET connect = NULL;
	HINTERNET req = NULL;
	*rep = NULL;
	int port;
	int ssl;

	BOOL ret = initURL(url, &host, &path, &port, &ssl);
	if(!ret)
		return FALSE;

	session = WinHttpOpen(L"CCF_REQ",
						  WINHTTP_ACCESS_TYPE_NO_PROXY,
						  NULL,
						  NULL,
						  0);
	ret = (session != NULL);
	if(!ret)
		goto Label_End;

	connect = WinHttpConnect(session, host, port, 0);
	ret = (connect != NULL);
	if(!ret)
		goto Label_End;

	req = WinHttpOpenRequest(connect,
							 method,
							 path,
							 NULL,
							 WINHTTP_NO_REFERER,
							 WINHTTP_DEFAULT_ACCEPT_TYPES,
							 ssl ? WINHTTP_FLAG_SECURE : 0);
	ret = (req != NULL);
	if(!ret)
		goto Label_End;

	while(cnt > 0)
	{
		cnt--;
		ret = WinHttpAddRequestHeaders(req, headers[cnt], (DWORD)-1, WINHTTP_ADDREQ_FLAG_ADD & WINHTTP_ADDREQ_FLAG_REPLACE);
		if(!ret)
			goto Label_End;
	}

	if(content)
	{
		if(len == 0)
			len = strlen(content);
	}
	else
		len = 0;

	ret = WinHttpSendRequest(req, NULL, 0, NULL, 0, len, 0);
	if(!ret)
		goto Label_End;
	 
	while(len)
	{
		ret = WinHttpWriteData(req, content, len, &cnt);
		if(!ret)
			goto Label_End;
		len -= cnt;
		cnt = 0;
	}

	ret = WinHttpReceiveResponse(req, NULL);
	if(!ret)
		goto Label_End;

	cnt = sizeof(DWORD);
	ret = WinHttpQueryHeaders(req, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, NULL, code, &cnt, NULL);
	if(!ret)
		goto Label_End; 

	*rep_len = 0;
	do
	{
		ret = WinHttpQueryDataAvailable(req, &len);
		if(!ret)
			goto Label_Err;

		if(len == 0)
			break;

		int buf_size = len + *rep_len + 1;
		char* buf = (char*)malloc(buf_size);
		if(buf == NULL)
			goto Label_Err;

		ret = WinHttpReadData(req, (void*)(buf + *rep_len), len, &cnt);
		if(!ret)
		{
			free(buf);
			goto Label_Err;
		}
		if(buf != *rep && *rep != NULL)
		{
			memcpy(buf, *rep, *rep_len);
			free(*rep);
		}
		*rep = buf;
		*rep_len = buf_size - 1;
		continue;

	Label_Err:
		if(*rep)
		{
			free(*rep);
			break;
		}
	} while(true);

	(*rep)[*rep_len] = 0;

Label_End:
	free(host);
	free(path);

	if(session)
		WinHttpCloseHandle(session);

	if(connect)
		WinHttpCloseHandle(connect);

	if(req)
		WinHttpCloseHandle(req); 
	return (int)ret;
}