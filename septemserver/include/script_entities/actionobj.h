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