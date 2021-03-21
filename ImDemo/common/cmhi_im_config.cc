/***************************************************************************
  * file name cmhi_im_config.cc
  * brief 用于存储公共配置信息
  * date 2019/02/25
  *************************************************************************/

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
#include <sys/time.h>

#include "logger.h"
#include "cmhi_im_config.h"
using namespace std;

using namespace cmhi_iov;

namespace cmhi_iov{

int CreateDir(const char *path_name)  {  
  char dir_name[256];  
  strcpy(dir_name, path_name);  
  int i,len = strlen(dir_name);
  for(i=1; i<len; i++)  
  {  
      if(dir_name[i]=='/')  
      {  
          dir_name[i] = 0; 
		  printf("path is = %s\n",dir_name);
          if(access(dir_name, NULL)!=0)  
          {  
              if(mkdir(dir_name, 0755)==-1)  
              {   
                  printf("mkdir   error\n");   
                  return -1;   
              }  
          }  
          dir_name[i] = '/';  

      }  
  }  

  return 0;  
} 

long long GetSecTime()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (tv.tv_sec);
}

void GetFilePath(const char *path, const char *filename,  char *file_path)
{
    strcpy(file_path, path);
    if(file_path[strlen(path) - 1] != '/')
        strcat(file_path, "/");
    strcat(file_path, filename);
}

int Delete24hbFile(const char* path)
{
    DIR *dir;
    struct dirent *dirinfo;
    struct stat statbuf;
    char file_path[256] = {0};
    lstat(path, &statbuf);
    cout << "now time:" << GetSecTime() << endl;
    cout << "file time:" << statbuf.st_mtime << endl;
    long long timeDiff = GetSecTime() - statbuf.st_mtime;
    bool bDel = timeDiff > 24 * 60 * 60;
    cout << "timeDiff: " << timeDiff << "bDel: " << timeDiff << endl;

    if (S_ISREG(statbuf.st_mode) && bDel)//判断是否是常规文件 和 超过24小时
    {
        remove(path);
    }
    else if (S_ISDIR(statbuf.st_mode))//判断是否是目录
    {
        if ((dir = opendir(path)) == NULL)
            return 1;
        while ((dirinfo = readdir(dir)) != NULL)
        {
            GetFilePath(path, dirinfo->d_name, file_path);
            if (strcmp(dirinfo->d_name, ".") == 0 || strcmp(dirinfo->d_name, "..") == 0)//判断是否是特殊目录
                continue;
            Delete24hbFile(file_path);
            rmdir(file_path);
        }
        closedir(dir);
    }

    return 0;
}

bool copy_file(const string& src,const string& dest)
{
	ifstream in(src, ios::binary);
	ofstream out(dest, ios::binary);
	if (!in){
		printf("open file error");
		return false;
	}
	if (!out){
		printf("open file error");
		return false;
	}
	char flush[8];
	while(!in.eof()){
		in.read(flush, 8);
		out.write(flush, in.gcount());
		printf("%d", in.gcount());
	}
	in.close();
	out.close();
	return true;
}

int DeleteFile(const char* path)
{
    DIR *dir;
    struct dirent *dirinfo;
    struct stat statbuf;
    char file_path[256] = {0};
    lstat(path, &statbuf);
    
    if (S_ISREG(statbuf.st_mode))//判断是否是常规文件
    {
        remove(path);
    }
    else if (S_ISDIR(statbuf.st_mode))//判断是否是目录
    {
        if ((dir = opendir(path)) == NULL)
            return 1;
        while ((dirinfo = readdir(dir)) != NULL)
        {
            GetFilePath(path, dirinfo->d_name, file_path);
            if (strcmp(dirinfo->d_name, ".") == 0 || strcmp(dirinfo->d_name, "..") == 0)//判断是否是特殊目录
            continue;
            DeleteFile(file_path);
            rmdir(file_path);
        }
        closedir(dir);
    }
	
    return 0;
}

void CCConfigManager::set_root_path(string root_path){
	root_path_ = root_path;
	if(root_path[root_path.length()-1] != '/'){
		root_path_ += "/";
	}
	root_path_ +="iovroot/";
	
	/* clean_cache_size(root_path_); */
	CreateDir(root_path_.c_str());
}

void CCConfigManager::set_log_path(const string log_path){
	log_path_ = log_path;
    string dir_path = root_path_ + log_path_ + "/";
    CreateDir(dir_path.c_str());
}

void CCConfigManager::set_data_path(const string data_path){
	data_path_ = data_path;
    string dir_path = root_path_ + data_path_ + "/";
    CreateDir(dir_path.c_str());
}

void CCConfigManager::set_upload_path(const string upload_path){
	upload_path_ = upload_path;
    string dir_path = root_path_ + upload_path_ + "/";
    CreateDir(dir_path.c_str());
}
void CCConfigManager::set_download_path(const string download_path){
	download_path_ = download_path;
    string dir_path = root_path_ + download_path_ + "/";
    CreateDir(dir_path.c_str());
}

void CCConfigManager::set_database_path(const string database_path){
	database_path_ = database_path;
    string dir_path = root_path_ + database_path_ + "/";
    CreateDir(dir_path.c_str());
}

void CCConfigManager::set_database_name(const string database_name){
	database_name_ =database_name;
}

void CCConfigManager::set_adapter_address(const string adapter_address){
	adapter_address_ = adapter_address;
}

void CCConfigManager::set_connector_address(const string connector_address){
	connector_address_ = connector_address;
}

void CCConfigManager::set_file_address(const string file_address){
	file_address_ = file_address;
}

void CCConfigManager::set_username(const string username){
	username_ = username;
}

void CCConfigManager::set_options(const cmhi_iov::options options){
	options_ = options;
}

void CCConfigManager::set_voip_address(const string voip_address){
	voip_address_= voip_address;
}

void CCConfigManager::set_conference_address(const string conference_address){
	conference_address_ = conference_address;
}

string CCConfigManager::get_root_path(){
	return root_path_;
}

string CCConfigManager::get_log_path(){
	return root_path_+log_path_;
}

string CCConfigManager::get_data_path(){
	return root_path_+data_path_;
}

string CCConfigManager::get_upload_path(){
	return root_path_+upload_path_;
}

string CCConfigManager::get_download_path(){
	return root_path_+download_path_;
}

string CCConfigManager::get_database_path(){
	return root_path_+database_path_;
}

string CCConfigManager::get_database_name(){
	return database_name_;
}

string CCConfigManager::get_adapter_address(){
	string adpter_url = adapter_address_ + "getConfig?appkey=";
	adpter_url.append(options_.appkey);
	return adpter_url;
}

string CCConfigManager::get_connector_address(){
	return connector_address_;
}

string CCConfigManager::get_small_file_address(){
	return file_address_ + "/file/rest/uploadservices/uploadfile";
}

string CCConfigManager::get_big_file_address(){
	return file_address_ + "/file/rest/uploadservices/breakpointupload";
}

string CCConfigManager::get_md5_file_address(){
	return file_address_ + "/file/rest/uploadservices/geturlbymd5";
}

string CCConfigManager::get_file_address(){
	return file_address_;
}

string CCConfigManager::get_appkey(){
	return options_.appkey;
}

string CCConfigManager::get_username(){
	return username_;
}

options CCConfigManager::set_options(){
	return options_;
}

string CCConfigManager::get_conference_address(){
	return conference_address_;
}
string CCConfigManager::get_voip_address(){
	return voip_address_;
}


int CCConfigManager::clean_cache_size(const string file_path){
	
    return DeleteFile(file_path.c_str());
}

void CCConfigManager::init(const cmhi_iov::options options,const string root_path){
		
	log_path_= "log";
	data_path_= "data";
	upload_path_= "upload";
	database_path_="db";
	database_name_= "testv4.db";
	options_ = options;
	set_root_path(root_path);
}

int CCConfigManager::get_config_from_json(string json){
	CCLog(kLogLevelDebug, "get_config_from_json json: %s",json.c_str());
	cmhi_iov::Reader reader;  
 	cmhi_iov::Value root;  
    if(reader.parse(json,root))  {  
		std::vector<std::string> name_list = root.getMemberNames();
		
		for (int i = 0; i < name_list.size(); ++i){
			if (root[name_list[i]].isString()) {
				if("confDomain" == name_list[i]){
					conference_address_ = root[name_list[i]].asString();
				}
				else if("voipDomain" == name_list[i]){
					voip_address_ = root[name_list[i]].asString();
				}
				else if("connDomain" == name_list[i]){
					connector_address_ = root[name_list[i]].asString();
					
				}
				else if("pafsDomain" == name_list[i]){
					file_address_= root[name_list[i]].asString();
				}
			}
		}
		if(conference_address_.empty() 
			|| voip_address_.empty() 
			|| connector_address_.empty()){
			return -1;
		}
		return 0;
    }
	return -1;
}

}
