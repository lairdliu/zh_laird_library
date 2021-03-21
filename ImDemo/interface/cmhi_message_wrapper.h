/***************************************************************************
  * file name cmhi_message_wrapper.h
  * brief 消息数据类，构造发送消息接口
  * date 2019/02/13
  *************************************************************************/

#ifndef CMCC_IM_MESSAGE_WRAPPER_H_
#define CMCC_IM_MESSAGE_WRAPPER_H_


#include <time.h>
#include <stdio.h>
#include <memory>
#include <functional>

#include "cmhi_text_message_body.h"
#include <uuid/uuid.h>

inline string generate_msg_uuid()
{
	uuid_t uu;
	uuid_generate(uu);
	static char buf[64] = {0};

    uuid_unparse(uu, buf);
    
	return string(buf);
}

namespace cmhi_iov{

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/
	typedef std::function<void (const int16_t * buf, int length, bool* is_end, bool* cancel)> readcbfunctype;	 
	typedef std::function<void (const int16_t * buf, int length, bool is_end, bool* cancel)> writecbfunctype;	
	typedef std::function<void (error_info err)> errorcbfunctype;

class MessageWrapper{
	
		public:
			
			MessageWrapper();
			~MessageWrapper(){}

			void init_with_body(MessageBodyPtr body, string to, EChatType chat_type);
			string get_msg_id();
			string get_from();
			string get_nick_from();
			string get_to();
			string get_nick_to();
			string get_conversation_id();
			string get_conversation_name();
			string get_local_path();
			EChatType get_chat_type();
			EMessageBodyType get_msg_type();
			MessageBodyPtr get_message_body();
			EMessageStatus get_status();
			bool get_burn_after_read();
			bool is_sender();
			bool is_msg_read();
			bool is_audio_read();
			int get_duration();
			uint64_t get_guid();
			uint64_t get_msg_sent_time();
			string get_msg_body_info();
			/*写入数据库的数据，需要base64编码*/
			string get_msg_body_info_base64();
			string get_content();
			bool is_online();
			void set_msg_id(const string msg_id);
			void set_from(const string from_user);
			void set_nick_from(const string nick_from);
			void set_to(const string to);
			void set_nick_to(const string nick_to);
			void set_conversation_id(const string conversation_id);
			void set_conversation_name(const string conversation_name);
			void set_local_path(const string local_path);
			void set_chat_type(const EChatType chat_type);
			void set_msg_type(const EMessageBodyType msg_type);
			void set_message_body(MessageBodyPtr msg_body);
			void set_status(const EMessageStatus status);
			void set_burn_after_read(const bool burn_after_read);
			void set_is_sender(const bool is_sender);
			void set_is_msg_read(const bool is_msg_read);
			void set_is_audio_read(const bool is_audio_read);
			void set_duration(const int duration);
			void set_guid(const uint64_t guid);
			void set_msg_sent_time(const uint64_t time);
			void set_msg_body_info(const string msg_body_info);
			/*读取数据库的数据，需要base64解码*/
			void set_msg_body_info_base64(const string msg_body_info);
			void set_content(const string content);
			void set_is_online(const bool is_online);
			
		private:
			
			string msg_id_;									//消息的唯一标识符					
			string from_;									//消息的发送方用户名/消息网关发送者
			string nick_from_;								//消息的发送方昵称
			string to_;										//消息的接收方(用户名/群的groupId）
			string nick_to_;                                //消息接收方的昵称
			string conversation_id_;					    //会话的唯一标识符,单聊时，等同to username
			string conversation_name_;						//会话名称，单聊时等同to nickname
			string local_path_;                             //文件本地路径
			EChatType chat_type_;								//聊天类型
			EMessageBodyType msg_type_;                     //消息类型
			MessageBodyPtr message_body_;						//消息体
			EMessageStatus status_;							//消息状态
			bool burn_after_read_;							//是否阅后即焚
			bool is_sender_;									//是否自己发的消息
			bool is_msg_read_;                                  //消息是否已读
			bool is_audio_read_;                              //语音消息是否已播放
			int audio_duration_;								//语音消息长度
			bool is_online_;                                   //该消息是否在线消息, true: 在线消息    false: 离线消息
			uint64_t guid_;                                   //消息唯一标识
			uint64_t msg_sent_time_;                        //消息发送时间
			string msg_body_info_;                           //序列化后的消息体
			string content_;							    //在会话列表显示的消息文本， 文本消息显示text， 图片消息显示 ‘图片’， 语音消息显示 ‘语音’…………

};

typedef std::shared_ptr<MessageWrapper> MessageWrapperPtr;


}
#endif //CMCC_IM_MESSAGE_WRAPPER_H_

