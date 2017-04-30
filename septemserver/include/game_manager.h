#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_
#include <plog/Log.h>
#include "entity_manager.h"
#include "script_entities/script_entity.h"
#include "script_entities/daemonobj.h"

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
    
    roomobj* get_void_room();
    
    daemonobj* get_command_proc();
    
    bool process_player_cmd(script_entity* p, std::string& cmd);


    
private:
    gameState m_state;
    
    bool SetState( gameState new_state )
    {
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
                LOG_DEBUG << "Game state changed to INITIALIZING";
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
                LOG_DEBUG << "Game state changed to STOPPING";
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
                LOG_DEBUG << "Game state changed to RUNNING";
                m_state = new_state;
                return true;
            }
        }
        else if( new_state == gameState::STOPPED )
        {
            if( m_state != gameState::STOPPING || m_state == gameState::STOPPED )
            {
                // do nothing if we aren't running, and
                // also do nothing if the state is already stopping
                return false;
            }
            else
            {
                m_state = new_state;
                LOG_DEBUG << "Game state changed to STOPPED";
                return true;
            }
        }
        else if( new_state == gameState::ERROR )
        {
            do_stop();
            m_state = new_state;
            LOG_DEBUG << "Game state changed to ERROR";
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