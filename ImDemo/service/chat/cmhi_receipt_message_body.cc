/***************************************************************************
  * file name cmim_receipt_message_body.cpp
  * brief 回执消息消息体
  * date 2019/02/12
  *************************************************************************/

#include "cmhi_receipt_message_body.h"

namespace cmhi_iov{

ReceiptMessageBody::ReceiptMessageBody(string receipt_msg_id)
{
	this->receipt_msg_id = receipt_msg_id;
	init();
}

ReceiptMessageBody::ReceiptMessageBody(string receipt_msg_id,string receipt_conversation_id)
{
	this->receipt_msg_id = receipt_msg_id;
	this->receipt_conversation_id = receipt_conversation_id;
	init();
}

void ReceiptMessageBody::init()
{
	set_message_body_type(kMessageBodyTypeReceipt);
}

void ReceiptMessageBody::set_receipt_msg_id(string msg_id)
{
	this->receipt_msg_id = msg_id;
}

void ReceiptMessageBody::set_receipt_conversation_id(string conversation_id)
{
	this->receipt_conversation_id = conversation_id;
}

string ReceiptMessageBody::get_receipt_msg_id()
{
	return receipt_msg_id;
}

string ReceiptMessageBody::get_receipt_conversation_id()
{
	return receipt_conversation_id;
}
}
