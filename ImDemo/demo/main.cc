#include <string.h>
#include "cmhi_message_wrapper.h"
#include "cmhi_im_client.h"
#include "cmhi_audio_message_body.h"
#include <iostream>
#include <unistd.h>
#include "cmhi_message_observer_imp.h"
#include "cmhi_connect_observer_imp.h"
#include "cmhi_contact_observer_imp.h"
#include "cmhi_custom_message_body.h"

using namespace std;
using namespace cmhi_iov;


shared_ptr<CCClientManagerBase> client = CCClientManagerBase::create_client_manager();

string request_id = "";
string friend_username = "";

//===========================CallBack==============================

//===========================Account Interface==============================
void test_init() {
	cout << "====================================================" << endl;
	std::cout << "INIT START ..." << std::endl;
	char* appkey = "201904cx";
	char* appkeyPassword = "123456";
    options opts = {appkey, appkeyPassword, kLogLevelAll};
    error_info err;

    client->init(&opts,"/cache", &err);
	std::cout << "INIT SUCCESS ..." << std::endl;
	cout << "====================================================" << endl;
}

void test_login() {
   	std::cout << "LOGIN START ..." << std::endl;
	const char* token = "12345678";

	int loginmode = 2;//loginmode=1为token登录模式；loginmode=2为debug模式

    string username = "18867110003";
    client->login(username.c_str(), token, loginmode,"deviceIdXXX");
}

void test_get_userinfo() {

    UserInfo user_info;
    client->get_user_info(user_info);
} 


void test_logout() {

    client->logout();

}

void test_uninit() {
    client->uninit();
}

//===========================Contact Interface==============================

void test_get_friends_list(){

    int page = 1;
    int count = 5;
    int total = 20;

    error_info error;

    list<FriendInfo> friend_list = client->get_friends_list(&error, page, count, &total);


	cout << "=====获取好友列表===== start-------------" << endl;

    list<FriendInfo>::iterator iter;

    for (iter = friend_list.begin(); iter != friend_list.end(); ++iter)
    {
        cout <<"get_friends_list info username: " << endl;
        cout <<"username: " << iter->get_user_name()<< endl;
        cout <<"nickname: " << iter->get_nick_name()<< endl;
        cout <<"remarkName: " << iter->get_remark_name()<< endl;
        cout <<"avatarThumbnailLink: " << iter->get_avatar_thumbnail_link()<< endl;
        cout <<"avatarOriginalLink: " << iter->get_avatar_original_link()<< endl;
        cout <<"phone: " << iter->get_phone()<< endl;
        cout <<"enable: " << iter->get_is_enable()<< endl; 
        cout <<"silent: " << iter->get_silent()<< endl;
        cout <<"nickname_pinyin: " << iter->get_nick_name_pinyin()<< endl;
        cout <<"remarkName_pinyin: " << iter->get_remark_name_pinyin()<< endl;
    }

	friend_username = (friend_list.back()).get_user_name();
	cout << "get the latest friend_username is " << friend_username<< endl;
    cout << "=====获取好友列表===== end-------------" << endl;
}

void test_get_add_friend_request(){
	list<RequestWrapper> request_list = client->get_add_friend_request();
	
	cout << "=====获取添加好友请求===== start-------------" << endl;
    list<RequestWrapper>::iterator iter;
	if(request_list.size() != 0){
		for (iter = request_list.begin(); iter != request_list.end(); ++iter) {
        	cout <<"get_add_friend_request info request_id: " << iter->get_request_id()<< endl;
			cout <<"get_add_friend_request info remark: " << iter->get_remark()<< endl;
			cout <<"get_add_friend_request info req_status: " << iter->get_req_status()<< endl;
			cout <<"get_add_friend_request info req_time: " << iter->get_request_time()<< endl;
    	}
	request_id = (request_list.back()).get_request_id();
	}

	cout << "get the latest friend request is " << request_id<< endl;
    cout << "====获取添加好友请求===== end-------------" << endl;
}

void test_get_friend_userinfo(){
    error_info error;
	char get_friend_username[40];
	strcpy(get_friend_username, friend_username.c_str());
	cout << "get friend username is " << get_friend_username << endl;
    FriendInfo friend_info = client->get_friend_user_info(get_friend_username, &error);
	
	cout << "=====获取好友信息===== start-------------" << endl;
	cout <<"get_friends_userinfo username: "<< endl;
	cout <<"username: " << friend_info.get_user_name()<< endl;
	cout <<"nickname: " << friend_info.get_nick_name()<< endl;
	cout <<"remarkName: " << friend_info.get_remark_name()<< endl;
	cout <<"avatarThumbnailLink: " << friend_info.get_avatar_thumbnail_link()<< endl;
	cout <<"avatarOriginalLink: " << friend_info.get_avatar_original_link()<< endl;
	cout <<"phone: " << friend_info.get_phone()<< endl;
	cout <<"enable: " << friend_info.get_is_enable()<< endl;
	cout <<"silent: " << friend_info.get_silent()<< endl;
	cout <<"nickname_pinyin: " << friend_info.get_nick_name_pinyin()<< endl;
	cout <<"remarkName_pinyin: " << friend_info.get_remark_name_pinyin()<< endl;
	cout << "=====获取好友信息===== end-------------" << endl;
}

void test_add_friend(){
    error_info error;
	cout << "=====添加好友===== start-------------" << endl;
    client->add_friend("18867110003","test3", &error);
}

void test_set_friend_silent(){
    error_info error;
    cout << "=====设置好友静默状态===== start-------------" << endl;
    cout << "set silent" << endl;
    
    client->set_friend_silent("18867110003", 0, &error);

    cout << "=====设置好友静默状态===== end-------------" << endl;
}

void test_agree_friend_request(){
    error_info error;
	cout << "=====同意好友请求===== start-------------" << endl;
	char friend_request_id[40];
	strcpy(friend_request_id, request_id.c_str());
    client->agree_friend_request(friend_request_id, &error);
}

void test_decline_friend_request(){    
    error_info error;
	cout << "=====拒绝好友请求===== start-------------" << endl;
	char friend_request_id[40];
	strcpy(friend_request_id, request_id.c_str());
    client->decline_friend_request(friend_request_id, "reason", &error);
}

void test_get_unread_msg(){
	int count = 2;
	char *conversation_id= "max";
	list<MessageWrapper> message_wrapper = client->get_unread_msg(count, conversation_id, true);

	cout << "=====获取未读消息===== start-------------" << endl;

    list<MessageWrapper>::iterator iter;
	if(message_wrapper.size() != 0){
		for (iter = message_wrapper.begin(); iter != message_wrapper.end(); ++iter) {
            cout << " msg_id:" << iter->get_msg_id() << endl;
            cout << " remoteUsername:" <<iter->get_conversation_name() << endl;
            cout << " remoteNickname:" << iter->get_nick_to() << endl;
            cout << " messageBody:" << iter->get_message_body() << endl;
            cout << " msgContent:" << iter->get_content() << endl;
            cout << " chatType:" << iter->get_chat_type() << endl;
            cout << " msgType:" << iter->get_msg_type() <<  endl;
            cout << " isSend:" << iter->is_sender() <<  endl;
            cout << " msg read:" << iter->is_msg_read() << endl;
            cout << " audio read:" << iter->is_audio_read() << endl;
            cout << " guid:" << iter->get_guid() << endl; 

		}
	}
    
    cout << "=====获取未读消息===== end-------------" << endl;
}


void test_remove_friend(){
    error_info error;
	cout << "=====删除好友===== start-------------" << endl;
	char remove_friend_username[40];
	strcpy(remove_friend_username, friend_username.c_str());
    client->remove_friend("18867110001", &error);
}

//===========================Chat Interface==============================
void test_send_message(){
	MessageWrapperPtr audio_msg = shared_ptr<MessageWrapper>(new MessageWrapper);
	AudioMessageBody audio_message_body("audio.amr", "/cache/test/db/audio.amr", 20);
	
	//essageBody *audio_msg_body = &audio_message_body;

	
	std::shared_ptr<AudioMessageBody> audio_msg_body = shared_ptr<AudioMessageBody>(new AudioMessageBody(audio_message_body));
	audio_msg->init_with_body(audio_msg_body, "18867110001", kChatTypeSingle);
	
	audio_msg->set_from("18867110003");
	audio_msg->set_burn_after_read(false);

	audio_msg->set_conversation_id("18867110001");  //车机端单聊中conversation id就是指接收方的username,群聊就是指group id
	audio_msg->set_conversation_name("hello");
	audio_msg->set_is_sender(true);
	audio_msg->set_nick_from("660001");

	client->send_message(audio_msg);
	client->send_message(audio_msg);
}


void test_get_next_page_msg_1(){
	int count = 1;
	int page = 1;
	int total = 2;
	char *conversation_id= "18867110001";
	list<MessageWrapper> message_wrapper = client->get_next_page_msg(count, page, &total, conversation_id);

	cout << "=====获取分页消息===== start-------------" << endl;

    list<MessageWrapper>::iterator iter;
	if(message_wrapper.size() != 0){
		for (iter = message_wrapper.begin(); iter != message_wrapper.end(); ++iter) {
            cout << " msg_id:" << iter->get_msg_id() << endl;
            cout << " remoteUsername:" <<iter->get_conversation_name() << endl;
            cout << " remoteNickname:" << iter->get_nick_to() << endl;
            cout << " fromUsername:" <<iter->get_from() << endl;
            cout << " fromNickname:" << iter->get_nick_from() << endl;        
            cout << " toUsername:" <<iter->get_to() << endl;
            cout << " toNickname:" << iter->get_nick_to() << endl;              
            cout << " messageBody:" << iter->get_message_body() << endl;
            cout << " msgContent:" << iter->get_content() << endl;
            cout << " chatType:" << iter->get_chat_type() << endl;
            cout << " msgType:" << iter->get_msg_type() <<  endl;
            cout << " isSend:" << iter->is_sender() <<  endl;
            cout << " msg read:" << iter->is_msg_read() << endl;
            cout << " audio read:" << iter->is_audio_read() << endl;
			 cout << " audio duration:" << iter->get_duration() << endl;
            cout << " guid:" << iter->get_guid() << endl; 
		}
	}
    
    cout << "=====获取分页消息===== end-------------" << endl;
}
void test_get_next_page_msg_2(){
	int count = 1;
	int page = 1;
	int total = 2;
	char *conversation_id= "18867110004";
	list<MessageWrapper> message_wrapper = client->get_next_page_msg(count, page, &total, conversation_id);

	cout << "=====获取分页消息===== start-------------" << endl;

    list<MessageWrapper>::iterator iter;
	if(message_wrapper.size() != 0){
		for (iter = message_wrapper.begin(); iter != message_wrapper.end(); ++iter) {
            cout << " msg_id:" << iter->get_msg_id() << endl;
            cout << " remoteUsername:" <<iter->get_conversation_name() << endl;
            cout << " remoteNickname:" << iter->get_nick_to() << endl;
            cout << " fromUsername:" <<iter->get_from() << endl;
            cout << " fromNickname:" << iter->get_nick_from() << endl;        
            cout << " toUsername:" <<iter->get_to() << endl;
            cout << " toNickname:" << iter->get_nick_to() << endl;              
            cout << " messageBody:" << iter->get_message_body() << endl;
            cout << " msgContent:" << iter->get_content() << endl;
            cout << " chatType:" << iter->get_chat_type() << endl;
            cout << " msgType:" << iter->get_msg_type() <<  endl;
            cout << " isSend:" << iter->is_sender() <<  endl;
            cout << " msg read:" << iter->is_msg_read() << endl;
            cout << " audio read:" << iter->is_audio_read() << endl;
            cout << " guid:" << iter->get_guid() << endl; 
		}
	}
    
    cout << "=====获取分页消息===== end-------------" << endl;
}

void test_delete_all_messages(){
	cout << "=====删除所有消息===== start-------------" << endl;
	client->delete_all_messages();
	cout << "=====删除所有消息===== end-------------" << endl;
}

void test_send_read_receipt(){
	cout << "=====发送已读回执===== start-------------" << endl;

	client->send_read_receipt("18867110002", "1234", "18867110002", true);// 第一个参数为对方号码，第二个为消息msg_id，第三个单聊下为对方号码
	
	cout << "=====发送已读回执===== end-------------" << endl;
}

void test_get_latest_contact(){
        list<FriendInfo> friend_list;

        cout << "=====获取最近联系人信息===== start-------------" << endl;
        friend_list = client->get_latest_contact();
    list<FriendInfo>::iterator friend_info;


    for (friend_info = friend_list.begin(); friend_info != friend_list.end(); ++friend_info)
    {
        cout <<"get_latest_contact info get_user_name: " << friend_info->get_user_name()<< endl;
        cout <<"get_nick_name: " << friend_info->get_nick_name()<< endl;
        cout <<"get_latest_contact info get_phone: " << friend_info->get_phone()<< endl;
        cout <<"get_latest_contact info get_avatar_original_link: " << friend_info->get_avatar_original_link()<< endl;
        cout <<"get_latest_contact info get_avatar_thumbnail_link: " << friend_info->get_avatar_thumbnail_link()<< endl;
        cout <<"get_latest_contact info get_remark_name: " << friend_info->get_remark_name()<< endl;
        MessageWrapperPtr message_wrapper = friend_info->get_friend_message();
        cout << " msg_id:" << message_wrapper->get_msg_id() << endl;
        cout << " remoteUsername:" <<message_wrapper->get_conversation_name() << endl;
        cout << " remoteNickname:" << message_wrapper->get_nick_to() << endl;
        cout << " fromUsername:" <<message_wrapper->get_from() << endl;
        cout << " fromNickname:" << message_wrapper->get_nick_from() << endl;         
        cout << " fromUsername:" <<message_wrapper->get_to() << endl;
        cout << " fromNickname:" << message_wrapper->get_nick_to() << endl;             
        cout << " messageBody:" << message_wrapper->get_message_body() << endl;
        cout << " msgContent:" << message_wrapper->get_content() << endl;
        cout << " chatType:" << message_wrapper->get_chat_type() << endl;
        cout << " msgType:" << message_wrapper->get_msg_type() <<  endl;
        cout << " isSend:" << message_wrapper->is_sender() <<  endl;
        cout << " msg read:" << message_wrapper->is_msg_read() << endl;
        cout << " audio read:" << message_wrapper->is_audio_read() << endl;
        cout << " guid:" << message_wrapper->get_guid() << endl;         
	cout << " body:"<<message_wrapper->get_msg_body_info()<<endl;
    }
    cout << "=====获取最近联系人信息===== end-------------" << endl;
}

void test_set_voice_played(){
    list<MessageWrapper> msg_list;
    MessageWrapper msg1;
    msg1.set_msg_id("1234567");
    msg1.set_to("18867111354");
    msg1.set_nick_to("nick");
    msg1.set_chat_type(kChatTypeSingle);
    msg1.set_msg_type(kMessageBodyTypeText);
    msg1.set_content("文本");
    msg1.set_is_sender(true);
    msg1.set_status(kMessageStatusSendSuccess);
    msg1.set_is_msg_read(false);
    msg1.set_is_audio_read(false);
    msg1.set_guid(123);
    msg1.set_conversation_id("123456");

    MessageWrapper msg2;
    msg2.set_msg_id("12345678");
    msg2.set_to("18867111354");
    msg2.set_nick_to("nick");
    msg2.set_chat_type(kChatTypeSingle);
    msg2.set_msg_type(kMessageBodyTypeVoice);
    msg2.set_content("语音");
    msg2.set_is_sender(true);
    msg2.set_status(kMessageStatusSendSuccess);
    msg2.set_is_msg_read(false);
    msg2.set_is_audio_read(false);
    msg2.set_guid(123);
    msg2.set_conversation_id("123456");

    msg_list.push_back(msg1);
    msg_list.push_back(msg2);

    //int i = update_chat_message_list(msg_list);       

    cout << "=====设置接收语音消息的播放状态===== start-------------" << endl;
    client->set_voice_played("18867111354", "12345678");
    cout << "=====设置接收语音消息的播放状态===== end-------------" << endl;

}


void test_insert_db()
{
	CustomMessageBody body;
	struct Custom_Entity entity={"000","234"};
	struct Custom_Entity entity1={"111","456"};
	struct Custom_Entity entity2={"222","789"};
	body.set_subtype("123");
	body.set_notification("ABC");
	body.set_entity(entity);
	body.set_entity(entity1);
	body.set_entity(entity2);
   	body.delete_entity(1); 
	string msg_id ="234567890000000";
	string to = "you";
	string from = "me";
	bool is_sender = false;
	bool is_msg_read = false;
	
	client->insert_custom_msg_to_db(body, msg_id,to,from,is_sender,is_msg_read);
	//	body.delete_entity(1);


}


void test_msg_size()
{
	printf("====test_msg_size:%d\n",client->get_msg_size());
}

void test_get_msg_by_id()
{
    string msg_id= "a3748e32-22d8-478d-a623-9061f763e27e";
    MessageWrapperPtr message_wrapper = client->get_msg_by_id(msg_id);
    cout << "=====通过msg id获取msg消息===== start-------------" << endl;
    std::cout <<"test_get_msg_by_id:" << message_wrapper->get_msg_id() << " remoteUsername:" <<
    message_wrapper->get_conversation_name() << " remoteNickname:" << message_wrapper->get_nick_to() << " messageBody:" <<
    message_wrapper->get_message_body() << " msgContent:" << message_wrapper->get_content() << " chatType:" <<
    message_wrapper->get_chat_type() << " msgType:" << message_wrapper->get_msg_type() << " isSend:" <<
    message_wrapper->is_sender() << " msg read:" << message_wrapper->is_msg_read() << " audio read:" <<
    message_wrapper->is_audio_read() << " guid:" << message_wrapper->get_guid() << std::endl;
    cout << "=====通过msg id获取msg消===== end-------------" << endl;
}


FILE * test_pcm = NULL;

void test_write_pcm_cb(const int16_t *buf, int length, bool isend, bool* cancel) 
{
    if (isend) 
     {
        fseek(test_pcm, 0, SEEK_SET);
        fclose(test_pcm);
    }
    else
    {
        if (test_pcm == NULL)
        {
            return;
        }
        fwrite(buf, length, 1, test_pcm);
    }
}


void test_read_pcm_cb(const int16_t * buf, int length, bool* is_end, bool* cancel) {
    if (test_pcm == NULL) 
    {
        char *infile = "/home/zty/iov_linux/src/common/amr/love.pcm";
        test_pcm = fopen(infile, "rb");
        if (!test_pcm) 
        {
            fprintf(stderr, "Unable to open pcm file %s\n", test_pcm);
            *is_end = true;
            return;
        }
    }
    int read = fread((void *)buf, 1, length, test_pcm);
    if (read < 160) 
    {
        *is_end = true;
    }
    else 
    {
        *is_end = false;
    }
    return;
}

void test_error_cb(error_info err) 
{
    /* err.error_code 携带错误信息类型，对应模块各自处理*/
}

void test_pcm_to_amr()
{
    char *outfile = "/home/zty/iov_linux/src/common/amr/out.amr";
    cout << "=====pcm_to_amr===== start-------------" << endl;
    client->pcm2amr(outfile, test_read_pcm_cb, test_error_cb);
    test_pcm = NULL;
    cout << "=====pcm_to_amr===== end-------------" << endl;
}

void test_amr_to_pcm(){
    char *infile = "/home/zty/iov_linux/src/common/amr/love.amr";
    char *outfile = "/home/zty/iov_linux/src/common/amr/out.pcm";
    test_pcm = fopen(outfile, "wb");
    if (!test_pcm)    
    {
        fprintf(stderr, "Unable to open pcm file %s\n", test_pcm);
        return;    
    }    
    cout << "=====amr_to_pcm===== start-------------" << endl;    
    client->amr2pcm(infile, test_write_pcm_cb, test_error_cb);  
    test_pcm = NULL;
    cout << "=====amr_to_pcm===== end-------------" << endl;
}

void test_pcm_resampler()
{
    FILE *fpin = NULL;
    FILE *fpout= NULL;
    short inbuffer[1024];
    short outbuffer[1024];
    int srcSampleRate = 16000;
    int dstSampleRate = 8000;
    int delaultLen = 360;
    int outLen = 0;
    int readLen = 0;
    EErrorCode ret;

    cout << "=====pcm_resampler===== start-------------" << endl;  
    fpin = fopen("/home/zty/iov_linux/src/common/amr/kaluli.pcm","r");
    if(fpin == NULL){
        printf("input pcm is opened failed.\n");
        return;
    }
    fpout= fopen("/home/zty/iov_linux/src/common/amr/kaluli-8000.pcm","w");
    if(fpout == NULL){
        printf("Create out pcm failed.\n");
        return;
    }
    
    while(1)
    {
        readLen = fread(inbuffer, 1, delaultLen, fpin);
        if (readLen < 160)
        {
            break;
        }
        
		/* 
		 * 调用者可以参考之前的准备流程准备PCM数据，也可以使用已有的PCM数据， 
		 * pcmResampler函数只负责重采样工作，
		 * inbuffer、outbuffer空间需要调用者自己申请并保证不会有越界风险
		 */
        ret = client->pcmResampler((const short *)inbuffer, (const short *)outbuffer, readLen, &outLen, srcSampleRate, dstSampleRate);
        
        fwrite(outbuffer, 1, outLen, fpout);
    }
    
    if(fpin != NULL)
    {
        fclose(fpin);
    }
    
    if(fpout != NULL)
    {
        fclose(fpout);
    }
    
	cout << "=====pcm_resampler===== start-------------" << endl;  
    return;
}

void test_set_friend_request_read()
{
	RequestWrapper friend_request;
	CReadFriendResponse friend_response;

	friend_request.set_request_id("359605626084524032");
	client->set_friend_request_read(friend_request,friend_response);

	cout<<"msg_id:"<<friend_response.get_msg_id()<<std::endl;
	cout<<"err_code:"<<friend_response.get_errorcode()<<std::endl;
}

#include "cmhi_location_message_body.h"
void test_send_location_msg()
{
		/* location msg */
	
		MessageWrapperPtr location_msg = shared_ptr<MessageWrapper>(new MessageWrapper);
		LocationMessageBody location_message_body("123.jpg", "/tmp/123.jpg");
		location_message_body.set_width(1);
		location_message_body.set_height(2);
		location_message_body.set_longitude(3);
		location_message_body.set_latitude(4);
		location_message_body.set_location_desc("test");
		location_message_body.set_location_address("cd");
		location_message_body.set_poi_id("poi_id");
	
		std::shared_ptr<LocationMessageBody> location_msg_body = shared_ptr<LocationMessageBody>(new LocationMessageBody(location_message_body));


	
		location_msg->init_with_body(location_msg_body, "18867110002", kChatTypeSingle);
		location_msg->set_from("18867110001");
		location_msg->set_burn_after_read(false);
		location_msg->set_conversation_id("18867110002");  //车机端单聊中conversation id就是指接收方的username,群聊就是指group id
		location_msg->set_conversation_name("hello");
		location_msg->set_is_sender(true);
		location_msg->set_nick_from("0001");
		client->send_message(location_msg);
}

void test_send_text_msg()
{
		/* text msg */
	
		MessageWrapperPtr test_msg = shared_ptr<MessageWrapper>(new MessageWrapper);
		TextMessageBody text_message_body("abcdefg");
	
		std::shared_ptr<TextMessageBody> text_msg_body = shared_ptr<TextMessageBody>(new TextMessageBody(text_message_body));


	
		test_msg->init_with_body(text_msg_body, "18867110003", kChatTypeSingle);
		test_msg->set_from("18867110001");
		test_msg->set_burn_after_read(false);
		test_msg->set_conversation_id("18867110003");  //车机端单聊中conversation id就是指接收方的username,群聊就是指group id
		test_msg->set_conversation_name("hello");
		test_msg->set_is_sender(true);
		test_msg->set_nick_from("0001");
		client->send_message(test_msg);
}


int main(int argc, char** argv) {

    test_init();

    TestMessageObserver *msg_observer = new TestMessageObserver();
	MessageWrapperPtr msg = shared_ptr<MessageWrapper>(new MessageWrapper());
	msg->set_msg_id("12345");
	client->message_observer_register(msg_observer);
	msg_observer->OnMessageReceived(msg);	
	TestConnectObserver *connect_observer = new TestConnectObserver();
	client->connect_observer_register(connect_observer);
	
	TestContactObserver *contact_observer = new TestContactObserver();
	client->contact_observer_register(contact_observer);
    
    test_login();
	sleep(20);
	//test_remove_friend();
	
 //   test_agree_friend_request();
//	test_insert_db();
//	test_get_latest_contact();
 //	test_get_friends_list();
// test_send_message();
	
//	test_get_next_page_msg_1();
	
	//test_send_message();
	//test_get_next_page_msg_2();

	extern int grpcdown;


	while(1)
		{
			std::cout<<"grpcdown:"<<grpcdown<<std::endl;
			if(grpcdown) 
				{
					const char* token = "12345678";

					int loginmode = 2;//loginmode=1为token登录模式；loginmode=2为debug模式

    				string username = "18867110001";
    				client->login_try(username.c_str(), token, loginmode,"deviceIdXXX");
				}
			sleep(5);
		}
	

#if 0

	sleep(1);
	test_insert_db();
	test_msg_size();
	test_get_userinfo();
	test_get_friends_list();
	test_get_friend_userinfo();
	test_get_add_friend_request();
	test_add_friend();
    test_set_friend_silent();	
	test_agree_friend_request();
	test_decline_friend_request();
	test_remove_friend();
	
    sleep(1);
    test_get_latest_contact();
    test_set_voice_played();	
	
	sleep(1);
    test_send_message();
    test_delete_all_messages();
	test_send_read_receipt();
    test_get_msg_by_id();
	test_set_friend_request_read();
	sleep(5);
    test_logout();
    
    test_uninit();

#endif

   while(1) sleep(1); 	
    return 0;

}

