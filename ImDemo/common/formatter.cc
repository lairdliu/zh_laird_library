/***************************************************************************
  * file name formatter.cc
  * brief 
  * date 2019/02/13
  *************************************************************************/

#include <json/json.h>
#include <uuid/uuid.h>
#include <time.h>

#include "formatter.h"
#include "md5.h"
#include "cmhi_file_message_body.h"

using namespace std;
namespace cmhi_iov{

string generate_uuid(){
  uuid_t uu;
	uuid_generate(uu);
	int i;

	static char buf[64] = {0};

    uuid_unparse(uu, buf);
    
	return string(buf);
}

string null_to_blank(string str) {
    if (str.empty()) {
        return "";
    } else {
        return str;
    }
}

uint64_t generate_time() {
	struct timeval tv; 
	gettimeofday(&tv,NULL);
	uint64_t time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return time;
}

int parse_status_from_resp(string response) {
  int index = response.find_last_of("{");
  string http_resp = response.substr(index);
  int ret = -1;
  cmhi_iov::Reader reader;  
  cmhi_iov::Value root;
  if(reader.parse(http_resp, root)){
    string status;
    if (! root["status"].isNull()) {
      status = root["status"].asString();
      if(! status.empty()){
        ret = stoi(status.c_str());
      }
    }
  }
  return ret;
}

string parse_original_link_from_resp(string response) {
    if(response.empty()) {
        return "";
    }
    
    int index = response.find_last_of("{");
    if(index == string::npos){
		return "";
	}
    string http_resp = response.substr(index);

    cmhi_iov::Reader reader;  
    cmhi_iov::Value root;
    if(reader.parse(http_resp, root))
    {
        string original_link;
        if (root["original_link"].isNull())
            original_link = ""; 
        else
            original_link = root["original_link"].asString();
        return original_link;
    } else {
        return "";
    }

}

uint64_t parse_msg_time_by_guid(uint64_t guid) {
    uint64_t guidBaseDate = 1468976307657;
    uint64_t time = (uint64_t)((guid >> 22) + guidBaseDate) / 1000.0;
    return time;
}

string generate_post_param(MessageBodyPtr msg_body){

	cmhi_iov::Value root;
	cmhi_iov::FastWriter writer;
	
	EMessageBodyType message_type = msg_body->get_message_body_type();
	string file_name = (dynamic_pointer_cast<FileMessageBody>(msg_body))->get_file_name();
	string file_path = (dynamic_pointer_cast<FileMessageBody>(msg_body))->get_local_path();
	if(file_path[file_path.length()-1] != '/'){
			file_path += "/";
	}
  file_path += file_name;
  
	switch (message_type){
		case kMessageBodyTypeVoice:
			root["type"] = "audio";
		    break;
		case kMessageBodyTypeImage:
			root["type"] = "image";
			break;
		case kMessageBodyTypeVideo:
			root["type"] = "video";
			break;
		default:
			root["type"] = file_name.substr(file_name.find("."));
	}
	root["fileName"] = file_name;
	root["md5"] = MD5Generation(file_path);
	return writer.write(root);
}

}

