// ==========================================================================
// db_interface.h
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
#ifndef _DB_INTERFACE_HPP_
#define _DB_INTERFACE_HPP_

enum class ConnectionEvent : int { PLAYER_LOGON=0, PLAYER_LOGOFF };

class db_interface
{
    
public:
    static db_interface & Instance()
    {
        // Since it's a static variable, if the class has already been created,
        // It won't be created again.
        // And it **is** thread-safe in C++11.

        static db_interface myInstance;

        // Return a reference to our instance.
        return myInstance;
    }

    // delete copy and move constructors and assign operators
    db_interface(db_interface const&) = delete;             // Copy construct
    db_interface(db_interface&&) = delete;                  // Move construct
    db_interface& operator=(db_interface const&) = delete;  // Copy assign
    db_interface& operator=(db_interface &&) = delete;      // Move assign
	
	void test();
	long int get_last_logon(std::string& player_name);
	long int get_last_logoff(std::string& player_name);
	void on_player_connection_event(std::string& player_name, ConnectionEvent evt, std::string& ip);
    
	void log_event(LOGLEVEL log_level, std::string script_path, std::string msg);
protected:
    db_interface()
    {
      
    }

    ~db_interface()
    {
         // Destructor code goes here.
    }    
    
    //void fs_manager::copyDirectoryRecursively(const boost::filesystem::path& sourceDir, const boost::filesystem::path& destinationDir);
    
private:
   
    
};


#endif