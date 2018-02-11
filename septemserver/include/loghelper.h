// ==========================================================================
// loghelper.h
//
// Copyright (C) 2018 Kenneth Thompson, All Rights Reserved.
// This file is covered by the MIT Licence:
//
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.
// ==========================================================================
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