/***************************************************************************
  * file name cmhi_receipt_message_body.h
  * brief 回执消息
  * date 2019/02/12
  *************************************************************************/

#ifndef CMCC_IM_RECEIPT_MESSAGE_BODY_H_
#define CMCC_IM_RECEIPT_MESSAGE_BODY_H_

#include "cmhi_message_body.h"

namespace cmhi_iov{

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

class ReceiptMessageBody: public MessageBody{
	
		public:
			
			ReceiptMessageBody(){}
			ReceiptMessageBody(string receipt_msg_id);
			ReceiptMessageBody(string receipt_msg_id,string receipt_conversation_id);
			~ReceiptMessageBody(){}

			void set_receipt_msg_id(string msg_id);
			void set_receipt_conversation_id(string conversation_id);
			string get_receipt_msg_id();
			string get_receipt_conversation_id();
			
		private:
			string receipt_msg_id;			//回执消息id
			string receipt_conversation_id;	//回执对应消息的会话邋ID
			void init();
		
	};

}
#endif //CMCC_IM_RECEIPT_MESSAGE_BODY_H_


