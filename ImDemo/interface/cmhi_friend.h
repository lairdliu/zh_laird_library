/***************************************************************************
  * file name cmhi_friend.h
  * brief 好友信息头文件
  * date 2019/02/02
  *************************************************************************/

#ifndef CMCC_IM_FRIEND_H_
#define CMCC_IM_FRIEND_H_

#include <string>
#include "common_types.h"
#include "cmhi_message_body.h"
using std::string;

namespace cmhi_iov{


/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

class FriendInfo{

	public:
		FriendInfo(){}
		~FriendInfo(){}
		string get_user_name(){ return user_name; }
		void set_user_name(string user_name);
		string get_nick_name(){ return nick_name; }
		void set_nick_name(string nick_name);
		string get_remark_name(){ return remark_name; }
		void set_remark_name(string remark_name);
		string get_phone(){ return phone; }
		void set_phone(string phone);
		int get_is_enable(){return is_enable; }
		void set_is_enable( int is_enable);
		bool get_is_registered(){return is_registered; }
		void set_is_registered( bool is_registered);
		int get_silent(){return silent; }
		void set_silent( int silent);
		string get_avatar_original_link(){ return avatar_original_link; }
		void set_avatar_original_link(string avatar_original_link);
		string get_avatar_thumbnail_link(){ return avatar_thumbnail_link; }
		void set_avatar_thumbnail_link(string avatar_thumbnail_link);
		bool get_is_online(){return is_online; }
		void set_is_online( bool is_online);
		string get_nick_name_pinyin(){return nick_name_pinyin;}
		string get_nick_name_pinyin_blank(){return nick_name_pinyin_blank;}
		void set_nick_name_pinyin(string nick_name_pinyin_);
		void set_nick_name_pinyin_blank(string nick_name_pinyin_blank_);
		string get_remark_name_pinyin(){return remark_name_pinyin;}
		string get_remark_name_pinyin_blank(){return remark_name_pinyin_blank;}
		void set_remark_name_pinyin(string remark_name_pinyin_);
		void set_remark_name_pinyin_blank(string remark_name_pinyin_blank_);

		int get_member_type(){return member_type; }
		void set_member_type( int member_type_);

		string get_series_name(){ return series_name; }
		void set_series_name(string series_name_);

        int get_user_enable(){return user_enable; }
        void set_user_enable( int user_enable_);

		std::shared_ptr<MessageWrapper> get_friend_message(){return message;}
		void set_friend_message(std::shared_ptr<MessageWrapper> message);

	private:
		string user_name;				//好友用户名
		string nick_name;				//好友昵称
		string remark_name;             //好友备注名
		string phone;					//好友电话
		bool is_registered = true;      //用户是否注册
		int  is_enable;			//0：异常状态 1：两端互为好友 2：两方都不是好友 3：本端主动解除好友 4：对端解除好友
		int silent = false;				//是否静默，0无效状态, 1为静默状态，2为非静默状态 
		string avatar_original_link;	//头像原图链接
		string avatar_thumbnail_link;	//缩略图链接
		bool is_online = true;           // 是否云端好友，1:云端好友；0:本地通讯录好友
		string nick_name_pinyin;         //好友昵称拼音
		string nick_name_pinyin_blank;         //好友昵称拼音
		string remark_name_pinyin;       //好友备注名拼音
		string remark_name_pinyin_blank;    //好友备注名拼音（增加）
		int member_type;						//好友类型（新加）
		string 	series_name;					//系列名称(新加)
		int user_enable = false;                        //好友是否注销（0==已注销）
		std::shared_ptr<MessageWrapper> message;        //携带好友最后一条消息
};

class CReadFriendResponse
{
	public:
		CReadFriendResponse(){}
		~CReadFriendResponse(){}
		EErrorCode get_errorcode(){return err;}
		string get_msg_id(){return msg_id;}
		void set_errorcode(EErrorCode err);
		void set_msg_id(string msg_id);
	private:
		EErrorCode err;
		string msg_id;
};
class RequestWrapper: public FriendInfo{

	public:
		RequestWrapper(){}
		~RequestWrapper(){}
		string get_request_id(){ return request_id; }
		string get_remark(){ return remark; }
		int get_req_status(){return req_status; }
		void set_request_id(string id);
		void set_remark(string remark);
		void set_req_status(int status);
	
	
		uint64_t get_request_time(){return request_time;}
		void set_request_time(uint64_t time);
	private:
		string request_id;		//请求ID
		string remark;			//请求原因
		int req_status;		//请求状态
		uint64_t request_time;
};

class FriendNotification{

	public:
		FriendNotification(){}
		~FriendNotification(){}

		ENotificationType get_type(){ return type; }
		string get_request_id(){ return request_id; }
		string get_reason(){ return reason; }
		void set_type(ENotificationType type);
		void set_request_id(string request_id);
		void set_reason(string reason);
		void set_user_info(FriendInfo user_info);
		FriendInfo get_user_info();
		
	private:
		ENotificationType type;
		FriendInfo user_info;
		string request_id;
		string reason;

};


}

#endif //CMCC_IM_FRIEND_H_

