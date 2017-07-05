#ifndef ACTIONOBJ_H_
#define ACTIONOBJ_H_


#include "boost/date_time/posix_time/posix_time.hpp"
namespace pt = boost::posix_time;

struct actionobj
{
    actionobj( sol::function func, unsigned int interval, sol::object userData = sol::nil )
    {
        this->func = func;
        this->interval = interval;
        this->user_data = userData;
        nt = pt::second_clock::local_time();
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
    
    void DoAction()
    {
        pt::ptime t2 = pt::second_clock::local_time();
        pt::time_duration diff = t2 - nt;
       // std::cout << diff.total_milliseconds() << std::endl;
        if( diff.total_milliseconds()/1000 > interval )
        {
            sol::protected_function_result result = func();
            if(!result.valid()) {
                sol::error err = result;
                //reason = err.what();
                // std::cout << "call failed, sol::error::what() is " << what << std::endl;
                //return false;
            }
            nt = t2;
            //pf.valid();
        }
    }
    
private:
    sol::protected_function func;
    sol::object user_data;
    unsigned int interval;
    
    boost::posix_time::ptime nt; // = boost::posix_time::second_clock::local_time();
    //boost::this_thread::sleep(boost::posix_time::millisec(500));

    //std::cout << diff.total_milliseconds() << std::endl;
};

#endif