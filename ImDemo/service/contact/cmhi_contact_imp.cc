/***************************************************************************
  * file name cmhi_contact_imp.cpp
  * brief ??? ??????
  * date 2019/01/31
  *************************************************************************/

#include <string.h>
#include "formatter.h"
#include "cmhi_contact_imp.h"
#include "cmhi_im_config.h"
#include "cmhi_im_account_imp.h"

#include "sqlite.h"
#include "cmhi_im_config.h"

#include "grpc/grpc_client.h"
#include "api/common/enumeration.pb.h"
#include "api/common/base.pb.h"

#include <thread>
#include <unistd.h>

using namespace outer::user;
using namespace outer::friends;
using namespace common::enumeration;
using namespace outer::connector;
using namespace std;

namespace cmhi_iov{

shared_ptr<ContactManagerImp> ContactManagerImp::getContactManager(){  
	return shared_ptr<ContactManagerImp>(new ContactManagerImp());} 
	
ContactManagerImp::ContactManagerImp() : contact_observer_(nullptr)
{
    service_name_ = "littlec-friend";
    friend_modified_time_ = 0;
    request_modified_time_ = 0;
    signal_modified_time_ = 0;
} 
ContactManagerImp::~ContactManagerImp(){ }


list<FriendInfo> ContactManagerImp::get_friends_list(error_info* error, int page, int count, int *total)
{
    if(!friend_list_ready)
    {
        friend_list_ready=true;
        std::thread syncFriendThread(std::bind(&ContactManagerImp::sync_friend,this));
        syncFriendThread.detach();
    }
    else
    {
        friend_list_ready=false;
    }

    list<FriendInfo> friend_list = cmhi_iov::get_friends_list(page, count, total);
    error->error_code = kSuccess;
    return friend_list;
}

//获取好友个人信息
FriendInfo ContactManagerImp::get_friend_user_info(char* user_id, error_info* error)
    {    
        CCLog(kLogLevelInfo, "username: %s, friend name: %s", CCConfigManager::GetInstance().get_username().c_str(), user_id);
        FriendInfo friend_inf;
        error->error_code = kSuccess;
        //2、数据库获取好友信息
        friend_inf = cmhi_iov::get_friend_info(user_id);

#if 0
    if (friend_inf.get_is_enable() == 0)
    {
        friend_inf.set_is_enable(2);
    }
    
	GetUserInfoRequest get_user_info_req;
	get_user_info_req.set_appkey(CCConfigManager::GetInstance().get_appkey());
	get_user_info_req.set_username(user_id);

	string req_data, response_data;
	get_user_info_req.SerializeToString(&req_data);
	bool send_req;
	send_req = CCGrpcClient::GetInstance().send_unary_request("littlec-user", "getUserInfo", req_data, response_data);
    if (!send_req)	
    {
        return friend_inf;	
    }   
    
	GetUserInfoResponse response;
    response.ParseFromString(response_data);
	CCLog(kLogLevelDebug, "get_user_info response is: %s", response.DebugString().c_str());

    friend_inf.set_avatar_original_link(response.user_infos().original_link());
    friend_inf.set_avatar_thumbnail_link(response.user_infos().thumbnail_link());
	friend_inf.set_nick_name(response.user_infos().nick()) ;
	friend_inf.set_nick_name_pinyin(response.user_infos().nick_pinyin());

    update_friend_Link(user_id, response.user_infos().original_link(), response.user_infos().thumbnail_link());
    update_friend_nick(user_id, response.user_infos().nick(), response.user_infos().nick_pinyin());
 #endif
    return friend_inf;
}

void ContactManagerImp::add_friend(char* friend_name, char* reason, error_info* error)
{

    ApplyFriendRequest request;
    request.set_appkey(CCConfigManager::GetInstance().get_appkey());
    request.set_from_username(CCConfigManager::GetInstance().get_username());
    request.set_to_username(friend_name);
    request.set_remark(reason);
    string msg_id = generate_uuid();
    request.set_msg_id(msg_id);
    request.set_from_client_type(ZEBRA);

    CCLog(kLogLevelInfo, "add_friend username: %s, freiend name: %s, msg_id: %s, reason: %s", 
        CCConfigManager::GetInstance().get_username().c_str(), friend_name, msg_id.c_str(), reason);    

    string request_data, response_data;
    bool send_req;
    request.SerializeToString(&request_data);

    send_req = CCGrpcClient::GetInstance().send_unary_request(service_name_, "applyFriend", request_data, response_data);   
    if (!send_req)	
    {
        return;	
    }    

    ApplyFriendResponse response;
    response.ParseFromString(response_data);
    CCLog(kLogLevelDebug, "add_friend ApplyFriendResponse: %s", response.DebugString().c_str());

    if(error) {
        error->error_code = EErrorCode(response.ret());
        error->error_description = common::errcode::EErrorCode_Name(response.ret());
        CCLog(kLogLevelInfo, "add_friend ret: %d, desc: %s", 
          error->error_code, error->error_description.c_str());
    }
}

void ContactManagerImp::agree_friend_request(char* requestid, error_info *error)
{
    AgreeFriendRequest request;
    request.set_appkey(CCConfigManager::GetInstance().get_appkey());
    request.set_from_username(CCConfigManager::GetInstance().get_username());
    request.set_req_id(requestid);
    string msg_id = generate_uuid();
    request.set_msg_id(msg_id);
    request.set_from_client_type(ZEBRA);
    CCLog(kLogLevelInfo, "agree_friend_request username: %s, requestid: %s, msg_id: %s", 
        CCConfigManager::GetInstance().get_username().c_str(), requestid, msg_id.c_str());

    string request_data, response_data;
    request.SerializeToString(&request_data);
    bool send_req;

    send_req = CCGrpcClient::GetInstance().send_unary_request(service_name_, "agreeFriend", request_data, response_data);
    if (!send_req)	
    {
        return;	
    }

    AgreeFriendResponse response;
    response.ParseFromString(response_data);

    int ret = response.ret();
    if(ret == kSuccess){
        update_friend_request_status(requestid, 1);
        set_same_friend_request_status(requestid, 1);
        sync_friend_change(CCConfigManager::GetInstance().get_username(), 
        	CCConfigManager::GetInstance().get_appkey());
    }
    CCLog(kLogLevelDebug, "agree_friend_request AgreeFriendResponse: %s", response.DebugString().c_str());
    if(error) {
        error->error_code = EErrorCode(response.ret());
        error->error_description = common::errcode::EErrorCode_Name(response.ret());
        CCLog(kLogLevelInfo, "agree_friend_request ret: %d, desc: %s", 
          error->error_code, error->error_description.c_str());
    }

}

void ContactManagerImp::decline_friend_request(char* requestid, char *reason, error_info *error)
{
    RefuseFriendRequest request;
    request.set_appkey(CCConfigManager::GetInstance().get_appkey());
    request.set_from_username(CCConfigManager::GetInstance().get_username());
    request.set_req_id(requestid);
    request.set_remark(reason);
    string msg_id = generate_uuid();
    request.set_msg_id(msg_id);
    request.set_from_client_type(ZEBRA);

    CCLog(kLogLevelInfo, "decline_friend_request username: %s, requestid: %s, msg_id: %s", 
        CCConfigManager::GetInstance().get_username().c_str(), requestid, msg_id.c_str());
    string request_data, response_data;
    request.SerializeToString(&request_data);
    bool send_req;
    
    send_req = CCGrpcClient::GetInstance().send_unary_request(service_name_, "refuseFriend", request_data, response_data);
    if (!send_req)	
    {
        return;	
    }    

    RefuseFriendResponse response;
    response.ParseFromString(response_data);

    CCLog(kLogLevelDebug, "decline_friend_request RefuseFriendResponse: %s", response.DebugString().c_str());

    int ret = response.ret();
    if(ret == kSuccess){
        update_friend_request_status(requestid, 2);
        set_same_friend_request_status(requestid, 2);
    }

    if(error) {
        error->error_code = EErrorCode(response.ret());
        error->error_description = common::errcode::EErrorCode_Name(response.ret());
        CCLog(kLogLevelInfo, "decline_friend_request ret: %d, desc: %s", 
          error->error_code, error->error_description.c_str());
    }
}

//忽略好友  状态为4
void ContactManagerImp::ignore_friend_request(char* requestid, char *reason, error_info *error)
{
    HasIgnoreFriendRequest request;
    request.set_appkey(CCConfigManager::GetInstance().get_appkey());
    request.set_username(CCConfigManager::GetInstance().get_username());
    request.add_req_id(requestid);
    string msg_id = generate_uuid();
    request.set_msg_id(msg_id);
    request.set_from_client_type(ZEBRA);

    CCLog(kLogLevelInfo, "username: %s, requestid: %s",CCConfigManager::GetInstance().get_username().c_str(), requestid);
    CCLog(kLogLevelInfo, "appkey: %s, client_type: %d  msg_id:%s ",CCConfigManager::GetInstance().get_appkey().c_str(), ZEBRA,msg_id.c_str());
    string request_data, response_data;
    request.SerializeToString(&request_data);
    bool send_req;

    send_req = CCGrpcClient::GetInstance().send_unary_request(service_name_, "hasIgnoreFriendApply", request_data, response_data);
    if (!send_req)
    {
        return;
    }

    HasIgnoreFriendResponse response;
    response.ParseFromString(response_data);

    CCLog(kLogLevelDebug, "ignore_friend_request RefuseFriendResponse: %s", response.DebugString().c_str());

    int ret = response.ret();
    if(ret == kSuccess){
        update_friend_request_status(requestid, 4);
        set_same_friend_request_status(requestid, 4);
    }

    if(error) {
        error->error_code = EErrorCode(response.ret());
        error->error_description = common::errcode::EErrorCode_Name(response.ret());
        CCLog(kLogLevelInfo, "ignore_friend_request ret: %d, desc: %s",
              error->error_code, error->error_description.c_str());
    }
}

void ContactManagerImp::remove_friend(char * friend_name,error_info * error)
{
    RemoveFriendRequest request;
    request.set_appkey(CCConfigManager::GetInstance().get_appkey());
    request.set_from_username(CCConfigManager::GetInstance().get_username());
    request.set_to_username(friend_name);
    string msg_id = generate_uuid();
    request.set_msg_id(msg_id);
    request.set_from_client_type(ZEBRA);
    CCLog(kLogLevelInfo, "remove_friend username: %s, friend_name: %s, msg_id: %s", 
        CCConfigManager::GetInstance().get_username().c_str(), friend_name, msg_id.c_str());
    string request_data, response_data;
    request.SerializeToString(&request_data);
    bool send_req;
    
    send_req = CCGrpcClient::GetInstance().send_unary_request(service_name_, "removeFriend", request_data, response_data);
    if (!send_req)	
    {
        CCLog(kLogLevelWarn, "send_req: %d", send_req);
        return;	
    }     

    RemoveFriendResponse response;
    response.ParseFromString(response_data);
    
    CCLog(kLogLevelDebug, "remove_friend RemoveFriendResponse: %s response.ret(): %d", response.DebugString().c_str(),response.ret());
    int ret = response.ret();
    if(ret == kSuccess)
    {
        error_info tmp_error;
        FriendInfo friendInfo;
        friendInfo = get_friend_user_info(friend_name, &tmp_error);

        if (ENABLED == friendInfo.get_is_enable())
        {
            update_friend_enable(friend_name, OWNER_DELETE);
        }
        else if (FRIEND_DELETE == friendInfo.get_is_enable())
        {
            update_friend_enable(friend_name, DISABLED);
        }

        CCLog(kLogLevelDebug, "friend_name:%s", friend_name);
        delete_single_chat_message(friend_name);
    }

    if(error) {
        error->error_code = EErrorCode(response.ret());
        error->error_description = common::errcode::EErrorCode_Name(response.ret());
        CCLog(kLogLevelInfo, "remove_friend ret: %d, desc: %s", error->error_code, error->error_description.c_str());
    }
}

list<RequestWrapper> ContactManagerImp::get_add_friend_request ()
{
	list<RequestWrapper> request_list;
	request_list = get_add_friend_request_list();
	return request_list;
}

int ContactManagerImp::get_recent_contacts(char *recent_contacts)
{
	char contacts_info[1024] = {0};
	strcpy(contacts_info,"name:name1,id:id1");

	return 0;
}

void ContactManagerImp::set_friend_silent(char* friend_name, int silent_type, error_info* error)
{
	UpdateSilentRequest request;
	
    request.set_appkey(CCConfigManager::GetInstance().get_appkey());
    request.set_from_username(CCConfigManager::GetInstance().get_username());
	request.set_friend_username(friend_name);
	request.set_friend_silent((::common::enumeration::ESilent)silent_type);
    string msg_id = generate_uuid();
    request.set_msg_id(msg_id);
	request.set_from_client_type(ZEBRA);

    CCLog(kLogLevelInfo, "set friend silent username: %s, freiend name: %s, msg_id: %s", 
        CCConfigManager::GetInstance().get_username().c_str(), friend_name, msg_id.c_str());   	

    string request_data, response_data;                                                                
    request.SerializeToString(&request_data);
    bool send_req;
    
    send_req = CCGrpcClient::GetInstance().send_unary_request(service_name_, "updateFriendSilent", request_data, response_data);	
    if (!send_req)	
    {
        return;	
    } 

    UpdateSilentRespone response;
    response.ParseFromString(response_data);

    int ret = response.ret();
    if(ret == kSuccess){
        update_friend_silent(friend_name, silent_type);
		 CCLog(kLogLevelInfo, "silent_type=: %d", silent_type );
    }
	
    if(error) {
        error->error_code = EErrorCode(response.ret());
        error->error_description = common::errcode::EErrorCode_Name(response.ret());
        CCLog(kLogLevelInfo, "set_friend_silent ret: %d, desc: %s", 
          error->error_code, error->error_description.c_str());
    }
}

void ContactManagerImp:: set_friend_request_read(RequestWrapper friend_request,CReadFriendResponse& friend_response)
{
	HasReadFriendRequest request;
	request.add_req_id(friend_request.get_request_id());
	request.set_appkey(CCConfigManager::GetInstance().get_appkey());
	request.set_msg_id(generate_uuid());
	request.set_username(CCConfigManager::GetInstance().get_username());
	request.set_from_client_type(ZEBRA);
	
    string request_data, response_data;
    request.SerializeToString(&request_data);
    bool send_req;
    
    send_req = CCGrpcClient::GetInstance().send_unary_request(service_name_, "hasReadFriendApply", request_data, response_data);
	CCLog(kLogLevelInfo, "set_friend_request_read:%d",send_req);
	if (send_req)
	{
		HasReadFriendResponse rsp;
		rsp.ParseFromString(response_data);
		friend_response.set_errorcode((EErrorCode)rsp.ret());
		friend_response.set_msg_id(rsp.msg_id());
		friend_request.set_req_status(3);
		update_friend_request_status((char*)friend_request.get_request_id().c_str(),3);
		set_same_friend_request_status((char*)friend_request.get_request_id().c_str(),3);
	}
	else
	{
		CCLog(kLogLevelInfo, "set_friend_request_read failed");
	}
	
	return ;
}

void ContactManagerImp::sync_friend() {
    SyncFriendRequest request;
    request.set_appkey(CCConfigManager::GetInstance().get_appkey());
    request.set_username(CCConfigManager::GetInstance().get_username());
    request.set_friend_modified(0);
    request.set_friend_req_modified(0);

    string msg_id = generate_uuid();
    request.set_msg_id(msg_id);

	CCLog(kLogLevelInfo, "sync_friend username: %s, msg_id: %s",CCConfigManager::GetInstance().get_username().c_str(), msg_id.c_str());
    string request_data, response_data;
    request.SerializeToString(&request_data);
    bool send_req;
    
    send_req = CCGrpcClient::GetInstance().send_unary_request(service_name_, "syncFriend", request_data, response_data);
    if (!send_req)	
    {
        CCLog(kLogLevelWarn, "sync_friend username: %s error",CCConfigManager::GetInstance().get_username().c_str());
        return;	
    } 

    SyncFriendResponse response;
    response.ParseFromString(response_data);
	CCLog(kLogLevelInfo, "sync_friend ret: %d", response.ret());
	CCLog(kLogLevelDebug, "sync_friend response is: %s", response.DebugString().c_str());
    list<cmhi_iov::FriendInfo> friend_list;
    list<cmhi_iov::RequestWrapper> request_list;
    for(int i=0; i<response.friend_info_size(); i++)
    {
        cmhi_iov::FriendInfo friend_info;
        friend_info.set_user_name(response.friend_info(i).friend_username());
        friend_info.set_nick_name(response.friend_info(i).friend_nick());
        friend_info.set_remark_name(response.friend_info(i).friend_display());
        friend_info.set_phone(response.friend_info(i).phone());
		friend_info.set_nick_name_pinyin(response.friend_info(i).friend_nick_pinyin());
		friend_info.set_nick_name_pinyin_blank(response.friend_info(i).friend_nick_pinyin_blank());
		friend_info.set_remark_name_pinyin(response.friend_info(i).friend_display_pinyin());
        friend_info.set_remark_name_pinyin_blank(response.friend_info(i).friend_display_pinyin_blank());

        friend_info.set_member_type(response.friend_info(i).member_type());
        friend_info.set_series_name(response.friend_info(i).serial_name());
        friend_info.set_user_enable(response.friend_info(i).user_enable());

		if (response.friend_info(i).modified() >friend_modified_time_)
        {
            friend_modified_time_ = response.friend_info(i).modified();
        }

		friend_info.set_is_enable(response.friend_info(i).enable());
		friend_info.set_silent(response.friend_info(i).silent());
        friend_info.set_avatar_original_link(response.friend_info(i).original_link());
        friend_info.set_avatar_thumbnail_link(response.friend_info(i).thumbnail_link());
        if(friend_info.get_is_enable()==ENABLED)
        {
            update_chat_message_remoteusername(friend_info.get_user_name(),friend_info.get_phone());
        }
        update_friend(friend_info);
        //friend_list.push_back(friend_info);
    }

    //这个函数有问题尽量不要用
    //update_friends_list(friend_list);
    
    for(int i=0; i<response.friend_request_size(); i++) {
        cmhi_iov::RequestWrapper friend_request;
        friend_request.set_user_name(response.friend_request(i).from_username());
        friend_request.set_nick_name(response.friend_request(i).from_nick());
        friend_request.set_remark(response.friend_request(i).remark());
        friend_request.set_avatar_original_link(response.friend_request(i).original_link());
        friend_request.set_avatar_thumbnail_link(response.friend_request(i).thumbnail_link());
        friend_request.set_request_id(response.friend_request(i).req_id());
		friend_request.set_request_time(response.friend_request(i).created());
		friend_request.set_req_status(response.friend_request(i).state());
		if (response.friend_request(i).modified() > request_modified_time_  )
			request_modified_time_ = response.friend_request(i).modified() ;
		
        request_list.push_back(friend_request);
    }
    update_friend_request_list(request_list);
	
	FriendNotification friendNotification;
	friendNotification.set_type(kNtfMessageFriendListReady);

	if (contact_observer_) {
        contact_observer_->OnFriendChanged(friendNotification);
    }
    signal_modified_time_ = friend_modified_time_ + 1000;
}

void ContactManagerImp::sync_friend_change(string username, string appkey) {
    SyncFriendRequest request;
    request.set_appkey(appkey);
    request.set_username(username);
    string msg_id = generate_uuid();
    request.set_msg_id(msg_id);
    request.set_friend_modified(friend_modified_time_);
    request.set_friend_req_modified(request_modified_time_);
    request.set_friend_signal_modified(signal_modified_time_);

	CCLog(kLogLevelInfo, "sync_friend_change username: %s, msgid: %s",
        username.c_str(), msg_id.c_str());

    string request_data, response_data;
    request.SerializeToString(&request_data);
    bool send_req;
    
    send_req = CCGrpcClient::GetInstance().send_unary_request(service_name_, "syncFriend", request_data, response_data);
    if (!send_req)	
    {
        return;	
    }     

    SyncFriendResponse response;
    response.ParseFromString(response_data);
	CCLog(kLogLevelInfo, "sync_friend_change ret: %d", response.ret());
	CCLog(kLogLevelDebug, "sync friend change response is: %s", response.DebugString().c_str());
    int ret = response.ret();
    if(ret == kSuccess) {
        if(response.friend_info_size() > 0) {
            for(int i=0; i<response.friend_info_size(); i++) {
				CCLog(kLogLevelDebug, "sync friend change, friend info's username: %s", (response.friend_info(i).friend_username()).c_str());
                cmhi_iov::FriendInfo friend_info;
			    cmhi_iov::FriendInfo friend_info_tmp;
                friend_info.set_user_name(response.friend_info(i).friend_username());
                friend_info.set_nick_name(response.friend_info(i).friend_nick());
                friend_info.set_remark_name(response.friend_info(i).friend_display());
                friend_info.set_phone(response.friend_info(i).phone());
				friend_info.set_nick_name_pinyin(response.friend_info(i).friend_nick_pinyin());
				friend_info.set_nick_name_pinyin_blank(response.friend_info(i).friend_nick_pinyin_blank());
				friend_info.set_remark_name_pinyin(response.friend_info(i).friend_display_pinyin());
				friend_info.set_remark_name_pinyin_blank(response.friend_info(i).friend_display_pinyin_blank());

                friend_info.set_member_type(response.friend_info(i).member_type());
                friend_info.set_series_name(response.friend_info(i).serial_name());
                friend_info.set_user_enable(response.friend_info(i).user_enable());

				if (response.friend_info(i).modified() >friend_modified_time_ )
					friend_modified_time_ = response.friend_info(i).modified();

				friend_info.set_is_enable(response.friend_info(i).enable());
				friend_info.set_silent(response.friend_info(i).silent());
                friend_info.set_avatar_original_link(response.friend_info(i).original_link());
                friend_info.set_avatar_thumbnail_link(response.friend_info(i).thumbnail_link());

                friend_info_tmp = get_friend_info((char *)response.friend_info(i).friend_username().c_str());
                update_friend(friend_info);

                if(friend_info.get_is_enable()==ENABLED)
                {
                    update_chat_message_remoteusername(friend_info.get_user_name(),friend_info.get_phone());
                }

                if(friend_info.get_is_enable()==OWNER_DELETE||friend_info.get_is_enable()==DISABLED)
                {
                    //删除好友消息
                    delete_single_chat_message(friend_info.get_user_name());
                }

				FriendNotification friendNotification;
                friendNotification.set_type(kNtfMessageFriendInfoChange);
				friendNotification.set_user_info(friend_info);
				if (contact_observer_) 
				{
                    contact_observer_->OnFriendChanged(friendNotification);
                }

				if (friend_info.get_is_enable() != friend_info_tmp.get_is_enable())
				{
				    if ((ENABLED == friend_info.get_is_enable()) && (OWNER_DELETE == friend_info_tmp.get_is_enable()))
				    {
                        friendNotification.set_type(kNtfFriendAgree);
        				friendNotification.set_user_info(friend_info);
        				if (contact_observer_) 
        				{
                            contact_observer_->OnFriendChanged(friendNotification);
                        }	
				    }
				    if ((OWNER_DELETE == friend_info.get_is_enable()) && (ENABLED == friend_info_tmp.get_is_enable()))
				    {
                        friendNotification.set_type(kNtfFriendRemove);
        				friendNotification.set_user_info(friend_info);
        				if (contact_observer_) 
        				{
                            contact_observer_->OnFriendChanged(friendNotification);
                        }				    
				    }				    
				}
            }
        }
        if(response.friend_request_size() > 0) {
            for(int i=0; i<response.friend_request_size(); i++) {
				CCLog(kLogLevelDebug, "username: %s state:%d ", (response.friend_request(i).from_username()).c_str(),response.friend_request(i).state());
                cmhi_iov::RequestWrapper friend_request;
                friend_request.set_user_name(response.friend_request(i).from_username());
                friend_request.set_nick_name(response.friend_request(i).from_nick());
                friend_request.set_remark(response.friend_request(i).remark());
                friend_request.set_avatar_original_link(response.friend_request(i).original_link());
                friend_request.set_avatar_thumbnail_link(response.friend_request(i).thumbnail_link());
                friend_request.set_request_id(response.friend_request(i).req_id());
				friend_request.set_request_time(response.friend_request(i).created());
				friend_request.set_req_status(response.friend_request(i).state());
				if (response.friend_request(i).modified() >request_modified_time_)
				request_modified_time_ = response.friend_request(i).modified();

                update_friend_request(friend_request);
					
                FriendNotification friendNotification;
                friendNotification.set_request_id(response.friend_request(i).req_id());

                if (1 == response.friend_request(i).state())
                {
                    friendNotification.set_type(kNtfFriendAgree);
                    friendNotification.set_reason("friend request agree");
                    
                    if (contact_observer_) {
                        contact_observer_->OnFriendChanged(friendNotification);
                    }
                    else 
                    {
    					CCLog(kLogLevelWarn, "add friend request received,but friend change observer not yet registered !!!");
                    }                    
                }
                else if (2 == response.friend_request(i).state())
                {
                    friendNotification.set_type(kNtfFriendRefused);
                    friendNotification.set_reason("friend request refused");
                    
                    if (contact_observer_) {
                        contact_observer_->OnFriendChanged(friendNotification);
                    }
                    else 
                    {
    					CCLog(kLogLevelWarn, "add friend request received,but friend change observer not yet registered !!!");
                    }                    
                }     
                else
                {
                    friendNotification.set_type(kNtfFriendInvite);
                    friendNotification.set_reason("add friend request");

                    if (contact_observer_) {
                        contact_observer_->OnFriendChanged(friendNotification);
                    }else {
    					CCLog(kLogLevelWarn, "add friend request received,but friend change observer not yet registered !!!");
                    }                
                }
            }
        }
        if(response.friend_signal_size() > 0) {
            for(int i=0; i<response.friend_signal_size(); i++) {
                notify_friend_change(response.friend_signal(i));
            }
        }
    }
}

void ContactManagerImp::notify_friend_change(FriendSignal friend_signal) {
    if(friend_signal.friend_signal_type() == outer::friends::REFUSE) {
		CCLog(kLogLevelDebug, "sync friend change, friend refused's username: %s", (friend_signal.from_username()).c_str());
        FriendInfo friendInfo;
        friendInfo.set_user_name(friend_signal.from_username());
        friendInfo.set_nick_name(friend_signal.from_nick());
        FriendNotification friendNotification;
        friendNotification.set_type(kNtfFriendRefused);
        friendNotification.set_reason(friend_signal.remark());
        friendNotification.set_user_info(friendInfo);
		if (friend_signal.modified() > signal_modified_time_)
		{
			signal_modified_time_ = friend_signal.modified();
		}
					
        if (contact_observer_) {
        	contact_observer_->OnFriendChanged(friendNotification);
        } else {
        	CCLog(kLogLevelWarn, "refuse friend request received,but friend change observer not yet registered !!!");
        }
    }
    if(friend_signal.friend_signal_type() == outer::friends::AGREE) {
		CCLog(kLogLevelDebug, "sync friend change, friend agree's username: %s", (friend_signal.from_username()).c_str());
        FriendInfo friendInfo;
        friendInfo.set_user_name(friend_signal.from_username());
        friendInfo.set_nick_name(friend_signal.from_nick());
        FriendNotification friendNotification;
        friendNotification.set_user_info(friendInfo);
        friendNotification.set_type(kNtfFriendAgree);
        friendNotification.set_reason("agree friend request");

		if (friend_signal.modified() > signal_modified_time_)
		{
			signal_modified_time_ = friend_signal.modified();
		}
		
         if (contact_observer_) {
        	contact_observer_->OnFriendChanged(friendNotification);
        } else {
           	CCLog(kLogLevelWarn, "agree friend request received,but friend change observer not yet registered !!!");
        }
    }
	if(friend_signal.friend_signal_type() == outer::friends::REMOVE) {
		CCLog(kLogLevelDebug, "sync friend change, friend remove: %s", (friend_signal.from_username()).c_str());
        FriendInfo friendInfo;
        friendInfo.set_user_name(friend_signal.from_username());
        friendInfo.set_nick_name(friend_signal.from_nick());
        FriendNotification friendNotification;
        friendNotification.set_user_info(friendInfo);
        friendNotification.set_type(kNtfFriendRemove);
        friendNotification.set_reason("remove friend request");
		char from_username[40];
		strcpy(from_username, friend_signal.from_username().c_str());
		
		if (friend_signal.modified() > signal_modified_time_)
		{
			signal_modified_time_ = friend_signal.modified();
		}	
		
		//update_friend_enable(from_username, false);
		
        if (contact_observer_) {
        	contact_observer_->OnFriendChanged(friendNotification);
        } else {
           	CCLog(kLogLevelWarn, "remove friend request received,but friend change observer not yet registered !!!");
        }
    }
}

void ContactManagerImp::update_friend_change(const outer::connector::SessionNotify & notify){
	string appkey = CCConfigManager::GetInstance().get_appkey();
	string username = CCConfigManager::GetInstance().get_username();
	sync_friend_change(username, appkey);
}

void ContactManagerImp::contact_observer_register(ContactObserverBase* contact_observer){
	contact_observer_ = contact_observer;
}

bool ContactManagerImp::is_allow_send_message(string username)
{
    int tmp_enable=get_friend_relation(username);
    if(tmp_enable==FRIEND_DELETE||tmp_enable==OWNER_DELETE||tmp_enable==DISABLED)
    {
        return false;
    }
    return true;
}

}
