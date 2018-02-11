// ==========================================================================
// exitobj.h
//
// Copyright (C) 2018 Kenneth Thompson, All Rights Reserved.
// This file is covered by the MIT Licence:
//
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.
// ==========================================================================
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