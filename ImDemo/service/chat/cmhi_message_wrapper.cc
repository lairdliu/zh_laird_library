/***************************************************************************
  * file name cmim_message_wrapper.cpp
  * brief 消息数据类
  * date 2019/02/13
  *************************************************************************/

#include "cmhi_message_wrapper.h"
#include "formatter.h"
#include "base64.h"
namespace cmhi_iov{

MessageWrapper::MessageWrapper()
{	
	msg_sent_time_ = (uint64_t)time(NULL);
	is_online_ = true;
	is_sender_ = true;
}

void MessageWrapper::init_with_body(MessageBodyPtr body, string to, EChatType chat_type)
{
	this->to_ = to;
	msg_id_ = generate_uuid();
	from_ = "";
	nick_from_ = "";
	conversation_id_ = to;
    chat_type_ = chat_type;
	status_ = kMessageStatusSending;
	is_sender_ = true;
	message_body_ = body;
}
string MessageWrapper::get_msg_id(){
	return msg_id_;
}

string MessageWrapper::get_from(){
	return from_;
}

string MessageWrapper::get_nick_from(){
	return nick_from_;
}

string MessageWrapper::get_to(){
	return to_;
}

string MessageWrapper::get_conversation_id(){
	return conversation_id_;
}

string MessageWrapper::get_nick_to(){
	return nick_to_;
}

string MessageWrapper::get_conversation_name(){
	return conversation_name_;
}

string MessageWrapper::get_local_path(){
	return local_path_;
}

EChatType MessageWrapper::get_chat_type(){
	return chat_type_;
}

EMessageBodyType MessageWrapper::get_msg_type(){
    return msg_type_;
}

MessageBodyPtr MessageWrapper::get_message_body(){
	return message_body_;
}

EMessageStatus MessageWrapper::get_status(){
	return status_;
}

bool MessageWrapper::get_burn_after_read(){
	return burn_after_read_;
}

bool MessageWrapper::is_sender(){
	return is_sender_;
}

bool MessageWrapper::is_msg_read(){
    return is_msg_read_;
}

bool MessageWrapper::is_audio_read(){
    return is_audio_read_;
}
int MessageWrapper::get_duration(){
    return audio_duration_;
}

uint64_t MessageWrapper::get_guid(){
    return guid_;
}

uint64_t MessageWrapper::get_msg_sent_time(){
    return msg_sent_time_;
}

string MessageWrapper::get_msg_body_info(){
    return msg_body_info_;
}

string MessageWrapper::get_msg_body_info_base64()
{
	return base64_encode((unsigned char const*)msg_body_info_.c_str(),msg_body_info_.length());
}

string MessageWrapper::get_content(){
	return content_;
}

bool MessageWrapper::is_online(){
    return is_online_;
}


void MessageWrapper::set_msg_id(const string msg_id){
    msg_id_ = msg_id;
}

void MessageWrapper::set_from(const string from_user){
	from_ = from_user;
}

void MessageWrapper::set_nick_from(const string nick_from){
	nick_from_ =nick_from;
}

void MessageWrapper::set_to(const string to){
	to_ = to;
}

void MessageWrapper::set_nick_to(const string nick_to){
	nick_to_ = nick_to;
}

void MessageWrapper::set_conversation_id(const string conversation_id){
	conversation_id_ = conversation_id;
}

void MessageWrapper::set_conversation_name(const string conversation_name){
	conversation_name_ = conversation_name;
}

void MessageWrapper::set_local_path(const string local_path){
	local_path_= local_path;
}

void MessageWrapper::set_chat_type(const EChatType chat_type){
	chat_type_ = chat_type;
}

void MessageWrapper::set_msg_type(const EMessageBodyType msg_type){
    msg_type_ = msg_type;
}

void MessageWrapper::set_message_body(MessageBodyPtr msg_body){
    message_body_ = msg_body;
}

void MessageWrapper::set_status(const EMessageStatus status){
	status_ = status;
}

void MessageWrapper::set_burn_after_read(const bool burn_after_read){
	burn_after_read_ = burn_after_read;
}

void MessageWrapper::set_is_sender(const bool is_sender){
	is_sender_ = is_sender;
}

void MessageWrapper::set_is_msg_read(const bool is_msg_read){
    is_msg_read_ = is_msg_read;
}

void MessageWrapper::set_is_audio_read(const bool is_audio_read){
    is_audio_read_ = is_audio_read;
}
void MessageWrapper::set_duration(const int duration){
    audio_duration_ = duration;
}

void MessageWrapper::set_guid(const uint64_t guid){
    guid_ = guid;
}

void MessageWrapper::set_msg_sent_time(const uint64_t time){
    msg_sent_time_ = time;
}

void MessageWrapper::set_msg_body_info(const string msg_body_info){
    msg_body_info_ =  msg_body_info;
}

void MessageWrapper::set_msg_body_info_base64(const string msg_body_info){
    msg_body_info_ = base64_decode(msg_body_info);
}

void MessageWrapper::set_content(const string content){
    content_ = content;
}

void MessageWrapper::set_is_online(const bool is_online){
    is_online_ = is_online;
}

}

