#ifndef FS_MANAGER_H_
#define FS_MANAGER_H_

#include <vector>
#include <string>
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
    bool translate_path( std::string& relative_path, playerobj * p );
    
    bool do_copy( std::string& patha, std::string& pathb, playerobj * p );
    
    bool do_remove( std::string& path, playerobj * p );

protected:
    fs_manager()
    {
      
    }

    ~fs_manager()
    {
         // Destructor code goes here.
    }    
    
private:
   
    
};


#endif
