/***************************************************************************
  * file name cmhi_text_message_body.h
  * brief 消息类容
  * date 2019/02/02
  *************************************************************************/

#ifndef CMCC_IM_TEXT_MESSAGE_BODY_H_
#define CMCC_IM_TEXT_MESSAGE_BODY_H_

#include "cmhi_message_body.h"

namespace cmhi_iov{

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

class TextMessageBody: public MessageBody{
	
		public:
			
			TextMessageBody(){}
			TextMessageBody(string text);
			~TextMessageBody(){}

			string get_text();
			void set_text(string text);
			
		private:
			string text_;
			void init(string text);
};

}
#endif //CMCC_IM_TEXT_MESSAGE_BODY_H_

