
#include <string>
#include <iostream>

#include "cmhi_message_observer.h"


namespace cmhi_iov{

class TestMessageObserver: public MessageObserverBase{

public:	
	TestMessageObserver(){}	
	~TestMessageObserver(){}	
	
	virtual void OnUploadProcessing(uint64_t uploaded_size, uint64_t file_size) override; 
	virtual void OnMessageReceived(MessageWrapperPtr message) override; 
	virtual void OnMessageSendStatus(string msg_id, EMessageStatus status, EErrorCode errorCode) override;
};

}

