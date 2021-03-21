/***************************************************************************
  * file name cmhi_im_account_imp.h
  * brief 账号管理
  * date 2019/02/13
  *************************************************************************/

#ifndef CMHI_IM_ACCOUNT_IMPLEMENT_H_
#define CMHI_IM_ACCOUNT_IMPLEMENT_H_

#include <string>
#include <list>
#include <memory>

#include "common_types.h"
#include "cmhi_user_info.h"
#include "grpc_client.h"
#include "cmhi_connect_observer.h"


namespace cmhi_iov{


/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

class CCAccountManagerImp{
 public:	
  CCAccountManagerImp(){}
  ~CCAccountManagerImp(){}	

  virtual void init (options *options, error_info *error ) ;
  virtual void login(const char* username, const char* token, int login_mode,const string &deviceId);
  
  virtual void login_try(const char* user_name, const char* token,int login_mode,const string &deviceId) ;
  virtual void logout() ;
  virtual EErrorCode get_user_info(UserInfo & user) ;
  virtual list<FriendInfo> get_latest_contact() ;
  virtual void get_voip_config(string &conf_domain, string &voip_domain);
  virtual void clear_download_by_time();

  void connect_observer_register(ConnectObserverBase* connect_observer);
  static std::shared_ptr<CCAccountManagerImp> getAccountManager();

  
private:
  CCGrpcClient *grpc_client_;
  options *options_;
  UserInfo user_info_;
  string service_name_;
  ConnectObserverBase* connect_observer_;
};

}
#endif  //CMHI_IM_ACCOUNT_IMPLEMENT_H_


