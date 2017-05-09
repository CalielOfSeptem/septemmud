#ifndef _LOGHELPER_H_
#define _LOGHELPER_H_
#include "spdlog/spdlog.h"
#include <spdlog/sinks/sink.h>

#include <list>

namespace spd = spdlog;

class my_sink : public spd::sinks::sink
{
public:
    
    void log(const spd::details::log_msg& msg) override
    {
        // Your code here. 
        // details::log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
        // msg.formatted contains the formatted log.
        // msg.raw contains pre formatted log
        //std::cout << msg.formatted.str();
        if( m_events.size() < 1000 )
        {
            m_events.pop_front();
        }
        m_events.push_back( msg.formatted.str() );
    }

    void flush() 
    {
        //std::cout << std::flush;
    }

    std::list< std::string > m_events;
};

#endif