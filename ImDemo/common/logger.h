/***************************************************************************
  * file name logger.h
  * brief 基础类型定义
  * date 2019/01/31
  *************************************************************************/

#ifndef CMCC_IM_LOGGER_H_
#define CMCC_IM_LOGGER_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
#include <stdint.h>
#include <stdarg.h>
#include <memory>
#include "common_types.h"
#include "cmhi_log_observer.h"

namespace cmhi_iov{

class Logger {
   
public:
   //构造函数
   Logger() {};
   
   ~Logger() {};

   void init_logger(const std::string& file_name);

   //格式化日志输出
   static std::ostream& add(const ELogLevel log_level, const char* msg, ...);

    static std::shared_ptr<CLogObserverBase> GetLogObserver()
    {
        return msp_logObserver;
    }

    static void SetLogObserver(std::shared_ptr<CLogObserverBase> logObserver, bool bLogFile)
    {
        msp_logObserver = logObserver;
        s_m_bLogFile = bLogFile;
    }
    static bool GetBLogFile()
    {
        return s_m_bLogFile;
    }

    static std::string getLogInfo( const ELogLevel log_level ,const char * msg ,... );
private:
    static std::shared_ptr<CLogObserverBase> msp_logObserver;
    static bool s_m_bLogFile;
private:
   static std::ostream& getStream();
   static std::string fileName;
   static std::ofstream m_default_log_file_;                 // 调试日志的输出流
};


#define CCLog(_log_level, _str, ...) \
do \
{ \
    if(NULL != _str) \
    { \
        std::string log_str = "CMIM-cmhi [%s]:[%d]:[%s]";log_str += _str;log_str += "\n\0"; \
        if (Logger::GetBLogFile())\
        {\
            Logger().add(_log_level, log_str.c_str(), __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
        }\
        else\
        {\
            std::string strLog = Logger::getLogInfo(_log_level, log_str.c_str(), __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
            if (kLogLevelDebug == _log_level)\
            {\
                if(Logger::GetLogObserver()) \
                    Logger::GetLogObserver()->Log_D(strLog);\
            }\
            else if (kLogLevelInfo == _log_level)\
            {\
                if(Logger::GetLogObserver()) \
                    Logger::GetLogObserver()->Log_I(strLog);\
            }\
            else if (kLogLevelWarn == _log_level)\
            {\
                if(Logger::GetLogObserver()) \
                    Logger::GetLogObserver()->Log_W(strLog);\
            }\
            else if (kLogLevelError == _log_level)\
            {\
                if(Logger::GetLogObserver()) \
                    Logger::GetLogObserver()->Log_E(strLog);\
            }\
        }\
    } \
}while(0)

}

#endif


