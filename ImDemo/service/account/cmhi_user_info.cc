/***************************************************************************
  * file name cmhi_user_info.cpp
  * brief 用户信息
  * date 2019/02/02
  *************************************************************************/

#include "cmhi_user_info.h"

namespace cmhi_iov{

void UserInfo::set_user_name(string name)
{
	user_name = name;
}

void UserInfo::set_nick_name(string name)
{
	nick_name = name;
}


void UserInfo::set_phone(string phone)
{
	this->phone = phone;
}


void UserInfo::set_remark_name(string name)
{
	this->remark_name= name;
}


void UserInfo::set_avatar_original_link(string link)
{
	this->avatar_original_link= link;
}

void UserInfo::set_avatar_thumbnail_link(string link)
{
	this->avatar_thumbnail_link= link;
}
void UserInfo::set_nick_name_pinyin(string nick_nname_pinyin_)
{
	this->nick_name_pinyin = nick_nname_pinyin_;
}

void UserInfo::set_member_type( int member_type_)
{
	this->member_type = member_type_;
}

void UserInfo::set_series_name(string series_name_)
{
	this->series_name = series_name_;
}

}
