// ==========================================================================
// entity_ptr.h
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
#ifndef ENTITY_PTR_H_
#define ENTITY_PTR_H_

#include "script_entities/script_entity.h"

struct entity_ptr
{
       /**
     * @brief Constructor to allow entity_ptr objs to be instantiated by script entities
     * @param func
     * @param userData
     * @return 
     */
    entity_ptr(sol::state_view lua_state, script_entity * se);
    ~entity_ptr();
   
   void set_alive(bool b);
   bool get_alive();
   
   script_entity* get_entity();
   
private:
	script_entity * m_se;
	bool m_bAlive;
    sol::state_view m_lua_state;
	boost::signals2::connection m_c;
};

#endif