
#include <string>
#include <iostream>

#include "cmhi_text_message_body.h"
#include "cmhi_file_message_body.h"
#include "cmhi_audio_message_body.h"
#include "cmhi_im_client.h"
#include "cmhi_contact_imp.h"
#include "cmhi_chat_imp.h"
#include "cmhi_user_info.h"
#include "sqlite.h"
#include <unistd.h>
#include "test_message_observer.h"
#include "test_connect_observer.h"
#include "test_contact_observer.h"
#include "http_curl.h"


using namespace std;
using namespace cmhi_iov;

void sb_logout_error_cb(EErrorCode error){
    
    std::cout << "return logout error code:" << error << std::endl;
}

shared_ptr<CCClientManagerBase> client = CCClientManagerBase::create_client_manager();

shared_ptr<ContactManagerImp> contact = ContactManagerImp::getContactManager();

shared_ptr<ChatManagerImp> chat = ChatManagerImp::getChatManager();

void testgetvoipconfig(){
	string confadd ;
	string voipadd ;
	//CCConfigManager::GetInstance().set_conference_address("12.11.12.13:8080");
	client->get_voip_config(confadd,voipadd);;
	std::cout << "test get voip config" << confadd << std::endl;
	std::cout << "test get_conference_address" << CCConfigManager::GetInstance().get_conference_address() << std::endl;
	std::cout << "test get_voip_address" << CCConfigManager::GetInstance().get_voip_address() << std::endl;
}


void testgetuserinfo(){
	UserInfo  user;
    EErrorCode err = client->get_user_info(user);	
    std::cout << "test get user info return error code:" << err << std::endl;
	std::cout << "user name:" << user.get_user_name().c_str() << std::endl
		<< "nick_name:" << user.get_nick_name().c_str() << std::endl
		<< "phone:" << user.get_phone().c_str() << std::endl
		<< "original_link:" << user.get_avatar_original_link().c_str() << std::endl 
		<< "thumbnail_link:" << user.get_avatar_thumbnail_link().c_str() << std::endl;
	
}


// contact test
void testgetaddfriendrequest(){
	list<RequestWrapper> request_list = client->get_add_friend_request();
	
	cout << "get_add_friend_request start-------------" << endl;

    list<RequestWrapper>::iterator iter;
    for (iter = request_list.begin(); iter != request_list.end(); ++iter)
    {
        cout <<"get_add_friend_request info request_id: " << iter->get_request_id()<< endl;
		cout <<"get_add_friend_request info remark: " << iter->get_remark()<< endl;
		cout <<"get_add_friend_request info req_status: " << iter->get_req_status()<< endl;
    }
    cout << "get_add_friend_request end-------------" << endl;
}

void testgetfriendslist(){
    int page = 1;
    int count = 5;
    int total = 20;

    error_info error;

    list<FriendInfo> friend_list = client->get_friends_list(&error, page, count, &total);
	cout << "get_friends_list start-------------" << endl;

    list<FriendInfo>::iterator iter;
    for (iter = friend_list.begin(); iter != friend_list.end(); ++iter)
    {
        cout <<"get_friends_list info request_id: " << iter->get_user_name()<< endl;
		cout <<"get_friends_list info remark: " << iter->get_remark_name()<< endl;
		cout <<"get_friends_list info phone: " << iter->get_phone()<< endl;
    }
    cout << "get_friends_list end-------------" << endl;
}


void testgetfrienduseinfo() {
    error_info error;
    FriendInfo friend_info = client->get_friend_user_info("sdsds", &error);
    cout << "testgetfriendinfo friend_info username: " << friend_info.get_user_name()<< endl;
}

void testaddfriend(){

    error_info error;

    client->add_friend("18867110002","tongxue", &error);
}

void testagreefriendrequest(){
	error_info error;
	char *requestid = "347654506072244224";
	contact->agree_friend_request(requestid, &error) ;
}

void testdeclinefriendrequest(){
	
	error_info err;
	contact->decline_friend_request("", "bad gay", &err);
}

void testremovefriend(){
	error_info err;
	contact->remove_friend("18867111354", &err);
}


void testsyncfriend(){
	
	contact->sync_friend();
}

void testsyncfriendchange(){
	contact->sync_friend_change("18867110000", "201903cx");
}

int testsqliteupdatefriendlist(){
	list<FriendInfo> friend_list;
	FriendInfo friend_inf1;
	friend_inf1.set_user_name("jim1");
	friend_inf1.set_phone("18867111354");
	friend_inf1.set_nick_name("jim1");
	friend_inf1.set_remark_name("remake_name");
	friend_inf1.set_avatar_original_link("orginal_link");
	friend_inf1.set_avatar_thumbnail_link("thumbnail_link");

	FriendInfo friend_inf2;
	friend_inf2.set_user_name("jim2");
	friend_inf2.set_phone("18867111355");
	friend_inf2.set_nick_name("jim2");
	friend_inf2.set_remark_name("remake_name");
	friend_inf2.set_avatar_original_link("orginal_link");
	friend_inf2.set_avatar_thumbnail_link("thumbnail_link");

	friend_list.push_back(friend_inf1);
	friend_list.push_back(friend_inf2);
	int i = update_friends_list(friend_list);
	return i;
}

void testsqliteupdatefriend(){
	FriendInfo friend_inf1;
	friend_inf1.set_user_name("jim3");
	friend_inf1.set_phone("18867111356");
	friend_inf1.set_nick_name("jim3");
	friend_inf1.set_remark_name("remake_name");
	friend_inf1.set_avatar_original_link("orginal_link");
	friend_inf1.set_avatar_thumbnail_link("thumbnail_link");
	update_friend(friend_inf1);
}

int testsqliteaddfriend(){
	FriendInfo friend_inf1;
	friend_inf1.set_user_name("jim4");
	friend_inf1.set_phone("18867111357");
	friend_inf1.set_nick_name("jim4");
	friend_inf1.set_remark_name("remake_name");
	friend_inf1.set_avatar_original_link("orginal_link");
	friend_inf1.set_avatar_thumbnail_link("thumbnail_link");
	int i = add_friend(friend_inf1);
	return i;
}

int testsqlitedeletefriend(){
	char *deleteusername = "jim4";
	int i = delete_friend(deleteusername);
	return i;
}

void testsqlitegetfriendslist(){
	int page = 1;
	int count = 2;
	int total = 3;
	//list<FriendInfo> friendinfo_list = get_friends_list(page,count,total);
}

void testsendmessage(){
	MessageWrapperPtr audio_msg = shared_ptr<MessageWrapper>(new MessageWrapper);
    MessageWrapperPtr text_msg = shared_ptr<MessageWrapper>(new MessageWrapper);;
	TextMessageBody text_message_body("hahahaha");
	AudioMessageBody audio_message_body("test.amr","/root/test.amr", 14);
	
	MessageBody *audio_msg_body = &audio_message_body;
	audio_msg->init_with_body(audio_msg_body,"18867110004",kChatTypeSingle);
	
	audio_msg->set_from("18867110003");
	audio_msg->set_burn_after_read(false);

	audio_msg->set_conversation_id("18867110004");
	audio_msg->set_conversation_name("hhh");
	audio_msg->set_is_sender(true);
	audio_msg->set_nick_from("660003");

	MessageBody *text_msg_body = &text_message_body;
	text_msg->init_with_body(text_msg_body,"18867110004",kChatTypeSingle);
	
	text_msg->set_from("18867110003");
	text_msg->set_burn_after_read(false);

	text_msg->set_conversation_id("18867110004");
	text_msg->set_conversation_name("hhh");
	text_msg->set_is_sender(true);
	text_msg->set_nick_from("660003");

	client->send_message(audio_msg);
}

void testsqlite3(){
	std::cout << "testsqliteupdatefriendlist and result is " << testsqliteupdatefriendlist() << std::endl;
	testsqliteupdatefriend();
	std::cout << "testsqliteaddfriend and result is " << testsqliteaddfriend() << std::endl;
	std::cout << "testsqlitedeletefriend and result is " << testsqlitedeletefriend() << std::endl;
}

void sb_friend_notify_cb(FriendNotification ntf) {
    
    std::cout << "friend notify type:" << ntf.get_type() << std::endl;
}

// chat sqlite test
void testupdatechatmessagelist() {
    list<MessageWrapper> msg_list;
	MessageWrapper msg1;
	msg1.set_msg_id("1234567");
	msg1.set_to("18867111354");
	msg1.set_nick_to("nick");
	msg1.set_chat_type(kChatTypeSingle);
	msg1.set_msg_type(kMessageBodyTypeText);
	msg1.set_content("ÎÄ±¾");
    msg1.set_is_sender(true);
    msg1.set_is_msg_read(false);
	msg1.set_status(kMessageStatusSendSuccess);
    msg1.set_is_audio_read(false);
    msg1.set_guid(12345);

	MessageWrapper msg2;
	msg2.set_msg_id("12345678");
	msg2.set_to("18867111354");
	msg2.set_nick_to("nick");
	msg2.set_chat_type(kChatTypeSingle);
	msg2.set_msg_type(kMessageBodyTypeVoice);
	msg2.set_content("ÓïÒô");
    msg2.set_is_sender(true);
    msg2.set_is_msg_read(false);
	msg2.set_status(kMessageStatusSendSuccess);
    msg2.set_is_audio_read(true);
    msg2.set_guid(23345);

	MessageWrapper msg3;
	msg3.set_msg_id("123456789");
	msg3.set_to("18867111355");
	msg3.set_nick_to("nick");
	msg3.set_chat_type(kChatTypeSingle);
	msg3.set_msg_type(kMessageBodyTypeVoice);
	msg3.set_content("ÓïÒô");
    msg3.set_is_sender(true);
    msg3.set_is_msg_read(false);
	msg3.set_status(kMessageStatusSendSuccess);
    msg3.set_is_audio_read(true);
    msg3.set_guid(233344);

	msg_list.push_back(msg1);
	msg_list.push_back(msg2);
	msg_list.push_back(msg3);
	int i = update_chat_message_list(msg_list);
    std::cout<< "update chat message list result:" << i << std::endl;
	
}

void testgetchatmessagelist(){
	char *remoteusername = "18867110004";	

    int count = 5;
    int page = 1;
    int total;
	list<MessageWrapper> message_wrapper = chat->get_next_page_msg(count, page, &total, remoteusername);
	cout << "get_chat_message_list start-------------" << endl;

    list<MessageWrapper>::iterator iter;
    for (iter = message_wrapper.begin(); iter != message_wrapper.end(); ++iter)
    {
        std::cout <<"get_chat_message_list msgId:" << iter->get_msg_id() << " remoteUsername:" << 
			iter->get_conversation_name() << " remoteNickname:" << iter->get_nick_to() << " messageBody:" <<
			iter->get_message_body()->get_message_body_type() << " msgContent:" << iter->get_content() << " chatType:" << 
			iter->get_chat_type() << " msgType:" << iter->get_msg_type() << " isSend:" << 
			iter->is_sender() << " isRead:" << iter->is_msg_read() << " unreadFlag:" << 
			iter->is_audio_read() << " guid:" << iter->get_guid() << std::endl;
    }
    cout << "get_chat_message_list end-------------" << endl;
	
}

void testsetchatvoicemessagestatus(){
	set_chat_voice_message_status("1234567");
	cout << "testsetchatvoicemessagestatus -------------" << endl;
	testgetchatmessagelist();
}

void testsetchatmessagereadstatus(){
	set_chat_message_read_status("18867111354");
	cout << "testsetchatmessagereadstatus -------------" << endl;
	testgetchatmessagelist();
}

void testupdatechatmessageguid(){
	update_chat_message_guid("1234567","112233");
	cout << "testupdatechatmessageguid -------------" << endl;
	testgetchatmessagelist();
}

void testdeleteallchatmessage(){
	delete_all_chat_message();
	cout << "testdeleteallchatmessage -------------" << endl;
	testgetchatmessagelist();
}

void test_get_md5_file_address(){
  string url = CCConfigManager::GetInstance().get_md5_file_address();
  string md5 = "5fd32cb367d3dad2084389d3b6435104";
  string file_url;
  http_get_url_by_md5(url, md5, file_url);
}

int main(int argc, char** argv) {

	options opts = {"201903cx", "123456", kLogLevelAll};
	error_info err;
	
	const char* token = "12345678";

	int loginmode = 2;
	
	client->init(&opts,"/root", &err);
	string username = "18867110003";

	TestMessageObserver *msg_observer = new TestMessageObserver();
	client->message_observer_register(msg_observer);
	
	TestConnectObserver *connect_observer = new TestConnectObserver();
	client->connect_observer_register(connect_observer);
	
	TestContactObserver *contact_observer = new TestContactObserver();
	client->contact_observer_register(contact_observer);
	client->login(username.c_str(), token, loginmode);

    sleep(2);
    //testaddfriend();
    /*testupdatechatmessagelist();
	
	testsetchatvoicemessagestatus();
	testsetchatmessagereadstatus();
	testupdatechatmessageguid();
	testdeleteallchatmessage();*/
    //test_get_md5_file_address();
    
    testsendmessage();
    

    testgetchatmessagelist();
	
	//testgetvoipconfig();

	//sleep(10);

	//testgetfriendslist();
	//testgetaddfriendrequest();

	//testagreefriendrequest();
	//testgetvoipconfig();
	//testsendmessage();
    //testgetfriendlist();
    //testgetfriendinfo();
	//testsqlite3();
	//testaddfriend();
	//testdeclinefriendrequest();
	//testremovefriend();
	//testsyncfriend();
	
	//client->uninit();
	
	

	while (1){}
		
    return 0;
}

