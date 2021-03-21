#include <iostream>

#include "cmhi_contact_observer.h"
#include "cmhi_friend.h"

namespace cmhi_iov{

class TestContactObserver: public ContactObserverBase{

public:	
	TestContactObserver(){}	
	~TestContactObserver(){}	
	
	virtual void OnFriendChanged(FriendNotification ntf) override{
		std::cout << "OnFriendChanged,notify type:" << ntf.get_type() << std::endl;
	}

};

}