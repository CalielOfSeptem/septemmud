// ==========================================================================
// fs_manager.h
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
#ifndef FS_MANAGER_H_
#define FS_MANAGER_H_

#include "script_entities/playerobj.h"

struct file_entity
{
    std::string name;
    bool isDirectory;
};



class fs_manager
{
    
public:
    static fs_manager & Instance()
    {
        // Since it's a static variable, if the class has already been created,
        // It won't be created again.
        // And it **is** thread-safe in C++11.

        static fs_manager myInstance;

        // Return a reference to our instance.
        return myInstance;
    }

    // delete copy and move constructors and assign operators
    fs_manager(fs_manager const&) = delete;             // Copy construct
    fs_manager(fs_manager&&) = delete;                  // Move construct
    fs_manager& operator=(fs_manager const&) = delete;  // Copy assign
    fs_manager& operator=(fs_manager &&) = delete;      // Move assign
    
    
    std::vector<file_entity> get_dir_list(std::string& relative_path);
    
    bool change_directory( std::string& relative_path, playerobj* p);
    
    /**
     * @brief Translates a path provided by a script into a fully-qualified filesystem path
     * @param path
     * @param p
     * @return 
     */
    bool translate_path( std::string& relative_path, playerobj* p, std::string& reason );
    
    bool translate_path( std::string& relative_path, std::string& reason );
    
    bool do_copy( std::string& patha, std::string& pathb, playerobj * p );
    
    bool do_remove( std::string& path, playerobj * p );
    
    bool get_player_save_dir( const std::string& pname, std::string& fullpath, bool bCreate=false );
    
    bool get_account_save_dir (const std::string& aname, std::string& fullpath, bool bCreate=false );
    
    bool get_workspace_dir (const std::string& aname, std::string& fullpath, bool bCreate=false );
    
    bool get_account_exists( const std::string& aname );
    
    bool get_workspace_exists( const std::string& aname);
    
    //bool do_create_workspace( std::string& aname, std::string& reason);
    
    bool do_create_new_workspace(std::string& aname, std::string& workroom_path, std::string& reason);
    
    //bool create_directory( std::string& path, 

protected:
    fs_manager()
    {
      
    }

    ~fs_manager()
    {
         // Destructor code goes here.
    }    
    
    //void fs_manager::copyDirectoryRecursively(const boost::filesystem::path& sourceDir, const boost::filesystem::path& destinationDir);
    
private:
   
    
};


#endif
