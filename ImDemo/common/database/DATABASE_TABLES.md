###好友列表Friend建表
DROP TABLE IF EXISTS Friend;
CREATE TABLE Friend
(ID integer primary key autoincrement,  //自增id
username text NOT NULL UNIQUE,          //好友用户名，非空，唯一
nickname text,                          //好友昵称
remarkName text,                        //好友备注名
phone text,                             //好友号码
modifyTime timestamp DEFAULT (datetime('now','localtime')),  //修改时间
silent integer,                         //好友静默
enable bool DEFAULT 1,                  //好友是否已删除，0为已删，1为未删
avatarThumbnailLink text,               //好友头像缩略图链接
avatarOriginalLink text);               //好友头像原图链接

###好友请求列表Request建表
DROP TABLE IF EXISTS Request;
CREATE TABLE Request 
(username text NOT NULL,                //请求好友用户名
nickname text,                          //请求好友昵称
requestId text NOT NULL UNIQUE,         //请求id，唯一
modifyTime text timestamp DEFAULT (datetime('now','localtime')),  //修改时间
status bool DEFAULT 0,                         //请求状态，0未处理，1已接收，2已拒绝
remark text,                            //请求备注消息
unreadFlag bool DEFAULT 0,              //请求已读标识，0未读，1已读
avatarThumbnailLink text,               //请求好友头像缩略图链接
avatarOriginalLink text);               //请求好友头像原图链接

###消息列表Chat建表
DROP TABLE IF EXISTS Chat;
CREATE TABLE Chat 
(msgId text primary key,                //消息id，由发送方sdk产生，消息唯一标识
remoteUsername text NOT NULL,           //远端用户名
remoteNickname text,                    //远端用户昵称
msgBody text,                           //整个消息体序列化后字符串
msgContent text,                        //会话列表上显示的消息文本，如语音消息显示[语音]
createDate double timestamp DEFAULT (datetime('now','localtime')),  //消息创建时间
chatType integer,                       //聊天类型（单聊、群聊）
msgType integer,                        //消息类型（文本，语音等）
status integer DEFAULT 1,               //消息状态（0为已删，1为有效）
isSender bool,                          //是否为发送方(0为否，1为是)
isMsgRead bool DEFAULT 0,               //消息是否已读（0为否，1为是）
isAudioRead bool DEFAULT 0,             //语音消息是否已点开（0为否，1为是）
time double,                            //消息发送、接收时间
localPath text,                         //本地路径
guid text NOT NULL,                     //guid,由平台产生，用于后面同步历史消息，同步历史消息时需带上最后一条消息的guid
extra text);                            //预留字段               