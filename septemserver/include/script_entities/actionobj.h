// ==========================================================================
// actionobj.h
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
#ifndef ACTIONOBJ_H_
#define ACTIONOBJ_H_
#define SOL_CHECK_ARGUMENTS
#include "sol.hpp"

#include "boost/date_time/posix_time/posix_time.hpp"
namespace pt = boost::posix_time;

struct actionobj
{
    actionobj( sol::protected_function func, unsigned int interval, sol::object userData = sol::nil )
    {
        this->func = func;
        this->interval = interval;
        this->user_data = userData;
        lastTick = pt::second_clock::local_time();
    }
    ~actionobj()
    {
        
    }
    
    const unsigned int& GetInterval()
    {
        return interval;
    }
    
    sol::protected_function GetFunc()
    {
        return func;
    }
    
    sol::object GetUserData()
    {
        return user_data;
    }
    
    void DoAction();


   
private:
    sol::protected_function func;
    sol::object user_data;
    unsigned int interval;
    boost::posix_time::ptime lastTick;
    
    //boost::posix_time::ptime nt; // = boost::posix_time::second_clock::local_time();
    //boost::this_thread::sleep(boost::posix_time::millisec(500));

    //std::cout << diff.total_milliseconds() << std::endl;
};

#endif