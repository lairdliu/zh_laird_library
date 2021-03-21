/***************************************************************************
  * file name grpc_notification_parser.cc
  * brief process session nitification info
  * date 2019/02/21
  *************************************************************************/

#include "grpc_notification_parser.h"

using namespace std;
using namespace common::ntf;

using namespace outer::grpc;
using namespace outer::connector;
using namespace common::base;
using namespace std;

namespace cmhi_iov{

EErrorCode NotificationParser::LoginNotifyParser(const SessionNotify& notify,
	outer::user::UserInfo& user_info){

	LoginResponse login_response ;
	login_response.ParseFromString(notify.data());
	int err = login_response.ret();
	user_info  = login_response.user_infos();

	return EErrorCode(err);
}

EErrorCode NotificationParser::LogoutNotifyParser(const SessionNotify& notify){

	LogoutResponse logout_resp;
	logout_resp.ParseFromString(notify.data());
	int err = logout_resp.ret();
	return EErrorCode(err);
	
}

void NotificationParser::KickNotifyParser(const SessionNotify& notify,
	EConnectionStatus& login_status,string& reason,string &deviceId){
	KickNotification  kick_notify;
	login_status = cmhi_iov::kStatusBekicked;
	kick_notify.ParseFromString(notify.data());
	reason = kick_notify.reason();
	deviceId = kick_notify.imei();
}
}
