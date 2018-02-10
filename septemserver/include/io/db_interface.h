
#ifndef _DB_INTERFACE_HPP_
#define _DB_INTERFACE_HPP_

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