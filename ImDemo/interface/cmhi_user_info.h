/***************************************************************************
  * file name cmhi_user_info.h
  * brief 用户信息头文件
  * date 2019/02/14
  *************************************************************************/

#ifndef CMCC_IM_USER_INFO_H_
#define CMCC_IM_USER_INFO_H_

#include <string>

using std::string;

namespace cmhi_iov{

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

class UserInfo{

	public:
		UserInfo(){}
		~UserInfo(){}
		string get_user_name(){ return user_name; }
		void set_user_name(string user_name);
		string get_nick_name(){ return nick_name; }
		void set_nick_name(string nick_name);
		string get_phone(){ return phone; }
		void set_phone(string phone);
		string get_remark_name(){ return remark_name; }
		void set_remark_name(string remark_name);
		string get_avatar_original_link(){ return avatar_original_link; }
		void set_avatar_original_link(string avatar_original_link);
		string get_avatar_thumbnail_link(){ return avatar_thumbnail_link; }
		void set_avatar_thumbnail_link(string avatar_thumbnail_link);
		string get_nick_name_pinyin(){return nick_name_pinyin;}
		void set_nick_name_pinyin(string nick_name_pinyin_);

		int get_member_type(){return member_type; }
		void set_member_type( int member_type_);

		string get_series_name(){ return series_name; }
		void set_series_name(string series_name_);

	private:
		string user_name;				//好友用户名
		string nick_name;				//好友昵称
		string phone;					//好友电话
		string remark_name;				//好友备注名
		string avatar_original_link;	//头像原图链接
		string avatar_thumbnail_link;	//缩略图链接
		string nick_name_pinyin;

		int member_type;						//好友类型（新加）
		string 	series_name;					//系列名称(新加)

};

}
#endif //CMCC_IM_USER_INFO_H_


