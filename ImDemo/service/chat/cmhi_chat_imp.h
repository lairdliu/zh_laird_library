/***************************************************************************
  * file name cmim_chat_imp.h
  * brief 单聊 接口实现头文件
  * date 2019/02/11
  *************************************************************************/

#ifndef CMCC_IM_IMPLEMENT_CHAT_H_
#define CMCC_IM_IMPLEMENT_CHAT_H_

#include <string>
#include <list>
#include <memory>

#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <queue>
#include <chrono>

#include "common_types.h"
#include "cmhi_message_wrapper.h"
#include "cmhi_message_observer.h"
#include "api/outer/connector.pb.h"

#include "cmhi_custom_message_body.h"
#include "cmhi_msg_encoder.h"
#include "cmhi_user_info.h"

using namespace std;
namespace cmhi_iov{

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

typedef void (*message_arrived_cb)(MessageWrapper* message);


class ChatManagerImp {

public:	
	
	ChatManagerImp(MessageObserverBase* message_observer = NULL);	
	~ChatManagerImp();	
	virtual int get_msg_size();
	virtual void insert_custom_msg_to_db(CustomMessageBody body,string msg_id,string to,string from,bool is_sender,bool is_msg_read);
	virtual EErrorCode send_message(MessageWrapperPtr message);
	virtual EErrorCode resend_message(const string &msg_id);
	virtual EErrorCode message_send_thread() ;
	virtual void message_timestamp_clear() ;
	virtual void receive_message(const outer::connector::SessionNotify& notify,UserInfo user) ;
	virtual void receive_message_1(const outer::connector::SessionNotify& notify,UserInfo user) ;
	virtual void carbon_msg_recv();
	virtual void transparent_msg_recv(const outer::connector::SessionNotify & notify);
	virtual void transparent_carbon_msg_recv(const outer::connector::SessionNotify & notify);
	virtual void get_offline_custom_message(const outer::connector::SessionNotify& notify);
	virtual bool delete_all_messages() ;
	virtual int delete_single_message(string remoteUsername);
	virtual std::list<MessageWrapper> get_next_page_msg(int count, int page, int* total, char* conversation_id) ;
	virtual std::list<MessageWrapper> get_unread_msg(const int count, const char* conversation_id, const bool flag);
	virtual bool send_read_receipt(char* to_user, char* msg_id, char* conversationid, bool single) ;
	virtual uint64_t get_cache_size() ;
	virtual void clean_cache_size( char* path,error_info *error) ;
	virtual bool set_voice_played(char* conversationid,char* msgid) ;
    virtual std::shared_ptr<MessageWrapper> get_msg_by_id(string msg_id);
	virtual int pcm2amr(const char* file_name,  readcbfunctype read_cb, errorcbfunctype error_cb) ;
	virtual int amr2pcm(const char* file_name, writecbfunctype write_cb, errorcbfunctype error_cb); 
	virtual EErrorCode pcmResampler(const int16_t* in_buf, const int16_t* out_buf, int in_pcm_len, int * out_pcm_len, int src_sample_rate, int out_sample_rate);

	virtual bool is_send_msg(const string& to_username);
	virtual bool update_conversation_msg(const string& username, const string& phone);

	string generate_serial_msg_body(MessageWrapper& message, string origin_link);
	
	string generate_serial_msg_data(MessageWrapperPtr message);

	void set_message_observer(MessageObserverBase* msg_observer);

	void set_send_msg_guid(uint64_t guid);
	uint64_t get_send_msg_guid();
	void set_rec_msg_guid(uint64_t guid);
	uint64_t get_rec_msg_guid();

	static std::shared_ptr<ChatManagerImp> getChatManager();  
	private:
        EErrorCode message_send_post_file(MessageWrapper& message,string& response);
		EErrorCode message_send_chat(MessageWrapper& message, string post_response);
		int get_disk_size(string dir_name);
		static int progress_callback(void *msg_observer, double dltotal, double dlnow, double ultotal, double ulnow);
			
		string service_name_ = "littlec-chat";
		MessageObserverBase* message_observer_;
		uint64_t send_msg_guid_ = 0;
		uint64_t rec_msg_guid_ = 0;

    std::mutex mut;
    std::condition_variable data_cond;
    std::queue<MessageWrapper> produced_nums;
};

}
#endif  //CMCC_IM_IMPLEMENT_CHAT_H_

