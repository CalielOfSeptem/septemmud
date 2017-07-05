#ifndef LOOKOBJ_H_
#define LOOKOBJ_H_

struct lookobj
{
    lookobj( const sol::as_table_t<std::vector<std::string>>& v, const std::string& description )
    {
        this->description = description;
        const auto& vex = v.source;
        for( auto ex : vex )
        {
            this->look.push_back(ex);
        }
    }
    const std::vector<std::string>& GetLook()
    {
        return look;
    }
    const std::string& GetDescription()
    {
        return description;
    }
    
private:
    std::string description;
    std::vector<std::string> look;
};

#endif