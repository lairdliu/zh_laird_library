
#include "test_message_observer.h"
#include "iostream"

namespace cmhi_iov{

void TestMessageObserver::OnUploadProcessing(uint64_t uploaded_size, uint64_t file_size){
	std::cout << "uploaded_size:" << uploaded_size << "file_size:" << file_size << std::endl;
}
void TestMessageObserver::OnMessageReceived(MessageWrapperPtr message){
	std::cout<<"msg_id:"<<message->get_msg_id()<<std::endl;
}
void TestMessageObserver::OnMessageSendStatus(string msg_id, EMessageStatus status, EErrorCode errorCode){
    std::cout<<"OnMessageSendStatus:"<<std::endl;
	std::cout<<"msg_id:"<<msg_id<<std::endl;
	std::cout<<"status:"<<status<<std::endl;
	std::cout<<"errorCode:"<<errorCode<<std::endl;
}

}

