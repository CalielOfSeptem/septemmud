#ifndef EXITOBJ_H_
#define EXITOBJ_H_

struct exitobj
{
    exitobj()
    {
        
    }
    exitobj(sol::as_table_t<std::vector<std::string>>& exit, const std::string& exit_path, bool bobvious)
        : exit_path(exit_path)
    {
        this->bobvious = true;
        const auto& vex = exit.source;
        for( auto ex : vex )
        {
            this->exit.push_back(ex);
        }
    }
    
    void SetExit( const sol::as_table_t<std::vector<std::string>>& v, const std::string& exit_path, bool bobvious)
    {
        this->bobvious = bobvious;
        const auto& vex = v.source;
        for( auto ex : vex )
        {
            this->exit.push_back(ex);
        }
    }
    const std::vector<std::string>& GetExit()
    {
        return exit;
    }
    
    const std::string& GetExitPath()
    {
        return exit_path;
    }
    
    void SetObvious(const bool b)
    {
        bobvious = b;
    }
    
    void SetExitPath( const std::string& exit_path )
    {
        this->exit_path = exit_path;
    }
    
    void SetExitDesc( const sol::as_table_t<std::vector<std::string>>& v )
    {
        this->exit_path = exit_path;
        const auto& vex = v.source;
        for( auto ex : vex )
        {
            this->exit.push_back(ex);
        }
    }
    



private:
    std::string exit_path; // path the script the exit is linked to
    std::string bobvious; // whether the exit is shown by default
    std::vector<std::string> exit;
};

#endif