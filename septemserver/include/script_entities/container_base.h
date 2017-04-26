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
     
     virtual void AddEntityToInventory(const std::shared_ptr< entity_wrapper >& ew)
     {
         //weak_ptr< entity_wrapper> ewp = ew;
         inventory.insert(ew);
     }
     
     virtual bool RemoveEntityFromInventory( const std::string& id  )
     {
         // TODO: implement this and be sure to nuke a removed items environment_ pointer..
         std::set< std::shared_ptr<entity_wrapper> >::iterator it = this->inventory.begin();
         
          while (it != this->inventory.end()) {
                // Check if key's first character is Fi
                if( (*it)->get_object_uid().compare(id) == 0 ) 
                {
                    it = this->inventory.erase(it);
                    break;
                }
                else
                    it++;
          }
          
         return true;
     }
     
     virtual const  std::set< std::shared_ptr<entity_wrapper> >& GetInventory()
     {
         return inventory;
     }
     protected:
     std::set< std::shared_ptr<entity_wrapper> > inventory;
 };
 #endif