/***************************************************************************
  * file name cmim_text_message_body.cpp
  * brief 文本消息内容
  * date 2019/02/12
  *************************************************************************/

#include "cmhi_file_message_body.h"

namespace cmhi_iov{

FileMessageBody::FileMessageBody(string local_path)
{
  set_file_name("");
  set_origin_link("");
  set_file_length(0);
  set_local_path(local_path);

	set_message_body_type(kMessageBodyTypeFile);
}

void FileMessageBody::set_file_name(string file_name)
{
	this->file_name = file_name;
}

void FileMessageBody::set_origin_link(string origin_link)
{
	this->origin_link = origin_link;
}

void FileMessageBody::set_local_path(string local_path)
{
	this->local_path = local_path;
}

void FileMessageBody::set_file_length(uint32_t len)
{
	this->file_length = len;
}

}
