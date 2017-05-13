 #ifndef CONTAINER_BASE_H_
 #define CONTAINER_BASE_H_
 
 #include <string>
 #include <vector>
 #include <memory>
 #include <set>
 #include "entity_wrapper.h"
 

 struct container_base
 {
     //container() = default;
     
     virtual void AddEntityToInventory(script_entity * se)
     {
         inventory.insert(se);
     }
     
     virtual bool RemoveEntityFromInventoryByID( const std::string& id  )
     {
         // TODO: implement this and be sure to nuke a removed items environment_ pointer..
         std::set< script_entity*  >::iterator it = this->inventory.begin();

          while (it != this->inventory.end()) {
                // Check if key's first character is Fi
                if( (*it)->GetInstancePath() == id )
                {
                    it = this->inventory.erase(it);
                    return true;
                }
                else
                    it++;
          }
        return false;
     }
     
    virtual bool RemoveEntityFromInventory( script_entity * se )
    {
         // TODO: implement this and be sure to nuke a removed items environment_ pointer..
         std::set< script_entity*  >::iterator it = this->inventory.begin();

          while (it != this->inventory.end()) {
                // Check if key's first character is Fi
                if( (*it) == se )
                {
                    it = this->inventory.erase(it);
                    return true;
                }
                else
                    it++;
          }
        return false;
    }
     
     virtual const std::set< script_entity*  >& GetInventory()
     {
         return inventory;
     }
     protected:
     std::set< script_entity* > inventory;
 };
 #endif