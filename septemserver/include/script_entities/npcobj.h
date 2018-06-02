// ==========================================================================
// npcobj.h
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
#ifndef NPC_OBJ_H_
#define NPC_OBJ_H_
 

#include "script_entities/livingentity.h"
#include "script_entities/container_base.h"


struct npcobj : living_entity 
{
    npcobj();
    npcobj(sol::this_state ts, sol::this_environment te, std::string name);
     
    ~npcobj();
    
    virtual void debug(const std::string& msg) override;
    
    void SendToEntity(const std::string& msg) override;
     
    void SendToEnvironment(const std::string& msg) override;
     
    virtual void on_environment_change(EnvironmentChangeEvent evt, script_entity * env) override;
    
    std::function<void(const std::string&)> onOutput;
    
    virtual std::string get_npcArticle();
     
    virtual void set_npcArticle(std::string article);
    
    virtual std::string get_npcNoun();
     
    virtual void set_npcNoun(std::string noun);
    
    virtual std::string get_npcAdjectives();
     
    virtual void set_npcAdjectives(std::string adjectives);
    
    virtual void SetName(const std::string& name) override;
     
    virtual const std::string& GetName() override;
    //Output onOutput;

    // TODO: move this stuff to living
    std::string roomPath; // current room the npc is in
    unsigned long roomID;  // current ID of the room the npc is in
    
    std::string npcNoun;
    std::string npcArticle;
    std::string npcAdjectives;
};
 
#endif
