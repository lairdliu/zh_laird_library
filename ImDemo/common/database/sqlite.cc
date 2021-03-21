/***************************************************************************
  * file sqlite.cpp
  * brief 账号管理 接口实现
  * date 2019/02/13
  *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "sqlite.h"
#include<base64.h>


using namespace std;

namespace cmhi_iov
{

//把好友添加进数据库
string get_add_friend_sql(string username, string nickname,string nickname_pinyin, string nickname_pinyin_blank,string remarkName,string remarkName_pinyin,string remarkName_pinyin_blank,string phone, int enable, int silent, string thumblink, string orignallink ,int member_type ,string series_name,int user_enable) {
    string sql = "INSERT INTO Friend(username, nickname,nickname_pinyin, nickname_pinyin_blank, remarkName,remarkName_pinyin, remarkName_pinyin_blank, phone, enable, silent, avatarThumbnailLink, avatarOriginalLink, member_type, series_name, user_enable)VALUES('";
    sql += username + "','";
    sql += nickname + "','";
    sql += nickname_pinyin + "','";
	sql += nickname_pinyin_blank + "','";
    sql += remarkName + "','";
    sql += remarkName_pinyin + "','";
    sql += remarkName_pinyin_blank + "','";
	sql += phone + "','";
	sql += std::to_string(enable) + "','";
	sql += std::to_string(silent) + "','";
    sql += thumblink + "','";
    sql += orignallink + "','";
    sql += std::to_string(member_type) + "','";
    sql += series_name + "','";
    sql += std::to_string(user_enable) + "');";

    //CCLog(kLogLevelDebug, "sql: %s", sql.c_str());

    return sql;
}

string get_add_friend_request_sql(string username, string nickname, string requestId, string remark,string thumblink, string orignallink,bool status,uint64_t request_time) {
    string sql = "INSERT INTO Request(username, nickname, requestId, remark, avatarThumbnailLink, avatarOriginalLink, status,request_time)VALUES('";
    sql += username + "','";
    sql += nickname + "','";
    sql += requestId + "','";
    sql += remark + "','";
    sql += thumblink + "','";
    sql += orignallink + "',";
    sql += std::to_string(status) + ",";
	sql += std::to_string(request_time) + ");";
    return sql;
}

string get_add_message_sql(string msgId, string remoteUsername, string remoteNickname, string fromUsername, string fromNickname, 
                  string toUsername, string toNickname, string msgBody, string msgContent, int chatType, 
                 int msgType, int status, int isSender, int isMsgRead, int isAudioRead,int duration, uint64_t time, uint64_t guid, string localPath) {
    string sql = "INSERT INTO Chat(msgId, remoteUsername, remoteNickname, fromUsername, fromNickname, toUsername, toNickname, msgBody, msgContent, chatType,\
        msgType, status, isSender, isMsgRead, isAudioRead, duration, time, guid, localPath)VALUES('";
    sql += msgId + "','";
    sql += remoteUsername + "','";
    sql += remoteNickname + "','";
    sql += fromUsername + "','";
    sql += fromNickname + "','";
    sql += toUsername + "','";
    sql += toNickname + "','";
    sql += msgBody + "','";
    sql += msgContent + "',";
    sql += std::to_string(chatType) + ",";
    sql += std::to_string(msgType) + ",";
	sql += std::to_string(status) + ",";
    sql += std::to_string(isSender) + ",";
    sql += std::to_string(isMsgRead) + ",";
    sql += std::to_string(isAudioRead) + ",";
	sql += std::to_string(duration) + ",";
    sql += std::to_string(time) + ",";
    sql += std::to_string(guid) + ",'";
    sql += localPath + "');";
    return sql;
}


string get_database_path() {
    string db_path = CCConfigManager::GetInstance().get_database_path();
    string db_name = CCConfigManager::GetInstance().get_database_name();
    string db = db_path + "/" + db_name;
    return db;
}


void create_table_friend() {
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        CCLog(kLogLevelDebug, "database path: %s", db_name);
        db.open(db_name);
        string sql = "CREATE TABLE IF NOT EXISTS Friend\
            (ID integer primary key autoincrement,\
            username text NOT NULL UNIQUE,\
            nickname text,\
            nickname_pinyin text,\
            nickname_pinyin_blank text,\
            remarkName text,\
            remarkName_pinyin text,\
            remarkName_pinyin_blank text,\
            phone text,\
            modifyTime timestamp DEFAULT (datetime('now','localtime')),\
            silent integer,\
            enable integer,\
            avatarThumbnailLink text,\
            avatarOriginalLink text,\
            member_type integer DEFAULT 0,\
            series_name text,\
            user_enable integer DEFAULT 0);";

        db.execDML(sql.c_str());       
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "create table Friend failed: %s", e.errorMessage());
    }
    db.close();
}

//check 新字段是否存在，不存在，则 alter table
void update_table_friend()
{
    CppSQLite3DB db;
    try
    {
		bool bHasColumn = false;
        string path = get_database_path();
        const char* db_name = path.c_str();
        CCLog(kLogLevelDebug, "update_table_friend database path: %s", db_name);

        db.open(db_name);
		string sql = "select * from sqlite_master where type = 'table' and name = 'Friend';";
		CppSQLite3Query q = db.execQuery(sql.c_str());  //执行查询
		while(!q.eof())		//遍历所有行
        {
			string createSql = q.getStringField("sql");   //获取sql语句
            CCLog(kLogLevelDebug, "update_table_friend database path: %s", createSql.c_str());
			if (createSql.find("member_type integer") == string::npos)
			{
                //CCLog(kLogLevelDebug, "11111111111111111111111111: %s", createSql.c_str());
                sql = "ALTER TABLE Friend ADD member_type integer DEFAULT 0;";
                db.execDML(sql.c_str());
                sql = "ALTER TABLE Friend ADD series_name text;";
                db.execDML(sql.c_str());
                //CCLog(kLogLevelDebug, "update_table_friend database path: %s", createSql.c_str());
				//break;
			}

            if (createSql.find("user_enable integer") == string::npos)
            {
                //CCLog(kLogLevelDebug, "22222222222222222222222222222222222222: %s", createSql.c_str());
                sql = "ALTER TABLE Friend ADD user_enable integer DEFAULT 0;";
                db.execDML(sql.c_str());
            }


			//message_wrapper.set_chat_type(EChatType(q.getIntField("chatType")));
			q.nextRow();
        }

        //删除表内所有数据
        //sql = "DELETE FROM Friend;";
        //db.execDML(sql.c_str());
        q.finalize(); //结束查询，释放内存
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update table Friend failed: %s", e.errorMessage());
    }
    db.close();
}

void create_table_request() {
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
		CCLog(kLogLevelDebug, "database path: %s", db_name);
        db.open(db_name);
        string sql = "DROP TABLE IF EXISTS Request;\
            CREATE TABLE Request\
            (username text NOT NULL,\
            nickname text,\
            requestId text NOT NULL UNIQUE,\
            modifyTime timestamp DEFAULT (datetime('now','localtime')),\
            status integer DEFAULT 0,\
            remark text,\
            unreadFlag bool DEFAULT 0,\
            avatarThumbnailLink text,\
            avatarOriginalLink text,\
			request_time integer);";
        db.execDML(sql.c_str());       
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "create table Request failed: %s", e.errorMessage());
    }
    db.close();
}

void create_table_chat() {
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
		
        db.open(db_name);
        string sql = "CREATE TABLE IF NOT EXISTS Chat\
            (msgId text primary key,\
            remoteUsername text NOT NULL,\
            remoteNickname text,\
            fromUsername text,\
            fromNickname text,\
            toUsername text,\
            toNickname text,\
            msgBody text,\
            msgContent text,\
            createDate timestamp DEFAULT (strftime('%Y-%m-%d %H:%M:%f','now')),\
            chatType integer,\
            msgType integer,\
            status integer DEFAULT 1,\
            isSender bool,\
            isMsgRead bool DEFAULT 0,\
            isAudioRead bool DEFAULT 0,\
            duration integer DEFAULT 0,\
            time double ,\
            localPath text,\
            guid double NOT NULL,\
            extra text);";
        db.execDML(sql.c_str());       
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "create table Chat failed: %s", e.errorMessage());
    }
    db.close();
}


/*
	降序，取每个用户第一条消息，取不到，返回-1，否则返回对应的消息类型
*/
int  get_last_msg_by_username(char* remoteUsername,MessageWrapper& message_wrapper) 
{
    CppSQLite3DB db;
	int ret = -1;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);

        string sql = "SELECT * FROM Chat WHERE remoteUsername = '";
        sql += remoteUsername;
        sql += "' ORDER BY time DESC ";
        CppSQLite3Query q = db.execQuery(sql.c_str()); 
        while(!q.eof())		
        {
            for(int i = 0; i<=q.numFields()-1; i++)
            {
                if(0 == strcmp(q.fieldName(i), "msgId")) {
                    message_wrapper.set_msg_id(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "remoteUsername")) {
                    message_wrapper.set_conversation_name(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "remoteNickname")) {
                } else if(0 == strcmp(q.fieldName(i), "fromUsername")) {
                    message_wrapper.set_from(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "fromNickname")) {
                    message_wrapper.set_nick_from(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "toUsername")) {
                    message_wrapper.set_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "toNickname")) {
                    message_wrapper.set_nick_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgBody")) {
                    message_wrapper.set_msg_body_info_base64(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgContent")) {
                    message_wrapper.set_content(base64_decode(q.getStringField(i)));
                } else if(0 == strcmp(q.fieldName(i), "chatType")) {
                    message_wrapper.set_chat_type(EChatType(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "isMsgRead")) {
                    message_wrapper.set_is_msg_read(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgType")) {
                    message_wrapper.set_msg_type(EMessageBodyType(q.getIntField(i)));
					ret = q.getIntField(i);
                } else if(0 == strcmp(q.fieldName(i), "status")) {
                    message_wrapper.set_status(EMessageStatus(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "isAudioRead")) {
                    message_wrapper.set_is_audio_read(q.getIntField(i));
				} else if(0 == strcmp(q.fieldName(i), "duration")) {
                    message_wrapper.set_duration(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "isSender")) {
                    message_wrapper.set_is_sender(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "time")) {
                    message_wrapper.set_msg_sent_time(q.getInt64Field(i));
                } else if(0 == strcmp(q.fieldName(i), "guid")) {
                    message_wrapper.set_guid(q.getInt64Field(i));
                } else if(0 == strcmp(q.fieldName(i), "localPath")) {
                    message_wrapper.set_local_path(q.getStringField(i));
                } else {
                
                }
            }
			break;
        }
        q.finalize();
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "get_last_msg_by_username error: %s", e.errorMessage());
    }
    db.close();
	return ret;
   
}

void update_table(string tableName,string colum_name, string type,string op_string)
{
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "SELECT * FROM " + tableName;
        CppSQLite3Query q = db.execQuery(sql.c_str()); 
		int insert_flag = 0;
        while(!q.eof())		//遍历所有行
        {
            for(int i = 0; i<=q.numFields()-1; i++)
            {	
                if(0 == strcmp(q.fieldName(i), colum_name.c_str())) 
				{
					insert_flag = 1;
                } 
            }
            q.nextRow();
        }
        q.finalize();//结束查询,释放内存
        if (!insert_flag)
        { 
	        string op = "alter table "  + tableName + " " +"add" + colum_name + " " + type + " " + op_string;
	        db.execDML(op.c_str());
        }
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "get chat message list error: %s", e.errorMessage());
    }
    db.close();
}

int update_friends_list(std::list<FriendInfo> friends_list) {
    
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        list<FriendInfo>::iterator it;
        for(it = friends_list.begin(); it != friends_list.end(); it++)
        {
            string sql_str = get_add_friend_sql(it->get_user_name(),
                it->get_nick_name(),
                it->get_nick_name_pinyin(),
                it->get_nick_name_pinyin_blank(),
                it->get_remark_name(),
	            it->get_remark_name_pinyin(),
	            it->get_remark_name_pinyin_blank(),
                it->get_phone(),
                it->get_is_enable(),
                it->get_silent(),
                it->get_avatar_thumbnail_link(),
                it->get_avatar_original_link(),
                it->get_member_type(),
                it->get_series_name(),
                it->get_user_enable()
                );
            //CCLog(kLogLevelDebug, "sql_str:%s", sql_str.c_str());
            const char* execute_sql = sql_str.c_str();
            db.execDML(execute_sql);
        }
        /*for (int i=0; i<friends_list.size(); i++) {
            const char* sql = get_add_friend_sql(friends_list.get(i).get_user_name(),
                                                 friends_list.get(i).get_nick_name(),
                                                 friends_list.get(i).get_remark_name(),
                                                 friends_list.get(i).get_avatar_thumbnail_link(),
                                                 friends_list.get(i).get_avatar_original_link()).c_str();
            db.execDML(sql);
        }*/
        
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update friends list error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}


void update_friend(FriendInfo friend_info) {
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
		
        string sql = "SELECT * FROM Friend WHERE username = '";
        sql += friend_info.get_user_name();
        sql += "';";
        CppSQLite3Query q = db.execQuery(sql.c_str());
        if(q.eof()) {
            string str = get_add_friend_sql(friend_info.get_user_name(),
                friend_info.get_nick_name(),
                friend_info.get_nick_name_pinyin(),
                friend_info.get_nick_name_pinyin_blank(),
                friend_info.get_remark_name(),
                friend_info.get_remark_name_pinyin(),
                friend_info.get_remark_name_pinyin_blank(),
                friend_info.get_phone(),
                friend_info.get_is_enable(),
                friend_info.get_silent(),
                friend_info.get_avatar_thumbnail_link(),
                friend_info.get_avatar_original_link(),
                friend_info.get_member_type(),
                friend_info.get_series_name(),
                friend_info.get_user_enable()
                );
            const char* execute_sql = str.c_str();
            db.execDML(execute_sql);
        } else {
			string sql = "UPDATE Friend SET nickname = '" +  friend_info.get_nick_name() + "', ";
			sql += "silent = " + std::to_string(friend_info.get_silent())+ ",";
			sql += "nickname_pinyin = '" + friend_info.get_nick_name_pinyin()+ "',";
			sql += "nickname_pinyin_blank = '" + friend_info.get_nick_name_pinyin_blank()+ "',";
			sql += "remarkName = '" + friend_info.get_remark_name()+ "',";
			sql += "remarkName_pinyin = '" + friend_info.get_remark_name_pinyin()+ "',";
			sql += "remarkName_pinyin_blank = '" + friend_info.get_remark_name_pinyin_blank()+ "',";
			sql += "enable = '" + std::to_string(friend_info.get_is_enable())+"',";
            sql += "member_type = '" + std::to_string(friend_info.get_member_type())+"',";
            sql += "series_name = '" + friend_info.get_series_name()+"',";
            sql += "user_enable = '" + std::to_string(friend_info.get_user_enable())+"'";
			sql += " WHERE username = '";
        	sql += friend_info.get_user_name();
        	sql += "';";
        	db.execDML(sql.c_str());
            CCLog(kLogLevelWarn, "update friends, username exit: %s", friend_info.get_user_name().c_str());
        }
        q.finalize();
        
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update friends error: %s", e.errorMessage());
    }
    db.close();
}
int get_friend_relation(const string &to_name)
{
    int enable = 0;
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "SELECT enable FROM Friend WHERE username = '";
        sql += to_name;
        sql += "';";
        CppSQLite3Query q = db.execQuery(sql.c_str()); 

        for(int i = 0; i<=q.numFields()-1; i++)
        {
			if(0 == strcmp(q.fieldName(i), "enable")) 
			{
				enable = q.getIntField(i);
			} 
        }
        q.finalize();//结束查询,释放内存
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "get_friend_relation error: %s", e.errorMessage());
    }
    db.close();
    
    return enable;

}

list<FriendInfo> get_friends_list(int page, int count, int* total) {

    list<FriendInfo> friend_list;
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
		string sql_cn =  "SELECT * FROM Friend";
		int cn = -1;
		db.execDMLGetPara(sql_cn.c_str(),&cn);
		*total = cn;
		
        int offset = count * (page - 1);
        string sql = "SELECT * FROM Friend LIMIT ";
        sql += std::to_string(count) + " OFFSET ";
        sql += std::to_string(offset) + ";";
        CppSQLite3Query q = db.execQuery(sql.c_str());  //执行查询

        while(!q.eof())		//遍历所有行
        {
            FriendInfo friend_info;
            for(int i = 0; i<=q.numFields()-1; i++)
            {
                if(0 == strcmp(q.fieldName(i), "username")) {
                    friend_info.set_user_name(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "nickname")) {
                    friend_info.set_nick_name(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "remarkName")) {
                    friend_info.set_remark_name(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "avatarThumbnailLink")) {
                    friend_info.set_avatar_thumbnail_link(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "avatarOriginalLink")) {
                    friend_info.set_avatar_original_link(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "phone")) {
                    friend_info.set_phone(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "enable")) {
                    friend_info.set_is_enable(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "silent")) {
                    friend_info.set_silent(q.getIntField(i));
                } else if (0 == strcmp(q.fieldName(i), "nickname_pinyin")){
            		friend_info.set_nick_name_pinyin(q.getStringField(i));				
				}else if (0 == strcmp(q.fieldName(i), "nickname_pinyin_blank")){
                    friend_info.set_nick_name_pinyin_blank(q.getStringField(i));	
            	}else if (0 == strcmp(q.fieldName(i), "remarkName_pinyin")){
    				friend_info.set_remark_name_pinyin(q.getStringField(i));
                }else if (0 == strcmp(q.fieldName(i), "remarkName_pinyin_blank")){
                    friend_info.set_remark_name_pinyin_blank(q.getStringField(i));
    			}else if (0 == strcmp(q.fieldName(i), "member_type")){
                    friend_info.set_member_type(q.getIntField(i));
                }else if (0 == strcmp(q.fieldName(i), "series_name")){
                    friend_info.set_series_name(q.getStringField(i));
                }
                else if (0 == strcmp(q.fieldName(i), "user_enable")){
                    friend_info.set_user_enable(q.getIntField(i));
                }else{
                }
            }
            q.nextRow();
            friend_list.push_back(friend_info);
        }
        //*total = friend_list.size();
        q.finalize();//结束查询,释放内存
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "get friends list error: %s", e.errorMessage());
    }
    db.close();
    
	return friend_list;
}

int add_friend(FriendInfo friend_info) {
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string str = get_add_friend_sql(friend_info.get_user_name(),
            friend_info.get_nick_name(),
            friend_info.get_nick_name_pinyin(),
            friend_info.get_nick_name_pinyin_blank(),
            friend_info.get_remark_name(),
            friend_info.get_remark_name_pinyin(),
            friend_info.get_remark_name_pinyin_blank(),
            friend_info.get_phone(),
            friend_info.get_is_enable(),
            friend_info.get_silent(),
            friend_info.get_avatar_thumbnail_link(),
            friend_info.get_avatar_original_link(),
            friend_info.get_member_type(),
            friend_info.get_series_name(),
            friend_info.get_user_enable()
            );
		const char* execute_sql = str.c_str();
        db.execDML(execute_sql);
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "add friends error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}


int delete_friend(char* friend_name) {
    
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "DELETE FROM Friend WHERE username = '";
        sql += friend_name;
        sql += "';";
        db.execDML(sql.c_str());
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "delete friends error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;

}

FriendInfo get_friend_info(char* friend_name) {

    FriendInfo friend_info;
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "SELECT * FROM Friend WHERE username = '";
        sql += friend_name;
        sql += "';";
        CppSQLite3Query q = db.execQuery(sql.c_str());  //execute query

       for(int i = 0; i<=q.numFields()-1; i++)
        {
            if(0 == strcmp(q.fieldName(i), "username")) {
                friend_info.set_user_name(q.getStringField(i));
            } else if(0 == strcmp(q.fieldName(i), "nickname")) {
                friend_info.set_nick_name(q.getStringField(i));
            } else if(0 == strcmp(q.fieldName(i), "remarkName")) {
                friend_info.set_remark_name(q.getStringField(i));
            } else if(0 == strcmp(q.fieldName(i), "avatarThumbnailLink")) {
                friend_info.set_avatar_thumbnail_link(q.getStringField(i));
            } else if(0 == strcmp(q.fieldName(i), "avatarOriginalLink")) {
                friend_info.set_avatar_original_link(q.getStringField(i));
            } else if(0 == strcmp(q.fieldName(i), "phone")) {
                friend_info.set_phone(q.getStringField(i));
            } else if(0 == strcmp(q.fieldName(i), "enable")) {
                friend_info.set_is_enable(q.getIntField(i));
            } else if(0 == strcmp(q.fieldName(i), "silent")) {
                friend_info.set_silent(q.getIntField(i));
            } else if (0 == strcmp(q.fieldName(i), "nickname_pinyin")){
        		friend_info.set_nick_name_pinyin(q.getStringField(i));
			}else if (0 == strcmp(q.fieldName(i), "nickname_pinyin_blank")){
                friend_info.set_nick_name_pinyin_blank(q.getStringField(i));	
        	}else if (0 == strcmp(q.fieldName(i), "remarkName_pinyin")){
				friend_info.set_remark_name_pinyin(q.getStringField(i));
            }else if (0 == strcmp(q.fieldName(i), "remarkName_pinyin_blank")){
                friend_info.set_remark_name_pinyin_blank(q.getStringField(i));
            }else if (0 == strcmp(q.fieldName(i), "member_type")){
                friend_info.set_member_type(q.getIntField(i));
            }else if (0 == strcmp(q.fieldName(i), "series_name")){
                friend_info.set_series_name(q.getStringField(i));
			}
            else if (0 == strcmp(q.fieldName(i), "user_enable")){
                friend_info.set_user_enable(q.getIntField(i));
            }else{
            
            }

        }
        q.finalize();//结束查询,释放内存
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "get friends info error: %s", e.errorMessage());
    }
    db.close();
    
    return friend_info;

}

int update_friend_silent(char* username, int silent_type){
	CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "UPDATE Friend SET silent = " + std::to_string(silent_type) + " WHERE username = '";
        sql += username;
        sql += "';";
        db.execDML(sql.c_str());
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update friend silent error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

int update_friend_request_read(char* requestId, bool status){
	CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "UPDATE Request SET status = " + std::to_string(status) + " WHERE requestId = '";
        sql += requestId;
        sql += "';";
        db.execDML(sql.c_str());
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update request is_read error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

int update_friend_enable(char* username, int is_enable){
	CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "UPDATE Friend SET enable = " + std::to_string(is_enable) + " WHERE username = '";
        sql += username;
        sql += "';";
        db.execDML(sql.c_str());
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update friend enable error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

int update_friend_Link(char* username, string thumblink, string orignallink){
	CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "UPDATE Friend SET avatarThumbnailLink = '" + thumblink + "', avatarOriginalLink = '" + orignallink + "' WHERE username = '";
        sql += username;
        sql += "';";
        db.execDML(sql.c_str());
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update friend link error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

int update_friend_nick(char* username, string nickname, string nickname_pinyin){
	CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "UPDATE Friend SET nickname = '" + nickname + "', nickname_pinyin = '" + nickname_pinyin + "' WHERE username = '";
        sql += username;
        sql += "';";
        db.execDML(sql.c_str());
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update friend nick error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}


int update_friend_request_list(std::list<RequestWrapper> request_list) {
    
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        list<RequestWrapper>::iterator it;
        for(it = request_list.begin(); it != request_list.end(); it++) {
            string str = get_add_friend_request_sql(it->get_user_name(),
                it->get_nick_name(),
                it->get_request_id(),
                it->get_remark(),
                it->get_avatar_thumbnail_link(),
                it->get_avatar_original_link(),
                it->get_req_status(),
                it->get_request_time());
            const char* sql = str.c_str();
            db.execDML(sql);
        }
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update friend request list error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

void update_friend_request(RequestWrapper request_info) {
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "SELECT * FROM request WHERE requestId = '";
        sql += request_info.get_request_id();
        sql += "';";
        CppSQLite3Query q = db.execQuery(sql.c_str());
        if(q.eof()) {
            string str = get_add_friend_request_sql(request_info.get_user_name(),
                request_info.get_nick_name(),
                request_info.get_request_id(),
                request_info.get_remark(),
                request_info.get_avatar_thumbnail_link(),
                request_info.get_avatar_original_link(),
                request_info.get_req_status(),
                request_info.get_request_time());
            const char* insert_sql = str.c_str();
			
            db.execDML(insert_sql);
        } else {
			
			string sql = "UPDATE Request SET status = " +  std::to_string(request_info.get_req_status()) + ", ";
			sql += "request_time = " +std::to_string(request_info.get_request_time());
			sql += " WHERE requestId = '";
        	sql +=  request_info.get_request_id();
        	sql += "';";

			
        	db.execDML(sql.c_str());
            CCLog(kLogLevelInfo, "update request, requst exit: %s, do update operation", request_info.get_request_id().c_str());
        }
        q.finalize();
        
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update request error: %s", e.errorMessage());
    }
    db.close();
}

int set_same_friend_request_status(string requestId, int status) {
    CppSQLite3DB db;
    try
    {
        string username;
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "SELECT * FROM request WHERE requestId = '";
        sql += requestId;
        sql += "';";
        CppSQLite3Query q = db.execQuery(sql.c_str());
        if(q.eof()) {
            CCLog(kLogLevelWarn, "request id is not exist, not resonable: %s", requestId.c_str());
			db.close();
        } else {
            for(int i = 0; i<=q.numFields()-1; i++)
            {
                if(0 == strcmp(q.fieldName(i), "username")) {
                    username = q.getStringField(i);
					std::cout<<username;
                }
            }
            q.finalize();
			string str = "UPDATE Request SET status = ";
		    str += std::to_string(status);
		    str += " WHERE username = '";
		    str += username;
		    str += "'";
			str += "and (status=0 or status=3);";
		    const char* update_sql = str.c_str();
		    db.execDML(update_sql);
			db.close();
        }
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update same friend request error: %s", e.errorMessage());
    }
}


list<RequestWrapper> get_add_friend_request_list () {
    
    list<RequestWrapper> request_list;
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        //好友申请状态说明 0.未读  1.同意   2.拒绝   3.已读
        CppSQLite3Query q = db.execQuery("SELECT * FROM Request WHERE status = 0 or status = 3");
        while(!q.eof())
        {
            RequestWrapper request_wrapper;
            for(int i = 0; i<=q.numFields()-1; i++)
            {
                if(0 == strcmp(q.fieldName(i), "username")) {
                    request_wrapper.set_user_name(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "nickname")) {
                    request_wrapper.set_nick_name(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "remark")) {
                    request_wrapper.set_remark(q.getStringField(i));        //TODO:lach remark name
                } else if(0 == strcmp(q.fieldName(i), "avatarThumbnailLink")) {
                    request_wrapper.set_avatar_thumbnail_link(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "avatarOriginalLink")) {
                    request_wrapper.set_avatar_original_link(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "status")) {
                    request_wrapper.set_req_status(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "requestId")){
                    request_wrapper.set_request_id(q.getStringField(i));
                }else if(0 == strcmp(q.fieldName(i), "request_time")){
					request_wrapper.set_request_time(q.getInt64Field(i));	 
				
				}else {

                }
            }
            q.nextRow();
            request_list.push_back(request_wrapper);
        }
        q.finalize();
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "get add friend request list error: %s", e.errorMessage());
    }
   
    db.close();
    
    return request_list;
}

int delete_friend_request(char* request_id) {
    
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "DELETE FROM Request WHERE requestId = '";
        sql += request_id;
        sql += "';";
        db.execDML(sql.c_str());
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "delete friend request error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

int update_friend_request_status(char* request_id, int status){
	CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "UPDATE Request SET status = " + std::to_string(status) + " WHERE requestId = '";
        sql += request_id;
        sql += "';";
        db.execDML(sql.c_str());
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update friend request status error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

std::list<MessageWrapper> get_chat_message_cloud_unread_list(int count, char* remote_user_name)
{
    list<MessageWrapper> message_list;
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        
        string sql = "SELECT * FROM Chat WHERE remoteUsername = '";
        sql += remote_user_name;
        sql += "' AND isMsgRead = 0 ORDER BY time DESC LIMIT ";
        sql += std::to_string(count) + ";";
        CppSQLite3Query q = db.execQuery(sql.c_str()); 
        
        while(!q.eof())     //遍历所有行
        {
            MessageWrapper message_wrapper;
            for(int i = 0; i<=q.numFields()-1; i++)
            {
                if(0 == strcmp(q.fieldName(i), "msgId")) {
                    message_wrapper.set_msg_id(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "remoteUsername")) {
                    message_wrapper.set_conversation_name(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "remoteNickname")) {
                    //message_wrapper.set_nick_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "fromUsername")) {
                    message_wrapper.set_from(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "fromNickname")) {
                    message_wrapper.set_nick_from(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "toUsername")) {
                    message_wrapper.set_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "toNickname")) {
                    message_wrapper.set_nick_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgBody")) {
                    message_wrapper.set_msg_body_info_base64(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgContent")) {
                    message_wrapper.set_content(base64_decode(q.getStringField(i)));
                } else if(0 == strcmp(q.fieldName(i), "chatType")) {
                    message_wrapper.set_chat_type(EChatType(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "isMsgRead")) {
                    message_wrapper.set_is_msg_read(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgType")) {
                    message_wrapper.set_msg_type(EMessageBodyType(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "status")) {
                    message_wrapper.set_status(EMessageStatus(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "isAudioRead")) {
                    message_wrapper.set_is_audio_read(q.getIntField(i));
				} else if(0 == strcmp(q.fieldName(i), "duration")) {
                    message_wrapper.set_duration(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "isSender")) {
                    message_wrapper.set_is_sender(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "time")) {
                    message_wrapper.set_msg_sent_time(q.getInt64Field(i));
                } else if(0 == strcmp(q.fieldName(i), "guid")) {
                    message_wrapper.set_guid(q.getInt64Field(i));
                } else if(0 == strcmp(q.fieldName(i), "localPath")) {
                    message_wrapper.set_local_path(q.getStringField(i));
                } else {
                
                }
                
            }
            q.nextRow();
            message_list.push_back(message_wrapper);
        }
        //*total = message_list.size();
        q.finalize();//结束查询,释放内存
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "get chat message list error: %s", e.errorMessage());
    }
    db.close();
    
    return message_list;
}


std::list<MessageWrapper> get_chat_message_local_unread_list(int count, char* remote_user_name)
{
    list<MessageWrapper> message_list;
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        
        string sql = "SELECT * FROM Chat WHERE remoteUsername = '";
        sql += remote_user_name;
        sql += "' AND isAudioRead = 0 ORDER BY time DESC LIMIT ";
        sql += std::to_string(count) + ";";
        CppSQLite3Query q = db.execQuery(sql.c_str()); 
        
        while(!q.eof())     //遍历所有行
        {
            MessageWrapper message_wrapper;
            for(int i = 0; i<=q.numFields()-1; i++)
            {
                if(0 == strcmp(q.fieldName(i), "msgId")) {
                    message_wrapper.set_msg_id(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "remoteUsername")) {
                    message_wrapper.set_conversation_name(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "remoteNickname")) {
                    //message_wrapper.set_nick_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "fromUsername")) {
                    message_wrapper.set_from(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "fromNickname")) {
                    message_wrapper.set_nick_from(q.getStringField(i));        
                } else if(0 == strcmp(q.fieldName(i), "toUsername")) {
                    message_wrapper.set_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "toNickname")) {
                    message_wrapper.set_nick_to(q.getStringField(i));                      
                } else if(0 == strcmp(q.fieldName(i), "msgBody")) {
                    message_wrapper.set_msg_body_info_base64(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgContent")) {
                    message_wrapper.set_content(base64_decode(q.getStringField(i)));
                } else if(0 == strcmp(q.fieldName(i), "chatType")) {
                    message_wrapper.set_chat_type(EChatType(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "isMsgRead")) {
                    message_wrapper.set_is_msg_read(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgType")) {
                    message_wrapper.set_msg_type(EMessageBodyType(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "status")) {
                    message_wrapper.set_status(EMessageStatus(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "isAudioRead")) {
                    message_wrapper.set_is_audio_read(q.getIntField(i));
				} else if(0 == strcmp(q.fieldName(i), "duration")) {
                    message_wrapper.set_duration(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "isSender")) {
                    message_wrapper.set_is_sender(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "time")) {
                    message_wrapper.set_msg_sent_time(q.getInt64Field(i));
                } else if(0 == strcmp(q.fieldName(i), "guid")) {
                    message_wrapper.set_guid(q.getInt64Field(i));
                } else if(0 == strcmp(q.fieldName(i), "localPath")) {
                    message_wrapper.set_local_path(q.getStringField(i));
                } else {
                
                }
                
            }
            q.nextRow();
            message_list.push_back(message_wrapper);
        }
        //*total = message_list.size();
        q.finalize();//结束查询,释放内存
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "get chat message list error: %s", e.errorMessage());
    }
    db.close();
    
    return message_list;
}

std::list<MessageWrapper> get_chat_message_list(int count, int page, int* total, char* remote_user_name) 
{
	CCLog(kLogLevelInfo, "count:%d page:%d total:%d remote_user_name=:%s ", count, page, *total, remote_user_name);
    list<MessageWrapper> message_list;
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);

		string sql_cn =  "SELECT count(1)  FROM Chat WHERE remoteUsername = '";
        sql_cn += remote_user_name;
        //sql_cn += "'AND status = 0 or status = 1 or status = 2 or status = 4";
        sql_cn += "'AND status IN(0,1,2,4)";

		int cn = -1;
		db.execDMLGetPara(sql_cn.c_str(),&cn);
		*total = cn;
		
        int offset = count * (page - 1);
        string sql = "SELECT * FROM Chat WHERE remoteUsername = '";
        sql += remote_user_name;
        //sql += "' AND status = 0 or status = 1 or status = 2 or status = 4 ORDER BY createDate DESC LIMIT ";
        sql += "' AND status IN(0,1,2,4) ORDER BY createDate DESC LIMIT ";
        sql += std::to_string(count) + " OFFSET ";
        sql += std::to_string(offset) + ";";
        CppSQLite3Query q = db.execQuery(sql.c_str()); 

		
        while(!q.eof())		//遍历所有行
        {

		//	CCLog(kLogLevelError, "!q.eof()");
            MessageWrapper message_wrapper;
            for(int i = 0; i<=q.numFields()-1; i++)
            {
				//CCLog(kLogLevelError, "get chat message sqlite");
                if(0 == strcmp(q.fieldName(i), "msgId")) {
					//CCLog(kLogLevelDebug, "sqlite_msgId=:%s",q.getStringField(i));
                    message_wrapper.set_msg_id(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "remoteUsername")) {
                    message_wrapper.set_conversation_name(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "remoteNickname")) {
                    message_wrapper.set_nick_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "fromUsername")) {
                    message_wrapper.set_from(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "fromNickname")) {
                    message_wrapper.set_nick_from(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "toUsername")) {
                    message_wrapper.set_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "toNickname")) {
                    message_wrapper.set_nick_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgBody")) {
                    message_wrapper.set_msg_body_info_base64(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgContent")) {
					//CCLog(kLogLevelDebug, "sqlite_content=:%s",q.getStringField(i));
                    message_wrapper.set_content(base64_decode(q.getStringField(i)));
					
                } else if(0 == strcmp(q.fieldName(i), "chatType")) {
                    message_wrapper.set_chat_type(EChatType(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "isMsgRead")) {
                    message_wrapper.set_is_msg_read(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgType")) {
                    message_wrapper.set_msg_type(EMessageBodyType(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "status")) {
                    message_wrapper.set_status(EMessageStatus(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "isAudioRead")) {
                    message_wrapper.set_is_audio_read(q.getIntField(i));
				} else if(0 == strcmp(q.fieldName(i), "duration")) {
                    message_wrapper.set_duration(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "isSender")) {
                    message_wrapper.set_is_sender(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "time")) {
                    message_wrapper.set_msg_sent_time(q.getInt64Field(i));
                } else if(0 == strcmp(q.fieldName(i), "guid")) {
                    message_wrapper.set_guid(q.getInt64Field(i));
                } else if(0 == strcmp(q.fieldName(i), "localPath")) {
                    message_wrapper.set_local_path(q.getStringField(i));
                } else {
                }
            }
            q.nextRow();
            message_list.push_back(message_wrapper);
        }
        //*total = message_list.size();
        q.finalize();//结束查询,释放内存
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "get chat message list error: %s", e.errorMessage());
    }
    db.close();
	CCLog(kLogLevelDebug, "get chat message list end");
    
	return message_list;
}

std::shared_ptr<MessageWrapper> get_chat_message_by_guid(uint64_t guid) {
	MessageWrapperPtr message = shared_ptr<MessageWrapper>(new MessageWrapper());
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "SELECT * FROM Chat WHERE guid = '";
        sql +=std::to_string(guid);
        sql += "';";

        CppSQLite3Query q = db.execQuery(sql.c_str());  //执行查询

        while(!q.eof())		//遍历所有行
        {
            for(int i = 0; i<=q.numFields()-1; i++)
            {
                if(0 == strcmp(q.fieldName(i), "msgId")) {
                    message->set_msg_id(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "remoteUsername")) {
                    message->set_conversation_name(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "remoteNickname")) {
                    //message->set_nick_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "fromUsername")) {
                    message->set_from(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "fromNickname")) {
                    message->set_nick_from(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "toUsername")) {
                    message->set_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "toNickname")) {
                    message->set_nick_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgBody")) {
                    message->set_msg_body_info_base64(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgContent")) {
                    message->set_content(base64_decode(q.getStringField(i)));
                } else if(0 == strcmp(q.fieldName(i), "chatType")) {
                    message->set_chat_type(EChatType(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "isMsgRead")) {
                    message->set_is_msg_read(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgType")) {
                    message->set_msg_type(EMessageBodyType(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "status")) {
                    message->set_status(EMessageStatus(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "isAudioRead")) {
                    message->set_is_audio_read(q.getIntField(i));
				} else if(0 == strcmp(q.fieldName(i), "duration")) {
                     message->set_duration(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "isSender")) {
                    message->set_is_sender(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "time")) {
                    message->set_msg_sent_time(q.getInt64Field(i));
                } else if(0 == strcmp(q.fieldName(i), "guid")) {
                    message->set_guid(q.getInt64Field(i));
                } else if(0 == strcmp(q.fieldName(i), "localPath")) {
                    message->set_local_path(q.getStringField(i));
                } else {
                
                }
                
            }

			q.nextRow();
        }
        q.finalize();//结束查询,释放内存
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "get chat message wrapper error: %s", e.errorMessage());
    }
    db.close();
    
	return message;
}


std::shared_ptr<MessageWrapper> get_chat_message_by_id(string msg_id) {
	MessageWrapperPtr message = shared_ptr<MessageWrapper>(new MessageWrapper());
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "SELECT * FROM Chat WHERE msgId = '";
        sql += msg_id;
        sql += "';";

        CppSQLite3Query q = db.execQuery(sql.c_str());  //执行查询

        while(!q.eof())		//遍历所有行
        {
            for(int i = 0; i<=q.numFields()-1; i++)
            {
                if(0 == strcmp(q.fieldName(i), "msgId")) {
                    message->set_msg_id(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "remoteUsername")) {
                    message->set_conversation_name(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "remoteNickname")) {
                    //message->set_nick_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "fromUsername")) {
                    message->set_from(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "fromNickname")) {
                    message->set_nick_from(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "toUsername")) {
                    message->set_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "toNickname")) {
                    message->set_nick_to(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgBody")) {
                    message->set_msg_body_info_base64(q.getStringField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgContent")) {
                    message->set_content(base64_decode(q.getStringField(i)));
                } else if(0 == strcmp(q.fieldName(i), "chatType")) {
                    message->set_chat_type(EChatType(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "isMsgRead")) {
                    message->set_is_msg_read(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "msgType")) {
                    message->set_msg_type(EMessageBodyType(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "status")) {
                    message->set_status(EMessageStatus(q.getIntField(i)));
                } else if(0 == strcmp(q.fieldName(i), "isAudioRead")) {
                    message->set_is_audio_read(q.getIntField(i));
				} else if(0 == strcmp(q.fieldName(i), "duration")) {
                     message->set_duration(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "isSender")) {
                    message->set_is_sender(q.getIntField(i));
                } else if(0 == strcmp(q.fieldName(i), "time")) {
                    message->set_msg_sent_time(q.getInt64Field(i));
                } else if(0 == strcmp(q.fieldName(i), "guid")) {
                    message->set_guid(q.getInt64Field(i));
                } else if(0 == strcmp(q.fieldName(i), "localPath")) {
                    message->set_local_path(q.getStringField(i));
                } else {
                }
            }
			q.nextRow();
        }
        q.finalize();//结束查询,释放内存
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "get chat message wrapper error: %s", e.errorMessage());
    }
    db.close();
    
	return message;
}

MessageWrapper get_chat_message_wrapper(char* msg_id){
	MessageWrapper message_wrapper;
    CppSQLite3DB db;   //创建数据库
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);//打开数据库
        string sql = "SELECT * FROM Chat WHERE msgId = '";
        sql += msg_id;
        sql += "' AND isSender = 0 ORDER BY createDate DESC LIMIT 1;";
        CppSQLite3Query q = db.execQuery(sql.c_str());  //执行查询

        while(!q.eof())		//遍历所有行
        {
			message_wrapper.set_msg_id(q.getStringField("msgId"));
			message_wrapper.set_to(q.getStringField("toUsername"));
			message_wrapper.set_nick_to(q.getStringField("msgBody"));
			message_wrapper.set_content(base64_decode(q.getStringField("msgContent")));
			message_wrapper.set_chat_type(EChatType(q.getIntField("chatType")));
			message_wrapper.set_is_msg_read(q.getIntField("isMsgRead"));
			message_wrapper.set_msg_type(EMessageBodyType(q.getIntField("msgType")));
			message_wrapper.set_guid(q.getInt64Field( "guid"));
			q.nextRow();
        }
        q.finalize();//结束查询，释放内存
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "get chat message wrapper error: %s", e.errorMessage());
    }
    db.close();//关闭数据库
    
	return message_wrapper;
}

int update_chat_message_list(std::list<MessageWrapper> message_list) {
	CCLog(kLogLevelDebug, "update_chat_message_list begin!!!");
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        list<MessageWrapper>::iterator it;
        for(it = message_list.begin(); it != message_list.end(); it++) {
            string str = get_add_message_sql(it->get_msg_id(),
                it->get_conversation_name(),
                "",
                it->get_from(),
                it->get_nick_from(),
                it->get_to(),
                it->get_nick_to(),
                it->get_msg_body_info_base64(),
              base64_encode( reinterpret_cast<const unsigned char *>(it->get_content().c_str()),
                                                         strlen(it->get_content().c_str())),
                it->get_chat_type(),
                it->get_msg_type(),
                it->get_status(),
                it->is_sender(),
                it->is_msg_read(),
                it->is_audio_read(),
                it->get_duration(),
                it->get_msg_sent_time(),
                it->get_guid(),
                it->get_local_path());
            const char* execute_sql = str.c_str();
            db.execDML(execute_sql);
        }
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update chat message list error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    CCLog(kLogLevelDebug, "update_chat_message_list end!!!");
    return 0;
}

int set_chat_voice_message_status(string msg_id) {
    
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "UPDATE Chat SET isAudioRead = 1 WHERE msgId = '";
        sql += msg_id;
        sql += "';";
        db.execDML(sql.c_str());
        
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update chat message voice status error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

int set_chat_message_read_status(string username) {
    
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "UPDATE Chat SET isMsgRead = 1 WHERE remoteUsername = '";
        sql += username;
        sql += "';";
        db.execDML(sql.c_str());
        
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update chat message read status error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

int set_chat_message_read_status_by_time(string username, uint64_t time) {
    
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "UPDATE Chat SET isMsgRead = 1 WHERE remoteUsername = '";
        sql += username;
        sql += "' AND time <= '";
        sql +=std::to_string(time);
        sql += "';";

        db.execDML(sql.c_str());
        
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update chat message read status error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}


int set_chat_message_read_status_by_id(string msg_id) {
    
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "UPDATE Chat SET isMsgRead = 1 WHERE msgId = '";
        sql += msg_id;
        sql += "';";
        db.execDML(sql.c_str());
		CCLog(kLogLevelDebug, "Locally read");
        
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update chat message read status error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

int set_chat_message_read_status_by_guid(uint64_t guid) {
        
        CppSQLite3DB db;
        try
        {
            string path = get_database_path();
            const char* db_name = path.c_str();
            db.open(db_name);
            string sql = "UPDATE Chat SET isMsgRead = 1 WHERE guid = '";
            sql +=std::to_string(guid);
            sql += "';";
            db.execDML(sql.c_str());
            
        }
        catch(CppSQLite3Exception& e)
        {
            CCLog(kLogLevelError, "update chat message read status error: %s", e.errorMessage());
            db.close();
            return -1;
        }
        db.close();
        
        return 0;
    }


int delete_all_chat_message() {
    
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "DELETE FROM Chat;";
        db.execDML(sql.c_str());
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "delete chat message error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}
int delete_single_chat_message(string    remoteUsername)
{
    CCLog(kLogLevelInfo, "remoteUsername:%s", remoteUsername.c_str());
    CppSQLite3DB db;
    try
    {
	    string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "DELETE FROM Chat WHERE remoteUsername='" ;
			sql += remoteUsername;
			sql +="';";
        db.execDML(sql.c_str());
        
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update chat message guid error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
	return 0;
}

int update_chat_message_guid(string msg_id, string guid) {
    
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "UPDATE Chat SET guid = '";
        sql += guid;
        sql += "' WHERE msgId = '";
        sql += msg_id;
        sql += "';";
        db.execDML(sql.c_str());
        
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update chat message guid error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

int update_chat_message_status(uint64_t guid, int status) {
    
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "UPDATE Chat SET status = '";
        sql += std::to_string(status);
        sql += "' WHERE guid = '";
        sql += std::to_string(guid);
        sql += "';";
        db.execDML(sql.c_str());
        
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update chat message guid error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

int delete_chat_message_by_date() {
    CCLog(kLogLevelInfo, "delete message");
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "DELETE From Chat where createDate <= datetime('now', 'start of day', '-1 day');";
        db.execDML(sql.c_str());
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "delete chat message error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

int delete_chat_message_by_id(string msg_id) {

    CCLog(kLogLevelInfo, "msg_id:%s", msg_id.c_str());
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "DELETE From Chat WHERE msgId = '";
        sql += msg_id;
        sql += "';";
        db.execDML(sql.c_str());
		CCLog(kLogLevelDebug, "delete message");
        
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "delete chat message error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

int update_chat_message_status_by_id(string msg_id, int status) {
    
    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "UPDATE Chat SET status = '";
        sql += std::to_string(status);
        sql += "' WHERE msgId = '";
        sql += msg_id;
        sql += "';";
        db.execDML(sql.c_str());
        
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update chat message msg_id error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}



int get_phone(const string& username,string& phone)
{
    CCLog(kLogLevelInfo, "username=:%s ",  username.c_str());
    CppSQLite3DB db;
    int counts = 0;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);

        string sql = "SELECT phone FROM friend WHERE username = '";
        sql += username;
        sql += "';";
        CppSQLite3Query q = db.execQuery(sql.c_str());  //执行查询
        while(!q.eof())		//遍历所有行
        {
            for(int i = 0; i<=q.numFields()-1; i++)
            {
                if(0 == strcmp(q.fieldName(i), "phone")) {
                    phone=q.getStringField(i);
                }
            }
            q.nextRow();
        }
        q.finalize();//结束查询,释放内存
    }
    catch (CppSQLite3Exception & e)
    {
        CCLog(kLogLevelError, "get chat message list error: %s", e.errorMessage());
    }
    db.close();

    CCLog(kLogLevelDebug, "get chat message list end counts:%d", counts);
    return counts;
}

int update_chat_message_remoteusername(const string& username,const string& phone) {

    CppSQLite3DB db;
    try
    {
        string path = get_database_path();
        const char* db_name = path.c_str();
        db.open(db_name);
        string sql = "UPDATE Chat SET remoteUsername = '";
        sql += username;
        sql += "', toUsername = '";
        sql += username;
        sql += "' WHERE remoteUsername = '";
        sql += phone;
        sql += "' AND toUsername = '";
        sql += phone;
        sql += "';";
        db.execDML(sql.c_str());
        CCLog(kLogLevelDebug, "sql:%s", sql.c_str());

        sql = "UPDATE Chat SET remoteUsername = '";
        sql += username;
        sql += "', fromUsername = '";
        sql += username;
        sql += "' WHERE remoteUsername = '";
        sql += phone;
        sql += "' AND fromUsername = '";
        sql += phone;
        sql += "';";
        CCLog(kLogLevelDebug, "sql:%s", sql.c_str());
        db.execDML(sql.c_str());
    }
    catch(CppSQLite3Exception& e)
    {
        CCLog(kLogLevelError, "update chat message guid error: %s", e.errorMessage());
        db.close();
        return -1;
    }
    db.close();
    
    return 0;
}

//统计添加指定好友申请条数    入参需要加const   ，不加 const 代表出参,   本地存储库中没有他发出的消息则认为他是第一次发消息
int get_request_count(const string& remote_user_name)
{
	CCLog(kLogLevelInfo, "remote_user_name=:%s ",  remote_user_name.c_str());
	CppSQLite3DB db;
	int counts = 0;
	try
	{
		string path = get_database_path();
		const char* db_name = path.c_str();
		db.open(db_name);

		string sql_cn = "SELECT count(1)  FROM chat WHERE remoteUsername = '";
		sql_cn += remote_user_name;
        sql_cn += "'AND toUsername = '";
        sql_cn += remote_user_name;
		sql_cn += "'AND status IN(1)";

		int cn = -1;
		db.execDMLGetPara(sql_cn.c_str(), &cn);
		counts = cn;
		//q.finalize();//结束查询,释放内存
	}
	catch (CppSQLite3Exception & e)
	{
		CCLog(kLogLevelError, "get chat message list error: %s", e.errorMessage());
	}
	db.close();

	CCLog(kLogLevelDebug, "get chat message list end counts:%d", counts);
	return counts;
}

}
