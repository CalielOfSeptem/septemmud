#ifndef _SECURITY_CONTEXT_HPP_
#define _SECURITY_CONTEXT_HPP_

#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_sinks.h>

namespace spd = spdlog;

#include "json.hpp"

using json = nlohmann::json;

class security_context
{
    
public:
    static security_context & Instance()
    {
        // Since it's a static variable, if the class has already been created,
        // It won't be created again.
        // And it **is** thread-safe in C++11.

        static security_context myInstance;

        // Return a reference to our instance.
        return myInstance;
    }

    // delete copy and move constructors and assign operators
    security_context(security_context const&) = delete;             // Copy construct
    security_context(security_context&&) = delete;                  // Move construct
    security_context& operator=(security_context const&) = delete;  // Copy assign
    security_context& operator=(security_context &&) = delete;      // Move assign
    
    
    void SetCurrentEntity( script_entity * ce )
    {
        _currentEntity = ce;
    }
    
    script_entity * GetCurrentEntity( script_entity * ce )
    {
        return _currentEntity;
    }

protected:
    security_context()
    {
      
    }

    ~security_context()
    {
         // Destructor code goes here.
    }

    
    
private:

    /*
     *  The currentEntity is the entity which has control.  When a user executes a command 
     *  the currentEntity will be the user.  Otherwise, the current entity will be an NPC
     *  or other entity.. TBD once this is fully implemented.
     *  null will result in rejection of any action that requires a heightened privilege
     */
    script_entity * _currentEntity; // Entity presently in control.  

};


#endif