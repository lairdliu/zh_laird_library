#include "logger.h"
#include <cstdlib>
#include <time.h>
#include <sys/time.h>
#include<unistd.h>

namespace cmhi_iov {
    std::shared_ptr<CLogObserverBase> Logger::msp_logObserver = nullptr;
    bool Logger::s_m_bLogFile = true;
	//默认配置
	static const std::string LOG_PATH = "./iovroot/log/default0.log";	//默认日志文件名称
	static const int LOG_SIZE = 1;							//默认历史文件个数
	//static const int LOG_LEVEL = 0;							//默认日志等级
	static const int FILE_SIZE = 1024*1024*20;				//默认单个文件大小

	//静态成员初始化
	std::string Logger::fileName= LOG_PATH;

    std::ofstream Logger::m_default_log_file_;
    //日志保存规则：按照次数保存10次日志，每次采用到记法，需要检查文件是否存在
    void Logger::init_logger ( const std::string& file_name ) {
		//1、重命名历史日志
		Logger::fileName = file_name;
		std::string newLogName;
		std::string old_log_path;
		for (int i = LOG_SIZE; i > 0; i--)
		{
			newLogName = Logger::fileName;
			old_log_path = Logger::fileName;

			newLogName.replace(Logger::fileName.find(".log") - 1, 1, std::to_string(i).c_str());

			if ((access(newLogName.c_str(), F_OK) != -1))
			{
				//删除文件
				if (remove(newLogName.c_str()) == 0)
				{
					std::cout << "remove success" << std::endl;
				}
				else
				{
					std::cout << "remove fail" << std::endl;
				}
			}
			old_log_path.replace(Logger::fileName.find(".log") - 1, 1, std::to_string(i - 1).c_str());
			std::cout << "old_log_path: " << old_log_path << "  newLogName: " << newLogName << std::endl;
			if ((access(old_log_path.c_str(), F_OK) == -1))
			{
				continue;
			}
			if (rename(old_log_path.c_str(), newLogName.c_str()) == 0)
			{
				//std::cout<<"old_log_path: "<< old_log_path << "  newLogName: " << newLogName << std::endl;
			}
			std::cout << "old_log_path: " << old_log_path << "  newLogName: " << newLogName << std::endl;
		}

        //2、打开新的日志
        Logger::m_default_log_file_.open (Logger::fileName.c_str ( ));
    }
    
    std::ostream& Logger::getStream ( ) {
        
        return m_default_log_file_.is_open ( ) ? m_default_log_file_ : std::cout;
    }
    
    std::ostream& Logger::add ( const ELogLevel log_level ,const char * msg ,... ) {
        
		//控制文件大小： 检车文件大小，若文件大于20M重新初始化文件
        if(m_default_log_file_.tellp ()> FILE_SIZE)
        {
            std::cout <<"size:"<<m_default_log_file_.tellp ()<<std::endl;
            m_default_log_file_.close ();
            Logger logger;
            logger.init_logger(Logger::fileName);
        }
        
        char temp_buff[10240];
        char * buff=0;
        std::string level;
        if( msg ) {
            va_list args;
            va_start( args ,msg );
            vsnprintf ( temp_buff ,10240-1 ,msg ,args );
            va_end( args );
            buff=temp_buff;
        }
        #if 0
        time_t timep;
        time (&timep);
        char tmp[64];
        strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S.%f", localtime(&timep));
        #endif
        
        struct timeval tv;
        gettimeofday ( & tv ,NULL );
        struct tm * pTime;
        pTime=localtime ( & tv.tv_sec );
        
        char sTemp[60]={ 0 };
        //snprintf ( sTemp ,sizeof ( sTemp ) ,"[%04d-%02d-%02d]:[%02d:%02d:%02d:%03d.%03d" ,pTime->tm_year+1900 ,\
            pTime->tm_mon+1 ,pTime->tm_mday ,pTime->tm_hour ,pTime->tm_min ,pTime->tm_sec ,\
            tv.tv_usec/1000 ,tv.tv_usec%1000 );
        snprintf ( sTemp ,sizeof ( sTemp ) ,"[%04d-%02d-%02d]:[%02d:%02d:%02d:%03d]" ,pTime->tm_year+1900 ,\
                pTime->tm_mon+1 ,pTime->tm_mday ,pTime->tm_hour ,pTime->tm_min ,pTime->tm_sec ,tv.tv_usec/1000);
        if( kLogLevelDebug==log_level ) {
            level=":[DEBUG]:";
        }else if( kLogLevelInfo==log_level ) {
            level=":[INFO]:";
        }else if( kLogLevelWarn==log_level ) {
            level=":[WARN]:";
        }else {
            level=":[ERROR]:";
        }
        return getStream ( )<< sTemp <<level << buff /*<<std::endl*/<<std::flush;
    }

    std::string Logger::getLogInfo( const ELogLevel log_level ,const char * msg ,... )
    {
        const int STR_LEN = 2048;
        char temp_buff[STR_LEN + 1] = {0};
        std::string level;
        if( msg )
        {
            va_list args;
            va_start( args ,msg );
            vsnprintf ( temp_buff ,STR_LEN ,msg ,args );
            va_end( args );
        }
        std::string ret(temp_buff);
        return ret;
    }
}


