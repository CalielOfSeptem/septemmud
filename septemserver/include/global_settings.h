// ==========================================================================
// global_settings.h
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
#ifndef GLOBAL_SETTINGS_H_
#define GLOBAL_SETTINGS_H_

#include "json.hpp"
using json = nlohmann::json;
class global_settings
{
    
public:
    static global_settings & Instance()
    {
        // Since it's a static variable, if the class has already been created,
        // It won't be created again.
        // And it **is** thread-safe in C++11.

        static global_settings myInstance;

        // Return a reference to our instance.
        return myInstance;
    }

    // delete copy and move constructors and assign operators
    global_settings(global_settings const&) = delete;             // Copy construct
    global_settings(global_settings&&) = delete;                  // Move construct
    global_settings& operator=(global_settings const&) = delete;  // Copy assign
    global_settings& operator=(global_settings &&) = delete;      // Move assign
    
    
    std::string GetSetting( const std::string setting )
    {
        std::string tempstring;
        try
        {
            tempstring = _json[setting];
        }
        catch( std::exception &ex )
        {
            
        }
        return tempstring;
    }
    
    void SetSetting( const std::string& key, const std::string& value )
    {
        _json[key] = value;
    }


protected:
    global_settings()
    {
      
    }

    ~global_settings()
    {
         // Destructor code goes here.
    }

    
    
private:
    json _json;
    
};


#endif