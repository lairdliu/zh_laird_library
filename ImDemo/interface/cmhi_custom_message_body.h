/***************************************************************************
  * file name cmhi_audio_message_body.h
  * brief 语音消息体
  * date 2019/02/12
  *************************************************************************/

#ifndef CMCC_IM_CUSTOM_MESSAGE_BODY_H_
#define CMCC_IM_CUSTOM_MESSAGE_BODY_H_
#include <string>
#include <list>
#include<iostream>
using namespace std;



#include "cmhi_message_body.h"

namespace cmhi_iov{

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

struct Custom_Entity
{
	string key;
	string value;
};

class CustomMessageBody: public MessageBody{
	
		public:
			CustomMessageBody(){};
			~CustomMessageBody(){};
			void set_subtype(string subtype);
			string get_subtype();
			
			void set_notification(string notification);
			string get_notification();

			void set_key(string key);
			string get_key();
		    void update_entity(const Custom_Entity& entity);//用entity->value更新list表
			void set_entity(struct Custom_Entity entity);
			void delete_entity(int i);
			struct Custom_Entity get_entity(int i);
			int get_entity_size();

		private:
			string subtype;
			string notification;
			
			std::list<struct Custom_Entity> entity_list;

	};

}
#endif //CMCC_IM_CUSTOM_MESSAGE_BODY_H_


