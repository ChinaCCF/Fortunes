int urlRequest(const wchar_t* url,
			   const wchar_t* method,
			   const wchar_t** headers,
			   unsigned long cnt,
			   const char* content,
			   unsigned long len,
			   unsigned long* code,
			   char** rep,
			   int* rep_len); 