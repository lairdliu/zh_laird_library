/***************************************************************************
  * file name cmhi_im_client_imp.h
  * brief SDK接入 接口头文件
  * date 2019/02/19
  *************************************************************************/

#ifndef CMHI_IM_CLIENT_IMPLEMENT_H_
#define CMHI_IM_CLIENT_IMPLEMENT_H_
#include <string>

#include "cmhi_im_client.h"
#include "cmhi_im_config.h"
#include "cmhi_im_account_imp.h"
#include "cmhi_contact_imp.h"
#include "cmhi_chat_imp.h"
#include "logger.h"
#include "sqlite.h"
#include "cmhi_notification_listener.h"
#include "cmhi_connection_listener.h"
#include "cmhi_message_wrapper.h"


#include "cmhi_custom_message_body.h"
namespace cmhi_iov{

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/
class CCClientManager: public INotificationListener, public IConnectionListener, public CCClientManagerBase{
 public:
  CCClientManager();
  ~CCClientManager();
	
  /********************
    Account Interface 
  ********************/
  virtual void init(options* options, char* work_path, error_info* error) override;
  virtual void login(const char* username,const char* token,int login_mode,const string &deviceId) override;
  
  virtual void login_try(const char* username,const char* token,int login_mode,const string &deviceId) override;
  virtual void logout() override;
  virtual void uninit() override;	
  virtual EErrorCode get_user_info(UserInfo & user) override;
  virtual list<FriendInfo> get_latest_contact() override;

  virtual void OnConnectionChanged(EConnectionStatus connection_status, const std::string& reason) override;
  virtual void OnAccountInfoChanged(UserInfo user)override;
  virtual void connect_observer_register(ConnectObserverBase* connect_observer) override;
  virtual void log_observer_register(std::shared_ptr<CLogObserverBase> logObserver, bool bLogFile) override;

  /********************
    Contact Interface
  ********************/
  
    virtual std::list<FriendInfo> get_friends_list(error_info* error, int page, int count, int *total) override; 
	virtual std::list<RequestWrapper> get_add_friend_request () override;
	virtual FriendInfo get_friend_user_info(char* user_id, error_info* error) override;
	virtual void set_friend_silent(char* friend_name, int silent_type, error_info* error) override;
	virtual void set_friend_request_read(RequestWrapper friend_request, CReadFriendResponse& friend_response) override;
	virtual void add_friend(char * friend_name,char * reason, error_info *error) override;
	virtual void agree_friend_request(char* requestid, error_info *error) override;
	virtual void decline_friend_request(char* requestid, char *reason, error_info *error) override;
	virtual void ignore_friend_request(char* requestid, char *reason, error_info *error) override;
	virtual void remove_friend(char * friend_name, error_info *error) override;
	virtual void update_friend() override;
  
	virtual void OnReceiveNotification(const outer::connector::SessionNotify& notify) override;
	virtual void contact_observer_register(ContactObserverBase* contact_observer) override;

  /*****************************
	  voip getconfig Interface
  *****************************/
  virtual void get_voip_config(string &conf_domain, string &voip_domain) override;

  
  /********************
    Chat Interface
  ********************/
  virtual EErrorCode send_message(MessageWrapperPtr message) override;
  virtual EErrorCode resend_message(const string &msg_id) override;
  virtual std::list<MessageWrapper> get_next_page_msg(int count,int page,int * total,char * conversation_id) override;
  virtual std::list<MessageWrapper> get_unread_msg(const int count, const char* conversation_id, const bool flag) override;
  virtual bool delete_all_messages() override;
  virtual int delete_single_message(string remoteUsername)override;
  virtual void insert_custom_msg_to_db (CustomMessageBody body,string msg_id,string to,string from,bool is_sender,bool is_msg_read) override;
  virtual int get_msg_size() override;
  virtual bool send_read_receipt(char* to_user, char* msg_id, char* conversationid, bool single) override;
  virtual void message_observer_register(MessageObserverBase* msg_observer);
  virtual bool set_voice_played(char* conversationid, char* msgid) override;
  virtual std::shared_ptr<MessageWrapper> get_msg_by_id(string msg_id) override;
  virtual int pcm2amr(const char* file_name,  readcbfunctype read_cb, errorcbfunctype error_cb) override;
  virtual int amr2pcm(const char* file_name, writecbfunctype write_cb, errorcbfunctype error_cb) override;    
  virtual EErrorCode pcmResampler(const int16_t* in_buf, const int16_t* out_buf, int in_pcm_len, int * out_pcm_len, int src_sample_rate, int out_sample_rate) override; 
  
 private:
  options options_info;
  CCAccountManagerImp* account_manager;
  ContactManagerImp* contact_manager;
  ChatManagerImp* chat_manager;
  NotificationParser notification_parser_;
  MessageObserverBase* message_observer_;
  ConnectObserverBase* connect_observer_;
  std::shared_ptr<CLogObserverBase> log_observer_;
};

}

#endif //CMHI_IM_CLIENT_IMPLEMENT_H_

