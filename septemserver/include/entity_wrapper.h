 #ifndef ENTITY_WRAPPER_H_
 #define ENTITY_WRAPPER_H_
 
 #include <string>
 #include <vector>
 #include <boost/shared_ptr.hpp>
 #include "script_entities/script_entity.h"
 #include <plog/Log.h>

 struct entity_wrapper
 {
     
  //  bool operator == (const entity_wrapper& rhs) const{
   //     return script_path == rhs.script_path && instance_id == rhs.instance_id;
  //  }

  //  bool operator != (const entity_wrapper& rhs) const{
   //     return !(*this == rhs);
  //  }

     entity_wrapper()
     {
         
     }
     ~entity_wrapper()
     {
         LOG_DEBUG << "Object destroyed.";
     }
     
     
     entity_wrapper( EntityType& entity_type, unsigned int instance_id )
        : entity_type(entity_type)
     {
         
     }
     
     std::string get_object_uid()
     {
         // example: /home/ken/git-repos/septem/game_data/realms/void:obj=room1:id=0
         return script_ent->GetInstancePath(); //script_path + ":id=" + std::to_string(instance_id);
     }
     
     void clear()
     {
         script_ent->clear_props();
         script_ent = NULL;
     }
     
     std::string script_path;
     script_entity* script_ent;
     sol::protected_function _script_f_; // holds script function to kill environment when necessary.
     //sol::optional<sol::userdata&> script_ent;
    // sol::optional<sol::environment> script_env;
    // sol::optional<sol::environment> parent_env;
     //std::shared_ptr<sol::state> script_state;
     EntityType entity_type;
     //unsigned int instance_id; // zero based.
 };
namespace std
{
   // template <>
   // struct hash<entity_wrapper>
   // {
   //     size_t operator()(entity_wrapper const & x) const noexcept
   //     {
   //         return hash<string>()(x.script_path) ^ hash<string>()(std::to_string(x.instance_id));
   //     }
   // };
}
 
#endif