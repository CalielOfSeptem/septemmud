#include "stdafx.h"
#include "script_entities/exitobj.h"

exitobj::exitobj()
{
}

exitobj::exitobj(sol::as_table_t<std::vector<std::string> >& exit,
                 const std::string& exit_path,
                 bool bobvious)
    : exit_path(exit_path)
{
    this->bobvious = true;
    const auto& vex = exit.source;
    for(auto ex : vex) {
        this->exit.push_back(ex);
    }
}

void exitobj::SetExit(const sol::as_table_t<std::vector<std::string> >& v,
                      const std::string& exit_path,
                      bool bobvious)
{
    this->bobvious = bobvious;
    const auto& vex = v.source;
    for(auto ex : vex) {
        this->exit.push_back(ex);
    }
}

const std::vector<std::string>& exitobj::GetExit()
{
    return exit;
}

const std::string& exitobj::GetExitPath()
{
    return exit_path;
}

void exitobj::SetObvious(const bool b)
{
    bobvious = b;
}

void exitobj::SetExitPath(const std::string& exit_path)
{
    this->exit_path = exit_path;
}

void exitobj::SetExitDesc(const sol::as_table_t<std::vector<std::string> >& v)
{
    this->exit_path = exit_path;
    const auto& vex = v.source;
    for(auto ex : vex) {
        this->exit.push_back(ex);
    }
}
