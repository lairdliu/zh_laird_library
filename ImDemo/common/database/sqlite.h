/***************************************************************************
  * file sqlite.h
  * brief sqlite数据库操作
  * date 2019/02/18
  *************************************************************************/

#ifndef CMCC_IM_SQLITE_H_
#define CMCC_IM_SQLITE_H_

#include "cpp_sqlite3.h"
#include "cmhi_chat_imp.h"
#include "cmhi_friend.h"
#include "cmhi_im_config.h"
#include "cmhi_message_wrapper.h"
#include "logger.h"
#include <list>


/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/
namespace cmhi_iov
{

	string get_database_path();

	void create_table_friend();
	void update_table_friend();

	void create_table_request();

	void create_table_chat();
	
    int update_friends_list(std::list<FriendInfo> friends_list);
	void update_table(string tableName,string colum_name, string type,string op_string="");

	void update_friend(FriendInfo friend_info);

	std::list<FriendInfo> get_friends_list(int page, int count, int* total);

	int add_friend(FriendInfo friend_info);

	int delete_friend(char* friend_name);

	FriendInfo get_friend_info(char* friend_name);
	
	int update_friend_silent(char* username, int silent_type);
	
	int update_friend_request_read(char* requestId, bool is_read);

	int update_friend_enable(char* username,       int is_enable);

	int update_friend_request_list(std::list<RequestWrapper> request_list);

	void update_friend_request(RequestWrapper request_wrapper);

	int set_same_friend_request_status(string requestId, int status);

	std::list<RequestWrapper> get_add_friend_request_list ();

	int delete_friend_request(char* request_id);

	int update_friend_request_status(char* request_id, int status);
	
	std::list<MessageWrapper> get_chat_message_cloud_unread_list(int count, char* remote_user_name);

	std::list<MessageWrapper> get_chat_message_local_unread_list(int count, char* remote_user_name);

	std::list<MessageWrapper> get_chat_message_list(int count, int page, int* total, char* remote_user_name);

	int update_chat_message_list(std::list<MessageWrapper> message_list);

	MessageWrapper get_chat_message_wrapper(char* msg_id);

	int set_chat_voice_message_status(string msg_id);

	int set_chat_message_read_status(string username);

	int set_chat_message_read_status_by_time(string username, uint64_t time) ;

	int set_chat_message_read_status_by_id(string msg_id);

	int delete_all_chat_message();
	
	int delete_single_chat_message(string remoteUsername);

	int update_chat_message_guid(string msg_id, string guid);

	int update_chat_message_status(uint64_t guid, int status);
	
	int set_chat_message_read_status_by_guid(uint64_t guid);

	std::shared_ptr<MessageWrapper> get_chat_message_by_id(string msg_id);
	
	std::shared_ptr<MessageWrapper> get_chat_message_by_guid(uint64_t guid);
	
	int delete_chat_message_by_date();

	int update_chat_message_status_by_id(string msg_id, int status);

	int delete_chat_message_by_id(string msg_id);

	int get_friend_relation(const string &to_name);
	
	int  get_last_msg_by_username(char* remoteUsername,MessageWrapper& message_wrapper);

	int update_friend_Link(char* username, string thumblink, string orignallink);

	int update_friend_nick(char* username, string nickname, string nickname_pinyin);

	int get_phone(const string& username,string& phone);
	int update_chat_message_remoteusername(const string& username, const string& phone);

	int get_request_count(const string& remote_user_name);
}

#endif  //CMCC_IM_SQLITE_H_


