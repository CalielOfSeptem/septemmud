#include "script_entities/actionobj.h"

#include <sys/time.h>
void actionobj::DoAction()
{

    pt::ptime t2 = pt::second_clock::local_time();
    pt::time_duration diff = t2 - lastTick;
   // std::cout << diff.total_milliseconds() << std::endl;
    if( diff.total_milliseconds()/1000 > interval )
    {
        lastTick = t2;
        sol::protected_function_result result = func();
        if(!result.valid()) {
            sol::error err = result;
            //reason = err.what();
            // std::cout << "call failed, sol::error::what() is " << what << std::endl;
            //return false;
        }
        
        //pf.valid();
    }

}

