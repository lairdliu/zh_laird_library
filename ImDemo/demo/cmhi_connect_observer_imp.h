/***************************************************************************
  * file name cmhi_connect_observer_imp.h
  * brief Listen for login/logout/connectionChanged notify from sdk
  * date 2019/03/13
  *************************************************************************/

  
#ifndef CMHI_IM_CONNECT_OBSERVER_IMP_H_
#define CMHI_IM_CONNECT_OBSERVER_IMP_H_

#include <string>
#include <iostream>

#include "cmhi_connect_observer.h"

namespace cmhi_iov{

class TestConnectObserver: public ConnectObserverBase{

public:	
	TestConnectObserver(){}	
	~TestConnectObserver(){}	
	
	virtual void OnLogin(EErrorCode error) override{
		std::cout << "return login error code:" << error << std::endl;
	}

	virtual void OnLogout(EErrorCode error) override{
		std::cout << "return logout error code:" << error << std::endl;
	}

	virtual void OnConnectionChanged(EConnectionStatus connection_status, const std::string& reason) override{
		std::cout << "connect status changed to " << connection_status  << ",reason:" << reason << std::endl;
	}
		virtual void OnAccountInfoChanged(UserInfo user) override{		std::cout << "account info update:"<<std::endl;		std::cout<<"get_avatar_original_link:"<<user.get_avatar_original_link()<<std::endl;		std::cout<<"get_avatar_thumbnail_link:"<<user.get_avatar_thumbnail_link()<<std::endl;		std::cout<<"get_nick_name:"<<user.get_nick_name()<<std::endl;		std::cout<<"get_nick_name_pinyin:"<<user.get_nick_name_pinyin()<<std::endl;		std::cout<<"get_phone:"<<user.get_phone()<<std::endl;		std::cout<<"get_remark_name:"<<user.get_remark_name()<<std::endl;		std::cout<<"get_user_name:"<<user.get_user_name()<<std::endl;		}};

}
#endif  //CMHI_IM_CONNECT_OBSERVER_IMP_H_


