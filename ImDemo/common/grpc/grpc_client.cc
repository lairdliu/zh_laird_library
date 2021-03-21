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

#include "grpc_client.h"
#include "logger.h"
#include "formatter.h"
#include <iostream>
using namespace std;
//add2
//#include <grpc_types.h>
using grpc::Channel;
using grpc::ChannelArguments;




using namespace outer::connector;
using namespace common::ntf;

//33环境的证书
const char SERVER_CRT[] =R"(

-----BEGIN CERTIFICATE-----
MIIDYjCCAkoCCQD1uU14sVJ4SDANBgkqhkiG9w0BAQsFADBzMQswCQYDVQQGEwJD
TjERMA8GA1UECAwIWmhlamlhbmcxETAPBgNVBAcMCEhhbmd6aG91MQ0wCwYDVQQK
DARDTUNDMQ0wCwYDVQQLDARSSFRYMSAwHgYDVQQDDBdjYXJjaGF0LWdycGMuY20t
aW92LmNvbTAeFw0xOTA3MTcwODA1NDlaFw0yMDA3MTYwODA1NDlaMHMxCzAJBgNV
BAYTAkNOMREwDwYDVQQIDAhaaGVqaWFuZzERMA8GA1UEBwwISGFuZ3pob3UxDTAL
BgNVBAoMBENNQ0MxDTALBgNVBAsMBFJIVFgxIDAeBgNVBAMMF2NhcmNoYXQtZ3Jw
Yy5jbS1pb3YuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAolxT
86wqM5BPVoyxxOSIYGRyhcMVzutEOPwd92WYxhieoDFw/EZ9xqgomgCF+ACn8IQC
PJWJ6iBz3NBuDFyUiDGRw5BfhPqP0zkMXYw3pl6wMOiEw6yhngfXd8RrizEe2FyS
3JdWXJAXpzvcU+rM7+K572TcIWZOJv1u8fQdHqKByvJXxDaURzlD+gIsSFNgDM9i
GWj22doX48VdhWveWc6i8+5mk7x5BlDUmXn9q7p2ks7K+VjsRxDmbnmSjsfBUJ6j
qxRIh72TLgFnJeIklYpB67EwnoBfBarEiNvCyr5hu3fWd7QdZw40v1d95w9SYSJY
6Ec+kVZUbCqvqQAxqQIDAQABMA0GCSqGSIb3DQEBCwUAA4IBAQAmJ7BUgkF8ZqHU
X3957zhmNDZwPOHlO9pPtQxalt6bniLgkOl9maJe7WgXfeQLl3CiDY5FwAl8ajPu
NO5MG5c73YKVQc5GA84A8HNA5gWIi8PK5lAU/Kql4D6iSGR04IHXIcqelZcIWKPL
SlbZk6xYnrekL0VG4cmptX3MWFcdx7LbNW6JVmw00AqwYP29jXHgtQ5Hm7do6VoV
EQ9hzG/sjUPx/hren/Bo6l1pjxQhc8GKxnC4iinShLR8nx0961nP8mzoM09WQGoP
LsIYNscu8FvvjixcrlCx8TDqCy5R/nnU0QCwiUdaKJ1ZRbrq9iUwRIsJq3Lpw1KV
B8kDOcae
-----END CERTIFICATE-----
)";

//66环境 的证书
const char SERVER_SRT[] =R"(

-----BEGIN CERTIFICATE-----
MIICrzCCAhgCCQDKvcuL8z6mmDANBgkqhkiG9w0BAQsFADCBmzELMAkGA1UEBhMC
Y24xEzARBgNVBAgMCnpoZW5namlhbmcxETAPBgNVBAcMCGhhbmd6aG91MQ0wCwYD
VQQKDARjbWNjMQ0wCwYDVQQLDARyaHR4MSMwIQYDVQQDDBpjYXJjaGF0LWdycGMt
cHAuY20taW92LmNvbTEhMB8GCSqGSIb3DQEJARYSaHVhbmd5eV9oeUAxMzkuY29t
MB4XDTE5MTAzMTExMjY1MVoXDTIwMTAzMDExMjY1MVowgZsxCzAJBgNVBAYTAmNu
MRMwEQYDVQQIDAp6aGVuZ2ppYW5nMREwDwYDVQQHDAhoYW5nemhvdTENMAsGA1UE
CgwEY21jYzENMAsGA1UECwwEcmh0eDEjMCEGA1UEAwwaY2FyY2hhdC1ncnBjLXBw
LmNtLWlvdi5jb20xITAfBgkqhkiG9w0BCQEWEmh1YW5neXlfaHlAMTM5LmNvbTCB
nzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEAr8hoHyF0EwIkt0atsh9ukoGfxmi5
2kXrWIJJqcsvQNNFuILzcio+rd+MRC6rKBZkydn53dQCA8yYIkD/jQFbLM8KIwvz
8Jtez/iFGJtW8ia4H1zhvWBxHQWLTH8XQvDKKtb0iT6FqPHYhvCuiXbszc06LytY
koGkWoGDfvhnUbECAwEAATANBgkqhkiG9w0BAQsFAAOBgQCA6fpbikB5wO6rOcY3
YYxbEp8oAv9LRQk0jIlixbR0r44jP7uen6D/kZclkA+n/UluIHInGLD0f6wPINNm
lX+0Nc9F3dzzPpE7GflHGl6D+hf+vJNbvPjvN+ZV1GxLfHKCRf30Q42RvcKSKfhG
XrfzpuEJMIrhATh+WPMnBgZzow==
-----END CERTIFICATE-----
)";



int grpcdown = 0;
namespace cmhi_iov{

void CCGrpcClient::handle_session_notify(const outer::connector::SessionNotify& notify){
	CCLog(kLogLevelInfo, "HandleSessionNotify, notify: %s", notify.DebugString().c_str());

	notification_listener_->OnReceiveNotification( notify);
}

void CCGrpcClient::on_connected() {

}

void CCGrpcClient::on_disconnected() {
  CCLog(kLogLevelInfo, "disconencting...");
  if (base_)
  {
  
    CCLog(kLogLevelInfo, "CCGrpcClient::on_disconnected");
	//base_->close_stream();
	//CCLog(kLogLevelInfo,"begin to close_stream \n"); 
	base_->uninit(this);
	CCLog(kLogLevelInfo,"begin to base_uninit \n"); 
	
  }
  if(connection_listener_) {
    //connection_listener_->OnConnectionChanged(kStatusDisconnected, "stream closed");
  }
}


void CCGrpcClient::uninit(){
	void *ret = NULL;
	CCLog(kLogLevelInfo,"begin to uninit \n"); 
	//pthread_mutex_lock(&CCGrpcClient::GetInstance().mutex);
	on_disconnected();
	#if 0
	if(tid !=-1)
	{
		pthread_cancel(tid);//发送取消线程的请求
		pthread_join(tid, &ret);  //等待线程结束
		if (ret == PTHREAD_CANCELED)  // 判断是否成功取消线程
			CCLog(kLogLevelInfo,"thread has stopped,and exit code: %d\n", ret);  
		else
			CCLog(kLogLevelError,"some error occured");
	}
	#endif
	CCLog(kLogLevelInfo,"end to uninit \n"); 
    //if(base_) {delete(base_); base_ = NULL;}
	//pthread_mutex_unlock(&CCGrpcClient::GetInstance().mutex);
}

void CCGrpcClient::init(string server_address){
	CCLog(kLogLevelDebug, "CCGrpcClient init start, connDomain server_address =%s", server_address.c_str());
	void *ret = NULL; 
	int err;
	

	sleep(2);
	
		//add1
    grpc::ChannelArguments args;
	args.SetInt( "grpc.keepalive_time_ms",5000);
	args.SetInt( "grpc.keepalive_timeout_ms",5000);
	args.SetInt(GRPC_ARG_ENABLE_RETRIES, 1);
	args.SetInt(GRPC_ARG_HTTP2_MIN_SENT_PING_INTERVAL_WITHOUT_DATA_MS,15000);

	#if 0
	if(tid !=-1)
	{
		pthread_cancel(tid);//发送取消线程的请求
		pthread_join(tid, &ret);  //等待线程结束
		if (ret == PTHREAD_CANCELED)  // 判断是否成功取消线程
			CCLog(kLogLevelInfo,"thread has stopped,and exit code: %d\n", ret);  
		else
			CCLog(kLogLevelError,"some error occured:%d",ret);

	}

	//if(base_) {delete(base_);}
	
	__sync_synchronize();
	#endif

	 //同时支持https和http
     string::size_type s_t;
	 string t = "https://";

	if((s_t=server_address.find(t, 0)) != string::npos)
	{
	    //支持https
		 CCLog(kLogLevelDebug, "it is https");
         server_address.erase(s_t,t.length());
// 		 auto options = grpc::SslCredentialsOptions();
//   	 options.pem_root_certs=SERVER_CRT;
// 		 auto creds=grpc::SslCredentials(options);
		 // Create a default SSL ChannelCredentials object.
		 auto creds = grpc::SslCredentials(grpc::SslCredentialsOptions());
    	 auto channel =  grpc::CreateCustomChannel(server_address,creds,args);
   		 base_=new CCGrpcClientBase(channel);
	}
	else
 	{
 		//支持http
 		CCLog(kLogLevelDebug, "it is http");
		base_ = new CCGrpcClientBase(grpc::CreateCustomChannel(server_address, 
		grpc::InsecureChannelCredentials(),args));
 	}


	//base_ = new CCGrpcClientBase(grpc::CreateCustomChannel(server_address, 
	//grpc::InsecureChannelCredentials(),args));

	__sync_synchronize();
	base_->init(
			this, 
			&CCGrpcClient::handle_session_notify,
			&CCGrpcClient::on_connected,
			&CCGrpcClient::on_disconnected);
	
	 __sync_synchronize();

     CCLog(kLogLevelInfo, "tid:%d",tid);

	     //kill(tid);
     if(tid!=-1)
     {
         pthread_cancel(tid);
         tid=-1;
     }

     err = pthread_create(&tid, NULL, &CCGrpcClientBase::thread_recv_session_notify, base_);
     if(err !=0)
     {
         CCLog(kLogLevelError, "create thread failed! err:%d",err);
     }
     else
     {
         //thread thread1(CCGrpcClientBase::thread_recv_session_notify, base_);
         //thread1.detach();
         CCLog(kLogLevelInfo, "Create ThreadRecvSessionNotify success err:%d",err);
         pthread_detach(tid);
     }
}


bool CCGrpcClient::send_heart_beat() {

	PingRequest ping_request;
	ping_request.set_msg_id(generate_uuid());
	string request_data;
	ping_request.SerializeToString(&request_data);

	SessionRequest session_request;
	session_request.set_data(request_data);
	session_request.set_type(SessionRequest_ESessionRequestType_PING);

	return do_send_session_request(session_request);

}

bool CCGrpcClient::do_send_unary_request(const UnaryRequest& request, UnaryResponse& response) {

	CCLog(kLogLevelDebug, "do_unary_request begin!!!");

	bool ret = false;
	if (base_)
	{
    	ret = base_->send_unary_request(request, &response);
	}
	//CCLog(kLogLevelDebug, "DoSendUnaryRequest, request: %s, \nresponse: %s, \nret:%d\n\n",request.DebugString().c_str(), response.DebugString().c_str(), ret);
	
	return ret;
}

bool CCGrpcClient::do_send_session_request(const SessionRequest& request) {
  
  bool ret = false;
  if (base_)
  {
  	CCLog(kLogLevelDebug, "CCGrpcClient::GetInstance().base_=:%d",CCGrpcClient::GetInstance().base_);
  	ret = base_->send_session_request(request);
  }
  CCLog(kLogLevelDebug, "DoSendSessionRequest, request:%s, ret:%d", 
    request.DebugString().c_str(), ret);
  return ret;
}
#if 0
// send normal unary request // !!!!!!!!!! 没有被调用到
string CCGrpcClient::SendUnaryRequest(string service_name, string method_name, const string& request_data) {

	UnaryRequest request;
	UnaryResponse response;
	request.set_service_name(service_name);
	request.set_method_name(method_name);
	request.set_data(request_data);
  request.set_request_id(generate_uuid());
  bool ret = do_send_unary_request(request, response);

  // TODO:
  if(!ret) {
    CCLog(kLogLevelWarn, "SendUnaryRequest failed. service_name:%s, method_name:%S, request:%s", 
      service_name.c_str(), method_name.c_str(), request_data.c_str());
    return "";
  }
  else {
	  return response.data();
  }
}
#endif
// send normal unary request
bool CCGrpcClient::send_unary_request(string service_name, string method_name, const string& request_data, string& response_data) {

	//CCLog(kLogLevelDebug, "send_unary_request begin!!!");
	UnaryRequest request;
	UnaryResponse response;
	request.set_service_name(service_name);
	request.set_method_name(method_name);
	request.set_data(request_data);
	request.set_request_id(generate_uuid());
	bool ret;

#if 0
	int try_times = 1;
	while(try_times--)
	{
		ret = do_send_unary_request(request, response);
		if (ret)
		{
			break;
		}

		sleep(1);
	}
#endif

  ret = do_send_unary_request(request, response);

  if(!ret)
  {
    CCLog(kLogLevelWarn, "SendUnaryRequest failed. service_name:%s, method_name:%s, request:%s", service_name.c_str(), method_name.c_str(), request_data.c_str());
    if(connection_listener_)
    {
      	connection_listener_->OnConnectionChanged(kStatusCannotAccess, "invoke request failed.");
    }
  }
  else {
	  response_data = response.data();
  }
  //CCLog(kLogLevelDebug, "send_unary_request end!!!");
  return ret;
}

// send normal session request
bool CCGrpcClient::send_session_request(string request_data, SessionRequest::ESessionRequestType type,const string deviceId) {

	MobileBase* base_info = new MobileBase();
	base_info->set_device_id(deviceId);
	base_info->set_imei(deviceId);
	base_info->set_sdk_version("1.2.0");
	base_info->set_client_type(ZEBRA);
	base_info->set_terminal_device_type(CAR_MACHINE);

	SessionRequest session_request;
	session_request.set_allocated_base_info(base_info);
	session_request.set_data(request_data);
	session_request.set_type(type);
  	session_request.set_request_id(generate_uuid());
	bool ret = do_send_session_request(session_request);
  	if(!ret)
  	{
    	CCLog(kLogLevelWarn, "SendSessionRequest failed. type:%u, request_data:%s", type, request_data.c_str());
    	if(connection_listener_)
    	{
      		connection_listener_->OnConnectionChanged(kStatusCannotAccess, "invoke request failed.");
    	}
  	}
  	else
  	{

  	}
  	return ret;
}

extern int gRecvPingFlag;
extern bool keepaliveSendFlag;
void CCGrpcClient::keep_session_alive()
{
	thread thread_alive([]()
	{
		bool status;
		int failed_count = 0;
		unsigned int sleep_time =6; // 6s
		int try_times = 3;
		while (1) 
		{
			sleep(sleep_time);
			//CCLog(kLogLevelDebug, "CCGrpcClient::GetInstance().base_=:%d",CCGrpcClient::GetInstance().base_);
			if (1 == grpcdown)
			{
				CCLog(kLogLevelWarn, "*******grpc not connect try again later.*******");
				string reason = "Heatbeat failed.";
				CCGrpcClient::GetInstance().connection_listener_->OnConnectionChanged(kStatusDisconnected, reason);
				continue;
			}
			if (CCGrpcClient::GetInstance().base_)
			{
				
				//pthread_mutex_lock(&CCGrpcClient::GetInstance().mutex);
				//CCLog(kLogLevelDebug, "keepaliveSendFlag&&CCGrpcClient::GetInstance().base_->is_alive()=:%d",keepaliveSendFlag&&CCGrpcClient::GetInstance().base_->is_alive());
			    if (keepaliveSendFlag&&CCGrpcClient::GetInstance().base_->is_alive())
				{
					if(gRecvPingFlag == 1)
					{
						grpcdown = 0;
						failed_count = 0; /*收到ping报文*/
					}
					status = CCGrpcClient::GetInstance().send_heart_beat();
					//CCLog(kLogLevelDebug, "status:%d gRecvPingFlag:%d",status,gRecvPingFlag);
					if (!status || (gRecvPingFlag == 0))
					{
						failed_count++;
						CCLog(kLogLevelWarn, "SendHeartBeat faild!  failed_count: %d", failed_count);
					}
					else
					{
						failed_count = 0;
					}
					
					gRecvPingFlag = 0;  /*gRecvPingFlag置为0，等待ping报文*/
				}
				//pthread_mutex_unlock(&CCGrpcClient::GetInstance().mutex);
			}

			//CCLog(kLogLevelDebug, "CCGrpcClient::GetInstance().connection_listener_=:%d",CCGrpcClient::GetInstance().connection_listener_);
			if(CCGrpcClient::GetInstance().connection_listener_) 
			{
				string reason = "Heatbeat failed.";
				if(failed_count > 2)
				{
                    CCLog(kLogLevelWarn, "Heatbeat failed.failed_count:%d",failed_count);
					CCGrpcClient::GetInstance().connection_listener_->OnConnectionChanged(kStatusDisconnected, reason);
					failed_count = 0;
					grpcdown  =1;
				}
			}
		}
	});
	thread_alive.detach();
	
	CCLog(kLogLevelDebug, "Create thread KeepSessionAlive...");
}

void CCGrpcClient::set_connection_listener(IConnectionListener* connection_listener){
	connection_listener_ = connection_listener;
}


void CCGrpcClient::set_notification_listener(INotificationListener* notification_listener){
	notification_listener_ = notification_listener;
}
}
