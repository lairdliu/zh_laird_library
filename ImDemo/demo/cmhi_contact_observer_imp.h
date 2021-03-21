/***************************************************************************
  * file name cmhi_contact_observer_imp.h
  * brief Listen for friend change notify from sdk
  * date 2019/03/13
  *************************************************************************/

  
#ifndef CMHI_IM_CONTACT_OBSERVER_IMP_H_
#define CMHI_IM_CONTACT_OBSERVER_IMP_H_

#include <string>
#include <iostream>

#include "cmhi_contact_observer.h"

namespace cmhi_iov{
  
  class TestContactObserver:public ContactObserverBase{
  public:
  	
	TestContactObserver(){}
	~TestContactObserver(){}

	virtual void OnFriendChanged(FriendNotification ntf) override {
		std::cout << "return friend notification, notification's type is:" << ntf.get_type() << std::endl;
	}
  };
  
  }
#endif  //CMHI_IM_CONTACT_OBSERVER_IMP_H_
