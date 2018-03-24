// ==========================================================================
// account.h
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
#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

enum AccountType { UNKNOWN_TYPE, PLAYER, CREATOR, ARCH };


class account
{
public:
    account(const std::string& playername);
    account() {};
    
    //~account();

    
    std::string get_accountStorageLocation();
   
    static std::string AccountTypeToString(AccountType at);
    
    bool do_compare(std::string pass);
    
    bool do_load();
    
    bool do_save();
    
    std::string _playername;
    std::string _email;
    std::string _secure_password;
    
    std::string _lastLogon;
    AccountType _accountType;
    nlohmann::json _j;
    
    std::string _cwd; // current directory the player is targetting
    std::string _workspacePath;  // path to the player's workspace
};

#endif // ACCOUNT_HPP
