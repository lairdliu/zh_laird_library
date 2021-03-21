/***************************************************************************
  * file name cmhi_message_body.h
  * brief 消息类容
  * date 2019/02/02
  *************************************************************************/

#ifndef CMCC_IM_MESSAGE_BODY_H_
#define CMCC_IM_MESSAGE_BODY_H_

#include <string>
#include "common_types.h"
#include <time.h>
#include <stdio.h>
#include <memory>

using std::string;

namespace cmhi_iov{

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

class MessageBody{
	
		public:
			
			MessageBody(){}
			~MessageBody(){}

			virtual void set_message_body_type(EMessageBodyType type);
			EMessageBodyType get_message_body_type(){return type; }
			
		protected:
			
			EMessageBodyType type;
		
};

typedef std::shared_ptr<MessageBody> MessageBodyPtr;


}
#endif //CMCC_IM_MESSAGE_BODY_H_

