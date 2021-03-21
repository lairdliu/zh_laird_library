/***************************************************************************
  * file name cmhi_log_observer.h
  * brief to write log to logcat
  * date 2019/12/13
  *************************************************************************/

#ifndef CMHI_IM_LOG_OBSERVER_H_
#define CMHI_IM_LOG_OBSERVER_H_

#include <string>

namespace cmhi_iov
{
    class CLogObserverBase
    {
    public:
        CLogObserverBase(){}
        virtual ~CLogObserverBase(){}

        virtual void Log_D(const std::string& info) = 0;
        virtual void Log_I(const std::string& info) = 0;
        virtual void Log_W(const std::string& info) = 0;
        virtual void Log_E(const std::string& info) = 0;
    };
  
  }
#endif  //CMHI_IM_LOG_OBSERVER_H_

