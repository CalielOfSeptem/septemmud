#include "account.h"
#include "fs/fs_manager.h"

bool account::do_load()
{
    try {
        std::string account_path = "";
        if( !fs_manager::Instance().get_account_save_dir( username, account_path) )
        {
            return false;
        }
       // std::string acct_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
       // acct_path += "\\" + global_settings::Instance().GetSetting(DEFAULT_ACCOUNTS_PATH);
        // auto game_root_path =
        // boost::filesystem::canonical(global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH));
        //  auto player_save = global_settings::Instance().GetSetting(DEFAULT_BASE_SAVE_PATH);

        // std::string first_letter = boost::to_lower_copy(pname).substr(0,1);

        // auto patha = boost::filesystem::weakly_canonical(game_root_path/player_save/first_letter);

        // Check if path is within web_root_path
        // if(std::distance(game_root_path.begin(), game_root_path.end())>std::distance(patha.begin(), patha.end()) ||
        //     !std::equal(game_root_path.begin(), game_root_path.end(), patha.begin()))
        //     throw std::invalid_argument("path must be within root path");
        // if(!boost::filesystem::is_directory(patha))
        //    throw std::invalid_argument("must be a directory");
        // if(!(boost::filesystem::exists(patha) ))// && boost::filesystem::is_regular_file(path)))
        //     boost::filesystem::create_directory(patha);

        // auto pathb=boost::filesystem::weakly_canonical(game_root_path/player_save/first_letter/pname);
        // if(!(boost::filesystem::exists(pathb) ))// && boost::filesystem::is_regular_file(path)))
        //     boost::filesystem::create_directory(pathb);

        // std::ifstream i(this->get_entityStorageLocation() + "/player_save");
    } catch(std::exception& ex) {
        //this->debug(ex.what());
    }
    /*
    if( this->get_entityStorageLocation().size() == 0 )
        return false;

    try
    {
        std::ifstream i(this->get_entityStorageLocation() + "/player_save");
        json j;
        i >> j;
        const json& rh = j["rightHand"]; //<<<< this bit was hard to figure out
        for (auto& element : json::iterator_wrapper(rh)) {
            if( element.key().size() > 0 && element.value() != NULL )
            {
                //std::cout << element.key() << " maps to " << element.value() << std::endl;
                std::string s1 = element.key();
                std::string s2 = element.value();
                entity_manager::Instance().clone_item_to_hand( s2, &m_RightHand, s1);
            }

        }
       const json& lh = j["leftHand"]; //<<<< this bit was hard to figure out
       for (auto& element : json::iterator_wrapper(lh)) {
            if( element.key().size() > 0 && element.value() != NULL )
            {
               // std::cout << element.key() << " maps to " << element.value() << std::endl;
                std::string s1 = element.key();
                std::string s2 = element.value();
                entity_manager::Instance().clone_item_to_hand( s2, &m_LeftHand, s1);
            }

        }

        const json& inventory = j["inventory"]; //<<<< this bit was hard to figure out
        for (auto& element : json::iterator_wrapper(inventory)) {
            if( element.key().size() > 0 && element.value() != NULL )
            {
                //std::cout << element.key() << " maps to " << element.value() << std::endl;
                std::string s1 = element.key();
                std::string s2 = element.value();
                entity_manager::Instance().clone_item( s2, dynamic_cast<script_entity*>(this), s1 );
            }

        }
        //if( j["cwd"] )
        this->cwd = j["cwd"];

        if( j["gender"] == "male" )
        {
            this->set_gender(EntityGender::MALE);
        }
        else if( j["gender"] == "female" )
        {
            this->set_gender(EntityGender::FEMALE);
        }
        else if( j["gender"] == "unknown" )
        {
            this->set_gender(EntityGender::UNKNOWN);
        }


        this->roomPath = j["roomPath"];
        this->roomID = j["roomID"];

    }
    catch(std::exception &ex)
    {
        this->debug(ex.what());
    }
    */
    return true;
}
