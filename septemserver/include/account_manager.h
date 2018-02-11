// ==========================================================================
// account_manager.h
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
#ifndef ACCOUNT_MANAGER_H_
#define ACCOUNT_MANAGER_H_

#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_sinks.h>
#include "account.h"
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
    
    bool load_account( const std::string& name, account & ac );
    

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