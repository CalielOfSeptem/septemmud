#include "fs/fs_manager.h"
#include <iostream>
#include <algorithm>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/format.hpp>
#include <iomanip>
#include <regex>
#include <boost/filesystem.hpp>
#include "config.h"
#include "global_settings.h"

namespace fs = boost::filesystem;

bool myfunction (file_entity a, file_entity b) { return a.isDirectory; }


std::vector<file_entity> fs_manager::get_dir_list(std::string& relative_path)
{
    std::vector<file_entity> file_entities;
    
    try 
    {

        auto game_root_path = boost::filesystem::canonical(global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH));
        
        auto path=boost::filesystem::canonical(game_root_path/relative_path);
        
        //Check if path is within web_root_path
        if(std::distance(game_root_path.begin(), game_root_path.end())>std::distance(path.begin(), path.end()) ||
           !std::equal(game_root_path.begin(), game_root_path.end(), path.begin()))
            throw std::invalid_argument("path must be within root path");
        if(!boost::filesystem::is_directory(path))
            throw std::invalid_argument("must be a directory");
        if(!(boost::filesystem::exists(path) ))// && boost::filesystem::is_regular_file(path)))
            throw std::invalid_argument("directory does not exist");

          fs::directory_iterator end_itr; // default construction yields past-the-end
          for ( fs::directory_iterator itr( path );
            itr != end_itr;
            ++itr )
          {
              file_entity fe;
              fe.isDirectory = is_directory(itr->status());
              fe.name = itr->path().filename().string();
              file_entities.push_back(fe);
            /*
            if ( is_directory(itr->status()) )
            {
                //load_directory_map( itr->path(), new_child);
            }
            else //if ( itr->leaf() == file_name ) // see below
            {
                
                json new_child;
                new_child["children"] = false;
                std::size_t str_hash2 = std::hash<std::string>{}(itr->path().string());
            
                new_child["id"] = std::to_string( str_hash2 );
                new_child["text"] = itr->path().filename().string();//
                new_child["data"]["relativePath"] = std::regex_replace(itr->path().string(), std::regex("\\" + std::string(DEFAULT_GAME_DATA_PATH)), "");
                new_child["data"]["systemPath"] = itr->path().string();
                new_child["icon"] = "jstree-file";
                new_child["type"] = "file";
               // load_directory_map( itr->path(), new_child);
                j["children"].push_back(new_child);
               
                
            }
            */
          }
        }
        catch(const std::exception &e) {
          //  string content="Could not open path "+request->path+": "+e.what();
          //  *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
        }
        std::sort (file_entities.begin(), file_entities.end(), myfunction); 
        return file_entities;
}

bool fs_manager::change_directory(std::string& relative_path, playerobj* p)
{
    try 
    {
        if( relative_path.size() == 0 )
             throw std::invalid_argument("A path must be specificed.");
             
        
        
        auto game_root_path = boost::filesystem::canonical(global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH));
        
        fs::path path;
        
        if( relative_path[0] == '/' )
            path = boost::filesystem::canonical(game_root_path/relative_path);
        else if( relative_path[0] == '~')
            path = boost::filesystem::canonical(game_root_path/p->workspacePath);
        else
            path = boost::filesystem::canonical(game_root_path/p->cwd/relative_path);

        if( !fs::exists(path) )
             throw std::invalid_argument("path does not exist");
        if( !fs::is_directory(path) )
              throw std::invalid_argument("path must be a valid directory");
        //Check if path is within web_root_path
        if(std::distance(game_root_path.begin(), game_root_path.end())>std::distance(path.begin(), path.end()) ||
           !std::equal(game_root_path.begin(), game_root_path.end(), path.begin()))
        {
            throw std::invalid_argument("path must be within root path");
        }
        //p->SendToEntity(std::string("OK: ") + path.string());
        
        std::string ps = path.string();
        if( ps[ ps.size()-1 ] != '/' )
            ps += '/';
        /*
        std::string gp = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
        if( ps[ ps.size()-1 ] != '/' )
            ps += '/';
        { 
          
          boost::replace_all(ps, gp, "");
        }
        */
        //ps = std::regex_replace(ps, std::regex("\\" + 
        //    global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH)), "");
        p->cwd = std::regex_replace(ps, std::regex("\\" + 
            global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH)), "");
        p->do_save();
    }
    catch(const std::exception &e) {
      //  string content="Could not open path "+request->path+": "+e.what();
      //  *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
      if( p != NULL )
        p->SendToEntity(std::string("Error: ") +e.what());
      return false;
    }
    
    return true;
}

bool fs_manager::translate_path(std::string& relative_path, playerobj* p, std::string &reason)
{
    try 
    {
        if( relative_path.size() == 0 )
             throw std::invalid_argument("A path must be specified.");
             
        
        auto game_root_path = boost::filesystem::canonical(global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH));
        
        fs::path path;
        
        if( relative_path[0] == '/' )
            path = boost::filesystem::canonical(game_root_path/relative_path);
        else
            path = boost::filesystem::canonical(game_root_path/p->cwd/relative_path);

        if( !fs::exists(path) )
             throw std::invalid_argument("path does not exist");
        
        if(std::distance(game_root_path.begin(), game_root_path.end())>std::distance(path.begin(), path.end()) ||
           !std::equal(game_root_path.begin(), game_root_path.end(), path.begin()))
        {
            throw std::invalid_argument("path must be within root path");
        }

        relative_path = path.string();
    }
    catch(const std::exception &e) {
      //  string content="Could not open path "+request->path+": "+e.what();
      //  *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
      //if( p != NULL )
       // p->SendToEntity(std::string("Error: ") +e.what());
       reason = e.what();
      return false;
    }
    
    return true;
}

bool fs_manager::do_copy(std::string& patha, std::string& pathb, playerobj * p)
{
    try 
    {
        if( patha.size() == 0 || pathb.size() == 0 )
             throw std::invalid_argument("A path must be specificed.");
             
        auto game_root_path = boost::filesystem::canonical(global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH));
        
        fs::path path_a;
        
        if( patha[0] == '/' )
            path_a = boost::filesystem::canonical(game_root_path/patha);
        else
            path_a = boost::filesystem::canonical(game_root_path/p->cwd/patha);
            
        fs::path path_b;
        
        if( pathb[0] == '/' )
            path_b = boost::filesystem::weakly_canonical(game_root_path/pathb);
        else
            path_b = boost::filesystem::weakly_canonical(game_root_path/p->cwd/pathb);


        if( !fs::exists(path_a) )
             throw std::invalid_argument("path does not exist");
             
       // if( !fs::is_regular(path_a) )
       //      throw std::invalid_argument("path must be a file");
             
       // if( fs::exists(path_b) )
       //      throw std::invalid_argument("destination already exists");
             
        
        if(std::distance(game_root_path.begin(), game_root_path.end())>std::distance(path_a.begin(), path_a.end()) ||
           !std::equal(game_root_path.begin(), game_root_path.end(), path_a.begin()))
        {
            throw std::invalid_argument("path must be within root path");
        }
        
        if(std::distance(game_root_path.begin(), game_root_path.end())>std::distance(path_b.begin(), path_b.end()) ||
           !std::equal(game_root_path.begin(), game_root_path.end(), path_b.begin()))
        {
            throw std::invalid_argument("path must be within root path");
        }
      //  if(std::distance(game_root_path.begin(), game_root_path.end())>std::distance(path_b.begin(), path_b.end()) ||
      //     !std::equal(game_root_path.begin(), game_root_path.end(), path_b.begin()))
      //  {
      //      throw std::invalid_argument("path must be within root path");
      //  }
        
       // std::cout << "PATHA: " << path_a.string() << std::endl;
       // std::cout << "PATHB: " << path_b.string() << std::endl;
       fs::copy(path_a, path_b);
        if( p != NULL )
            p->SendToEntity("OK");

        //relative_path = path.string();
    }
    catch(const std::exception &e) {
      //  string content="Could not open path "+request->path+": "+e.what();
      //  *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
      if( p != NULL )
        p->SendToEntity(std::string("Error: ") +e.what());
      return false;
    }
    
    return true;   
}
bool fs_manager::do_remove(std::string& path, playerobj* p)
{
    try 
    {
        if( path.size() == 0 )
             throw std::invalid_argument("A path must be specificed.");
             
        auto game_root_path = boost::filesystem::canonical(global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH));
        
        fs::path path_a;
        
        if( path[0] == '/' )
            path_a = boost::filesystem::canonical(game_root_path/path);
        else
            path_a = boost::filesystem::canonical(game_root_path/p->cwd/path);

        if( !fs::exists(path_a) )
             throw std::invalid_argument("path does not exist");
             
        if(std::distance(game_root_path.begin(), game_root_path.end())>std::distance(path_a.begin(), path_a.end()) ||
           !std::equal(game_root_path.begin(), game_root_path.end(), path_a.begin()))
        {
            throw std::invalid_argument("path must be within root path");
        }
        

        fs::remove(path_a);
        if( p != NULL )
            p->SendToEntity("OK");
        //relative_path = path.string();
    }
    catch(const std::exception &e) {
      //  string content="Could not open path "+request->path+": "+e.what();
      //  *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
      if( p != NULL )
        p->SendToEntity(std::string("Error: ") +e.what());
      return false;
    }
    
    return true;   
}

bool fs_manager::get_player_save_dir(const std::string& pname, std::string& fullpath)
{
    try
    {
        auto game_root_path = boost::filesystem::canonical(global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH));
        auto player_save = global_settings::Instance().GetSetting(DEFAULT_BASE_SAVE_PATH);
        
        std::string first_letter = boost::to_lower_copy(pname).substr(0,1);
        
        auto patha = boost::filesystem::weakly_canonical(game_root_path/player_save/first_letter);
        
                //Check if path is within web_root_path
        if(std::distance(game_root_path.begin(), game_root_path.end())>std::distance(patha.begin(), patha.end()) ||
            !std::equal(game_root_path.begin(), game_root_path.end(), patha.begin()))
            throw std::invalid_argument("path must be within root path");
        //if(!boost::filesystem::is_directory(patha))
        //    throw std::invalid_argument("must be a directory");
        if(!(boost::filesystem::exists(patha) ))// && boost::filesystem::is_regular_file(path)))
            boost::filesystem::create_directory(patha);
            
        auto pathb=boost::filesystem::weakly_canonical(game_root_path/player_save/first_letter/pname);
        if(!(boost::filesystem::exists(pathb) ))// && boost::filesystem::is_regular_file(path)))
            boost::filesystem::create_directory(pathb); 
            
        fullpath = pathb.string();
    }
    catch( std::exception& ex )
    {
        // TODO: report error to log
        return false;
    }

    return true;
}

bool fs_manager::get_account_save_dir(const std::string& aname, std::string& fullpath)
{
    try
    {
        auto game_root_path = boost::filesystem::canonical(global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH));
        auto account_save = global_settings::Instance().GetSetting(DEFAULT_ACCOUNTS_PATH);
        
        std::string first_letter = boost::to_lower_copy(aname).substr(0,1);
        
        auto patha = boost::filesystem::weakly_canonical(game_root_path/account_save/first_letter);
        
                //Check if path is within web_root_path
        if(std::distance(game_root_path.begin(), game_root_path.end())>std::distance(patha.begin(), patha.end()) ||
            !std::equal(game_root_path.begin(), game_root_path.end(), patha.begin()))
            throw std::invalid_argument("path must be within root path");
        //if(!boost::filesystem::is_directory(patha))
        //    throw std::invalid_argument("must be a directory");
        if(!(boost::filesystem::exists(patha) ))// && boost::filesystem::is_regular_file(path)))
            boost::filesystem::create_directory(patha);
            
        auto pathb=boost::filesystem::weakly_canonical(game_root_path/account_save/first_letter/aname);
        if(!(boost::filesystem::exists(pathb) ))// && boost::filesystem::is_regular_file(path)))
            boost::filesystem::create_directory(pathb); 
            
        fullpath = pathb.string();
    }
    catch( std::exception& ex )
    {
        // TODO: report error to log
        return false;
    }

    return true;    
}
