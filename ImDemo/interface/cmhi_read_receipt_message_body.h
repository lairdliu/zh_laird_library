/***************************************************************************
  * file name cmhi_read_receipt_message_body.h
  * brief 语音消息体
  * date 2019/02/12
  *************************************************************************/

#ifndef CMCC_IM_READ_RECEIPT_MESSAGE_BODY_H_
#define CMCC_IM_READ_RECEIPT_MESSAGE_BODY_H_

#include "cmhi_file_message_body.h"

namespace cmhi_iov{

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

class ReadReceiptMessageBody: public FileMessageBody{
	
		public:
			
			ReadReceiptMessageBody(){}

			void set_receipt_guid(uint64_t receipt_guid){this->receipt_guid = receipt_guid;};
			int get_receipt_guid(){return receipt_guid;};
			
			
			
		private:
			uint64_t receipt_guid;
		
	};

}
#endif //CMCC_IM_READ_RECEIPT_MESSAGE_BODY_H_


