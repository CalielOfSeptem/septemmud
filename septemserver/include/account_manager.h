#ifndef ACCOUNT_MANAGER_H_
#define ACCOUNT_MANAGER_H_

#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_sinks.h>

namespace spd = spdlog;

#include "json.hpp"


using json = nlohmann::json;

class account_manager
{
    
public:
    static account_manager & Instance()
    {
        // Since it's a static variable, if the class has already been created,
        // It won't be created again.
        // And it **is** thread-safe in C++11.

        static account_manager myInstance;

        // Return a reference to our instance.
        return myInstance;
    }

    // delete copy and move constructors and assign operators
    account_manager(account_manager const&) = delete;             // Copy construct
    account_manager(account_manager&&) = delete;                  // Move construct
    account_manager& operator=(account_manager const&) = delete;  // Copy assign
    account_manager& operator=(account_manager &&) = delete;      // Move assign
    
    
    bool get_accountExists(const std::string& name);
    
    bool create_account( const std::string& player_name, const std::string& pass, const std::string& email );
    

protected:
    account_manager()
    {
      
    }

    ~account_manager()
    {
         // Destructor code goes here.
    }

    
    
private:

};

#endif