#include "stdafx.h"
#include "script_entities/lookobj.h"

lookobj::lookobj(const sol::as_table_t<std::vector<std::string> >& v, const std::string& description)
{
    this->description = description;
    const auto& vex = v.source;
    for(auto ex : vex) {
        this->look.push_back(ex);
    }
}

const std::vector<std::string>& lookobj::GetLook()
{
    return look;
}

const std::string& lookobj::GetDescription()
{
    return description;
}
