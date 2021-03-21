/* 
 *------------------------------------------------------------------------------
 *
 * File Name  :cc_grpc_client.h
 * todo : description
 */
 
#ifndef GRPC_CC_GRPC_LIENT_H
#define GRPC_CC_GRPC_LIENT_H

#include <string>
#include <thread>
#include <unistd.h>

#include "grpc_client_base.h"
#include "grpc_notification_parser.h"
#include "cmhi_notification_listener.h"
#include "cmhi_connection_listener.h"
#include "common/sigslot.h"
#include "api/outer/grpc.pb.h"
#include "cmhi_contact_imp.h"

using namespace outer::grpc;
using namespace outer::connector;
using namespace common::base;
using namespace std;
using sigslot::has_slots;
using sigslot::signal1;

namespace cmhi_iov{

	class CCGrpcClient : public has_slots<>
	{
	
  /*------ public method ------*/
  public:
	  ~CCGrpcClient() {}
		
		static CCGrpcClient & GetInstance()
		{
			static CCGrpcClient instance_;
			return instance_;
		}

    void keep_session_alive();

    // suggest! send normal unary request
    bool send_unary_request(string service_name, string method_name, const string& request_data, string& response_data);

    // UNSUGGEST! send normal unary request
	// !!!!!!!!!! 好像没有被调用到吧
//		string SendUnaryRequest(string service_name, string method_name, const string& request_data);
    
    // send normal session request
		bool send_session_request(string request_data, SessionRequest::ESessionRequestType type,const string deviceId);
		void handle_session_notify(const outer::connector::SessionNotify& notify);
		void on_connected();
		void on_disconnected();
		void init(string server_address);
		void uninit();
		
		void set_notification_listener(INotificationListener* notification_listener);
		void set_connection_listener(IConnectionListener* connection_listener);

  /*------ private method ------*/
  private:

	  bool send_heart_beat();
    //UnaryResponse DoSendUnaryRequest(const UnaryRequest& request);
    bool do_send_unary_request(const UnaryRequest& request, UnaryResponse& 
    response);
    bool do_send_session_request(const SessionRequest& request);
	
    
  /*------ public parameter ------*/
	public:    	
		IConnectionListener* connection_listener_;
  		CCGrpcClientBase* base_;
  		pthread_mutex_t mutex;
  		pthread_t tid;
  /*------ private parameter ------*/
  private:
		INotificationListener* notification_listener_;
	};

}

#endif //GRPC_LC_GRPC_LIENT_H

