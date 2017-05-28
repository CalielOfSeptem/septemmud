#ifndef ENTITY_MANAGER_H_
#define ENTITY_MANAGER_H_

//#include <unordered_map>
#include <map>
//#include <unordered_set>
#include <set>
#include "entity_wrapper.h"
#include "script_entities/roomobj.h"
#include "script_entities/daemonobj.h"
#include "script_entities/commandobj.h"
#include "script_entities/playerobj.h"
#include "script_entities/itemobj.h"
#include "heartbeat_manager.h"
#include <memory.h> //for shared_ptr

class entity_manager
{
    
public:

struct _internal_lua_
{
    sol::environment _current_loading_env;
    sol::protected_function _current_script_f_;
    script_entity * _last_registered_object_;
};
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
    
    bool do_update ( std::string& entitypath, playerobj* p );
    
    bool compile_entity( std::string& relative_script_path, EntityType etype, std::string& script_text, std::string& reason );
    
    bool compile_virtual_file( std::string& relative_script_path, EntityType etype, std::string& script_text, std::string& reason );
    
    bool compile_script_file( std::string& file_path, std::string& reason );
    
    bool compile_and_clone(std::string& relative_script_path, std::string& script_path_virtual,
                            std::string& tag_text,
                            std::string& tag_replace,
                            std::string& addl_lua,
                            std::string& reason );
                            
    bool clone_item ( std::string& relative_script_path, script_entity * obj );
    bool clone_item_to_hand(std::string& relative_script_path, handobj* obj);
    
    void invoke_heartbeat();
    
    void register_command( commandobj * cmd );
    void deregister_command( commandobj * cmd );
    
    void register_room( roomobj * room );
    void deregister_room( roomobj * room );
    
    void register_daemon( daemonobj * daemon );
    void deregister_daemon( daemonobj * daemon );
    
    void register_item( itemobj * item );
    void deregister_item( itemobj * item );
    
    void register_entity(script_entity *entityobj, std::string& sp, EntityType etype);
    void deregister_entity(script_entity *entityobj, EntityType etype);
    
    
    bool move_living(script_entity* target, const std::string& roomid);
    bool move_entity(script_entity* target, script_entity* dest);
    
    bool do_command(living_entity * e, const std::string cmd);
    bool do_goto(std::string& entitypath, playerobj* p );
    bool do_tp(std::string& entitypath, playerobj* p_targ, playerobj * p_caller );
    
    /**
     * @brief A bloody work around to link scripts with their instantiated objects
     * @return 
     */
    sol::environment GetCurrentLoadingEnv() { return m_state_internal->_current_loading_env; }
    
    /**
     * @brief Loads a player into the game world
     */
    bool load_player(std::string playerName);
    
    
    
    /**
     * @brief Destroys lua state and resets everything!
     */
    void reset();
    
    /**
     * @brief Get room by script path, note the path is relative not full
     * @param script_path
     * @return 
     */
    roomobj * GetRoomByScriptPath(std::string & script_path, unsigned int instance_id);
    
    itemobj * GetItemByScriptPath(std::string & script_path, unsigned int instance_id);
    
    itemobj* GetItemByScriptPath(std::string& script_path);
    
    roomobj* GetRoomByScriptPath(std::string& script_path);
    
    daemonobj * GetDaemonByScriptPath(std::string & script_path, unsigned int & instance_id);
    
    playerobj* get_player( const std::string& player_name );
    
    
    void debug(std::string& msg);
    
    void debug_script( script_entity * se, const std::string & msg );
    
    std::vector<std::string> tail_entity_log( script_entity * se );
    
    void garbage_collect();
protected:
    entity_manager()
    ;

    ~entity_manager()
    {
         // Destructor code goes here.
    }

    
    
private:
    // the following maps make sure we keep a solid connection between script paths and
    // the objects that get loaded from the paths.  In short, they keep objects alive and findable
    std::map< std::string, std::set<std::shared_ptr<entity_wrapper>> > m_room_objs;
    std::map< std::string, std::set<std::shared_ptr<entity_wrapper>> > m_daemon_objs;
    std::map< std::string, std::set<std::shared_ptr<entity_wrapper>> > m_cmd_objs;
    std::map< std::string, std::set<std::shared_ptr<entity_wrapper>> > m_item_objs;
    std::map< std::string, std::shared_ptr<entity_wrapper> > m_player_objs;
    
    
    // default commands is a lookup map, register/deregister updates it based on
    // objects being added/removed
    std::map< std::string, commandobj * > m_default_cmds; // commands everyone has access to
    std::map< std::string, roomobj * > m_room_lookup;
    std::map< std::string, daemonobj * > m_daemon_lookup;
    std::map< std::string, itemobj * > m_item_lookup;
    
    std::shared_ptr < sol::state > m_state;
    std::shared_ptr<_internal_lua_> m_state_internal;
    heartbeat_manager _heartbeat;
    
    

    
    /*
     *  Initiliaze lua state and user types 
     */
    void init_lua();
    
    bool _init_lua_env_( sol::environment parent, sol::environment inherit, std::string new_child_env_name, 
        sol::environment& new_child_env );
        
    bool lua_safe_script(std::string& script_text, sol::environment env, std::string &reason);

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
    bool destroy_room(std::string& script_path);
    
    /**
     * @brief Destroys player associated with a script
     * @param script_path
     * @param lua
     * @return 
     */
    bool destroy_player(std::string& script_path);
    
    /**
     * @brief Destroys all daemons associated with a script
     * @param script_path
     * @param lua
     * @return 
     */
    bool destroy_daemon(std::string& script_path);
    
    bool destroy_command(std::string& script_path);
    
    bool destroy_item(std::string& script_path);
    
    bool destroy_entity(std::shared_ptr<entity_wrapper>& ew);
    
   
    
    
    /**
     * @brief Inits environments based on script_path
     * @param script_path
     * @param envstr
     */
    bool _init_entity_env( std::string& script_path, EntityType etype, sol::environment& env );
    
    /**
     * @brief Returns an environment based on entity type and envstr
     * @param envstr String value of environment
     * @param type  EntityType
     * @param env  Set to environment if found
     * @return Returns false if the environment cannot be found
     */
    bool get_evn_from_string(std::string& envstr, EntityType& type, sol::environment& env);
    
    
    void get_rooms_from_path(std::string& script_path, std::set< std::shared_ptr<entity_wrapper> >& rooms);
    
    void get_daemons_from_path(std::string& script_path, std::set< std::shared_ptr<entity_wrapper> >& daemons);
    
    /**
     * @brief Gets the parent env for a given entity and also the name of the entity's env within the parent
     * @param script_path
     * @param env
     * @param env_name
     * @return 
     */
    bool get_parent_env_of_entity( std::string& script_path, sol::environment& env, std::string& env_name );
    



};

#endif