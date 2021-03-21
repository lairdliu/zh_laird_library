/***************************************************************************
  * file name cmhi_im_client_imp.cc
  * brief SDK接入 接口实现
  * date 2019/02/18
  *************************************************************************/

#include <string.h>
#include "cmhi_im_client_imp.h"
#include "grpc_client.h"
#include "http_curl.h"
#include "sqlite.h"
#include "common_types.h"
#include "cmhi_custom_message_body.h"
#include <grpcpp/channel.h>


#include <grpc/grpc.h>

using namespace std;
using namespace outer::connector;
using namespace common::ntf;

extern int grpcdown;

namespace cmhi_iov{

shared_ptr<CCClientManagerBase> CCClientManagerBase::create_client_manager(){  
	return shared_ptr<CCClientManagerBase>(new CCClientManager());} 

/* 避免重复登录会踢的问题 */
bool loginFlag = false;
bool loginReturnFlag = false;
bool keepaliveSendFlag = false;

bool logoutFlag = true;

CCClientManager::CCClientManager() {}
CCClientManager::~CCClientManager() {}

void CCClientManager::init(options* options, char* work_path, error_info* error)
{
	CCConfigManager::GetInstance().init(*options,work_path);

    Logger logger;
    CCConfigManager::GetInstance().set_log_path("log");
    string log_path = CCConfigManager::GetInstance().get_log_path();
    string log_file = log_path + "/default0.log";
    if(Logger::GetBLogFile())
    {
        logger.init_logger(log_file);
    }
	//get adpter config
	string adapter_url = CCConfigManager::GetInstance().get_adapter_address();
	CCLog(kLogLevelInfo, "get_adapter_address()= %s",adapter_url.c_str());
	
	string adapter_ret;
	string http_error;
        
	int ret = Http_Get_Request(adapter_url.c_str(),adapter_ret,http_error);
	if (ret != 0 ){
		error->error_code = kSystemError;
		error->error_description = "get adapter info failed.";
        CCLog(kLogLevelError, "error_code：%d description:%s",error->error_code,error->error_description.c_str());
		return;
	}
	
	if(0 != CCConfigManager::GetInstance().get_config_from_json(adapter_ret)){
		
		error->error_code = kSystemError;
		error->error_description = "init failed.";
        CCLog(kLogLevelError, "error_code：%d description:%s",error->error_code,error->error_description.c_str());
		return;
	}

	account_manager = new CCAccountManagerImp();
	account_manager->init(options,error);
	
	chat_manager = new ChatManagerImp(message_observer_);

	options_info.appkey = new char[64];
	options_info.appkeyPassword = new char[64];
	
	strcpy(options_info.appkey, options->appkey);
	strcpy(options_info.appkeyPassword , options->appkeyPassword);
	options_info.logLevel = options->logLevel;

    contact_manager = new ContactManagerImp();
}

int gLoginTryFlag = 0;       /*  是否是login重试流程，login和login_try互斥，因此和回调接口可以认为时序保持一??*/
int gKeepAliveInitFlag = 0;  /*  避免重复创建keeapalive 线程，是有初次调用login时创??*/
bool gLogoutSuccess = false;
void CCClientManager::login(const char* username,const char* token, int loginmode,const string &deviceId)
{
	CCLog(kLogLevelDebug, "login begin!!!");

	pthread_mutex_lock(&CCGrpcClient::GetInstance().mutex);
	gLoginTryFlag = 0;
    CCGrpcClient::GetInstance().init(CCConfigManager::GetInstance().get_connector_address());
	CCGrpcClient::GetInstance().set_notification_listener(this);
	CCGrpcClient::GetInstance().set_connection_listener(this);
    loginFlag = true;
	
	__sync_synchronize();
	account_manager->login(username, token,loginmode,deviceId);
	loginReturnFlag = false; 
	keepaliveSendFlag = true;
	gLogoutSuccess = false;
	pthread_mutex_unlock(&CCGrpcClient::GetInstance().mutex);
	gKeepAliveInitFlag = 1;
 
}

void CCClientManager::login_try(const char* username,const char* token, int loginmode,const string &deviceId)
{

	CCLog(kLogLevelDebug, "come in login_try !!!");

	if (keepaliveSendFlag == false)
				return;

		CCLog(kLogLevelDebug, "come in logout1_begin!!!");	
			if (gLogoutSuccess	 == true)
				return;
			CCLog(kLogLevelDebug, "logout1 begin!!!");
			pthread_mutex_lock(&CCGrpcClient::GetInstance().mutex);
			gLoginTryFlag = 0;
			keepaliveSendFlag = false;
			logoutFlag = true;
			__sync_synchronize();
			account_manager->logout();
			CCLog(kLogLevelDebug, "log_out1 begin uninit!!!");
			CCGrpcClient::GetInstance().uninit();
			pthread_mutex_unlock(&CCGrpcClient::GetInstance().mutex);
			CCLog(kLogLevelDebug, "end out begin uninit!!!");

		//CCClientManager::logout();
	
		CCLog(kLogLevelDebug, "login try begin!!!");
		pthread_mutex_lock(&CCGrpcClient::GetInstance().mutex);
		gLoginTryFlag  = 0;

		CCLog(kLogLevelDebug, "login try begin grpc init!!!");
		
		CCGrpcClient::GetInstance().init(CCConfigManager::GetInstance().get_connector_address());
		CCLog(kLogLevelDebug, "login try begin grpc set_notification_listener!!!");
 		CCGrpcClient::GetInstance().set_notification_listener(this);
		CCLog(kLogLevelDebug, "login try begin grpc set_connection_listener!!!");
		CCGrpcClient::GetInstance().set_connection_listener(this);
		
		__sync_synchronize();

		CCLog(kLogLevelDebug, "login try begin login!!!");
		account_manager->login(username, token,loginmode,deviceId);

		CCLog(kLogLevelDebug, "login try login over!!!");
		gLoginTryFlag = 1;
		keepaliveSendFlag = true;
		pthread_mutex_unlock(&CCGrpcClient::GetInstance().mutex);

		CCLog(kLogLevelDebug, "end retry login!!!");
}

void CCClientManager::logout()
{
	CCLog(kLogLevelDebug, "login out begin!!!  gLogoutSuccess: %d gLoginTryFlag: %d keepaliveSendFlag: %d ",
		     gLogoutSuccess, gLoginTryFlag, keepaliveSendFlag);

	
	//退出登录，无网络
	if (gLogoutSuccess   == true)
		return;
    CCLog(kLogLevelDebug, "login out begin!!!");
	pthread_mutex_lock(&CCGrpcClient::GetInstance().mutex);
	//gLoginTryFlag = 0;
	//keepaliveSendFlag = false;
	logoutFlag = true;
	//__sync_synchronize();

    //chat_manager->message_timestamp_clear();
    
	CCLog(kLogLevelDebug, "login out begin op!!!");
	account_manager->logout();

	//CCLog(kLogLevelDebug, "login out begin uninit!!!");
	//CCGrpcClient::GetInstance().uninit();
	pthread_mutex_unlock(&CCGrpcClient::GetInstance().mutex);

	CCLog(kLogLevelDebug, "end out begin uninit!!!");
}

void CCClientManager::uninit()
{
	
	pthread_mutex_lock(&CCGrpcClient::GetInstance().mutex);
	delete account_manager;
	delete chat_manager;
	delete contact_manager;
	delete options_info.appkey;
	delete options_info.appkeyPassword;
	pthread_mutex_unlock(&CCGrpcClient::GetInstance().mutex);
	
}

EErrorCode CCClientManager::get_user_info(UserInfo & user){

	return account_manager->get_user_info(user);
	
}

list<FriendInfo> CCClientManager::get_latest_contact(){
	return account_manager->get_latest_contact();
}

void CCClientManager::get_voip_config(string &conf_domain, string &voip_domain){
	account_manager->get_voip_config(conf_domain,voip_domain);
}


std::list<FriendInfo> CCClientManager::get_friends_list(error_info* error, int page, int count
, int *total) {
  return contact_manager->get_friends_list(error, page, count, total);
}

std::list<RequestWrapper> CCClientManager::get_add_friend_request () {
  return contact_manager->get_add_friend_request();
}

FriendInfo CCClientManager::get_friend_user_info(char* user_id, error_info* error) {
  return contact_manager->get_friend_user_info(user_id, error);
}

void CCClientManager::add_friend(char * friend_name,char * reason, error_info *error) {
  contact_manager->add_friend(friend_name, reason, error);
}

void CCClientManager::set_friend_silent(char* friend_name, int silent_type, error_info* error) {
  contact_manager->set_friend_silent(friend_name, silent_type, error);
  
}

void CCClientManager::set_friend_request_read(RequestWrapper friend_request,CReadFriendResponse& friend_response)
{
	contact_manager->set_friend_request_read(friend_request,friend_response);
}

void CCClientManager::agree_friend_request(char* requestid, error_info *error) {
  contact_manager->agree_friend_request(requestid, error);
}

void CCClientManager::decline_friend_request(char* requestid, char *reason, error_info *error) 
{
  contact_manager->decline_friend_request(requestid, reason, error);
}
void CCClientManager::ignore_friend_request(char* requestid, char *reason, error_info *error)
{
	contact_manager->ignore_friend_request(requestid, reason, error);
}

void CCClientManager::remove_friend(char * friend_name, error_info *error) {
  contact_manager->remove_friend(friend_name, error);
}

void CCClientManager::update_friend() {
	string appkey = CCConfigManager::GetInstance().get_appkey();
	string username = CCConfigManager::GetInstance().get_username();
	contact_manager->sync_friend_change(username, appkey);
}

std::list<MessageWrapper> CCClientManager::get_next_page_msg(int count, int page, int* total, char* conversation_id) {
	return chat_manager->get_next_page_msg(count, page, total, conversation_id);
}

std::list<MessageWrapper> CCClientManager::get_unread_msg(const int count, const char* conversation_id, const bool flag) {
	return chat_manager->get_unread_msg(count, conversation_id, flag);
}

bool CCClientManager::delete_all_messages(){
	return chat_manager->delete_all_messages();
}

int CCClientManager::delete_single_message(string remoteUsername){
	return chat_manager->delete_single_message(remoteUsername);
}

void CCClientManager::insert_custom_msg_to_db(CustomMessageBody body,string msg_id,string to,string from,bool is_sender,bool is_msg_read){
	return chat_manager->insert_custom_msg_to_db( body, msg_id, to, from, is_sender, is_msg_read);
}

int CCClientManager::get_msg_size()
{
	return chat_manager->get_msg_size();
}
bool CCClientManager::send_read_receipt(char* to_user, char* msg_id, char* conversationid, bool single) {
	return chat_manager->send_read_receipt(to_user, msg_id, conversationid, single);
}

EErrorCode CCClientManager::send_message(MessageWrapperPtr message){
	//第一次给非好友发送消息需要 发送好友请求,无法确定手机端有没有发送过添加好友请求？
	//依据是否给平台用户发送过好友申请，若发送过统一认为是发送过消息，若没有，则发送一条邀新
	//未发送过消息且允许发送消息，则发送一条好友邀请。
	if (!chat_manager->is_send_msg(message->get_to()) && contact_manager->is_allow_send_message(message->get_to()))
	{
		error_info error;
		contact_manager->add_friend((char *)message->get_to().c_str(), "", &error);
	}
	return chat_manager->send_message(message);
}

EErrorCode CCClientManager::resend_message(const string &msg_id){
    return chat_manager->resend_message(msg_id);
}

bool CCClientManager::set_voice_played(char* conversationid, char* msgid){
    return chat_manager->set_voice_played(conversationid, msgid);
}

int CCClientManager::pcm2amr(const char* file_name,  readcbfunctype read_cb, errorcbfunctype error_cb) {
	return chat_manager->pcm2amr(file_name, read_cb, error_cb);
}

int CCClientManager::amr2pcm(const char* file_name, writecbfunctype write_cb, errorcbfunctype error_cb) {
	return chat_manager->amr2pcm(file_name, write_cb, error_cb);
}

EErrorCode CCClientManager::pcmResampler(const int16_t* in_buf, const int16_t* out_buf, int in_pcm_len, int * out_pcm_len, int src_sample_rate, int out_sample_rate){
	return chat_manager->pcmResampler(in_buf, out_buf, in_pcm_len, out_pcm_len, src_sample_rate, out_sample_rate);
}

std::shared_ptr<MessageWrapper> CCClientManager::get_msg_by_id(string msg_id){
    return chat_manager->get_msg_by_id(msg_id);
}


void CCClientManager::OnConnectionChanged(EConnectionStatus connection_status, 
const std::string& reason) {
  connect_observer_->OnConnectionChanged(connection_status, reason);
}

void CCClientManager::OnAccountInfoChanged(UserInfo user) {
  connect_observer_->OnAccountInfoChanged(user);
}

extern string gDeviceId;
int gRecvPingFlag = 0;
extern int gLoginTryFlag;


void CCClientManager::OnReceiveNotification(const outer::connector::SessionNotify& notify){
    CCLog(kLogLevelInfo, "---------------------------------------------Notification type:%d",notify.type());
    EErrorCode error_code;
	outer::user::UserInfo user_info;
	EConnectionStatus login_status;
	string reason;
	UserInfo user;
	string deviceId;
	bool kick = true;
	
	switch(notify.type()){
		case LOGIN_RESPONSE:
			error_code = notification_parser_.LoginNotifyParser(notify,user_info);
			gRecvPingFlag = 1;
			grpcdown = 0;
			if ((kSuccess == error_code) || (kConnectorUserAlreadyLogin == error_code)){
				CCLog(kLogLevelDebug, "(kSuccess == error_code) || (kConnectorUserAlreadyLogin == error_code)");
				
                create_table_friend(); //不存在原来的 Friend 表则创建
				update_table_friend(); //升级 Friend 表
                create_table_request();
                create_table_chat();			
				contact_manager->sync_friend();     //1、同步好友信息
				chat_manager->get_offline_custom_message(notify);//2.获取自定义离线消息
                account_manager->get_user_info(user);           //3、获取用户信息
				chat_manager->receive_message_1(notify,user);   //4、消息同步（获取应该掉）
				logoutFlag = false;
			}
			
            keepaliveSendFlag = true;
            
			CCLog(kLogLevelDebug, "login respone error_code: %d loginReturnFlag: %d gLoginTryFlag: %d", error_code, loginReturnFlag, gLoginTryFlag);
			if(connect_observer_)
			{
			    if (!loginReturnFlag || (gLoginTryFlag == 0))
			    {
			    	CCLog(kLogLevelDebug, "normal_login");
			        loginReturnFlag = true;
					connect_observer_->OnLogin(error_code);
			    }
				else
				{
				    if ((kSuccess == error_code) || (kConnectorUserAlreadyLogin == error_code))
				    {
				    	CCLog(kLogLevelDebug, "kTrySuccess");
				        error_code = kTrySuccess;
				    	connect_observer_->OnLogin(error_code);
				    }
					else
					{
				   		CCLog(kLogLevelDebug, "else");
					    connect_observer_->OnLogin(error_code);
					}
				}
			}
			loginFlag = false;
			//pthread_mutex_unlock(&CCGrpcClient::GetInstance().mutex);
			
			break;
			
		case LOGOUT_RESPONSE:
			error_code = notification_parser_.LogoutNotifyParser(notify);
			CCLog(kLogLevelDebug,"call logout callback");
			
		     //CCLog(kLogLevelInfo,"MYTEST_connect_observer_=p%",connect_observer_);
			if (logoutFlag)			
            {
			    //收到退出回调后才能清除状态
                pthread_mutex_lock(&CCGrpcClient::GetInstance().mutex);
                gLoginTryFlag = 0;
                keepaliveSendFlag = false;
                __sync_synchronize();
                chat_manager->message_timestamp_clear();
                CCLog(kLogLevelDebug, "login out success!!!");
                pthread_mutex_unlock(&CCGrpcClient::GetInstance().mutex);
				if(connect_observer_)
	            {
                    //CCLog(kLogLevelDebug, "login out success!!!");
					connect_observer_->OnLogout(error_code);
					gLogoutSuccess  = true;
				}
				else
				{
					CCLog(kLogLevelDebug,"connect_observer_ null");
				}			    
			}
			else
			{
			    CCLog(kLogLevelDebug,"try login send. log out response ");
			}
			break;
		case KICK_NOTIFICATION:
			notification_parser_.KickNotifyParser(notify,login_status,reason,deviceId);
			
			if (gDeviceId == deviceId)
				kick = false;
			
			std::cout<<gDeviceId<<deviceId<<std::endl;
			CCLog(kLogLevelDebug,"deviceId:%s,gDeviceId:%s,loginFlag:%d,kick:%d",deviceId.c_str(),gDeviceId.c_str(),loginFlag,kick);
            if (!loginFlag && kick)
            {
                keepaliveSendFlag = false;
			    if(connect_observer_)
				    connect_observer_->OnConnectionChanged(login_status, reason);
			}
			break;
		case PING_RESPONSE:
			gRecvPingFlag = 1;
			CCLog(kLogLevelDebug, "recv ping response, requestid:%s", notify.request_id().c_str());
      		break;
		case FRIEND_UPDATE_NOTIFICATION:
			contact_manager->update_friend_change(notify);
			break;
		case NEW_MSG:
        case RETRACT_NOTIFICATION:
            account_manager->get_user_info(user);

			chat_manager->receive_message(notify,user);          //接收消息
			break;
		case CARBON_MSG:
			CCLog(kLogLevelDebug, "CARBON_MSG:");
			chat_manager->carbon_msg_recv();
			break;
		case TRANSPARENT_MSG_NOTIFICATION:
			CCLog(kLogLevelDebug, " TRANSPARENT_MSG_NOTIFICATION:");
			chat_manager->transparent_msg_recv(notify);
			break;
		case TRANSPARENT_MSG_CARBON_NOTIFICATION:
			CCLog(kLogLevelDebug, " TRANSPARENT_MSG_CARBON_NOTIFICATION");
			chat_manager->transparent_carbon_msg_recv(notify);
			break;
		case PERSONAL_INFORMATION_CHANGE_NOTIFICATION:
			CCLog(kLogLevelDebug, " PERSONAL_INFORMATION_CHANGE_NOTIFICATION");
			account_manager->get_user_info(user);
			if(connect_observer_)
				connect_observer_->OnAccountInfoChanged(user);		
			break;
		default:
			CCLog(kLogLevelWarn, "Receive Notification is unknow type: %d", notify.type());
	}
}

void CCClientManager::message_observer_register(MessageObserverBase* msg_observer){
	message_observer_ = msg_observer;
	chat_manager->set_message_observer(msg_observer);
}

void CCClientManager::contact_observer_register(ContactObserverBase* contact_observer){
	contact_manager->contact_observer_register(contact_observer);
}

void CCClientManager::connect_observer_register(ConnectObserverBase* connect_observer){
	connect_observer_ = connect_observer;
	account_manager->connect_observer_register(connect_observer);
}

void CCClientManager::log_observer_register(std::shared_ptr<CLogObserverBase> logObserver, bool bLogFile)
{
	log_observer_ = logObserver;
	Logger::SetLogObserver(logObserver, bLogFile);
}

}

