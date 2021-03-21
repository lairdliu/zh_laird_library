/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "grpc_client_base.h"
#include "logger.h"
#include "grpc/grpc.h"
namespace cmhi_iov{

	 void *CCGrpcClientBase::thread_recv_session_notify(void* base1) {
		
	  SessionNotify notify;
	  CCGrpcClientBase* base = (CCGrpcClientBase*)base1;

  while (base->stream()->Read(&notify)) {
    CCLog(kLogLevelDebug, "RecvSessionNotify, to_username:%s, appkey:%s, type:%u, request_id:%s", 
      notify.to_username().c_str(), notify.appkey().c_str(), 
      notify.type(), notify.request_id().c_str());
    
    base->handle_message_signal_(notify);
  }
  return NULL;
}

// TODO: bool should be changed to ErrorCode
bool CCGrpcClientBase::send_unary_request(const UnaryRequest& request, UnaryResponse* response) {

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

   gpr_timespec timespec;
	timespec.tv_sec = 5;//设置超时时间
	timespec.tv_nsec = 0;
    timespec.clock_type = GPR_TIMESPAN;
	
	context.set_deadline(timespec); 

  // The actual RPC.
  Status status = stub_->SendUnaryRequest(&context, request, response);

  // Act upon its status.
  if (status.ok()) {
    CCLog(kLogLevelDebug, "invoke SendUnaryRequest success: %u, error_msg: %s", 
      status.error_code(), status.error_message().c_str());
    
    return true;
  } else {
    CCLog(kLogLevelWarn, "invoke SendUnaryRequest failed: %u, error_msg: %s", 
      status.error_code(), status.error_message().c_str());
    
    return false;
  }
}

// TODO: bool should be changed to ErrorCode
// ！！！好像没有被用过
bool CCGrpcClientBase::health_check(const HealthCheckRequest& request, HealthCheckResponse* response) {

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

  // The actual RPC.
  Status status = stub_->HealthCheck(&context, request, response);

  // Act upon its status.
  if (status.ok()) {
    CCLog(kLogLevelDebug, "invoke HealthCheck success: %u, error_msg: %s", 
      status.error_code(), status.error_message().c_str());
    
    return true;
  } else {
    CCLog(kLogLevelWarn, "invoke HealthCheck failed: %u, error_msg: %s", 
      status.error_code(), status.error_message().c_str());
    
    return false;
  }
}

bool CCGrpcClientBase::send_session_request(const SessionRequest& request) {
    bool status = stream_->Write(request);
    if (!status) {
      CCLog(kLogLevelWarn, "invoke SendSessionRequest failed");
    }
    //stream_->WritesDone();
    //Status status = stream_->Finish();

  
    return status;
}

bool CCGrpcClientBase::close_stream() {
  Status status = stream_->Finish();
  if (status.ok()) {
  CCLog(kLogLevelDebug, "CloseStream success: %u, error_msg: %s", 
    status.error_code(), status.error_message().c_str());
  
    return true;
  } else {
    CCLog(kLogLevelWarn, "CloseStream failed: %u, error_msg: %s", 
      status.error_code(), status.error_message().c_str());
    
    return false;
  }
}

std::shared_ptr<ClientReaderWriter<SessionRequest, SessionNotify>> CCGrpcClientBase::stream() {
  return stream_;
}

bool CCGrpcClientBase::is_alive() {
  // TODO
  return true;
}

bool CCGrpcClientBase::create_stream() {

//
/*	gpr_timespec timespec;
	timespec.tv_sec=5;
	timespec.tv_nsec=0;
	timespec.clock_type=GPR_TIMESPAN;
	context_.set_deadline(timespec);*/

    std::shared_ptr<ClientReaderWriter<SessionRequest, SessionNotify> > stream(
        stub_->SendSessionRequest(&context_));
    
    stream_ = stream;

    return true;
}

}

