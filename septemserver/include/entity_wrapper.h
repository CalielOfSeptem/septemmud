// ==========================================================================
// entity_wrapper.h
//
// Copyright (C) 2018 Kenneth Thompson, All Rights Reserved.
// This file is covered by the MIT Licence:
//
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.
// ==========================================================================

 #ifndef ENTITY_WRAPPER_H_
 #define ENTITY_WRAPPER_H_
 
 #include "script_entities/script_entity.h"
 
 //struct script_entity;

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
             
     }
     
     
     entity_wrapper( EntityType& entity_type, unsigned int instance_id )
        : entity_type(entity_type)
     {
         
     }
     
     std::string get_object_uid()
     ;
     
     void clear()
     ;
     
     std::string script_path;
     script_entity* script_ent;
     sol::protected_function _script_f_; // holds script function to kill environment when necessary.
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