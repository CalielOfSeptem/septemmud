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