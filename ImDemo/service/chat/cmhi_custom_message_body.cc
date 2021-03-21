/***************************************************************************
  * file name cmim_audio_message_body.cpp
  * brief 语音消息内容
  * date 2019/02/12
  *************************************************************************/


#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>

#include <json/json.h>
#include <string.h>

#include "cmhi_chat_imp.h"
#include "api/outer/chat.pb.h"
#include "api/common/msg.pb.h"
#include "api/outer/history.pb.h"
#include "api/outer/chat.pb.h"
#include "api/common/enumeration.pb.h"
#include "cmhi_text_message_body.h"
#include "cmhi_file_message_body.h"
#include "cmhi_audio_message_body.h"
#include "cmhi_custom_message_body.h"
#include "cmhi_read_receipt_message_body.h"
#include "cmhi_location_message_body.h"
#include "cmhi_friend_message_body.h"
#include "cmhi_im_config.h"
#include "http_curl.h"
#include "grpc_client.h"
#include "md5.h"
#include "sqlite.h"
#include "formatter.h"
#include "interf_amr.h"
using namespace std;
using namespace outer::chat;
//using namespace common::msg;
using namespace outer::history;


namespace cmhi_iov{

	void CustomMessageBody::set_subtype(string subtype)
	{
		this->subtype = subtype;
	}

	string CustomMessageBody::get_subtype()
	{
		return this->subtype;
	}

	void CustomMessageBody::set_notification(string notification)
	{
		 this->notification = notification;
	}

	string  CustomMessageBody::get_notification()
	{
		return this->notification;
	}


	void CustomMessageBody::set_entity(struct Custom_Entity entity)
	{
		this->entity_list.push_back(entity);
	}
	
	void CustomMessageBody::delete_entity(int i)
	{
		int j = 0;
		std::list<struct Custom_Entity>::iterator it;
		for(it = entity_list.begin(); it != entity_list.end();it++)
		{
			if(j == i)
			{
				it = this->entity_list.erase(it);
			}
			j++;
		}
		
	}
	
void CustomMessageBody::update_entity(const Custom_Entity& entity)
{
    CCLog(kLogLevelDebug,"update_entity_restar");
	CCLog(kLogLevelDebug,"entity.value=:%s",entity.value.c_str());
	CCLog(kLogLevelDebug,"entity.key=:%s",entity.key.c_str());

	std::list<struct Custom_Entity>::iterator it;
	
	for(it = entity_list.begin(); it != entity_list.end();++it)
		{
			auto &f=*it;  
			if(f.key==entity.key)
			{
				f.value=entity.value;
				CCLog(kLogLevelDebug,"f.value=:%s",f.value.c_str());
			}
		}	
	 CCLog(kLogLevelDebug,"update_entity_end");

}  

	struct Custom_Entity CustomMessageBody::get_entity(int i)
	{
		std::list<struct Custom_Entity>::iterator iter;
		
		int j = 0;
		 for(iter =  entity_list.begin(); iter != entity_list.end(); iter++)  
		{
		  if (i == j)
		  	return *iter;
		    j++;
		}
	}
	int CustomMessageBody::get_entity_size()
	{
		return this->entity_list.size();

	}


}

