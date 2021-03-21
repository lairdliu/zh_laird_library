/***************************************************************************
  * file name cmhi_audio_message_body.h
  * brief 语音消息体
  * date 2019/02/12
  *************************************************************************/

#ifndef CMCC_IM_FRIEND_MESSAGE_BODY_H_
#define CMCC_IM_FRIEND_MESSAGE_BODY_H_

#include "cmhi_file_message_body.h"

namespace cmhi_iov{

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/



class FriendMessageBody: public MessageBody{
	
		public:
			
			FriendMessageBody(){}
			FriendMessageBody(string text);
			~FriendMessageBody(){}
			string get_text();
			void set_text(string text);
		private:
			string text_;
			void init(string text);
};

}
#endif //CMCC_IM_FRIEND_MESSAGE_BODY_H_


