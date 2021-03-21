/***************************************************************************
  * file name cmim_chat_imp.cpp
  * brief 单聊 接口实现
  * date 2019/02/13
  *************************************************************************/
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>

#include <json/json.h>
#include <string.h>

#include "cmhi_chat_imp.h"
#include "api/outer/chat.pb.h"
#include "api/common/msg.pb.h"
#include "api/outer/history.pb.h"
#include "api/outer/chat.pb.h"
#include "api/common/enumeration.pb.h"
#include "cmhi_text_message_body.h"
#include "cmhi_file_message_body.h"
#include "cmhi_audio_message_body.h"
#include "cmhi_custom_message_body.h"
#include "cmhi_read_receipt_message_body.h"
#include "cmhi_location_message_body.h"
#include "cmhi_friend_message_body.h"
#include "cmhi_im_config.h"
#include "http_curl.h"
#include "grpc_client.h"
#include "md5.h"
#include "sqlite.h"
#include "formatter.h"
#include "interf_amr.h"
#include <thread>
#include <unistd.h>

using namespace std;
using namespace outer::chat;
//using namespace common::msg;
using namespace outer::history;

namespace cmhi_iov{

static uint64_t rcv_last_timestamp = 0;
static uint64_t send_last_timestamp = 0;


const int kBigFileSize = 1*1024*1024;
static const int PHONE_MAX_LEN = 20;

shared_ptr<ChatManagerImp> ChatManagerImp::getChatManager(){  
	return shared_ptr<ChatManagerImp>(new ChatManagerImp());} 
	
ChatManagerImp::ChatManagerImp(MessageObserverBase* message_observer){ 
	message_observer_ = message_observer;
} 

ChatManagerImp::~ChatManagerImp(){ ;}


/*
	插入自定义信息到数据库 Chat表
*/
static void dump_message_body_info(MessageWrapperPtr message)
{

		if(message == NULL)  return;
		EMessageBodyType msg_content_type = message->get_msg_type();
		if(kMessageBodyTypeVoice == msg_content_type) 
		{
			std::shared_ptr<AudioMessageBody>	audio_message =  std::dynamic_pointer_cast<AudioMessageBody>(message->get_message_body());
			if(audio_message){
				CCLog(kLogLevelWarn,"duration:%d,voice_to_word:%s,file_name:%s,file_length:%d,local_path:%s,origin_link:%s",
				audio_message->get_duration(),
				audio_message->get_voice_to_word().c_str(),
				audio_message->get_file_name().c_str(),
				audio_message->get_file_length(),
				audio_message->get_local_path().c_str(),
				audio_message->get_origin_link().c_str());	
			}
		}
		else if(kMessageBodyTypeLocation == msg_content_type) 
		{
			std::shared_ptr<LocationMessageBody> location_message =	std::dynamic_pointer_cast<LocationMessageBody>(message->get_message_body());
			if(location_message){
				CCLog(kLogLevelWarn,"width:%d,height:%d,longitude:%f,latitude:%f,location_desc:%s,location_address:%s,file_name:%s,file_length:%d,local_path:%s,origin_link:%s,poi_id",
				location_message->get_width(),
				location_message->get_height(),
				location_message->get_longitude(),
				location_message->get_latitude(),
				location_message->get_location_desc().c_str(),
				location_message->get_location_address().c_str(),
				location_message->get_file_name().c_str(),
				location_message->get_file_length(),
				location_message->get_local_path().c_str(),
				location_message->get_origin_link().c_str(),
				location_message->get_poi_id().c_str());
			}
		}
		return;

}


void ChatManagerImp::insert_custom_msg_to_db(CustomMessageBody body,string msg_id,string to,string from,bool is_sender,bool is_msg_read)
{
	common::msg::CustomMessage custom_msg;
	string data;
	custom_msg.set_subtype(body.get_subtype());
	custom_msg.set_notification(body.get_notification());
	 CCLog(kLogLevelDebug, "insert_custom_msg_to_db_is_msg_read=: %d",is_msg_read);
	
	for (int i = 0; i < body.get_entity_size();i++)
	{
		common::msg::CustomEntity *entity = custom_msg.add_custom_entity();
		string key = body.get_entity(i).key;
		string value = body.get_entity(i).value;
		//printf("key=%s,value=%s\n",key.c_str(),value.c_str());
		entity->set_key(key);
		entity->set_value(value);
	}


	
	std::shared_ptr<CustomMessageBody> custom_msg_body = shared_ptr<CustomMessageBody>(new CustomMessageBody(body));
	
	custom_msg.SerializeToString(&data);
	MessageWrapperPtr message = shared_ptr<MessageWrapper>(new MessageWrapper());
	custom_msg_body->set_message_body_type(kMessageBodyTypeCustom);
	message->init_with_body(custom_msg_body,to,kChatTypeSingle);

	/*填充其他字段*/
	message->set_from(from);
	message->set_to(to);
	message->set_msg_id(msg_id);
	message->set_is_sender(false);	
	message->set_msg_type(kMessageBodyTypeCustom);
	message->set_chat_type( kChatTypeSingle);
	message->set_burn_after_read(false);
	message->set_content(data);
	//message->set_guid(guid);
	message->set_msg_sent_time((uint64_t)time(NULL));
	message->set_is_audio_read(false);
	message->set_status(kMessageStatusSendSuccess); 
	message->set_is_msg_read(is_msg_read);
	message->set_msg_body_info(data);
    message->set_is_sender(is_sender);
    if (is_sender)
    {
        message->set_conversation_name(to);
    }
    else
    {
        message->set_conversation_name(from);
    }

    /* 插入数据库 */
    list<MessageWrapper> msg_list;
	MessageWrapper msg(*(MessageWrapper*) message.get());
	msg_list.push_back(msg);
    update_chat_message_list(msg_list);
	return;
}

/*
	返回msg占用的磁盘空间         字节数
*/
int ChatManagerImp::get_disk_size(string dir_name)
{
	char cmd[1024] = {0};
	char res[1024] = {0};
	FILE* fp = NULL;
	int64_t total_size = 0;
	sprintf(cmd,"du -sk %s|awk '{print $1}'",dir_name.c_str());
	//printf("%s\n",cmd);
	fp =popen(cmd,"r");
	
	if(fp)
	{	
		fgets(res,1024,fp);	
	}
	else
	{
		return 0;
	}
	
	pclose(fp);
	total_size = atoi(res);
	
	return total_size*1024;

}

int ChatManagerImp::get_msg_size()
{
	return get_disk_size(CCConfigManager::GetInstance().get_download_path()) + \
		get_disk_size(CCConfigManager::GetInstance().get_database_path());
}

string ChatManagerImp::generate_serial_msg_data(MessageWrapperPtr message){
    common::msg::TextMessage text_msg;
    common::msg::AudioMessage audio_msg;
	common::msg::LocationMessage location_msg;
    string data;
    if (kMessageBodyTypeText == message->get_message_body()->get_message_body_type()){
        text_msg.set_text(dynamic_pointer_cast<TextMessageBody>(message->get_message_body())->get_text());
        text_msg.SerializeToString(&data);
    } else if (kMessageBodyTypeFriend == message->get_message_body()->get_message_body_type()){
        text_msg.set_text(dynamic_pointer_cast<FriendMessageBody>(message->get_message_body())->get_text());
        text_msg.SerializeToString(&data);
    }
	else if(kMessageBodyTypeVoice == message->get_message_body()->get_message_body_type()) {
        shared_ptr<AudioMessageBody> audio_message = dynamic_pointer_cast<AudioMessageBody>(message->get_message_body());
        audio_msg.SerializeToString(&data);
    } else if (kMessageBodyTypeLocation == message->get_message_body()->get_message_body_type()){
        shared_ptr<LocationMessageBody> location_message = dynamic_pointer_cast<LocationMessageBody>(message->get_message_body());
		CCMessageEncoder::serialize_location_msgbody(*location_message, data);		
	}
	else {
        CCLog(kLogLevelWarn, "unknown msg type!");
    }
    return data;
}

//消息发送逻辑梳理：只有当发送消息返回后才会给出回调（业务层必须等待发送函数返回），线程发送中给出发送状态的回调；

EErrorCode ChatManagerImp::send_message(MessageWrapperPtr message)
{
    CCLog(kLogLevelInfo, "get_to:%s",message->get_to().c_str());
    if (get_friend_relation(message->get_to()) == FRIEND_DELETE || get_friend_relation(message->get_to()) == OWNER_DELETE ||get_friend_relation(message->get_to()) == DISABLED)
    {
        CCLog(kLogLevelWarn, "EErrorCode;%d",kFriendIsNotFriend);
        return kFriendIsNotFriend;
    }

	if (!message.get())
	{
		CCLog(kLogLevelWarn, "invalid message, message wrapper is null");
		return kChatMessageInvalid;
	}

    MessageWrapperPtr wrappers = get_chat_message_by_id(message->get_msg_id());
    if (wrappers->get_msg_id() == message->get_msg_id()&& wrappers->get_status()==kMessageStatusSendSuccess)
    {
        CCLog(kLogLevelWarn, "already send success!");
        return  kMeshasSend;
    }
    if (wrappers->get_msg_id() == message->get_msg_id())
    {
        delete_chat_message_by_id(message->get_msg_id());
    }

	EMessageBodyType message_type = message->get_message_body()->get_message_body_type();//获取消息类型
    string file_path,local_path;

    MessageWrapper message_wrapper;
    list<MessageWrapper> msg_list;
    string data = generate_serial_msg_data(message);
    message_wrapper.set_to(message->get_to());
    message_wrapper.set_nick_to(message->get_nick_to());
    message_wrapper.set_from(message->get_from());
    message_wrapper.set_nick_from(message->get_nick_from());
    message_wrapper.set_conversation_name(message->get_to());
    message_wrapper.set_msg_id(message->get_msg_id());
    message_wrapper.set_msg_body_info(data);
    message_wrapper.set_status(kMessageStatusSendFail);
    message_wrapper.set_is_msg_read(true);
    message_wrapper.set_is_sender(true);
    message_wrapper.set_is_audio_read(false);
    message_wrapper.set_duration(message->get_duration());
    message_wrapper.set_msg_type(message->get_message_body()->get_message_body_type());
    message_wrapper.set_chat_type(message->get_chat_type());
    message_wrapper.set_message_body(message->get_message_body());

    if(kMessageBodyTypeVoice == message_type)
    {
        std::shared_ptr<AudioMessageBody> audio_message = dynamic_pointer_cast<AudioMessageBody>(message->get_message_body());
        message->set_duration(audio_message->get_duration());
        message_wrapper.set_local_path(CCConfigManager::GetInstance().get_download_path() + "/" + audio_message->get_file_name());
        if (!audio_message)
        {
            msg_list.push_back(message_wrapper);//新消息写入消息列表中
            update_chat_message_list(msg_list);//更新聊天消息列表
            if (message_observer_) {
                CCLog(kLogLevelWarn,
                      "send message error, message: %d, error code: kChatGetMsgBodyFailed  message_type: %d",
                      message->get_msg_id().c_str(), message_type);
                message_observer_->OnMessageSendStatus(message->get_msg_id(), kMessageStatusSendFail, kChatGetMsgBodyFailed);
            }
            return kChatGetMsgBodyFailed;
        }
        file_path = audio_message->get_local_path();
        local_path = CCConfigManager::GetInstance().get_download_path() + "/" + audio_message->get_file_name();
    }
    else if(kMessageBodyTypeLocation == message_type)
    {
        std::shared_ptr<LocationMessageBody>  location_message =  dynamic_pointer_cast<LocationMessageBody>(message->get_message_body());
        message_wrapper.set_local_path(CCConfigManager::GetInstance().get_download_path() + "/" + location_message->get_file_name());
        if (!location_message)
        {
            msg_list.push_back(message_wrapper);//新消息写入消息列表中
            update_chat_message_list(msg_list);//更新聊天消息列表(写入数据库)
            if (message_observer_)
            {
                CCLog(kLogLevelWarn, "send message error, message: %d, error code: kChatGetMsgBodyFailed  message_type: %d", message->get_msg_id().c_str(), message_type);
                message_observer_->OnMessageSendStatus(message->get_msg_id(), kMessageStatusSendFail, kChatGetMsgBodyFailed);
            }
            return kChatGetMsgBodyFailed;
        }
        file_path = location_message->get_local_path();
        local_path =  CCConfigManager::GetInstance().get_download_path() + "/" + location_message->get_file_name();
    }

    //重发则不需要拷贝文件
    CCLog(kLogLevelDebug, "location_message->get_local_path()=:%s get_download_path:%s",file_path.c_str(),local_path.c_str());
    if(!copy_file(file_path,local_path))
    {
        CCLog(kLogLevelWarn, "copy_file error, message: %s, message_type: %d", message->get_msg_id().c_str(), message_type);
        message_observer_->OnMessageSendStatus(message->get_msg_id(), kMessageStatusSendFail, kSystemError);
        return  kSystemError;
    }

    static bool thread_start = false;
    if(thread_start == false )
    {
        thread_start = true;
        std::thread sendMsgThread(std::bind(&ChatManagerImp::message_send_thread, this));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        sendMsgThread.detach();
    }

    std::lock_guard<std::mutex> lk(mut);
    produced_nums.push(message_wrapper);
    data_cond.notify_one();

	CCLog(kLogLevelDebug, "send return:%d",kSuccess);
	return  kSuccess;
}

EErrorCode ChatManagerImp::resend_message(const string &msg_id)
{
    CCLog(kLogLevelInfo,"Resend message, msgId: %s ", msg_id.c_str());
    std::shared_ptr<MessageWrapper> message = get_msg_by_id(msg_id);

    if (get_friend_relation(message->get_to()) == FRIEND_DELETE || get_friend_relation(message->get_to()) == OWNER_DELETE ||get_friend_relation(message->get_to()) == DISABLED)
    {
        CCLog(kLogLevelWarn, "EErrorCode;%d",kFriendIsNotFriend);
        return kFriendIsNotFriend;
    }

    //判断消息是否为空
    if (!message.get())
    {
        CCLog(kLogLevelWarn, "invalid message, message wrapper is null");
        return kChatMessageInvalid;
    }

    MessageWrapperPtr wrappers = get_chat_message_by_id(message->get_msg_id());
    if (wrappers->get_msg_id() == message->get_msg_id()&& wrappers->get_status()==kMessageStatusSendSuccess)
    {
        CCLog(kLogLevelWarn, "already send success!");
        return  kMeshasSend;
    }
    MessageWrapper message_wrapper;
    list<MessageWrapper> msg_list;
    string data = generate_serial_msg_data(message);
    message_wrapper.set_to(message->get_to());
    message_wrapper.set_nick_to(message->get_nick_to());
    message_wrapper.set_from(message->get_from());
    message_wrapper.set_nick_from(message->get_nick_from());
    message_wrapper.set_conversation_name(message->get_to());
    message_wrapper.set_msg_id(message->get_msg_id());
    message_wrapper.set_msg_body_info(data);
    message_wrapper.set_is_msg_read(true);
    message_wrapper.set_is_sender(true);
    message_wrapper.set_is_audio_read(false);
    message_wrapper.set_duration(message->get_duration());
    message_wrapper.set_msg_type(message->get_message_body()->get_message_body_type());
    message_wrapper.set_chat_type(message->get_chat_type());
    message_wrapper.set_message_body(message->get_message_body());
    message_wrapper.set_guid(message->get_guid());

    EMessageBodyType message_type = message->get_message_body()->get_message_body_type();//获取消息类型
    if(kMessageBodyTypeVoice == message_type)
    {
        std::shared_ptr<AudioMessageBody> audio_message = dynamic_pointer_cast<AudioMessageBody>(message->get_message_body());

        string file_name(message->get_local_path().substr(message->get_local_path().find_last_of('/')+1));
        std::shared_ptr<MessageBody> audio_msg_body(new AudioMessageBody(file_name, message->get_local_path(), 10));
        message_wrapper.set_message_body(audio_msg_body);

        message_wrapper.set_duration(audio_message->get_duration());
        CCLog(kLogLevelInfo,"get_local_path: %s ", message->get_local_path().c_str());
        message_wrapper.set_local_path(message->get_local_path().c_str());
        message_wrapper.set_status(kMessageStatusSendFail);

        if (!audio_message)
        {
            msg_list.push_back(message_wrapper);//新消息写入消息列表中
            update_chat_message_list(msg_list);//更新聊天消息列表
            if (message_observer_) {
                CCLog(kLogLevelWarn,
                      "send message error, message: %d, error code: kChatGetMsgBodyFailed  message_type: %d",
                      message->get_msg_id().c_str(), message_type);
                message_observer_->OnMessageSendStatus(message->get_msg_id(), kMessageStatusSendFail, kChatGetMsgBodyFailed);
            }
            return kChatGetMsgBodyFailed;
        }
    }
    else if (kMessageBodyTypeLocation == message_type)    //位置信息
    {
        std::shared_ptr<LocationMessageBody>  location_message =  dynamic_pointer_cast<LocationMessageBody>(message->get_message_body());
        string file_name(message->get_local_path().substr(message->get_local_path().find_last_of('/')+1));
        std::shared_ptr<MessageBody> location_msg_body(new LocationMessageBody(file_name));
        message_wrapper.set_message_body(location_msg_body);
        message_wrapper.set_local_path(message->get_local_path().c_str());
        if (!location_message)
        {
            msg_list.push_back(message_wrapper);//新消息写入消息列表中
            update_chat_message_list(msg_list);//更新聊天消息列表(写入数据库)
            if (message_observer_)
            {
                CCLog(kLogLevelWarn, "send message error, message: %d, error code: kChatGetMsgBodyFailed  message_type: %d", message->get_msg_id().c_str(), message_type);
                message_observer_->OnMessageSendStatus(message->get_msg_id(), kMessageStatusSendFail, kChatGetMsgBodyFailed);
            }
            return kChatGetMsgBodyFailed;
        }
    }

    std::lock_guard<std::mutex> lk(mut);
    produced_nums.push(message_wrapper);
    data_cond.notify_one();
    CCLog(kLogLevelDebug, "send return:%d",kSuccess);
    return  kSuccess;
}

EErrorCode ChatManagerImp::message_send_thread()
{
    CCLog(kLogLevelDebug, "message_send_thread start!!! id:%d",std::this_thread::get_id());
    while (true) {
        std::unique_lock<std::mutex> lk(mut);   //这里使用unique_lock是为了后面方便解锁
        data_cond.wait(lk);

        MessageWrapper data = produced_nums.front() ;
        produced_nums.pop();
        lk.unlock();

        //数据处理
        string response;
        EErrorCode ret = message_send_post_file(data, response);
        if(ret!=kSuccess)
        {
            continue;
        }
        ret = message_send_chat(data, response);
        if(ret!=kSuccess)
        {
            continue;
        }
    }
}

//上传文件
EErrorCode ChatManagerImp::message_send_post_file(MessageWrapper& message, string& response) {
    CCLog(kLogLevelInfo, "message_id: %s  local_path:%s ", message.get_msg_id().c_str(), message.get_local_path().c_str());

    //删除数据库中的消息
    delete_chat_message_by_id(message.get_msg_id());

    list<MessageWrapper> msg_list;
    EMessageBodyType message_type = message.get_message_body()->get_message_body_type();//获取消息类型  这里取不到

    msg_list.push_back(message);//新消息写入消息列表中
    update_chat_message_list(msg_list);//更新聊天消息列表
    if (message_observer_)
    {
        CCLog(kLogLevelDebug, "sending message, message: %s", message.get_msg_id().c_str());
        message_observer_->OnMessageSendStatus(message.get_msg_id(), kMessageStatusSending, kSuccess);
    }

    //语音消上传文件
    message_type = message.get_message_body()->get_message_body_type();//获取消息类型
    if(kMessageBodyTypeVoice == message_type){
        string url;
        url = CCConfigManager::GetInstance().get_md5_file_address();
        string md5 = MD5Generation(message.get_local_path());
        if (0 == http_get_url_by_md5(url, md5, response)){
            // get url by md5 success.
        }
        else {
            string post_param = generate_post_param(message.get_message_body());
            url = CCConfigManager::GetInstance().get_small_file_address();
            if(Http_Post_File(url.c_str(),post_param.c_str(),message.get_local_path().c_str(),response,message_observer_,progress_callback))
            {
                if (message_observer_)
                {
                    CCLog(kLogLevelWarn, "send error, msg_id: %s, error code: kConnectServerFail", message.get_msg_id().c_str());
                    message_observer_->OnMessageSendStatus(message.get_msg_id(), kMessageStatusSendFail, kConnectServerFail);
                }
                return kConnectServerFail;
            }
        }
    }
    else if (kMessageBodyTypeLocation == message_type)//位置信息
    {
        string url;
        url = CCConfigManager::GetInstance().get_md5_file_address();
        string md5 = MD5Generation(message.get_local_path());
        if (0 == http_get_url_by_md5(url, md5, response)){
            // get url by md5 success.
        }
        else
        {
            string post_param = generate_post_param(message.get_message_body());
            url = CCConfigManager::GetInstance().get_small_file_address();
            if(Http_Post_File(url.c_str(),post_param.c_str(),message.get_local_path().c_str(),response,message_observer_,progress_callback))
            {
                if (message_observer_)
                {
                    CCLog(kLogLevelWarn, "send message error, message: %d, error code: kConnectServerFail", message.get_msg_id().c_str());
                    message_observer_->OnMessageSendStatus(message.get_msg_id(), kMessageStatusSendFail, kConnectServerFail);
                }
                return kConnectServerFail;
            }
        }
    }
    CCLog(kLogLevelDebug, "upload_Typevoice end!!!");
    return kSuccess;
}

EErrorCode ChatManagerImp::message_send_chat(MessageWrapper& message,string post_response){
    CCLog(kLogLevelDebug, "sending message, message: %s  post_response:%s", message.get_msg_id().c_str(), post_response.c_str());
	string method_name = "sendChat";
	ChatMessageRequest chat_message_req;
    string origin_link;

	chat_message_req.set_appkey(CCConfigManager::GetInstance().get_appkey());
	chat_message_req.set_from_username(message.get_from());
	chat_message_req.set_from_nick(message.get_nick_from());
	chat_message_req.set_to_username(message.get_to());
	int content_type = message.get_message_body()->get_message_body_type();
	chat_message_req.set_msg_content_type((common::msg::EMsgContentType)content_type);
	chat_message_req.set_msg_id(message.get_msg_id());
	chat_message_req.set_burn_after_read(message.get_burn_after_read());
	chat_message_req.set_from_client_type(common::enumeration::ZEBRA);
	chat_message_req.set_is_jimao(false);
	
	if(message.get_to().length()< PHONE_MAX_LEN)
    {
        chat_message_req.set_to_user_is_unregistered (true);
        chat_message_req.set_to_phone (message.get_to());   //如果时非平台用户，则需要添加手机号
    }
	if (kMessageBodyTypeText == message.get_message_body()->get_message_body_type())
	{
		CCLog(kLogLevelDebug, "text message");
        message.set_content(dynamic_pointer_cast<TextMessageBody>(message.get_message_body())->get_text());
    } 
	else if (kMessageBodyTypeFriend == message.get_message_body()->get_message_body_type()){
        message.set_content(dynamic_pointer_cast<FriendMessageBody>(message.get_message_body())->get_text());
    }else if(kMessageBodyTypeVoice == message.get_message_body()->get_message_body_type()) {
    	std::shared_ptr<AudioMessageBody>  audio_message =  dynamic_pointer_cast<AudioMessageBody>(message.get_message_body());
        message.set_content("[语音]");
        message.set_duration(audio_message->get_duration());
        origin_link = parse_original_link_from_resp(post_response);
		//local_path = message->get_local_path();
    }else if(kMessageBodyTypeLocation == message.get_message_body()->get_message_body_type()) {
        message.set_content("[位置]");
		std::shared_ptr<LocationMessageBody>  location_message =  dynamic_pointer_cast<LocationMessageBody>(message.get_message_body());
        origin_link = parse_original_link_from_resp(post_response);
		//local_path = message->get_local_path();
    }
    string data = generate_serial_msg_body(message, origin_link);
    chat_message_req.set_data(data);

    message.set_msg_body_info(data);

	string request_data, response;
    chat_message_req.SerializeToString(&request_data);//序列化
    bool send_req = CCGrpcClient::GetInstance().send_unary_request(service_name_, method_name, request_data, response);
	if (!send_req)
	{
        delete_chat_message_by_id(message.get_msg_id());//删除消息
        list<MessageWrapper> msg_list;
        message.set_status(kMessageStatusSendFail);
        msg_list.push_back(message);//新消息写入消息列表中
        update_chat_message_list(msg_list);//更新聊天消息列表
        
        if (message_observer_)
        {
			CCLog(kLogLevelWarn, "send message error, message: %d, error code: kConnectServerFail", message.get_msg_id().c_str());
        	message_observer_->OnMessageSendStatus(message.get_msg_id(), kMessageStatusSendFail, kConnectServerFail);
        }
	    return kConnectServerFail;
	}

	ChatMessageResponse response_data;
    EMessageStatus send_status;
	response_data.ParseFromString(response);
    EErrorCode data_ret = (EErrorCode)response_data.ret();
    if(data_ret==kSuccess)
    {
        send_status = kMessageStatusSendSuccess;
    }
    else{
        send_status = kMessageStatusSendFail;
    }
    message.set_guid(response_data.guid());
    message.set_status(send_status);

    delete_chat_message_by_id(message.get_msg_id());//删除消息
    list<MessageWrapper> msg_list;
    msg_list.push_back(message);//新消息写入消息列表中
    update_chat_message_list(msg_list);//更新聊天消息列表

    if (message_observer_)
    {
        CCLog(kLogLevelWarn, "message:%s send status:%d errorcode:%d", message.get_msg_id().c_str(),send_status, data_ret);
        message_observer_->OnMessageSendStatus(message.get_msg_id(), send_status, data_ret);
    }
	return data_ret ;
}

string ChatManagerImp::generate_serial_msg_body(MessageWrapper& message, string origin_link){
    common::msg::TextMessage text_msg;
    common::msg::AudioMessage audio_msg;
	common::msg::LocationMessage location_msg;
    string data;
    if (kMessageBodyTypeText == message.get_message_body()->get_message_body_type()){
        text_msg.set_text(dynamic_pointer_cast<TextMessageBody>(message.get_message_body())->get_text());
        text_msg.set_burn_after_read(message.get_burn_after_read());
        text_msg.SerializeToString(&data);
    } else if (kMessageBodyTypeFriend == message.get_message_body()->get_message_body_type()){
        text_msg.set_text(dynamic_pointer_cast<FriendMessageBody>(message.get_message_body())->get_text());
        text_msg.set_burn_after_read(message.get_burn_after_read());
        text_msg.SerializeToString(&data);
    }
	else if(kMessageBodyTypeVoice == message.get_message_body()->get_message_body_type()) {
        shared_ptr<AudioMessageBody> audio_message = dynamic_pointer_cast<AudioMessageBody>(message.get_message_body());
        audio_msg.set_file_name(audio_message->get_file_name());
        audio_msg.set_file_length(audio_message->get_file_length());
        audio_msg.set_duration(audio_message->get_duration());
        audio_msg.set_audio_link(origin_link);
        audio_msg.SerializeToString(&data);
    } else if (kMessageBodyTypeLocation == message.get_message_body()->get_message_body_type()){
        shared_ptr<LocationMessageBody> location_message = dynamic_pointer_cast<LocationMessageBody>(message.get_message_body());
        #if 0
		location_msg.set_file_name(location_message->get_file_name());
        location_msg.set_file_length(location_message->get_file_length());
        location_msg.set_origin_link(origin_link);
        location_msg.SerializeToString(&data);
		#endif
		location_message->set_origin_link(origin_link);
		CCMessageEncoder::serialize_location_msgbody(*location_message, data);		
	}
	else {
        CCLog(kLogLevelWarn, "unknown msg type!");
    }
    return data;
}
/*
 	自定义消息同端同步 不保存数据库
*/
void ChatManagerImp::transparent_carbon_msg_recv(const outer::connector::SessionNotify & notify)
{
	common::ntf::TransparentMsgCarbonNotification  trans_msg;
	trans_msg.ParseFromString(notify.data());
	common::msg::MessageUnit msg_unit = trans_msg.unit();
	common::msg::TransparentMessage custom_msg;
	custom_msg.ParseFromString(msg_unit.data());
		
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

	app_key = custom_msg.appkey();
	from_user_name = custom_msg.from_username();
	to_username = custom_msg.to_useranem();
	data = custom_msg.data();
	guid = custom_msg.guid();
	msg_id = custom_msg.msg_id();
	msg_content_type = common::msg::CUSTOM;
	from_nick = custom_msg.from_nick();

	/*
		先初始化 body，再初始化其他字段
	*/
	
	MessageWrapperPtr message = shared_ptr<MessageWrapper>(new MessageWrapper());
	std::shared_ptr<CustomMessageBody>  custom_msg_body = shared_ptr<CustomMessageBody>(new CustomMessageBody());

	CCMessageEncoder::deserialize_custom_msgbody(data, *custom_msg_body);
	CCLog(kLogLevelWarn, "transparent_carbon_msg_recv_custom_msg_body-> get_subtype()：%s",custom_msg_body-> get_subtype().c_str());
	message->init_with_body(custom_msg_body, to_username, kChatTypeSingle);
	message->set_msg_body_info(data);

	/*填充其他字段*/
	message->set_from(from_user_name);
	message->set_nick_from(from_nick);
	message->set_msg_id(msg_id);
	message->set_is_sender(false);	
	message->set_msg_type(message->get_message_body()->get_message_body_type());
	message->set_burn_after_read(burn_after_read);
	message->set_content(data);
	message->set_guid(guid);
	message->set_msg_sent_time(parse_msg_time_by_guid(guid));
	message->set_is_audio_read(false);
	message->set_is_msg_read(false);
	message->set_is_online(true);

	//去重
    MessageWrapperPtr messageTemp = shared_ptr<MessageWrapper>(new MessageWrapper());
    messageTemp = get_chat_message_by_id(msg_id);
    if (messageTemp->get_msg_id() == msg_id)
    {
        return;
    }

	/*
				业务层的回调callback 
	*/
	if (message_observer_)
	{
		CCLog(kLogLevelWarn, "msg_sent_time:%lld,%lld",message->get_msg_sent_time(),parse_msg_time_by_guid(guid));
		message_observer_->OnMessageReceived(message);
	}			
	return;
}

/*
	// 透传消息，用于拉取自定义通知消息&平台内部交互
*/
void ChatManagerImp::transparent_msg_recv(const outer::connector::SessionNotify & notify)
{
	common::ntf::TransparentMsgNotification  trans_msg;
	trans_msg.ParseFromString(notify.data());
	common::msg::MessageUnit msg_unit = trans_msg.unit();
	common::msg::TransparentMessage tran_custom_msg;
	tran_custom_msg.ParseFromString(msg_unit.data());
		
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

	app_key = tran_custom_msg.appkey();
	from_user_name = tran_custom_msg.from_username();
	CCLog(kLogLevelDebug, "tran_custom_msg.from_username()=%s",tran_custom_msg.from_username().c_str());
	to_username = tran_custom_msg.to_useranem();
	data = tran_custom_msg.data();
	guid = tran_custom_msg.guid();
	CCLog(kLogLevelDebug, " tran_custom_msg_guid=:%d",guid);
	msg_id = tran_custom_msg.msg_id();
	msg_content_type = common::msg::CUSTOM;

	/*
		先初始化 body，再初始化其他字段
	*/
	
	MessageWrapperPtr message = shared_ptr<MessageWrapper>(new MessageWrapper());
	std::shared_ptr<CustomMessageBody>  custom_msg_body = shared_ptr<CustomMessageBody>(new CustomMessageBody());
	
	CCMessageEncoder::deserialize_custom_msgbody(data, *custom_msg_body);
	CCLog(kLogLevelDebug, "custom_msg_body-> get_subtype()：%s",custom_msg_body-> get_subtype().c_str());
	message->init_with_body(custom_msg_body, to_username, kChatTypeSingle);
	message->set_msg_body_info(data);

	/*填充其他字段*/
	message->set_from(from_user_name);
	message->set_msg_id(msg_id);
	message->set_is_sender(false);	
	message->set_msg_type(message->get_message_body()->get_message_body_type());
	message->set_burn_after_read(burn_after_read);
	message->set_content(data);
	message->set_guid(guid);
	message->set_msg_sent_time(parse_msg_time_by_guid(guid));
	message->set_is_audio_read(false);
	message->set_is_msg_read(false);
	message->set_is_online(true);

    MessageWrapperPtr messageTemp = shared_ptr<MessageWrapper>(new MessageWrapper());
    messageTemp = get_chat_message_by_id(msg_id);//不包括自定义消息
 
    if (messageTemp->get_msg_id() == msg_id)
    {
        return;
    }
    
	/*
		业务层的回调callback 
	*/
	if (message_observer_)
	{
		#if 0
			if(custom_msg_body-> get_subtype()=="3000")
			   {
				   CCLog(kLogLevelWarn, "msg_sent_time:%lld,%lld",message->get_msg_sent_time(),parse_msg_time_by_guid(guid));
				   message_observer_->OnMessageReceived(message);
				}
		#endif

		CCLog(kLogLevelWarn, "msg_sent_time:%lld,%lld",message->get_msg_sent_time(),parse_msg_time_by_guid(guid));
			message_observer_->OnMessageReceived(message);
	}
	/*
	 	不保存数据库
	*/
	return;
}

//消息同端同步
void ChatManagerImp::carbon_msg_recv()
{
	MessageSyncRequest request_data; 
	string method_name = "messageSync";	
	
	/* 
		guid 不需要填充
	*/

    request_data.set_username(CCConfigManager::GetInstance().get_username());
	request_data.set_appkey(CCConfigManager::GetInstance().get_appkey());
	request_data.set_sor(true);
	request_data.set_type(outer::history::END_TYPE::WEB);
    request_data.set_modified_timestamp(send_last_timestamp);

	/*
		发送获取消息请求
	*/
	string request_data_serial;
	string response_data;
	bool send_req;
	request_data.SerializeToString(&request_data_serial);
	send_req = CCGrpcClient::GetInstance().send_unary_request("littlec-history", method_name, request_data_serial, response_data);
	if (!send_req)
	{
        CCLog(kLogLevelWarn,"send_req:%d",send_req);
	    return;
	}
	MsgGetResponse response;
	response.ParseFromString(response_data);

	int ret = response.ret();
	int16_t data_size =  response.data_size();
    send_last_timestamp = response.modified_timestamp();

	for (int i = data_size - 1; i >= 0; i--)
	{
		string response_str_data = response.data(i).data();
		common::msg::EMsgType  msg_type = response.data(i).msg_type();
		string from_user_name;
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

		usleep(1);

        if (msg_type == common::msg::TRANSPARENT_MSG)  //透传消息
		{
			CCLog(kLogLevelDebug, "TRANSPARENT_MSG:");
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
		else if(msg_type == common::msg::CHAT_MSG)          //单聊消息
		{
			CCLog(kLogLevelDebug, " common::msg::CHAT_MSG");
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
/*
		if ((msg_content_type != common::msg::TEXT) && (msg_content_type != common::msg::AUDIO) && 
			(msg_content_type != common::msg::RETRACT) && (msg_content_type != common::msg::CUSTOM))
		{
			continue;
		}
*/		
		string file_name;
		string audio_link;
		uint64_t file_length;
		uint32_t duration;
		string audio_to_word;
        
		/*	先初始化 body，再初始化其他字段  */		
		MessageWrapperPtr message = shared_ptr<MessageWrapper>(new MessageWrapper());	
		ENotificationType type = kNtfMessageReceived;
		
        if (kMessageBodyTypeText == (EMessageBodyType)msg_content_type)
        {     
			std::shared_ptr<TextMessageBody>  text_msg_body = shared_ptr<TextMessageBody>(new TextMessageBody());
			
			CCMessageEncoder::deserialize_text_msgbody(data, *text_msg_body);
            message->set_content(text_msg_body->get_text());
            message->init_with_body(text_msg_body, to_username, kChatTypeSingle);
            message->set_msg_body_info(data);
			
        }
		else if (kMessageBodyTypeFriend == (EMessageBodyType)msg_content_type)
        {     
			std::shared_ptr<FriendMessageBody>  text_msg_body = shared_ptr<FriendMessageBody>(new FriendMessageBody());
			
			CCMessageEncoder::deserialize_friend_msgbody(data, *text_msg_body);
            message->set_content(text_msg_body->get_text());
            message->init_with_body(text_msg_body, to_username, kChatTypeSingle);
            message->set_msg_body_info(data);
			
        }
        else if(kMessageBodyTypeVoice == (EMessageBodyType)msg_content_type) 
        {
        	/*语音文件下载到本地*/
            common::msg::AudioMessage audio_msg;
        	audio_msg.ParseFromString(data);
        	string local_path =  CCConfigManager::GetInstance().get_download_path() + "/" + audio_msg.file_name();
        	string url_path = CCConfigManager::GetInstance().get_file_address() + audio_msg.audio_link();
        	Http_Get_File(url_path.c_str(), local_path.c_str());
			
			std::shared_ptr<AudioMessageBody>  audio_msg_body = shared_ptr<AudioMessageBody> (new AudioMessageBody());
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
        	/*语音文件下载到本地*/
            common::msg::LocationMessage location_msg;
        	location_msg.ParseFromString(data);
        	string local_path =  CCConfigManager::GetInstance().get_download_path() + "/" + location_msg.file_name();
        	string url_path = CCConfigManager::GetInstance().get_file_address() + location_msg.origin_link();
        	Http_Get_File(url_path.c_str(), local_path.c_str());
			
			std::shared_ptr<LocationMessageBody>  location_msg_body = shared_ptr<LocationMessageBody> (new LocationMessageBody());
            CCMessageEncoder::deserialize_location_msgbody(data, *location_msg_body);
            location_msg_body->set_local_path(local_path);
            message->init_with_body(location_msg_body, to_username, kChatTypeSingle);
           
            message->set_msg_body_info(data);
            message->set_local_path(local_path);
            message->set_content("[位置]");
			
        }
		
        else if (kMessageBodyTypeCustom == (EMessageBodyType)msg_content_type)
		{
			std::shared_ptr<CustomMessageBody> custom_msg_body =  shared_ptr<CustomMessageBody>(new CustomMessageBody());
			CCMessageEncoder::deserialize_custom_msgbody(data, *custom_msg_body);
            message->init_with_body(custom_msg_body, to_username, kChatTypeSingle);
			message->set_msg_body_info(data);
		}
        else if (kMessageBodyTypeRetract == (EMessageBodyType)msg_content_type)
        {
            common::msg::RetractMessage retract_msg;
            message->set_status(kMessageStatusSendRetracted);            
            retract_msg.ParseFromString(data);
            message->set_msg_body_info(data);
			message->set_to(to_username);
            uint64_t guid = retract_msg.retract_guid();
                
            update_chat_message_status(guid, kMessageStatusSendRetracted);          
        } 
        else if (kMessageBodyTypeReceipt == (EMessageBodyType)msg_content_type)
        {
            uint64_t msg_guid;
            common::msg::ReadReceiptMessage read_receipt_msg;
            read_receipt_msg.ParseFromString(data);
            msg_guid = read_receipt_msg.receipt_guid();
        
            uint64_t time = parse_msg_time_by_guid(msg_guid);
            
            set_chat_message_read_status_by_time(to_username, time);

            MessageWrapperPtr messageTemp = shared_ptr<MessageWrapper>(new MessageWrapper());
            messageTemp = get_chat_message_by_guid(msg_guid);
            if (messageTemp->get_guid() == msg_guid)
            {
                msg_id = messageTemp->get_msg_id();
            }   
            
            std::shared_ptr<ReadReceiptMessageBody> read_receipt_msg_body =  shared_ptr<ReadReceiptMessageBody>(new ReadReceiptMessageBody());
            CCMessageEncoder::deserialize_read_receipt_msgbody(data, *read_receipt_msg_body);
            message->init_with_body(read_receipt_msg_body, to_username, kChatTypeSingle);
            message->set_msg_body_info(data);
        }
        else if (kMessageBodyTypeSingleReceipt == (EMessageBodyType)msg_content_type)
        {
            uint64_t msg_guid;
            common::msg::ReadReceiptMessage read_receipt_msg;
            read_receipt_msg.ParseFromString(data);
            msg_guid = read_receipt_msg.receipt_guid();
            set_chat_message_read_status_by_guid(msg_guid); 

            MessageWrapperPtr messageTemp = shared_ptr<MessageWrapper>(new MessageWrapper());
            messageTemp = get_chat_message_by_guid(msg_guid);
            if (messageTemp->get_guid() == msg_guid)
            {
                msg_id = messageTemp->get_msg_id();
            }           
             
            std::shared_ptr<ReadReceiptMessageBody> read_receipt_msg_body =  shared_ptr<ReadReceiptMessageBody>(new ReadReceiptMessageBody());
            CCMessageEncoder::deserialize_read_receipt_msgbody(data, *read_receipt_msg_body);
            
            message->init_with_body(read_receipt_msg_body, to_username, kChatTypeSingle);
            message->set_msg_body_info(data);            
        }        
        else 
        {
			
		}

        if ((kMessageBodyTypeText == (EMessageBodyType)msg_content_type) || (kMessageBodyTypeFriend == (EMessageBodyType)msg_content_type)
             || (kMessageBodyTypeVoice == (EMessageBodyType)msg_content_type) || (kMessageBodyTypeLocation == (EMessageBodyType)msg_content_type)
             || (kMessageBodyTypeCustom == (EMessageBodyType)msg_content_type))
        {
            MessageWrapperPtr messageTemp = shared_ptr<MessageWrapper>(new MessageWrapper());
            messageTemp = get_chat_message_by_id(msg_id);
            if (messageTemp->get_msg_id() == msg_id)
            {
                continue;
            }
        }

        /*填充其他字段*/
		message->set_from(from_user_name);
		message->set_nick_from(from_nick);
		message->set_to(to_username);	
		message->set_conversation_name(to_username);
		message->set_msg_id(msg_id);
		message->set_is_sender(true);	
		message->set_msg_type((EMessageBodyType)msg_content_type);
		message->set_burn_after_read(burn_after_read);
		message->set_guid(guid);
		message->set_msg_sent_time(parse_msg_time_by_guid(guid));
		message->set_is_msg_read(true);
		message->set_is_audio_read(true);
		message->set_is_online(true);

        if ((common::msg::RETRACT != msg_content_type) && (common::msg::CUSTOM != msg_content_type)
            && (common::msg::READ_RECEIPT != msg_content_type) && (common::msg::SINGLE_READ_RECEIPT != msg_content_type)) 
        {
            list<MessageWrapper> msg_list;
		
            MessageWrapper chat_recv_msg(*(MessageWrapper*) message.get());
            msg_list.push_back(chat_recv_msg);
            update_chat_message_list(msg_list);

        }
        
		/*
					业务层的回调callback 
		*/
		if (message_observer_)
		{
			CCLog(kLogLevelWarn, "msg_sent_time:%lld,%lld",message->get_msg_sent_time(),parse_msg_time_by_guid(guid));
			message_observer_->OnMessageReceived(message);
		}
	}
	return ;
}

//获取离线客户消息
void ChatManagerImp::get_offline_custom_message(const outer::connector::SessionNotify& notify)
{
	CCLog(kLogLevelDebug, "rcv_last_timestamp=:%lu",rcv_last_timestamp);
	GetServerTimestampRequest stamp_request_data;
    uint64_t timestamp = 0;
    
	/*
		发送获取消息请求
	*/
	string stamp_request_data_serial, stamp_response_data;
	stamp_request_data.SerializeToString(&stamp_request_data_serial);
	bool send_req;

	//获取服务器时间
	send_req = CCGrpcClient::GetInstance().send_unary_request("littlec-chat", "getServerTimestamp", stamp_request_data_serial, stamp_response_data);
	if (!send_req)	
	{
        CCLog(kLogLevelWarn,"send_req:%d",send_req);
        return;	
	}
    
	GetServerTimestampResponse stamp_response;
	stamp_response.ParseFromString(stamp_response_data);
	timestamp =  stamp_response.timestamp();//服务器当前的时间戳
	CCLog(kLogLevelDebug, "now_timestamp_2=:%lu",timestamp);

	if (0 == rcv_last_timestamp)
	{
        rcv_last_timestamp = timestamp;         //如果是正常登录 就获取服务器当前时间戳以后的内容 ,不推送消息
    }

    if (0 == send_last_timestamp)
    {
        send_last_timestamp = timestamp;
    }

    //毫秒级别，推送10小时的消息
    CCLog(kLogLevelDebug, "rcv_last_timestamp=:%lu",rcv_last_timestamp);
    timestamp = timestamp - (10 * 60 * 60 * 1000);



	OfflineCustomMessageSyncRequest request_data; 

    request_data.set_username(CCConfigManager::GetInstance().get_username());
	request_data.set_appkey(CCConfigManager::GetInstance().get_appkey());
	request_data.set_sor(false);
	request_data.set_modified_timestamp(  timestamp );

    CCLog(kLogLevelDebug, "username:%s appkey:%s sor:%d timestamp:%lu",
            CCConfigManager::GetInstance().get_username().c_str(),
            CCConfigManager::GetInstance().get_appkey().c_str(),false,
            timestamp);

    CCLog(kLogLevelDebug, "timestamp=:%lu",timestamp);

	/*
		发送获取消息请求
	*/
	string request_data_serial, response_data;
	request_data.SerializeToString(&request_data_serial);

	send_req = CCGrpcClient::GetInstance().send_unary_request("littlec-history", "offlineCustomMessageSync", request_data_serial, response_data);
	if (!send_req)	
	{
        CCLog(kLogLevelWarn, "send_req:%d",send_req);
        return;	
	}
    
	MsgGetResponse response;
	response.ParseFromString(response_data);
	int ret = response.ret();
	int16_t data_size =  response.data_size();
	list<MessageWrapper> msg_list;

    CCLog(kLogLevelDebug, "data_size:%d",data_size);
	for (int i = 0; i < data_size; i++)
	{
		string response_str_data = response.data(i).data();
		common::msg::EMsgType  msg_type = response.data(i).msg_type();
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

		usleep(1);

        if (msg_type == common::msg::TRANSPARENT_MSG)
		{
			CCLog(kLogLevelDebug, "common::msg::TRANSPARENT_MSG");
			common::msg::TransparentMessage trans_custom_msg;
			trans_custom_msg.ParseFromString(response_str_data);
			app_key = trans_custom_msg.appkey();
			from_user_name = trans_custom_msg.from_username();
			CCLog(kLogLevelDebug, "trans_custom_msg.from_username()=%s",trans_custom_msg.from_username().c_str());
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

		if (msg_content_type != common::msg::CUSTOM)
		{
			continue;
		}

        MessageWrapperPtr messageTemp = shared_ptr<MessageWrapper>(new MessageWrapper());
        messageTemp = get_chat_message_by_id(msg_id);
        if (messageTemp->get_msg_id() == msg_id)
        {
            continue;
        }
            
		/*	先初始化 body，再初始化其他字段  */		
		MessageWrapperPtr message = shared_ptr<MessageWrapper>(new MessageWrapper());	
		ENotificationType type = kNtfMessageReceived;		
        
        if (kMessageBodyTypeCustom == (EMessageBodyType)msg_content_type)
		{

			CCLog(kLogLevelDebug, "kMessageBodyTypeCustom == (EMessageBodyType)msg_content_type");
		    
			std::shared_ptr<CustomMessageBody> custom_msg_body = shared_ptr<CustomMessageBody>(new CustomMessageBody());
			CCMessageEncoder::deserialize_custom_msgbody(data, *custom_msg_body);
			CCLog(kLogLevelDebug, "custom_msg_body-> get_subtype()：%s",custom_msg_body-> get_subtype().c_str());
            message->init_with_body(custom_msg_body, to_username, kChatTypeSingle);
			message->set_msg_body_info(data);
		}		
        else
        {
            continue;    
        }

        /*填充其他字段*/
		message->set_from(from_user_name);
		message->set_nick_from(from_nick);
		message->set_msg_id(msg_id);
		message->set_is_sender(false);	
		message->set_msg_type((EMessageBodyType)msg_content_type);
		message->set_chat_type( kChatTypeSingle);
		message->set_burn_after_read(burn_after_read);
		message->set_guid(guid);
		message->set_msg_sent_time(parse_msg_time_by_guid(guid));
		message->set_is_audio_read(false);
		message->set_is_msg_read(false);
        message->set_is_online(false);
		message->set_to(from_user_name);
		CCLog(kLogLevelDebug, " offline_custom_msg_guid=:%d",guid);

			/*
				业务层的回调callback 
			*/
			if (message_observer_)
			{
                CCLog(kLogLevelInfo, "msg_id:%s username:%s",message->get_msg_id().c_str(),message->get_conversation_name().c_str());
				message_observer_->OnMessageReceived(message);
			}
		}
	
	return ;
}

void ChatManagerImp::message_timestamp_clear()
{
    rcv_last_timestamp = 0;
    send_last_timestamp = 0;
}

void ChatManagerImp::receive_message(const outer::connector::SessionNotify& notify,UserInfo user)
{
	MessageSyncRequest request_data;
    request_data.set_username(CCConfigManager::GetInstance().get_username());
	request_data.set_appkey(CCConfigManager::GetInstance().get_appkey());
	request_data.set_sor(false);//false接收消息
	request_data.set_type(outer::history::END_TYPE::WEB);
    request_data.set_modified_timestamp(rcv_last_timestamp);//要获取的最新消息的时间戳
    CCLog(kLogLevelInfo, "username:%s appkey:%s sor:%d timestamp:%lu type:%d ",request_data.username().c_str(),
          request_data.appkey().c_str(),request_data.sor(),request_data.modified_timestamp(),request_data.type());
	/*
		发送获取消息请求
	*/
	string request_data_serial, response_data;
	request_data.SerializeToString(&request_data_serial);//消息序列化
	bool send_req;

	send_req = CCGrpcClient::GetInstance().send_unary_request("littlec-history", "messageSync", request_data_serial, response_data);
	if (!send_req)	
	{
        CCLog(kLogLevelWarn,"send_req:%d",send_req);
        return;	
	}
    
	MsgGetResponse response;
	response.ParseFromString(response_data);//解析服务器返回的消息
	int ret = response.ret();
	int16_t data_size =  response.data_size();
 	CCLog(kLogLevelDebug, "receive_data_size=:%d",response.data_size());
	if(response.data_size()!=0)
	{
		rcv_last_timestamp = response.modified_timestamp();
	   CCLog(kLogLevelDebug, "rcv_last_timestamp_now=:%lu",rcv_last_timestamp);
	}
	for (int i = data_size - 1; i >= 0; i--)
	{
		string response_str_data = response.data(i).data();
		common::msg::EMsgType  msg_type = response.data(i).msg_type();
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

        usleep(1);
		
        if (msg_type == common::msg::TRANSPARENT_MSG)
		{
		    CCLog(kLogLevelDebug, "receive_common::msg::TRANSPARENT_MSG");
			common::msg::TransparentMessage trans_custom_msg;
			trans_custom_msg.ParseFromString(response_str_data);
			app_key = trans_custom_msg.appkey();
			from_user_name = trans_custom_msg.from_username();
			
			to_username = trans_custom_msg.to_useranem();
			data = trans_custom_msg.data();
		//	CCLog(kLogLevelDebug, "receive_TRANSPARENT_MSG_data=:%s",data.size());
			guid = trans_custom_msg.guid();
			msg_id = trans_custom_msg.msg_id();
			msg_content_type = common::msg::CUSTOM;\
			
		}
		else if(msg_type == common::msg::CHAT_MSG)
		{
			CCLog(kLogLevelDebug, "receive_common::msg::CHAT_MSG");
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
		CCLog(kLogLevelDebug, "receivemessage_guid=:%d",guid);
		
/*
		if ((msg_content_type != common::msg::TEXT) && (msg_content_type != common::msg::AUDIO) && 
			(msg_content_type != common::msg::RETRACT) && (msg_content_type != common::msg::CUSTOM))
		{
			continue;
		}
*/		
		//guid_last = guid;
		
		string file_name;
		string audio_link;
		uint64_t file_length;
		uint32_t duration;
		string audio_to_word;
        
		/*	先初始化 body，再初始化其他字段  */		
		MessageWrapperPtr message = shared_ptr<MessageWrapper>(new MessageWrapper());	
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
            CCLog(kLogLevelDebug, "from_user_name:%s", from_user_name.c_str());
            //delete_single_chat_message(from_user_name);
		}
        else if (kMessageBodyTypeVoice == (EMessageBodyType)msg_content_type) 
        {
        	/*语音文件下载到本地*/
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
		
		else if (kMessageBodyTypeLocation == (EMessageBodyType)msg_content_type) 
        {
        	/*语音文件下载到本地*/
            common::msg::LocationMessage location_msg;
        	location_msg.ParseFromString(data);
        	string local_path =  CCConfigManager::GetInstance().get_download_path() + "/" + location_msg.file_name();
        	string url_path = CCConfigManager::GetInstance().get_file_address() + location_msg.origin_link();
        	Http_Get_File(url_path.c_str(), local_path.c_str());
			
			std::shared_ptr<LocationMessageBody>  location_msg_body = shared_ptr<LocationMessageBody> (new LocationMessageBody());
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
			CCLog(	kLogLevelDebug, "receive_custom_msg_body-> get_subtype()：%s",custom_msg_body-> get_subtype().c_str());
            message->init_with_body(custom_msg_body, to_username, kChatTypeSingle);
			message->set_msg_body_info(data);
		}
        else if (kMessageBodyTypeRetract == (EMessageBodyType)msg_content_type)
        {
            common::msg::RetractMessage retract_msg;
            message->set_status(kMessageStatusSendRetracted);            
            retract_msg.ParseFromString(data);
            message->set_msg_body_info(data);
			message->set_to(to_username);
            uint64_t guid = retract_msg.retract_guid();
                
            update_chat_message_status(guid, kMessageStatusSendRetracted);          
        }
        else if (kMessageBodyTypeReceipt == (EMessageBodyType)msg_content_type)
        {
            /*
                set_chat_message_read_status(from_user_name);
                from_user_name = to_username;
                message->set_msg_body_info(data);
            */

            continue;
        }
        else if (kMessageBodyTypeSingleReceipt == (EMessageBodyType)msg_content_type)
        {
             /*
                 uint64_t msg_guid;
                 common::msg::ReadReceiptMessage read_receipt_msg;
                 read_receipt_msg.ParseFromString(data);
                 msg_guid = read_receipt_msg.receipt_guid();
                 
                 set_chat_message_read_status_by_guid(msg_guid);      
                 from_user_name = to_username;
                 message->set_msg_body_info(data);
             */
             continue;
        }
        else
        {
        
        }

        if ((kMessageBodyTypeText == (EMessageBodyType)msg_content_type) || (kMessageBodyTypeFriend == (EMessageBodyType)msg_content_type)
             || (kMessageBodyTypeVoice == (EMessageBodyType)msg_content_type) || (kMessageBodyTypeLocation == (EMessageBodyType)msg_content_type)
             || (kMessageBodyTypeCustom == (EMessageBodyType)msg_content_type))
        {
        
			CCLog(	kLogLevelDebug, "(kMessageBodyTypeCustom == (EMessageBodyType)msg_content_type)");
			
            MessageWrapperPtr messageTemp = shared_ptr<MessageWrapper>(new MessageWrapper());
            messageTemp = get_chat_message_by_id(msg_id);
            if (messageTemp->get_msg_id() == msg_id)
            {
            	CCLog(	kLogLevelDebug, "(messageTemp->get_msg_id() == msg_id)");
                continue;
            }
        }
        
        /*填充其他字段*/
		message->set_from(from_user_name);
		message->set_nick_from(from_nick);
		message->set_to(to_username);
		message->set_conversation_name(from_user_name);
		message->set_msg_id(msg_id);
		message->set_is_sender(false);	
		message->set_msg_type((EMessageBodyType)msg_content_type);
		message->set_chat_type( kChatTypeSingle);
		message->set_burn_after_read(burn_after_read);
		message->set_guid(guid);
		message->set_status(kMessageStatusReceiveSuccess); 
		message->set_msg_sent_time(parse_msg_time_by_guid(guid));
		message->set_is_audio_read(false);
		message->set_is_msg_read(false);
        message->set_is_online(true);

        //我从非平台用户成为平台用户， from_username为平台id   to_username
        //收到的消息判断name、 from、to； name
        //userinfo
//        if(from_user_name==user.get_phone())
//        {
//            message->set_from(user.get_user_name());
//        }
//
//        if(to_username==user.get_phone())
//        {
//            message->set_to(user.get_user_name());
//        }

        if ((common::msg::RETRACT != msg_content_type) && (common::msg::CUSTOM != msg_content_type)
            && (common::msg::READ_RECEIPT != msg_content_type) && (common::msg::SINGLE_READ_RECEIPT != msg_content_type)) 
        {
            list<MessageWrapper> msg_list;
			
            /*
                保存数据到本地数据库
            */
            MessageWrapper chat_recv_msg(*(MessageWrapper*) message.get());
            
            msg_list.push_back(chat_recv_msg);
            
            /* 写入数据库*/
			CCLog(kLogLevelDebug, "receivemessage update sqlite");
            update_chat_message_list(msg_list);
        }
        
        /*
        	业务层的回调callback 
        */
        dump_message_body_info(message);
        if (message_observer_)
        {
			CCLog(kLogLevelInfo, "msg_id:%s username:%s",message->get_msg_id().c_str(),message->get_conversation_name().c_str());
        	message_observer_->OnMessageReceived(message);
        }
	}
	return ;
}

void ChatManagerImp::receive_message_1(const outer::connector::SessionNotify& notify,UserInfo user)
{
	MessageSyncRequest request_data;
    request_data.set_username(CCConfigManager::GetInstance().get_username());
	request_data.set_appkey(CCConfigManager::GetInstance().get_appkey());
	request_data.set_sor(false);//false接收消息
	request_data.set_type(outer::history::END_TYPE::WEB);
    request_data.set_modified_timestamp(rcv_last_timestamp);//要获取的最新消息的时间戳
    CCLog(kLogLevelInfo, "username:%s appkey:%s sor:%d timestamp:%lu type:%d ",request_data.username().c_str(),
          request_data.appkey().c_str(),request_data.sor(),request_data.modified_timestamp(),request_data.type());

	//发送获取消息请求
	string request_data_serial, response_data;
	request_data.SerializeToString(&request_data_serial);//消息序列化
	bool send_req;

	send_req = CCGrpcClient::GetInstance().send_unary_request("littlec-history", "messageSync", request_data_serial, response_data);
	if (!send_req)	
	{
        CCLog(kLogLevelError, "send_req:%d",send_req);
        return;	
	}
    
	MsgGetResponse response;
	response.ParseFromString(response_data);//解析服务器返回的消息
	int ret = response.ret();
	int16_t data_size =  response.data_size();

 	CCLog(kLogLevelDebug, "receive_data_size:%d",response.data_size());

	if(response.data_size()!=0)
	{
		rcv_last_timestamp = response.modified_timestamp();
	    CCLog(kLogLevelDebug, "rcv_last_timestamp_now=:%lu",rcv_last_timestamp);
	}

	for (int i = data_size - 1; i >= 0; i--)
	{
		string response_str_data = response.data(i).data();
		common::msg::EMsgType  msg_type = response.data(i).msg_type();
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

        usleep(1);
		
        if (msg_type == common::msg::TRANSPARENT_MSG)			//透传消息(指的是自定义消息)
		{
		    CCLog(kLogLevelDebug, "receive_common::msg::TRANSPARENT_MSG");
            continue ;
				
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
			CCLog(kLogLevelDebug, "receive_common::msg::CHAT_MSG");
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

		CCLog(kLogLevelDebug, "receivemessage_guid=:%d  i:%d",guid,i);

		//guid_last = guid;
		
		string file_name;
		string audio_link;
		uint64_t file_length;
		uint32_t duration;
		string audio_to_word;
        
		/*	先初始化 body，再初始化其他字段  */		
		MessageWrapperPtr message = shared_ptr<MessageWrapper>(new MessageWrapper());	
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
            CCLog(kLogLevelDebug, "from_user_name:%s", from_user_name.c_str());

            //delete_single_chat_message(from_user_name);  //成为好友后需要保留消息
		}
        else if (kMessageBodyTypeVoice == (EMessageBodyType)msg_content_type) 
        {
        	/*语音文件下载到本地*/
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
		
		else if (kMessageBodyTypeLocation == (EMessageBodyType)msg_content_type) 
        {
        	/*语音文件下载到本地*/
            common::msg::LocationMessage location_msg;
        	location_msg.ParseFromString(data);
        	string local_path =  CCConfigManager::GetInstance().get_download_path() + "/" + location_msg.file_name();
        	string url_path = CCConfigManager::GetInstance().get_file_address() + location_msg.origin_link();
        	Http_Get_File(url_path.c_str(), local_path.c_str());
			
			std::shared_ptr<LocationMessageBody>  location_msg_body = shared_ptr<LocationMessageBody> (new LocationMessageBody());
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
			CCLog(	kLogLevelDebug, "receive_custom_msg_body-> get_subtype()：%s",custom_msg_body-> get_subtype().c_str());
            message->init_with_body(custom_msg_body, to_username, kChatTypeSingle);
			message->set_msg_body_info(data);
		}

        else if (kMessageBodyTypeRetract == (EMessageBodyType)msg_content_type)
        {
            common::msg::RetractMessage retract_msg;
            message->set_status(kMessageStatusSendRetracted);            
            retract_msg.ParseFromString(data);
            message->set_msg_body_info(data);
			message->set_to(to_username);
            uint64_t guid = retract_msg.retract_guid();
                
            update_chat_message_status(guid, kMessageStatusSendRetracted);          
        }
        else if (kMessageBodyTypeReceipt == (EMessageBodyType)msg_content_type)
        {
            /*
                set_chat_message_read_status(from_user_name);
                from_user_name = to_username;
                message->set_msg_body_info(data);
            */

            continue;
        }
        else if (kMessageBodyTypeSingleReceipt == (EMessageBodyType)msg_content_type)
        {
             /*
                 uint64_t msg_guid;
                 common::msg::ReadReceiptMessage read_receipt_msg;
                 read_receipt_msg.ParseFromString(data);
                 msg_guid = read_receipt_msg.receipt_guid();
                 
                 set_chat_message_read_status_by_guid(msg_guid);      
                 from_user_name = to_username;
                 message->set_msg_body_info(data);
             */
             continue;
        }
        else
        {
        
        }

        if ((kMessageBodyTypeText == (EMessageBodyType)msg_content_type) || (kMessageBodyTypeFriend == (EMessageBodyType)msg_content_type)
             || (kMessageBodyTypeVoice == (EMessageBodyType)msg_content_type) || (kMessageBodyTypeLocation == (EMessageBodyType)msg_content_type)
             )
        {
        
			CCLog(	kLogLevelDebug, "msg_content_type：%d",msg_content_type);
            MessageWrapperPtr messageTemp = shared_ptr<MessageWrapper>(new MessageWrapper());
            messageTemp = get_chat_message_by_id(msg_id);
            if (messageTemp->get_msg_id() == msg_id)
            {
            	CCLog(	kLogLevelDebug, "(messageTemp->get_msg_id() == msg_id)");
                continue;
            }
        }
        
        /*填充其他字段*/

		message->set_from(from_user_name);
		CCLog(	kLogLevelDebug, "from_user_name=:%s",from_user_name.c_str());	 
		message->set_nick_from(from_nick);
		message->set_to(to_username);
		CCLog(	kLogLevelDebug, "to_username=:%s",to_username.c_str());	
		message->set_conversation_name(from_user_name);
		message->set_msg_id(msg_id);
		message->set_is_sender(false);	
		message->set_msg_type((EMessageBodyType)msg_content_type);
		message->set_chat_type( kChatTypeSingle);
		message->set_burn_after_read(burn_after_read);
		message->set_guid(guid);
		message->set_status(kMessageStatusReceiveSuccess); 
		message->set_msg_sent_time(parse_msg_time_by_guid(guid));
		message->set_is_audio_read(false);
		message->set_is_msg_read(false);
        message->set_is_online(true);

        if ((common::msg::RETRACT != msg_content_type) && (common::msg::CUSTOM != msg_content_type)
            && (common::msg::READ_RECEIPT != msg_content_type) && (common::msg::SINGLE_READ_RECEIPT != msg_content_type)) 
        {
            list<MessageWrapper> msg_list;
			
            /*
                保存数据到本地数据库
            */
            MessageWrapper chat_recv_msg(*(MessageWrapper*) message.get());
            
            msg_list.push_back(chat_recv_msg);
            
            /* 写入数据库*/
			CCLog(kLogLevelDebug, "receivemessage update sqlite");
            update_chat_message_list(msg_list);
        }
        
        /*
        	业务层的回调callback 
        */
        dump_message_body_info(message);
        if (message_observer_)
        {
            CCLog(	kLogLevelInfo, "msg_id:%s username:%s",message->get_msg_id().c_str(),message->get_conversation_name().c_str());
        	message_observer_->OnMessageReceived(message);
        }
	}
	return ;
}

bool ChatManagerImp::delete_all_messages()
{
	string path = CCConfigManager::GetInstance().get_download_path();

    int ret = CCConfigManager::GetInstance().clean_cache_size(path);
	if (ret != 0)
	{
	    return false;
	}
	
    ret = delete_all_chat_message();
	if(ret == 0){
		return true;
	}

	CCLog(kLogLevelWarn, "delete all messages error!!!");
	
	return false;
}

int ChatManagerImp::delete_single_message(string remoteUsername)
{
    CCLog(kLogLevelWarn,"remoteUsername:%s",remoteUsername.c_str());
	return delete_single_chat_message(remoteUsername);
}

list<MessageWrapper> ChatManagerImp::get_unread_msg(const int count, const char* conversation_id, const bool flag)
{
    list<MessageWrapper> wrappers;
    list<MessageWrapper>::iterator iter;
    int32_t get_count;

    CCLog(kLogLevelDebug, "get_unread_msg count: %d, conversation_name: %s flag: %d", count, conversation_id, flag);
	
    if (0 == count)
    {
        get_count = 200;
    }
    else
    {
        get_count = count;
    }
    
    if (flag)
    {
        wrappers = get_chat_message_local_unread_list((int)get_count, (char*)conversation_id); 
    }
    else
    {
        wrappers = get_chat_message_cloud_unread_list((int)get_count, (char*)conversation_id);
    }
    
    for(iter = wrappers.begin(); iter != wrappers.end(); ++iter) {
        if(kMessageBodyTypeText == iter->get_msg_type()) {
            std::shared_ptr<TextMessageBody> text_msg_body = shared_ptr<TextMessageBody>(new TextMessageBody());
            CCMessageEncoder::deserialize_text_msgbody(iter->get_msg_body_info(), *text_msg_body);
            iter->set_message_body(text_msg_body);           
        }else if(kMessageBodyTypeFriend == iter->get_msg_type()) {
            std::shared_ptr<FriendMessageBody> text_msg_body = shared_ptr<FriendMessageBody>(new FriendMessageBody());
            CCMessageEncoder::deserialize_friend_msgbody(iter->get_msg_body_info(), *text_msg_body);
            iter->set_message_body(text_msg_body);           
        }else if(kMessageBodyTypeVoice == iter->get_msg_type()) {
            std::shared_ptr<AudioMessageBody> audio_message_body = shared_ptr<AudioMessageBody>(new AudioMessageBody());
            CCMessageEncoder::deserialize_audio_msgbody(iter->get_msg_body_info(), *audio_message_body);
            iter->set_message_body(audio_message_body);
			iter->set_duration(audio_message_body ->get_duration());
        } else if (kMessageBodyTypeLocation == iter->get_msg_type()){
            std::shared_ptr<LocationMessageBody> location_message_body = shared_ptr<LocationMessageBody>(new LocationMessageBody());
            CCMessageEncoder::deserialize_location_msgbody(iter->get_msg_body_info(), *location_message_body);
            iter->set_message_body(location_message_body);			
		}else if(kMessageBodyTypeCustom == iter->get_msg_type()) { 
            std::shared_ptr<CustomMessageBody> custom_msg_body = shared_ptr<CustomMessageBody>(new CustomMessageBody());
            CCMessageEncoder::deserialize_custom_msgbody(iter->get_msg_body_info(), *custom_msg_body);
            iter->set_message_body(custom_msg_body);
        } else{
            CCLog(kLogLevelWarn, "msg type is unknown!!!");
        }
    }
     CCLog(kLogLevelDebug, "get_unread_message_success");
    return wrappers;
}

list<MessageWrapper> ChatManagerImp::get_next_page_msg(int count, int page, int* total, char* conversation_id)
{
	CCLog(kLogLevelDebug, "conversation_id=:%s",conversation_id);

	list<MessageWrapper> wrappers = get_chat_message_list(count, page, total, conversation_id);
    list<MessageWrapper>::iterator iter;
    for(iter = wrappers.begin(); iter != wrappers.end(); ++iter) {
        if(kMessageBodyTypeText == iter->get_msg_type()) {
            std::shared_ptr<TextMessageBody> text_msg_body = shared_ptr<TextMessageBody>(new TextMessageBody());
			CCMessageEncoder::deserialize_text_msgbody(iter->get_msg_body_info(), *text_msg_body);
            iter->set_message_body(text_msg_body);           
        }else if(kMessageBodyTypeFriend == iter->get_msg_type()) {
            std::shared_ptr<FriendMessageBody> text_msg_body = shared_ptr<FriendMessageBody>(new FriendMessageBody());
			CCMessageEncoder::deserialize_friend_msgbody(iter->get_msg_body_info(), *text_msg_body);
            iter->set_message_body(text_msg_body);
        }else if(kMessageBodyTypeVoice == iter->get_msg_type()) {
            std::shared_ptr<AudioMessageBody> audio_message_body = shared_ptr<AudioMessageBody>(new AudioMessageBody());
			CCMessageEncoder::deserialize_audio_msgbody(iter->get_msg_body_info(), *audio_message_body);
            iter->set_message_body(audio_message_body);
			iter->set_duration(audio_message_body ->get_duration());
        }else if (kMessageBodyTypeLocation == iter->get_msg_type()){
            std::shared_ptr<LocationMessageBody> location_message_body = shared_ptr<LocationMessageBody>(new LocationMessageBody());
            CCMessageEncoder::deserialize_location_msgbody(iter->get_msg_body_info(), *location_message_body);
            iter->set_message_body(location_message_body);			
		} else if(kMessageBodyTypeCustom == iter->get_msg_type()) {	
            std::shared_ptr<CustomMessageBody> custom_msg_body = shared_ptr<CustomMessageBody>(new CustomMessageBody());
            CCMessageEncoder::deserialize_custom_msgbody(iter->get_msg_body_info(), *custom_msg_body);
			iter->set_message_body(custom_msg_body);
		} else{
        	CCLog(kLogLevelWarn, "msg type is unknown!!!");
        }
    }
	
	return wrappers;
}

std::shared_ptr<MessageWrapper> ChatManagerImp::get_msg_by_id(string msg_id)
{ 
    MessageWrapperPtr wrappers = get_chat_message_by_id(msg_id);

    if(kMessageBodyTypeText == wrappers->get_msg_type()) {
        std::shared_ptr<TextMessageBody> text_msg_body = shared_ptr<TextMessageBody>(new TextMessageBody());
		CCMessageEncoder::deserialize_text_msgbody(wrappers->get_msg_body_info(), *text_msg_body);
        wrappers->set_message_body(text_msg_body);           
    }else if(kMessageBodyTypeFriend == wrappers->get_msg_type()) {
        std::shared_ptr<FriendMessageBody> text_msg_body = shared_ptr<FriendMessageBody>(new FriendMessageBody());
		CCMessageEncoder::deserialize_friend_msgbody(wrappers->get_msg_body_info(), *text_msg_body);
        wrappers->set_message_body(text_msg_body);           
    }else if(kMessageBodyTypeVoice == wrappers->get_msg_type()) {
        std::shared_ptr<AudioMessageBody> audio_message_body = shared_ptr<AudioMessageBody>(new AudioMessageBody());
		CCMessageEncoder::deserialize_audio_msgbody(wrappers->get_msg_body_info(), *audio_message_body);
        wrappers->set_message_body(audio_message_body);
		wrappers->set_duration(audio_message_body ->get_duration());
    }else if (kMessageBodyTypeLocation == wrappers->get_msg_type()){
        std::shared_ptr<LocationMessageBody> location_message_body = shared_ptr<LocationMessageBody>(new LocationMessageBody());
        CCMessageEncoder::deserialize_location_msgbody(wrappers->get_msg_body_info(), *location_message_body);
        wrappers->set_message_body(location_message_body);			
	} else if(kMessageBodyTypeCustom == wrappers->get_msg_type()) {	
        std::shared_ptr<CustomMessageBody> custom_msg_body = shared_ptr<CustomMessageBody>(new CustomMessageBody());
        CCMessageEncoder::deserialize_custom_msgbody(wrappers->get_msg_body_info(), *custom_msg_body);
		wrappers->set_message_body(custom_msg_body);
	} else{
    	CCLog(kLogLevelWarn, "msg type is unknown!!!");
    }
	
    return wrappers;
}


uint64_t ChatManagerImp::get_cache_size() 
{
	uint64_t cache_size = 10240;
	return cache_size;
}

void ChatManagerImp::clean_cache_size( char* path,error_info *error) 
{
	error->error_code = kSuccess;
	error->error_description = (char*)"OK";

}

bool ChatManagerImp::set_voice_played(char* conversationid, char* msgid) 
{
	int32_t ret;

	ret = set_chat_voice_message_status(msgid);
	if (ret != 0)
	{
		return false;
	}

	return true;
}

int ChatManagerImp::pcm2amr(const char* file_name,  readcbfunctype read_cb, errorcbfunctype error_cb)
{
    int ret;
    error_info error;
    
    if (NULL == file_name)
    {
        error.error_code = kRequiredParmIsNil;
        error_cb(error);
        return -1;
    }
    
    ret = sdkpcm2amr(file_name, read_cb);    
    if (ret != 0)
    {
        error.error_code = kAccessResourceFailed;
        error_cb(error);
        return -1;
    }
    
    return 0;
}

int ChatManagerImp::amr2pcm(const char* file_name, writecbfunctype write_cb, errorcbfunctype error_cb)
{
    int ret;
    error_info error;
    
    if (NULL == file_name)
    {
        error.error_code = kRequiredParmIsNil;
        error_cb(error);    
        return -1;
    }

    ret = sdkamr2pcm(file_name, write_cb);
    
    if (ret != 0)
    {
        error.error_code = kAccessResourceFailed;
        error_cb(error);        
        return -1;
    }
    
    return 0;

}

EErrorCode ChatManagerImp::pcmResampler(const int16_t* in_buf, const int16_t* out_buf, int in_pcm_len, int * out_pcm_len, int src_sample_rate, int out_sample_rate)
{
    int srcSamples = 0;
    int outSamples = 0;    
    int i,j;
    float xratio = 0;
    int outLen = 0;
    int16_t * pSrc = (int16_t*)in_buf;
    int16_t * pDst = (int16_t*)out_buf;

    if ((NULL == in_buf) || (NULL == out_buf))
    {
        return kRequiredParmIsNil;
    }
    
    srcSamples = src_sample_rate/50;//¼ÆËã20msµÄ²ÉÑùµã¸öÊý
    outSamples = out_sample_rate/50;//¼ÆËã20msµÄ²ÉÑùµã¸öÊý    

    xratio = (float) srcSamples/outSamples;
    outLen = ((in_pcm_len * outSamples) / srcSamples) / 2;
    
    for (j = 0; j < outLen; j++)
    {
        float dist, relative_pos;
 
        relative_pos = j * xratio - 0.5f;
        i = (int)(relative_pos);
        dist = relative_pos - i;

        pDst[j] = (1 - dist) * pSrc[i] + dist * pSrc[i+1]; 

    }

    *out_pcm_len = outLen * 2;
    
    return kSuccess;
}


bool ChatManagerImp::is_send_msg(const string& to_username)
{
    if (get_request_count(to_username))
    {
		return true;
    }
	return false;
}

bool ChatManagerImp::update_conversation_msg(const string& username, const string& phone)
{
    if(update_chat_message_remoteusername(username,phone))
    {
        return false;
    }
    return true;
}

bool ChatManagerImp::send_read_receipt(char * to_user,char * msg_id, char * conversationid, bool single)
{
     CCLog(kLogLevelDebug, "send_read_receipt_restar");
	 if (single)
    {
    	set_chat_message_read_status_by_id(msg_id);//单聊消息本地设为已读
    }
    else
    {
        MessageWrapperPtr messageTemp = shared_ptr<MessageWrapper>(new MessageWrapper());
        messageTemp = get_chat_message_by_id(msg_id);
        if (messageTemp->get_msg_id() == msg_id)
        {
            set_chat_message_read_status_by_time(to_user, messageTemp->get_msg_sent_time());        
        }
    }

	string method_name = "sendChat";
	ChatMessageRequest chat_message_req;
    
	common::msg::ReadReceiptMessage read_receipt_msg;

	chat_message_req.set_from_username(CCConfigManager::GetInstance().get_username());//发送方username
	chat_message_req.set_appkey(CCConfigManager::GetInstance().get_appkey());//appkey
	chat_message_req.set_to_username(to_user);//接收方username
	if (single)
	{
	    chat_message_req.set_msg_content_type(common::msg::SINGLE_READ_RECEIPT);//单条消息已读
	}
	else
	{
	    chat_message_req.set_msg_content_type(common::msg::READ_RECEIPT);
	}
	
	chat_message_req.set_msg_id(msg_id);//消息id
	chat_message_req.set_burn_after_read(false);//不设置阅后即焚
	chat_message_req.set_from_client_type(common::enumeration::ZEBRA);//发送者终端类型
	chat_message_req.set_is_jimao(false);//不是鸡毛信
	MessageWrapper msgWrapper = get_chat_message_wrapper(msg_id);
	read_receipt_msg.set_receipt_guid(msgWrapper.get_guid());
	
	string data;
    read_receipt_msg.SerializeToString(&data);
	chat_message_req.set_data(data);

	string request_data, response;
	bool send_req;

	CCLog(kLogLevelInfo, "msg_id:%s", msg_id);

    chat_message_req.SerializeToString(&request_data);

	send_req = CCGrpcClient::GetInstance().send_unary_request(service_name_, method_name, request_data, response);
    if (!send_req)	
    {
		CCLog(kLogLevelWarn, "send_req:%d", send_req);
        return kConnectServerFail;	
    }

	ChatMessageResponse response_data;
	response_data.ParseFromString(response);
	int ret = response_data.ret();


	if(ret == kSuccess) {
		return true;
	}
	CCLog(kLogLevelWarn, "seng_read_receipt_end");
	return false;
}

int ChatManagerImp::progress_callback(void *msg_observer, double dltotal, double dlnow, double ultotal, double ulnow){

	MessageObserverBase * msg_ob = (MessageObserverBase*)msg_observer;
	if (ultotal > 0)
		if (msg_ob)
			msg_ob->OnUploadProcessing(ulnow,ultotal);
	
	return 0;
}

void ChatManagerImp::set_message_observer(MessageObserverBase* msg_observer){

	message_observer_ = msg_observer;
}

void ChatManagerImp::set_send_msg_guid(uint64_t guid){

	send_msg_guid_ = guid;
}
uint64_t ChatManagerImp::get_send_msg_guid(){

	return send_msg_guid_;
}
void ChatManagerImp::set_rec_msg_guid(uint64_t guid){

	rec_msg_guid_ = guid;
}
uint64_t ChatManagerImp::get_rec_msg_guid(){

	return rec_msg_guid_;
}

}

