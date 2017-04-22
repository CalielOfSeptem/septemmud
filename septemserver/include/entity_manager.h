#ifndef ENTITY_MANANGER_H_
#define ENTITY_MANAGER_H_

#include <unordered_map>
#include "entity_wrapper.h"

class entity_manager
{
    
public:
    static entity_manager & Instance()
    {
        // Since it's a static variable, if the class has already been created,
        // It won't be created again.
        // And it **is** thread-safe in C++11.

        static entity_manager myInstance;

        // Return a reference to our instance.
        return myInstance;
    }

    // delete copy and move constructors and assign operators
    entity_manager(entity_manager const&) = delete;             // Copy construct
    entity_manager(entity_manager&&) = delete;                  // Move construct
    entity_manager& operator=(entity_manager const&) = delete;  // Copy assign
    entity_manager& operator=(entity_manager &&) = delete;      // Move assign
    
    bool compile_script(std::string& path);

protected:
    entity_manager()
    {
      
    }

    ~entity_manager()
    {
         // Destructor code goes here.
    }

    
    
private:
    std::unordered_map< std::string, entity_wrapper > m_room_objs;
    std::shared_ptr < sol::state > m_state;
    
    /*
     *  Initiliaze lua state and user types 
     */
    void init_lua(sol::state& lua);
    
    bool _init_lua_env_( sol::state& lua, sol::environment parent, sol::environment inherit, std::string new_child_env_name, 
        sol::environment& new_child_env );
        
    bool lua_safe_script(std::string& script_path, sol::state& lua);
};


#endif