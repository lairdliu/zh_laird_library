/***************************************************************************
  * file name grpc_notification_parser.h
  * brief process notify info from server
  * date 2019/02/21
  *************************************************************************/

#ifndef CMHI_GRPC_NOTIFIATION_PARSER_H_
#define CMHI_GRPC_NOTIFIATION_PARSER_H_

#include <string>

#include "common_types.h"
#include "api/outer/grpc.pb.h"
#include "api/outer/connector.pb.h"
#include "api/common/ntf.pb.h"


namespace cmhi_iov{

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

class NotificationParser{
 public:
 	NotificationParser(){}
	~NotificationParser(){}

	EErrorCode LoginNotifyParser(const outer::connector::SessionNotify& notify,
		outer::user::UserInfo& user_info);
	EErrorCode LogoutNotifyParser(const outer::connector::SessionNotify& notify);
	void KickNotifyParser(const outer::connector::SessionNotify& notify,
		EConnectionStatus& connection_status, std::string& reason,std::string &deviceId);
	
 private:
};

}
#endif //CMHI_GRPC_NOTIFIATION_PARSER_H_

