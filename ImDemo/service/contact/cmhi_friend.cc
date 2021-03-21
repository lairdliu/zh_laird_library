/***************************************************************************
  * file name cmhi_friend.cpp
  * brief 好友信息实现文件
  * date 2019/02/02
  *************************************************************************/

#include "cmhi_chat_imp.h"
#include "cmhi_friend.h"

namespace cmhi_iov{

void FriendInfo::set_user_name(string name)
{
	user_name = name;
}

void FriendInfo::set_nick_name(string name)
{
	nick_name= name;
}

void FriendInfo::set_remark_name(string name)
{
	remark_name = name;
}

void FriendInfo::set_phone(string phone)
{
	this->phone = phone;
}

void FriendInfo::set_is_enable( int is_enable)
{
	this->is_enable = is_enable;
}

void FriendInfo::set_is_registered( bool is_registered)
{
	this->is_registered = is_registered;
}

void FriendInfo::set_silent( int silent)
{
	this->silent = silent;
}

void FriendInfo::set_avatar_original_link(string link)
{
	this->avatar_original_link= link;
}

void FriendInfo::set_avatar_thumbnail_link(string link)
{
	this->avatar_thumbnail_link= link;
}
void FriendInfo:: set_nick_name_pinyin(string nick_name_pinyin_)
{
	this->nick_name_pinyin = nick_name_pinyin_;
}

void FriendInfo::set_nick_name_pinyin_blank(string nick_name_pinyin_blank_)
{
	this->nick_name_pinyin_blank = nick_name_pinyin_blank_;
}

void FriendInfo::set_remark_name_pinyin(string remark_name_pinyin_)
{
	this->remark_name_pinyin = remark_name_pinyin_;
}
void FriendInfo::set_remark_name_pinyin_blank(string remark_name_pinyin_blank_)
{
	this->remark_name_pinyin_blank = remark_name_pinyin_blank_;
}

void FriendInfo::set_member_type( int member_type_)
{
	this->member_type = member_type_;
}

void FriendInfo::set_series_name(string series_name_)
{
	this->series_name = series_name_;
}

void FriendInfo::set_user_enable( int user_enable_)
{
	this->user_enable = user_enable_;
}

void FriendInfo::set_friend_message(std::shared_ptr<MessageWrapper> message)
{
        this->message = message;
}

void RequestWrapper::set_request_id(string id)
{	
	this->request_id= id;
}

void RequestWrapper::set_remark(string remark)
{
	this->remark = remark;
}

void RequestWrapper::set_req_status(int status)
{
	this->req_status = status; 
}


void RequestWrapper::set_request_time(uint64_t time)
{
	this->request_time = time;
}

void FriendInfo::set_is_online(bool is_online)
{
	this->is_online = is_online;
}

void FriendNotification::set_type(ENotificationType type)
{
	this->type = type;
}

void FriendNotification::set_request_id(string request_id)
{
	this->request_id = request_id;
}

void FriendNotification::set_reason(string reason)
{
	this->reason = reason;
}

void FriendNotification::set_user_info(FriendInfo user_info)
{
	this->user_info = user_info;
}

void CReadFriendResponse::set_errorcode(EErrorCode err)
{
	this->err = err;
}
void CReadFriendResponse::set_msg_id(string msg_id)
{
	this->msg_id = msg_id;
}

}

