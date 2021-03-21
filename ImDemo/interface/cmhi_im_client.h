/***************************************************************************
  * file name cmhi_im_client.h
  * brief SDK接入 接口头文件
  * date 2019/02/19
  *************************************************************************/

#ifndef INTERFACE_CMHI_IM_CLIENT_H_
#define INTERFACE_CMHI_IM_CLIENT_H_

#include <memory>
#include <list>
#include <string>

#include "base_typedefs.h"
#include "common_types.h"
#include "cmhi_user_info.h"
#include "cmhi_message_wrapper.h"
#include "cmhi_friend.h"
#include "cmhi_message_observer.h"
#include "cmhi_contact_observer.h"
#include "cmhi_connect_observer.h"
#include "cmhi_custom_message_body.h"
#include "cmhi_log_observer.h"

namespace cmhi_iov{



/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/
class CCClientManagerBase{
 public:
  CCClientManagerBase(){}
  virtual ~CCClientManagerBase(){}

  /********************
    Account Interface 
  ********************/
  virtual void init(options* options, char* work_path, error_info* error) =0;
  virtual void login(const char* user_name, const char* token,int login_mode,const string &deviceId) =0;
  
  virtual void login_try(const char* user_name, const char* token,int login_mode,const string &deviceId) =0;
  virtual void logout() =0;
  virtual void uninit() =0;	
  virtual EErrorCode get_user_info(UserInfo & user) =0;
  virtual std::list<FriendInfo> get_latest_contact() = 0;
  
  virtual void connect_observer_register(ConnectObserverBase* connect_observer) =0;
  virtual void log_observer_register(std::shared_ptr<CLogObserverBase> logObserver, bool bLogFile) = 0;

  /********************
    Contact Interface
  ********************/
	virtual std::list<FriendInfo> get_friends_list(error_info* error, int page, int count, int *total) =0;
	virtual std::list<RequestWrapper> get_add_friend_request () =0;
	virtual FriendInfo get_friend_user_info(char* user_id, error_info* error) =0;
	virtual void add_friend(char * friend_name,char * reason, error_info *error) =0;
	virtual void agree_friend_request(char* requestid, error_info *error) =0;
	virtual void decline_friend_request(char* requestid, char *reason, error_info *error) =0;
	virtual void ignore_friend_request(char* requestid, char *reason, error_info *error) =0;
	virtual void remove_friend(char * friend_name, error_info *error) =0;
	virtual void update_friend() =0;
	
	virtual void contact_observer_register(ContactObserverBase* contact_observer) =0;
	virtual void set_friend_silent(char* friend_name, int silent_type, error_info* error) =0;
	virtual void  set_friend_request_read(RequestWrapper friend_reques,CReadFriendResponse& friend_response)=0;

	
  /********************
    Chat Interface
    ********************/
    virtual EErrorCode send_message(MessageWrapperPtr message) =0;
	virtual EErrorCode resend_message(const string &msg_id) =0;
  	virtual std::list<MessageWrapper> get_next_page_msg(int count,int page,int * total,char * conversation_id) =0;
	virtual std::list<MessageWrapper> get_unread_msg(const int count, const char* conversation_id, const bool flag) =0;
  	virtual bool delete_all_messages() =0;
	virtual  int delete_single_message(string remoteUsername)=0; 
	virtual void insert_custom_msg_to_db(CustomMessageBody body,string msg_id,string to,string from,bool is_sender,bool is_msg_read)=0;
	virtual int get_msg_size()=0;
	virtual bool send_read_receipt(char* to_user, char* msg_id, char* conversationid, bool single) =0;
  	virtual void message_observer_register(MessageObserverBase* msg_observer) =0;
	virtual bool set_voice_played(char* conversationid, char* msgid) =0;
	virtual std::shared_ptr<MessageWrapper> get_msg_by_id(string msg_id) =0;
	virtual int pcm2amr(const char* file_name,  readcbfunctype read_cb, errorcbfunctype error_cb) =0;
	virtual int amr2pcm(const char* file_name, writecbfunctype write_cb, errorcbfunctype error_cb) = 0; 
	virtual EErrorCode pcmResampler(const int16_t* in_buf, const int16_t* out_buf, int in_pcm_len, int * out_pcm_len, int src_sample_rate, int out_sample_rate) = 0;
  /****************************
	  voip getconfig Interface
  ****************************/
  virtual void get_voip_config(string &conf_domain, string &voip_domain) =0;
  
  /********************
    Chat Interface
  ********************/



  static std::shared_ptr<CCClientManagerBase> create_client_manager();
};

}
#endif //INTERFACE_CMHI_IM_CLIENT_H_

