#ifndef ENTITY_MANAGER_H_
#define ENTITY_MANAGER_H_

//#include <unordered_map>
#include <map>
//#include <unordered_set>
#include <set>
#include "entity_wrapper.h"
#include "script_entities/roomobj.h"
#include "heartbeat_manager.h"
#include <memory.h> //for shared_ptr

//typedef std::pair<std::string, std::string> env_id_t;

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
    
    bool compile_script(std::string& file_path, std::string& reason);
    
    void invoke_heartbeat();
    
    void register_entity(script_entity &entityobj, EntityType etype);
    void deregister_entity(script_entity &entityobj, EntityType etype);
    
    /**
     * @brief A bloody work around to link scripts with their instantiated objects
     * @return 
     */
    std::string GetCurrentlyCompiledScript() { return _currently_loading_script; }
    
    /**
     * @brief A bloody work around to link scripts with their instantiated objects
     * @return 
     */
    sol::environment GetCurrentLoadingEnv() { return _current_loading_env; }
    
    /**
     * @brief Loads a player into the game world
     */
    bool load_player(/* TODO: FIX SIGNATURE WHEN NETWORK CODE IS BACK IN */);
    
    
    
    /**
     * @brief Destroys lua state and resets everything!
     */
    void reset();
    
    /**
     * @brief Get room by script path, note the path is relative not full
     * @param script_path
     * @return 
     */
    roomobj * GetRoomByScriptPath(std::string & script_path, unsigned int & instance_id);
    
protected:
    entity_manager()
    {
      
    }

    ~entity_manager()
    {
         // Destructor code goes here.
    }

    
    
private:
    std::map< std::string, std::set<std::shared_ptr<entity_wrapper>> > m_room_objs;
    std::map< std::string, std::set<std::shared_ptr<entity_wrapper>> > m_daemon_objs;
    std::map< std::string, std::shared_ptr<entity_wrapper> > m_player_objs;
    std::shared_ptr < sol::state > m_state;
    heartbeat_manager _heartbeat;
    std::string _currently_loading_script;
    sol::environment _current_loading_env;
    sol::protected_function _current_script_f_;
    
    /*
     *  Initiliaze lua state and user types 
     */
    void init_lua(sol::state& lua);
    
    bool _init_lua_env_( sol::state& lua, sol::environment parent, sol::environment inherit, std::string new_child_env_name, 
        sol::environment& new_child_env );
        
    bool lua_safe_script(std::string& script_text, sol::state& lua, sol::environment env, std::string &reason);

    bool load_script_text(std::string& script_path,
                                      std::string& script_text,
                                      EntityType& obj_type,
                                      std::string& reason);
                                      
    /*
     *  Room Helper Functions
     * 
    */

    /**
     * @brief Gets all rooms based on their associated script
     * @param script_path Path to the room script
     * @return Returns false if no rooms are found
     */
    //bool get_room_by_script(std::string& script_path, std::unordered_set<entity_wrapper>& room_ents);
    
    /**
     * @brief Destroys all rooms associated with a script
     * @param script_path Path to the room script
     * @return Returns false if the room does not exist
     */
    bool destroy_room(std::string& script_path, sol::state& lua);
    
    /**
     * @brief Destroys player associated with a script
     * @param script_path
     * @param lua
     * @return 
     */
    bool destroy_player(std::string& script_path, sol::state& lua);
    
    /**
     * @brief Destroys all daemons associated with a script
     * @param script_path
     * @param lua
     * @return 
     */
    bool destroy_daemon(std::string& script_path, sol::state& lua);
    
    bool destroy_entity(std::shared_ptr<entity_wrapper>& ew);
    
    
    /**
     * @brief Inits environments based on script_path
     * @param script_path
     * @param envstr
     */
    bool _init_entity_env( std::string& script_path, EntityType etype, sol::state& lua, sol::environment& env );
    
    /**
     * @brief Returns an environment based on entity type and envstr
     * @param envstr String value of environment
     * @param type  EntityType
     * @param env  Set to environment if found
     * @return Returns false if the environment cannot be found
     */
    bool get_evn_from_string(std::string& envstr, EntityType& type, sol::environment& env);
    
    /**
     * @brief Gets an environment based on a two-part vector
     * @param envstr
     * @param env_id ID of environment in the form of a pair of strings.
     * @param env
     * @return 
     */
    //bool get_evn_from_vec(std::string& envstr, env_id_t& env_id, sol::environment& env);
    
    void get_rooms_from_path(std::string& script_path, std::set< std::shared_ptr<entity_wrapper> >& rooms);
    
    /**
     * @brief Gets the parent env for a given entity and also the name of the entity's env within the parent
     * @param script_path
     * @param env
     * @param env_name
     * @return 
     */
    bool get_parent_env_of_entity( std::string& script_path, sol::environment& env, std::string& env_name, sol::state& lua );

};


#endif