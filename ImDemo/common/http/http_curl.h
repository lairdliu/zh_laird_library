/***************************************************************************
  * file name http_curl.h
  * brief http传输文件
  * date 2019/01/31
  *************************************************************************/

#ifndef CMCC_IM_HTTP_CURL_H_
#define CMCC_IM_HTTP_CURL_H_

#include <string>
#include <curl/curl.h>

using namespace std;

namespace cmhi_iov{

int http_get_url_by_md5(const string &url, const string &md5, string &response);

int Http_Post_File(const char *url, const char *param, const char *filename,std::string &response,
	void *progress_data,
	int (*cb)(void *clientp, double dltotal, double dlnow,  double ultotal, double ulnow));

int Http_Get_File(const char *url, const char *filename); 

int Http_Get_Request(const char* url, std::string& buffer, std::string& error_info);

static int writer(char*, size_t, size_t, std::string*);

static bool init(CURL*& curl, const char* url, std::string error_info, std::string& p_buffer);


}


#endif

