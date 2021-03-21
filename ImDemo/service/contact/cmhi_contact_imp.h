/***************************************************************************
  * file name cmhi_contact_imp.h
  * brief ??? ??????????
  * date 2019/01/31
  *************************************************************************/

#ifndef CONTACT_CMHI_CONTACT_IMP_H_
#define CONTACT_CMHI_CONTACT_IMP_H_

#include <list>
#include <memory>
#include "common_types.h"
#include "cmhi_chat_imp.h"
#include "cmhi_friend.h"
#include "api/outer/connector.pb.h"
#include "api/outer/friends.pb.h"
#include "common/sigslot.h"
#include "cmhi_contact_observer.h"

using sigslot::has_slots;

namespace cmhi_iov{


/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

class ContactManagerImp : public has_slots<> {

public:	
	ContactManagerImp();	
	~ContactManagerImp();	
	
	virtual std::list<FriendInfo> get_friends_list(error_info* error, int page, int count, int *total) ;	
	virtual FriendInfo get_friend_user_info(char* user_id, error_info* error) ;	
	virtual void add_friend(char * friend_name,char * reason, error_info *error) ;
	virtual void agree_friend_request(char* requestid, error_info *error) ;
	virtual void decline_friend_request(char* requestid, char *reason, error_info *error) ;
	virtual void ignore_friend_request(char* requestid, char *reason, error_info *error);
	virtual void remove_friend(char * friend_name, error_info *error) ;
	virtual std::list<RequestWrapper> get_add_friend_request () ;
	virtual void set_friend_silent(char* friend_name, int silent_type, error_info* error);
	virtual void  set_friend_request_read(RequestWrapper friend_request,CReadFriendResponse& friend_response);
	virtual int get_recent_contacts(char *recent_contacts);
	virtual void sync_friend();
	virtual void sync_friend_change(string username, string appkey);
	void notify_friend_change(outer::friends::FriendSignal friend_signal);
	void update_friend_change(const outer::connector::SessionNotify& notify);
	void contact_observer_register(ContactObserverBase* contact_observer);

	static std::shared_ptr<ContactManagerImp> getContactManager();
	bool is_allow_send_message(string username);
private:
  string service_name_;
  uint64_t friend_modified_time_;
  uint64_t request_modified_time_;
  uint64_t signal_modified_time_;
  ContactObserverBase* contact_observer_;
  bool friend_list_ready;

  //string username_;
  //string appkey_;
};

}
#endif  //CONTACT_CMHI_CONTACT_IMP_H_

