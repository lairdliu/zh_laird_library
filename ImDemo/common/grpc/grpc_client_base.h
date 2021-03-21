/* 
 *------------------------------------------------------------------------------
 *
 * File Name  :grpc_client_base.h
 * todo : description
 */
 
#ifndef GRPC_GRPC_LIENT_BASE_H
#define GRPC_GRPC_LIENT_BASE_H


#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <signal.h>

#include <unistd.h>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "api/outer/grpc.grpc.pb.h"
#include "api/outer/connector.pb.h"
#include "api/common/enumeration.pb.h"
#include "common/sigslot.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using grpc::CompletionQueue;
using namespace outer::grpc;
using namespace common::enumeration;
using namespace outer::connector;
using namespace std;
using google::protobuf::Message;
using sigslot::has_slots;
using sigslot::signal0;
using sigslot::signal1;

namespace cmhi_iov {

class CCGrpcClientBase {
  public:

  explicit CCGrpcClientBase(std::shared_ptr<Channel> channel) {

	  channel_ = channel;
	  stub_ = ConnectorService::NewStub(channel);

	  if (stream_ !=NULL){
	  	stream_.reset();
		printf("reset steam..........................");
	  }
      create_stream();

  
   /* explicit CCGrpcClientBase(std::shared_ptr<Channel> channel)
            : stub_(ConnectorService::NewStub(channel)) {

      create_stream();*/

    }


	 static void* thread_recv_session_notify(void* base1);

    // TODO: bool should be changed to ErrorCode
    bool send_unary_request(const UnaryRequest& request, UnaryResponse* response);

    // TODO: bool should be changed to ErrorCode
    bool health_check(const HealthCheckRequest& request, HealthCheckResponse* 
    response);

    bool send_session_request(const SessionRequest& request);

    bool close_stream();
	
	  template<class desttype>
    void init(
            desttype* pclass,
            void (desttype::*handle_message_signal_callback_func)(const 
            SessionNotify&),
            void (desttype::*on_connected_signal_callback_func)(),
            void (desttype::*on_disconnect_signal_callback_func)()
            ) {
        if(pclass == nullptr)
        {
            return;
        }

        handle_message_signal_.connect(pclass, handle_message_signal_callback_func);
        on_connected_signal_.connect(pclass, on_connected_signal_callback_func);
        on_disconnect_signal_.connect(pclass, on_disconnect_signal_callback_func);

        return;
    }

			template<class desttype>
	void uninit(desttype* pclass){

		
		handle_message_signal_.disconnect(pclass);
		on_connected_signal_.disconnect(pclass);
		on_disconnect_signal_.disconnect(pclass);

		channel_.reset();
		 grpc_shutdown();
		return;
	}

  	std::shared_ptr<ClientReaderWriter<SessionRequest, SessionNotify>> stream();
			
    
    bool is_alive();

  private:

    bool create_stream();

	public:
		 unique_ptr<ConnectorService::Stub> stub_;
	 
	  ClientContext context_;
    shared_ptr<ClientReaderWriter<SessionRequest, SessionNotify>> stream_;
		 std::shared_ptr<grpc::Channel> channel_;

  private:

    // Out of the passed in Channel comes the stub, stored here, our view of the
    // server's exposed services.
    
   
    // The producer-consumer queue we use to communicate asynchronously with the
    // gRPC runtime.
    CompletionQueue cq_;
    
    int steam_status_;
	
    signal1<const SessionNotify&> handle_message_signal_;
    signal0<> on_connected_signal_;
    signal0<> on_disconnect_signal_;
};

}
#endif //GRPC_GRPC_LIENT_BASE_H

