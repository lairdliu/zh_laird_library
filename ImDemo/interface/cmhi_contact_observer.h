/***************************************************************************
  * file name cmhi_contact_observer.h
  * brief Listen for friend change notify from sdk
  * date 2019/03/11
  *************************************************************************/

  
#ifndef CMHI_IM_CONTACT_OBSERVER_H_
#define CMHI_IM_CONTACT_OBSERVER_H_

#include "common_types.h"
#include "base_typedefs.h"

namespace cmhi_iov{
  
  class ContactObserverBase{
  public:
  	
	ContactObserverBase(){}
	virtual ~ContactObserverBase(){}

	virtual void OnFriendChanged(FriendNotification ntf) = 0;
  };
  
  }
#endif  //CMHI_IM_CONTACT_OBSERVER_H_

