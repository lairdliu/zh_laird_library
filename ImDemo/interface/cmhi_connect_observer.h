/***************************************************************************
  * file name cmhi_connect_observer.h
  * brief Listen for login and connect status notify from sdk
  * date 2019/03/11
  *************************************************************************/

  
#ifndef CMHI_IM_CONNECT_OBSERVER_H_
#define CMHI_IM_CONNECT_OBSERVER_H_

#include <string>
#include "common_types.h"

namespace cmhi_iov{
  
  class ConnectObserverBase{
  public:
  	
	ConnectObserverBase(){}
	virtual ~ConnectObserverBase(){}

	virtual void OnLogin(EErrorCode) = 0;
	virtual void OnLogout(EErrorCode) = 0;
	virtual void OnConnectionChanged(EConnectionStatus connection_status, const std::string& reason) = 0;
	virtual void OnAccountInfoChanged(UserInfo user)=0;
  };
  
  }
#endif  //CMHI_IM_CONNECT_OBSERVER_H_

