/***************************************************************************
  * file name cmim_text_message_body.cpp
  * brief 文本消息内容
  * date 2019/02/02
  *************************************************************************/

#include "cmhi_text_message_body.h"
namespace cmhi_iov{

TextMessageBody::TextMessageBody(string text)
{
	init(text);
}

void TextMessageBody::init(string text)
{
	this->text_ = text;
	set_message_body_type(kMessageBodyTypeText);
}

string TextMessageBody::get_text(){
	return text_;
}

void TextMessageBody::set_text(string text){
	text_ = text;
}


}

