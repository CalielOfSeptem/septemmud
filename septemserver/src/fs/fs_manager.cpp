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
    }
    catch(const std::exception &e) {
      //  string content="Could not open path "+request->path+": "+e.what();
      //  *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
      p->SendToEntity(std::string("Error: ") +e.what());
      return false;
    }
    
    return true;
}

bool fs_manager::translate_path(std::string& relative_path, playerobj* p)
{
    try 
    {
        if( relative_path.size() == 0 )
             throw std::invalid_argument("A path must be specificed.");
             
        
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
      p->SendToEntity(std::string("Error: ") +e.what());
      return false;
    }
    
    return true;
}
