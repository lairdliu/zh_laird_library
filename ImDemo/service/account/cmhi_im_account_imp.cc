/***************************************************************************
  * file name cmhi_im_account_imp.cpp
  * brief 账号管理 接口实现
  * date 2019/02/18
  *************************************************************************/

#include <string.h>

#include "cmhi_chat_imp.h"
#include "cmhi_im_config.h"
#include "cmhi_im_account_imp.h"
#include "api/outer/history.pb.h"
#include "logger.h"
#include "cmhi_text_message_body.h"
#include "cmhi_file_message_body.h"
#include "cmhi_audio_message_body.h"
#include "cmhi_custom_message_body.h"
#include "cmhi_location_message_body.h"
#include "cmhi_friend_message_body.h"
#include "formatter.h"
#include "http_curl.h"
#include "sqlite.h"

using namespace std;
using namespace outer::user;
using namespace outer::connector;
using namespace outer::history;
using namespace common::enumeration;

namespace cmhi_iov{
	
shared_ptr<CCAccountManagerImp> CCAccountManagerImp::getAccountManager(){  
	return shared_ptr<CCAccountManagerImp>(new CCAccountManagerImp());} 
	


void CCAccountManagerImp::init (options *options, error_info *error ){
    options_ = options;
	service_name_ = "littlec-user";
	
	grpc_client_ = &CCGrpcClient::GetInstance();
	grpc_client_->tid = -1;
	grpc_client_->base_ = NULL;
	if(!grpc_client_){
		error->error_code = kSystemError;
    	error->error_description = (char*)"init failed.";
	}
	
	error->error_code = kSuccess;
    error->error_description = (char*)"OK";

}
void CCAccountManagerImp::login_try(const char* username, const char* token, int login_mode,const string &deviceId) {
	return;
}

string gDeviceId;
extern int gKeepAliveInitFlag;

void CCAccountManagerImp::login(const char* username, const char* token, int login_mode,const string &deviceId) {
	LoginRequest login_request;
    login_request.set_appkey(CCConfigManager::GetInstance().get_appkey());
    login_request.set_username(username);
    if(login_mode == 1){
  	    login_request.set_login_mode(TOKENMODE);
    }else if(login_mode == 2){
	    login_request.set_login_mode(DEBUG_MODE);
    }else{
	    login_request.set_login_mode(UNKNOWNMODE);
    }
    gDeviceId = deviceId;
    
    login_request.set_sha256(true);
    login_request.set_token(token);
    
    string request_data;
    login_request.SerializeToString(&request_data);

    if (!grpc_client_){
        EErrorCode err_code = kSystemError;
	    if(connect_observer_)
		    connect_observer_->OnLogin(err_code);
    }

	user_info_.set_user_name(username);     
    CCConfigManager::GetInstance().set_username(username);

    string path = "db/" + user_info_.get_user_name();
    CCConfigManager::GetInstance().set_database_path(path);
    path = "download/" + user_info_.get_user_name();
    CCConfigManager::GetInstance().set_download_path(path);
   
    delete_chat_message_by_date();
    clear_download_by_time(); 

	bool status = grpc_client_->send_session_request(request_data, SessionRequest_ESessionRequestType_LOGIN,deviceId);
	if (status) {
	    if(gKeepAliveInitFlag == 0){
		    grpc_client_->keep_session_alive();
	    }
	    CCLog(kLogLevelInfo, "login success. username:%s, token:%s, login_mode:%d,gDeviceId:",username, token, login_mode,gDeviceId.c_str());
	}
	else {
	    CCLog(kLogLevelError, "login failed! username:%s, token:%s, login_mode:%d",username, token, login_mode);
	}
}

void CCAccountManagerImp::logout() {
    LogoutRequest logout_req;
	string request_data;

	logout_req.SerializeToString(&request_data);
	bool status = grpc_client_->send_session_request(request_data,SessionRequest_ESessionRequestType_LOGOUT,gDeviceId);
  if (status) {
    CCLog(kLogLevelInfo, "logout success.");
  }
  else {
    CCLog(kLogLevelError, "logout failed!");
  }
}

void CCAccountManagerImp::clear_download_by_time()
{
//    char cmd[1024] = {0};
//    int ret;
//
//    sprintf(cmd, "find %s -mtime +0  -type f -name \"*.*\" -exec rm -fr {} \\;",  CCConfigManager::GetInstance().get_download_path().c_str());
//
//    ret = system(cmd);
	Delete24hbFile(CCConfigManager::GetInstance().get_download_path().c_str());
    
    return;
}


EErrorCode CCAccountManagerImp::get_user_info(UserInfo & user){
	int error = kSuccess;

	if (!grpc_client_){
		 error= kSystemError;
	}

    CCLog(kLogLevelInfo, "get_user_info username: %s", user_info_.get_user_name().c_str());
    
	GetUserInfoRequest get_user_info_req;
	get_user_info_req.set_appkey(CCConfigManager::GetInstance().get_appkey());
	get_user_info_req.set_username(CCConfigManager::GetInstance().get_username());

	string req_data, response_data;
	get_user_info_req.SerializeToString(&req_data);
	bool send_req;
	send_req = grpc_client_->send_unary_request(service_name_, "getUserInfo", req_data, response_data);
    if (!send_req)	
    {
        CCLog(kLogLevelWarn,"send_req:%d",send_req);
        return kConnectServerFail;	
    }   
    
	GetUserInfoResponse response;
    response.ParseFromString(response_data);
	CCLog(kLogLevelDebug, "get_user_info response is: %s", response.DebugString().c_str());

    error = response.ret();

	user_info_.set_user_name(response.user_infos().username());
	user_info_.set_nick_name(response.user_infos().nick()) ;
	user_info_.set_phone(response.user_infos().phone());
	user_info_.set_avatar_original_link(response.user_infos().original_link());
	user_info_.set_avatar_thumbnail_link(response.user_infos().thumbnail_link());
	user_info_.set_remark_name(response.user_infos().display());
	user_info_.set_nick_name_pinyin(response.user_infos().nick_pinyin());

	user = user_info_;
	
	return (EErrorCode)error;
}

list<FriendInfo> CCAccountManagerImp::get_latest_contact(){
	list<FriendInfo> friend_list;
	string req_data, response_data;
	GetLatestContactRequest req;
	bool send_req;
	
	req.set_appkey(CCConfigManager::GetInstance().get_appkey());
	req.set_username(CCConfigManager::GetInstance().get_username());
	req.SerializeToString(&req_data);
	send_req = grpc_client_->send_unary_request("littlec-history", "getLatestContact", req_data, response_data);
	if (!send_req)	
	{
        CCLog(kLogLevelWarn,"send_req:%d",send_req);
        return friend_list;	
	} 

	GetLatestContactResponse response;
    response.ParseFromString(response_data);

    for(int i = 0; i < response.contact_info_size(); i++) 
	{
        cmhi_iov::FriendInfo friend_info;
        cmhi_iov::FriendInfo friend_inf;
        
        friend_info.set_user_name(response.contact_info(i).contact_username());
        friend_info.set_nick_name(response.contact_info(i).contact_nick());
        friend_info.set_phone(response.contact_info(i).phone());

        //若电话号与名字匹配则更新数据库（每个手机号查询一次数据库，若查询到结果，更新数据库）,因为无法知道具体什么时间成为平台用户，所以只能在获取最近联系人时去更新
        //update_chat_message_remoteusername(response.contact_info(i).contact_username(),response.contact_info(i).phone());
        
        friend_info.set_avatar_original_link(response.contact_info(i).original_link());
        friend_info.set_avatar_thumbnail_link(response.contact_info(i).thumbnail_link());
        friend_info.set_remark_name(response.contact_info(i).contact_display());

        friend_inf = cmhi_iov::get_friend_info((char *)response.contact_info(i).contact_username().c_str());
		//add
		friend_info.set_silent(friend_inf.get_silent());
        friend_info.set_is_enable(friend_inf.get_is_enable());
        friend_info.set_nick_name_pinyin(friend_inf.get_nick_name_pinyin());
		friend_info.set_nick_name_pinyin_blank(friend_inf.get_nick_name_pinyin_blank());
        friend_info.set_remark_name_pinyin(friend_inf.get_remark_name_pinyin());
        friend_info.set_remark_name_pinyin_blank(friend_inf.get_remark_name_pinyin_blank());

        friend_info.set_member_type(friend_inf.get_member_type());
        friend_info.set_series_name(friend_inf.get_series_name());
        friend_info.set_user_enable(friend_inf.get_user_enable());

        string response_str_data = response.message(i).data();
        common::msg::EMsgType  msg_type = response.message(i).msg_type();
        string from_user_name ;
        string app_key;
        string from_nick;
        common::msg::EMsgContentType msg_content_type;
        string data;
        uint64_t guid;
        string msg_id;
        bool burn_after_read = false;
        string to_username;
        bool is_jimao = false;
        bool is_read = false;

        if (msg_type == common::msg::TRANSPARENT_MSG)
        {
            common::msg::TransparentMessage trans_custom_msg;
            trans_custom_msg.ParseFromString(response_str_data);
            app_key = trans_custom_msg.appkey();
            from_user_name = trans_custom_msg.from_username();
            to_username = trans_custom_msg.to_useranem();
            data = trans_custom_msg.data();
            guid = trans_custom_msg.guid();
            msg_id = trans_custom_msg.msg_id();
            msg_content_type = common::msg::CUSTOM;

        }
        else if(msg_type == common::msg::CHAT_MSG)
        {
            common::msg::ChatMessage chat_msg;
            chat_msg.ParseFromString(response_str_data);
            from_user_name = chat_msg.from_username();
            app_key = chat_msg.appkey();
            from_nick =  chat_msg.from_nick();
            msg_content_type = chat_msg.msg_content_type();
            data = chat_msg.data();
            guid = (uint64_t)chat_msg.guid();
            msg_id = chat_msg.msg_id();
            burn_after_read = chat_msg.burn_after_read();
            to_username = chat_msg.to_username();
            is_jimao = chat_msg.is_jimao();
            is_read = chat_msg.is_read();
        }

        MessageWrapperPtr message = shared_ptr<MessageWrapper>(new MessageWrapper());   

		/*
			判断最后一条消息是否自定义消息
		*/
		if(-1 != get_last_msg_by_username((char*)response.contact_info(i).contact_username().c_str(),*message))
		{
			if((message->get_msg_sent_time() > parse_msg_time_by_guid(guid))&&(message->get_msg_type() == kMessageBodyTypeCustom))
			{
				CCLog(kLogLevelInfo,"contact_username:%s,msgId:%s,msgId:%s,%lld,%lld",
					(char*)response.contact_info(i).contact_username().c_str(),message->get_msg_id().c_str(),
					msg_id.c_str(),message->get_msg_sent_time(), parse_msg_time_by_guid(guid));
				msg_content_type = (common::msg::EMsgContentType)kMessageBodyTypeCustom;
				data = message->get_msg_body_info();
			}
		} 

		
        if ((msg_content_type == common::msg::TEXT) || (msg_content_type == common::msg::AUDIO) || (msg_content_type == common::msg::CUSTOM) || 
			(msg_content_type == common::msg::LOCATION) || (msg_content_type == common::msg::FRIEND_MSG))
        {
            string file_name;
            string audio_link;
            uint64_t file_length;
            uint32_t duration;
            string audio_to_word;
            
            ENotificationType type = kNtfMessageReceived; 

            if (kMessageBodyTypeText == (EMessageBodyType)msg_content_type)
            {     
                std::shared_ptr<TextMessageBody> text_msg_body =shared_ptr<TextMessageBody>( new TextMessageBody());
                CCMessageEncoder::deserialize_text_msgbody(data, *text_msg_body);
                message->set_content(text_msg_body->get_text());
                message->init_with_body(text_msg_body, to_username, kChatTypeSingle);
                message->set_msg_body_info(data);
            } 
            else if (kMessageBodyTypeFriend == (EMessageBodyType)msg_content_type)
            {     
                std::shared_ptr<FriendMessageBody> text_msg_body =shared_ptr<FriendMessageBody>( new FriendMessageBody());
                CCMessageEncoder::deserialize_friend_msgbody(data, *text_msg_body);
                message->set_content(text_msg_body->get_text());
                message->init_with_body(text_msg_body, to_username, kChatTypeSingle);
                message->set_msg_body_info(data);
            } 
            else if(kMessageBodyTypeVoice == (EMessageBodyType)msg_content_type) 
            {
                /* 语音文件下载到本地 */
                common::msg::AudioMessage audio_msg;
                audio_msg.ParseFromString(data);
                string local_path =  CCConfigManager::GetInstance().get_download_path() + "/" + audio_msg.file_name();
                string url_path = CCConfigManager::GetInstance().get_file_address() + audio_msg.audio_link();
                Http_Get_File(url_path.c_str(), local_path.c_str());
                std::shared_ptr<AudioMessageBody> audio_msg_body = shared_ptr<AudioMessageBody>(new AudioMessageBody());
                CCMessageEncoder::deserialize_audio_msgbody(data, *audio_msg_body);
                audio_msg_body->set_local_path(local_path);
                message->init_with_body(audio_msg_body, to_username, kChatTypeSingle);
               
                message->set_msg_body_info(data);
                message->set_local_path(local_path);
                message->set_content("[语音]");
				message->set_duration(audio_msg_body->get_duration());
            }        
            else if(kMessageBodyTypeLocation == (EMessageBodyType)msg_content_type) 
            {
                /* 位置文件下载到本地 */
                common::msg::LocationMessage location_msg;
                location_msg.ParseFromString(data);
                string local_path =  CCConfigManager::GetInstance().get_download_path() + "/" + location_msg.file_name();
                string url_path = CCConfigManager::GetInstance().get_file_address() + location_msg.origin_link();
                Http_Get_File(url_path.c_str(), local_path.c_str());
                std::shared_ptr<LocationMessageBody> location_msg_body = shared_ptr<LocationMessageBody>(new LocationMessageBody());
				
				location_msg_body->set_origin_link(location_msg.origin_link());
                CCMessageEncoder::deserialize_location_msgbody(data, *location_msg_body);
                location_msg_body->set_local_path(local_path);
                message->init_with_body(location_msg_body, to_username, kChatTypeSingle);
               
                message->set_msg_body_info(data);
                message->set_local_path(local_path);
                message->set_content("[位置]");
            }
            else if (kMessageBodyTypeCustom == (EMessageBodyType)msg_content_type)
            {
                std::shared_ptr<CustomMessageBody> custom_msg_body = shared_ptr<CustomMessageBody>(new CustomMessageBody());
                CCMessageEncoder::deserialize_custom_msgbody(data, *custom_msg_body);
                message->init_with_body(custom_msg_body, to_username, kChatTypeSingle);
            }

			if (kMessageBodyTypeCustom != (EMessageBodyType)msg_content_type)
			{
	            message->set_msg_id(msg_id);
	            message->set_is_sender(false);  
	            message->set_msg_type((EMessageBodyType)msg_content_type);
	            message->set_chat_type( kChatTypeSingle);
	            message->set_burn_after_read(burn_after_read);
	            message->set_guid(guid);
	            message->set_msg_sent_time(parse_msg_time_by_guid(guid));
	            message->set_is_audio_read(false);
	            message->set_is_msg_read(false);
			}
        }

        friend_info.set_friend_message(message);        

        friend_list.push_back(friend_info);
    }
    CCLog(kLogLevelInfo,"recent frient list size:%d",friend_list.size());
	return friend_list;
}

void CCAccountManagerImp::get_voip_config(string &conf_domain, string &voip_domain){
	conf_domain = CCConfigManager::GetInstance().get_conference_address();
	voip_domain = CCConfigManager::GetInstance().get_voip_address();
}

void CCAccountManagerImp::connect_observer_register(ConnectObserverBase* connect_observer){
	connect_observer_ = connect_observer;
}

}
