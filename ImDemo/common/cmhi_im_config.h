/***************************************************************************
  * file name cmhi_im_config.h
  * brief global config info
  * date 2019/02/25
  *************************************************************************/

#ifndef CMHI_IM_CONFIG_H_
#define CMHI_IM_CONFIG_H_

#include <string>
#include <json/json.h>
#include "common_types.h"

using namespace std;

namespace cmhi_iov{

long long GetSecTime();
void GetFilePath(const char *path, const char *filename,  char *file_path);
int Delete24hbFile(const char* path);
bool copy_file(const string& src,const string& dest);

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

class CCConfigManager{
 public:
    static CCConfigManager& GetInstance(){
		static CCConfigManager instance_;
		return instance_;
    }

	void set_log_path(const string log_path);
	void set_data_path(const string data_path);
	void set_upload_path(const string upload_path);
	void set_download_path(const string download_path);
	void set_database_path(const string database_path);
	void set_database_name(const string database_name);
	void set_adapter_address(const string adapter_address);
	void set_connector_address(const string connector_address);
	void set_file_address(const string file_address);
	void set_options(const cmhi_iov::options options);
    void set_username(const string username);
	void set_voip_address(const string voip_address);
	void set_conference_address(const string conference_address);
	
	string get_root_path();
	string get_log_path();
	string get_data_path();
	string get_upload_path();
	string get_download_path();
	string get_database_path();
	string get_database_name();
	string get_adapter_address();
	string get_connector_address();
	string get_small_file_address();
	string get_big_file_address();
	string get_md5_file_address();
	string get_file_address();
    string get_appkey();
    string get_username();
	cmhi_iov::options set_options();
	int clean_cache_size(const string file_path);
	void init(const cmhi_iov::options options,const string root_path);
	string get_conference_address();
	string get_voip_address();
	int get_config_from_json(string json);
	
 private:

	CCConfigManager(){
		//adapter_address_ = "http://112.35.72.180:9104/adapter/client/";
		//adapter_address_ = "http://carchat-im.cm-iov.com:81/adapter/client/";
		adapter_address_ = "https://carchat-im.cm-iov.com:7443/adapter/client/";
		//adapter_address_ = "https://carchat-im-pp.cm-iov.com:7443/adapter/client/";
	}
	void set_root_path(const string root_path);
	
 	string root_path_;
	string log_path_;
	string data_path_;
	string upload_path_;
	string download_path_;
	string database_path_;
	string database_name_;
	string adapter_address_;
	string connector_address_;
	string file_address_;
	string voip_address_;
	string conference_address_;
    string username_;
	cmhi_iov::options options_;
	int login_status_;
};

}
#endif //CMHI_IM_CONFIG_H_

