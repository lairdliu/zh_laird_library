//
// Created by yangwenfang on 2019/3/19.
//

#ifndef ALIOS_AMR_ENCODE_ERRORCODE_H
#define ALIOS_AMR_ENCODE_ERRORCODE_H

enum EErrorCode {
    kSuccess = 200,               // 成功
    kDatabaseError = 1,                 // server端数据库异常
    kSessionTimeouted,                  // token过期
    kUserNotExistedOrPasswordError,     // 用户名不存在或密码错误
    kSystemError,                       // 系统错误
    kAccessResourceFailed,              // 资源访问失败
    kPermissionDenied,                  // 该操作用户没有权限
    kParamInvalid,                      // 请求包含错误参数
    kProtoParseError = 8,               // PB解析出错

    kUserLoginedOtherDevice = 20,       // 账号在其他地方登陆
    kUserTempProhibit,                  // 系统临时禁止登陆
    kAppkeyOrAppkeyPasswordError = 23,  // appkey或者appkeyPassword不存在

    kServerInnerError = 500,            // 服务器内部错误
    kUserNotExisted,                    // 用户不存在
    kUserAlreadyRegistered,             // 用户已注册
    kUserPhoneInvalid = 503,            // 手机号不合法
    kUserCurrentSilent,                 // 已经是静默状态
    kUserCurrentNotSilent,              // 已经是非静默状态
    kPhoneAlreadyRegistered = 506,      // 号码已注册
    kPasswordEmpty = 507,               // 入参密码为空
    kUserCreateError = 511,             // 用户创建失败
    kUserDeleteError,                   // 用户删除失败
    kUserUpdateError,                   // 用户更新失败
    kUserQueryError,                    // 用户查询失败

    kUserNotLogin = 601,                // 用户未登陆
    kUserConflictLogin,                 // 其他用户登陆，被踢
    kSessionStatusError = 603,          // session连接异常
    kConnectorUserAlreadyLogin,         // 用户已登陆

    kChatSenderIsForbidden = 1001,      // 发送方被禁言
    kChatReceiverNotExist,              // 接收方不存在
    kChatMessageInvalid,                // 消息内容不正确
    kChatMessageSaveFailed = 1004,      // 消息存储失败
    kChatSenderIsEmpty,                 // 发送方为空
    kChatExceedMultiReceiverLimit,      // 群发接收者超出人数限制
    kChatSomeReceiverNotExist = 1007,   // 群发部分接收者不存在


    kSerialZKNodeNotFound = 2401,       // 未查询到ZK中的node
    kSerialSeqNotExist,                 // 查询seq不存在
    kSerialWorkerIdNotExist,            // 查询workerid不存在
    kSerialIDDeleteFailed,              //

    kSerialIDGenerateFailed = 2405,     // 生成serialId失败


    kEmailSendFailed = 2801,            // 邮件发送失败
    kEmailReceiverAddressEmpty = 2802,  // 接收地址为空


    kFriendQueryFromNickFailed = 3401,  // 用户中心查询好友请求者昵称失败
    kFriendParameterEmptyError,         // 好友参数为空
    kFriendRequestNotExit,              // 好友请求不存在
    kFriendRequestHasBeenProcessed,     // 好友请求已被处理
    kFriendRequestReceiverNotMatch,     // 同意／拒绝好友请求发起者与好友请求的接收者不匹配
    kFriendAppkeyNotMatch = 3406,       // appkey不匹配
    kFriendApplySelfError,              // 添加自己为好友
    kFriendUpdateSelfError,             // 更新自己备注信息
    kFriendIsFriendAlready,             // 已经是好友
    kFriendIsNotFriend = 3410,          // 不是好友

    kRequiredParmIsNil = 50000,         // 必填参数不能为nil
    kRequiredParmIsIllegal,             // 必填参数不合法
    kUsernameIsIllegal,                 // 用户名不合法
    kUsernameIsNil,                     // 用户名不能为nil
    kPhoneIsIllegal,                    // 手机号不合法
    kPhoneIsNil,                        // 手机号不能为nil
    kCaptchaIsIllegal,                  // 验证码不合法
    kCaptchaIsNil,                      // 验证码不能为nil
    kPasswordIsIllegal,                 // 密码不合法
    kPasswordIsNil,                     // 密码不能为nil
    kUserNickIsIllegal = 50010,        // 昵称不合法
    kUserNickIsNil,                     // 昵称不能为nil


    // 网络
            knoNet,                             // 没网
    kConnectServerFail,                 // 连接服务器失败

    //消息重发
            kResendFail,                        // 重发失败

    kUnknow,                           // 未知错误
    kAmrFileIsInvalid,                 //amr文件不正确
    kAmrHeaderIsInvalid ,                  //amr文件文件头不正确
    kPcmFileIsInvalid,                 //pcm文件不正确
    kPcmHeaderIsInvalid                //pcm文件文件头不正确


};

#endif //ALIOS_AMR_ENCODE_ERRORCODE_H
