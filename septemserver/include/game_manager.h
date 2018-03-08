// ==========================================================================
// game_manager.h
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
#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include "entity_manager.h"
#include "script_entities/script_entity.h"
#include "script_entities/daemonobj.h"

namespace ba = boost::asio;

enum class gameState { STOPPED, INITIALIZING, RUNNING, STOPPING, ERROR };

struct game_manager
{
    game_manager()
    {
        m_state = gameState::STOPPED;
    }
    
    bool start()
    {
        return SetState( gameState::INITIALIZING );
    }
    
    bool stop()
    {
        return SetState( gameState::STOPPING );
    }
    
    void do_heartbeats()
    {
        entity_manager::Instance().invoke_heartbeat();
    }
    
    bool move_entity_into_room( script_entity* en, std::string room_path );
    
   // roomobj* get_void_room();

    
    bool process_player_cmd(playerobj* p, std::string& cmd);
    
    bool load_room_cache( std::vector<std::string>& cache, std::string& reason );
    
private:
    gameState m_state;
    
    
    bool SetState( gameState new_state )
    {
        auto log = spd::get("main");//>info("loggers can be retrieved from a global registry using the spdlog::get(logger_name) function");

        if( new_state == gameState::INITIALIZING )
        {
            if( m_state != gameState::STOPPED || m_state == gameState::INITIALIZING )
            {
                // do nothing if we aren't stopped, and
                // also do nothing if the state is already initializing
                return false;
            }
            else
            {
                m_state = new_state;
                log->debug("Game state changed to INITIALIZING");
                init();
                return true;
            }
        }
        else if( new_state == gameState::STOPPING )
        {
            if( m_state != gameState::RUNNING || m_state == gameState::STOPPING )
            {
                // do nothing if we aren't running, and
                // also do nothing if the state is already stopping
                return false;
            }
            else
            {
                m_state = new_state;
                log->debug("Game state changed to STOPPING");
                do_stop();
                return true;
            }
        }
        else if( new_state == gameState::RUNNING )
        {
            if( m_state == gameState::RUNNING )
            {
                // do nothing if we aren't running, and
                // also do nothing if the state is already stopping
                return false;
            }
            else
            {
                log->debug("Game state changed to RUNNING");
                m_state = new_state;
                return true;
            }
        }
        else if( new_state == gameState::STOPPED )
        {
            if( m_state == gameState::STOPPED )
            {
                // do nothing if we are already stopped, and
                // also do nothing if the state is already stopping
                return false;
            }
            else
            {
                m_state = new_state;
                log->debug("Game state changed to STOPPED");
                return true;
            }
        }
        else if( new_state == gameState::ERROR )
        {

            log->debug("Game state changed to ERROR");
            m_state = new_state;
            do_stop();
            
            return true;
            
        }
        assert(false);
        return false;
    }
    
    void init()
    ;
    
    void do_stop()
    {
        entity_manager::Instance().reset();
        SetState( gameState::STOPPED );
    }
    
    void on_error(std::string err)
    {
        SetState( gameState::ERROR );
    }
    

};

#endif