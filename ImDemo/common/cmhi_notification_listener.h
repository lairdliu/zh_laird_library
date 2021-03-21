/***************************************************************************
  * file name cmhi_notification_listener.h
  * brief Listen for notification messages
  * date 2019/03/04
  *************************************************************************/

#ifndef CMHI_IM_NOTIFICATION_LISTENER_H_
#define CMHI_IM_NOTIFICATION_LISTENER_H_

#include "api/outer/connector.pb.h"


namespace cmhi_iov{

class INotificationListener{
public:
	virtual ~INotificationListener(){}
	virtual void OnReceiveNotification(const outer::connector::SessionNotify& notify) = 0;
	
};

}
#endif  //CMHI_IM_NOTIFICATION_LISTENER_H_
