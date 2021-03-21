/***************************************************************************
 * file name formatter.h
 * brief 基础类型定义
 * date 2019/01/31
 *************************************************************************/
 
#ifndef COMMON_FORMATTER_H
#define COMMON_FORMATTER_H

#include <string>
#include "base_typedefs.h"
#include "common_types.h"
#include "cmhi_message_body.h"


using namespace std;

namespace cmhi_iov {

string generate_uuid();

string null_to_blank(string str);

uint64_t generate_time();

int parse_status_from_resp(string response);

string parse_original_link_from_resp(string response);

uint64_t parse_msg_time_by_guid(uint64_t guid);

string generate_post_param(MessageBodyPtr msg_body);

}

#endif //COMMON_FORMATTER_H

