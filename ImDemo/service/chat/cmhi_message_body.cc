/***************************************************************************
  * file name cmim_message_body.cpp
  * brief 基础消息
  * date 2019/02/12
  *************************************************************************/

#include "cmhi_message_body.h"

namespace cmhi_iov{


void MessageBody::set_message_body_type(EMessageBodyType type)
{
	this->type = type;
}

}
