/***************************************************************************
  * file name cmhi_connection_listener.h
  * brief Listen for connection changed.
  * date 2019/03/21
  *************************************************************************/

#ifndef COMMON_CMHI_CONNECTION_LISTENER_H_
#define COMMON_CMHI_CONNECTION_LISTENER_H_

#include "common_types.h"
#include "cmhi_user_info.h"
namespace cmhi_iov{

class IConnectionListener{
public:
	virtual ~IConnectionListener(){}
	virtual void OnConnectionChanged(EConnectionStatus connection_status, const std::string& reason) = 0;
	
	virtual void OnAccountInfoChanged(UserInfo user)=0;
	
};

}
#endif  //COMMON_CMHI_CONNECTION_LISTENER_H_