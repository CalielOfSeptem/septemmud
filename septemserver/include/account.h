#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP
//#include <cereal/archives/xml.hpp>
//#include <cereal/types/string.hpp>
#include <string.h>
//#include <cereal/types/vector.hpp>
#include "json.hpp"

using json = nlohmann::json;

class account
{
public:
    account();
    
    account( const account& other ) :
     username(other.username), email(other.email), secure_password(other.secure_password)
    {
        for( auto eobj : other.char_names )
        {
            char_names.push_back(eobj);
        }
    }
    
    ~account();
    
    std::string username;
    std::string email;
    std::string secure_password;
    std::vector<std::string> char_names;
    
    bool do_load()
    ;
    
    bool do_save()
    {
        /*
        json j;
        
        try
        {
            if( m_RightHand.GetItem() )
            {
                j["rightHand"] = std::map<std::string, std::string> {{ 
                        m_RightHand.GetItem()->get_uid(),
                        m_RightHand.GetItem()->GetPhysicalScriptPath()
                        }};
            }
            else
                j["rightHand"] = NULL;
                
            if( m_LeftHand.GetItem() )
               j["leftHand"] = std::map<std::string, std::string> {{ 
                        m_LeftHand.GetItem()->get_uid(),
                        m_LeftHand.GetItem()->GetPhysicalScriptPath(), 
                        }};
            else
                j["leftHand"] = NULL;
                
                
            std::vector< script_entity* > items = this->GetInventory();
            std::map< std::string, std::string > item_objs;
            for( auto i : items )
            {
                itemobj * obj = dynamic_cast< itemobj * > (i);
                item_objs[obj->get_uid()] = i->GetPhysicalScriptPath();
            }
     
            j["inventory"] = item_objs;
            
            j["cwd"] = this->cwd;
            
            if( this->get_gender() == EntityGender::MALE )
            {
                j["gender"] = "male";
            }
            else if( this->get_gender() == EntityGender::FEMALE )
            {
                j["gender"] = "female";
            }
            else if( this->get_gender() == EntityGender::UNKNOWN )
            {
                j["gender"] = "unknown";
            }
            
            
            if( this->GetEnvironment() )
            {
                j["roomPath"] = this->GetEnvironment()->GetPhysicalScriptPath();
                j["roomID"] = this->GetEnvironment()->instanceID;
            }

           // std::ofstream o(this->get_entityStorageLocation() + "/" + this->get_uid() );
           // o << std::setw(4) << j << std::endl;
                
            std::ofstream o(this->get_entityStorageLocation() + "/player_save");
            o << std::setw(4) << j << std::endl;
        }
        catch( std::exception & ex )
        {
            std::stringstream ss;
            ss << "Error during save: " << ex.what();
            this->debug(ss.str());
            return false;
        }

        */
        return true;
    }
    /*
    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( cereal::make_nvp("UserName", username), cereal::make_nvp("Email", email),  
        cereal::make_nvp("SecurePassword", secure_password), cereal::make_nvp("CharNames", char_names)); // serialize things by passing them to the archive
    }
    }*/
};

#endif // ACCOUNT_HPP
