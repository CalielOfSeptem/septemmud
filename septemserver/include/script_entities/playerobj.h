// ==========================================================================
// playerobj.h
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
#ifndef PLAYER_OBJ_H_
#define PLAYER_OBJ_H_
 

#include "script_entities/livingentity.h"
#include "script_entities/container_base.h"
#include "account.h"

struct client;


namespace pt = boost::posix_time;


struct playerobj : living_entity 
{
    playerobj();
    playerobj(sol::this_state ts, sol::this_environment te, std::string name);
     
    ~playerobj();
    
    virtual void debug(const std::string& msg) override;
    
    void SendToEntity(const std::string& msg) override;
     
    void SendToEnvironment(const std::string& msg) override;
     
    bool isCreator();
     
    bool isArch();     
     
    std::string& GetPlayerName();
     
    void set_playerName( const std::string& aname );
     
    bool verify_password( const std::string& aname, const std::string& maybe_password );
     
    virtual void on_environment_change(EnvironmentChangeEvent evt, script_entity * env) override;
     
     
    AccountType get_accountType();
    
    void set_accountType(AccountType atype);
    
    std::string get_workspacePath();
    
    bool get_loggedIn();

    void set_loggedIn(bool b);
    
    
    boost::posix_time::ptime get_referenceTickCount();
    
        
    void set_referenceTickCount();
    
    void incrementCmdCount();
    
    void resetCmdCount();
    
    unsigned long get_commandCount();
    
    unsigned long get_maxCmdCount();
    
    void set_client(client* c);
    
    client* get_client();
    
    
    void disconnect_client();
    
    
    
    
    virtual bool do_save() override;
    virtual bool do_load() override;
    
    std::function<void(const std::string&)> onOutput;
    //Output onOutput;

    std::string roomPath; // current room the player is in
    unsigned long roomID;  // current ID of the room the player is in
    std::string cwd;
    account _ac;
    
    bool blogged_in;

private:
    boost::posix_time::ptime lastCmdTick;
    boost::posix_time::ptime tickReference;
    unsigned long commandCount;
    unsigned long maxCmdCnt;
    client * client_;
	
	bool do_inventory_validation();
};
 
#endif
