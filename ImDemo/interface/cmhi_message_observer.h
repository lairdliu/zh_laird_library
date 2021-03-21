/***************************************************************************
  * file name cmhi_message_observer.h
  * brief Listen for messages from sdk
  * date 2019/03/11
  *************************************************************************/

  
#ifndef CMHI_IM_MESSAGE_OBSERVER_H_
#define CMHI_IM_MESSAGE_OBSERVER_H_

#include "common_types.h"
#include "base_typedefs.h"
#include "cmhi_message_wrapper.h"

namespace cmhi_iov{
  
  class MessageObserverBase{
  public:
  	
	MessageObserverBase(){}
	virtual ~MessageObserverBase(){}
	
	virtual void OnMessageReceived(MessageWrapperPtr message) = 0;
	virtual void OnMessageSendStatus(string msg_id, EMessageStatus status, EErrorCode errorCode) = 0;
	virtual void OnUploadProcessing(uint64_t uploaded_size, uint64_t file_size) = 0;
  };
  
  }
#endif  //CMHI_IM_MESSAGE_OBSERVER_H_

