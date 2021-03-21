/***************************************************************************
  * file http_curl.cc
  * brief http实现接口
  * date 2019/02/13
  *************************************************************************/

#include "http_curl.h"
#include "logger.h"
#include "formatter.h"
#include <time.h>

#include <sstream>
#include <iostream>
#include <string>

using namespace std;

namespace cmhi_iov
{

static int writer(char* data, size_t size, size_t nmemb, std::string* writer_data){
	unsigned long sizes = size * nmemb;
 
	if (NULL == writer_data)
	{
		return 0;
	}
 
	writer_data->append(data, sizes);
	
	return sizes;
}

static bool init(CURL*& curl, const char* url, string error_info, std::string& p_buffer){
	CURLcode code;

 	curl = curl_easy_init();
	if (NULL == curl)
	{
		std::cout << stderr <<  " Failed to create CURL connection" << std::endl;
		exit(EXIT_FAILURE);
	}

	code = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_info.c_str());
	if (code != CURLE_OK)
	{
		std::cout << stderr << " Failed to set error buffer " << code << std::endl;
		return false;
	}
 
	code = curl_easy_setopt(curl, CURLOPT_URL, url);
	if (code != CURLE_OK)
	{
		std::cout << stderr << " Failed to set URL " << error_info << std::endl;
		return false;
	}
 
	code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	if (code != CURLE_OK)
	{
		std::cout << stderr << " Failed to set redirect option " << error_info << std::endl;
		return false;
	}
 
	code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
	if (code != CURLE_OK)
	{
		std::cout << stderr << " Failed to set writer " << error_info << std::endl;
		return false;
	}
 
	code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &p_buffer);
	if (code != CURLE_OK)
	{
		std::cout << stderr << " Failed to set write data " << error_info << std::endl;
		return false;
	}
 
	return true;

}

// 0: success, -1:failed

//http get
int http_get_url_by_md5(const string &url, const string &md5, string &response)  
{  
    // init curl  
    CURL *curl = curl_easy_init();  
    // res code  
    CURLcode res;
    int ret = -1;
    string post_params = "md5=" + md5;
    if (curl)  
    {  
        // set params  
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post req  
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url  
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_params.c_str()); // params  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);  
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
        // start req
        res = curl_easy_perform(curl);
    }
    if(res != CURLE_OK){
        CCLog(kLogLevelDebug, "post geturlbymd5 error, res:%u", res);
    }
    else {
        int status = parse_status_from_resp(response);
        CCLog(kLogLevelDebug, "post geturlbymd5 success, response:%s, status:%d", 
        response.c_str(), status);
        if(600 == status) {
          ret = 0;
        }
        else {
          
        }
    }
    // release curl
    curl_easy_cleanup(curl);
    return ret;
}

int Http_Post_File(const char *url, const char *param, const char *filepath,std::string &response,
	void *progress_data,
	int (*cb)(void *clientp, double dltotal, double dlnow,  double ultotal, double ulnow)){

    int ret = -1;
    CURL *curl = NULL;
    CURLcode code;
    CURLFORMcode formCode;
    int timeout = 15;
    static const char buf[] = "Expect:"; 

#define CHECK_FORM_ERROR(x)                                        \
    if ((formCode = (x)) != CURL_FORMADD_OK)                      \
    {                                                              \
        fprintf(stderr, "curl_formadd[%d] error.\n", formCode);    \
        goto out;                                                 \
    }

#define CHECK_SETOPT_ERROR(x)                                      \
    if ((code = (x)) != CURLE_OK)                                 \
    {                                                              \
        fprintf(stderr, "curl_easy_setopt[%d] error.\n", code);    \
        goto all;                                                 \
    }

    struct curl_httppost *post=NULL;
    struct curl_httppost *last=NULL;
    struct curl_slist *headerlist=NULL;

    headerlist = curl_slist_append(headerlist, buf);
	printf("upload file ,param is %s\n",param);
    CHECK_FORM_ERROR( curl_formadd(&post, &last, CURLFORM_COPYNAME, "file",
        CURLFORM_FILE, filepath,
        CURLFORM_END));

    CHECK_FORM_ERROR( curl_formadd(&post, &last, CURLFORM_COPYNAME, "param",
        CURLFORM_COPYCONTENTS, param,
        CURLFORM_END));

    curl = curl_easy_init();
    if(curl == NULL)
    {
        fprintf(stderr, "curl_easy_init() error.\n");
        goto out;
    }

    CHECK_SETOPT_ERROR(curl_easy_setopt(curl, CURLOPT_HEADER, headerlist));
    CHECK_SETOPT_ERROR(curl_easy_setopt(curl, CURLOPT_URL, url));
    CHECK_SETOPT_ERROR(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer));
    CHECK_SETOPT_ERROR(curl_easy_setopt(curl, CURLOPT_WRITEDATA, (string *)&response));
    CHECK_SETOPT_ERROR(curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L));
    CHECK_SETOPT_ERROR(curl_easy_setopt(curl, CURLOPT_HTTPPOST, post));
    CHECK_SETOPT_ERROR(curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout));
	CHECK_SETOPT_ERROR(curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION,cb));
	CHECK_SETOPT_ERROR(curl_easy_setopt(curl, CURLOPT_PROGRESSDATA,progress_data));
	CHECK_SETOPT_ERROR(curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L));


    code = curl_easy_perform(curl);
    if(code != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform[%d] error.\n", code);
        goto all;
    }

    ret = 0;

all:
    curl_easy_cleanup(curl);
out:
    curl_formfree(post);

    curl_slist_free_all (headerlist); 

    return ret;
}

/*  libcurl write callback function */  
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {  
    size_t written = fwrite(ptr, size, nmemb, stream);  
    return written;  
}  


int Http_Get_File(const char *url, const char *filename) 
{
    
    CURL *curl;
    FILE *fp;
    CURLcode res;
    /*   调用curl_global_init()初始化libcurl  */
    res = curl_global_init(CURL_GLOBAL_ALL);
    if (CURLE_OK != res)
    {
        printf("init libcurl failed.");
        curl_global_cleanup();
        return -1;
    }
    /*  调用curl_easy_init()函数得到 easy interface型指针  */
    curl = curl_easy_init();    
    if (curl) {

        fp = fopen(filename, "wb"); 
	if (NULL == fp)
	{
		if (filename)
		{
			printf("open file %s failed\n",filename);
		}
		return -1;
	}
        
        /*  调用curl_easy_setopt()设置传输选项 */
        res = curl_easy_setopt(curl, CURLOPT_URL, url);                  
        if (res != CURLE_OK)
        {   
            fclose(fp);
            curl_easy_cleanup(curl);  
            return -1;
        }
        /*  根据curl_easy_setopt()设置的传输选项，实现回调函数以完成用户特定任务  */
        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);  
        if (res != CURLE_OK)
        {   
            fclose(fp);
            curl_easy_cleanup(curl);  
            return -1;
        }
        /*  根据curl_easy_setopt()设置的传输选项，实现回调函数以完成用户特定任务  */
        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        if (res != CURLE_OK)
        {   
            fclose(fp); 
            curl_easy_cleanup(curl);  
            return -1;
        }

        res = curl_easy_perform(curl);                               // 调用curl_easy_perform()函数完成传输任务
        fclose(fp); 
        /* Check for errors */ 
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
            curl_easy_cleanup(curl);  
            return -1;
        }
            
        /* always cleanup */
        curl_easy_cleanup(curl);                                     // 调用curl_easy_cleanup()释放内存 
        
    }
    curl_global_cleanup();
    return 0;

}

int Http_Get_Request(const char* url, std::string& buffer, std::string& error_info){


//    /*HTTP GET json data*/
//    std::stringstream out;
//    curl_global_init(CURL_GLOBAL_DEFAULT);
//    void* curl = curl_easy_init();
//    // 设置URL
//
//    int code = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_info.c_str());
//    if (code != CURLE_OK)
//    {
//        std::cout << stderr << " Failed to set error buffer " << code << std::endl;
//        return false;
//    }
//
//    code = curl_easy_setopt(curl, CURLOPT_URL, url);
//    if (code != CURLE_OK)
//    {
//        std::cout << stderr << " Failed to set URL " << error_info << std::endl;
//        return false;
//    }
//
//    code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
//    if (code != CURLE_OK)
//    {
//        std::cout << stderr << " Failed to set redirect option " << error_info << std::endl;
//        return false;
//    }
//
//    code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
//    if (code != CURLE_OK)
//    {
//        std::cout << stderr << " Failed to set writer " << error_info << std::endl;
//        return false;
//    }
//
//    code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
//    if (code != CURLE_OK)
//    {
//        std::cout << stderr << " Failed to set write data " << error_info << std::endl;
//        return false;
//    }
//
//    //curl_easy_setopt(curl, CURLOPT_URL, url);
//    // 设置接收数据的处理函数和存放变量
//    //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//    //curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
//
//
//    // 执行HTTP GET操作
//    CURLcode res = curl_easy_perform(curl);
//    if (res != CURLE_OK) {
//        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
//        error_info.append("Failed to get ");
//		error_info.append(url);
//    }
//
//    // 接受数据存放在out中，输出之
//    //buffer = out.str();
//    printf("%s",buffer.c_str());
//    curl_easy_cleanup(curl);
//    return 0;



	void *curl = NULL;
	CURLcode ret;

	curl_global_init(CURL_GLOBAL_DEFAULT);

    CURLcode code;

    curl = curl_easy_init();
    if (NULL == curl)
    {
        std::cout << stderr <<  " Failed to create CURL connection" << std::endl;
        exit(EXIT_FAILURE);
    }

    code = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_info.c_str());
    if (code != CURLE_OK)
    {
        std::cout << stderr << " Failed to set error buffer " << code << std::endl;
        return false;
    }

    code = curl_easy_setopt(curl, CURLOPT_URL, url);
    if (code != CURLE_OK)
    {
        std::cout << stderr << " Failed to set URL " << error_info << std::endl;
        return false;
    }

    code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    if (code != CURLE_OK)
    {
        std::cout << stderr << " Failed to set redirect option " << error_info << std::endl;
        return false;
    }

    code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    if (code != CURLE_OK)
    {
        std::cout << stderr << " Failed to set writer " << error_info << std::endl;
        return false;
    }

    code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    if (code != CURLE_OK)
    {
        std::cout << stderr << " Failed to set write data " << error_info << std::endl;
        return false;
    }

    ret = curl_easy_perform(curl);
	if (ret != CURLE_OK)
	{
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
        error_info.append("Failed to get ");
		error_info.append(url);
		return -2;
	}

    curl_easy_cleanup(curl);
	return 0;
}

}




