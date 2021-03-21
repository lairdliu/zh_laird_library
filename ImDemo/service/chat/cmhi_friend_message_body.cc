
#include "cmhi_friend_message_body.h"


namespace cmhi_iov{

FriendMessageBody::FriendMessageBody(string text)
{
	init(text);
}

void FriendMessageBody::init(string text)
{
	this->text_ = text;
	set_message_body_type(kMessageBodyTypeFriend);
}

string FriendMessageBody::get_text(){
	return text_;
}

void FriendMessageBody::set_text(string text){
	text_ = text;
}


}


