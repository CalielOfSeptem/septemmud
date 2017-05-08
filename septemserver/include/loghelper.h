#ifndef _LOGHELPER_H_
#define _LOGHELPER_H_

#include "spdlog/sinks/sink.h"
#include "spdlog/spdlog.h"
namespace spd = spdlog;

class my_sink : public spd::sink
{
  void log(const spdlog::details::log_msg& msg) override
  {
    // Your code here. 
    // details::log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
    // msg.formatted contains the formatted log.
    // msg.raw contains pre formatted log
    std::cout << msg.formatted.str();
  }

 void flush() 
 {
     std::cout << std::flush;
 }

};

#endif