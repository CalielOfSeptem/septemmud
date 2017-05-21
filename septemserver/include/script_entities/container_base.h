 #ifndef CONTAINER_BASE_H_
 #define CONTAINER_BASE_H_
 
 #include <string>
 #include <vector>
 #include <memory>
 #include <set>
 #include "entity_wrapper.h"
 

 struct container_base
 {
     virtual script_entity * GetOwner() = 0;
     virtual void AddEntityToInventory(script_entity * se)
     {
         if( se == NULL )
             return;
             
         script_entity * env = se->GetEnvironment();
         
         if( env != NULL && env == GetOwner() )
             return;
             
         if( env != NULL )
         {
             // remove entity from inventory
             if( container_base * cb = dynamic_cast<container_base*>(env) )
             {
                 cb->RemoveEntityFromInventory(se);
             }
         }
         
         inventory.insert(se);
         auto t = GetOwner();
         if( t == NULL )
             return;
         se->SetEnvironment( GetOwner() );
     }
     
     virtual bool RemoveEntityFromInventoryByID( const std::string& id  )
     {
         // TODO: implement this and be sure to nuke a removed items environment_ pointer..
         std::set< script_entity*  >::iterator it = this->inventory.begin();

          while (it != this->inventory.end()) {
                // Check if key's first character is Fi
                if( (*it)->GetInstancePath() == id )
                {
                    (*it)->SetEnvironment(NULL);
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
                    (*it)->SetEnvironment(NULL);
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