#include "stdafx.h"
#include "script_entities/actionobj.h"

#include <sys/time.h>
void actionobj::DoAction()
{

    pt::ptime t2 = pt::second_clock::local_time();
    pt::time_duration diff = t2 - lastTick;

    if( diff.total_milliseconds()/1000 > interval )
    {
        lastTick = t2;
        sol::protected_function_result result = func();
        if(!result.valid()) {
            sol::error err = result;
        }
    }

}

