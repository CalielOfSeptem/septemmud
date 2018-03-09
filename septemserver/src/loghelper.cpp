#include "stdafx.h"
#include "loghelper.h"
#include "security_context.hpp"
#include "script_entities/itemobj.h"
#include "script_entities/livingentity.h"
#include "script_entities/playerobj.h"
#include "script_entities/roomobj.h"
#include "script_entities/script_entity.h"

void my_sink::SendToEntity(const spd::details::log_msg& msg, script_entity* se)
{

    if(se->GetType() == EntityType::PLAYER) {
        playerobj* po = dynamic_cast<playerobj*>(se);
        if(po->isCreator()) {
            po->SendToEntity(msg.formatted.str());
        }
    } else if(se->GetType() == EntityType::ITEM) {
        itemobj* io = dynamic_cast<itemobj*>(se);
        script_entity* se_tmp = io->GetEnvironment();
        while(io->GetEnvironment()) {
            SendToEntity(msg, se_tmp);
            se_tmp = io->GetEnvironment();
        }
    } else if(se->GetType() == EntityType::NPC) {

    } else if(se->GetType() == EntityType::ROOM) {
        roomobj* ro = dynamic_cast<roomobj*>(se);
        std::string pl = "";
        auto players = ro->GetPlayers(pl);
        for(auto i : players) {
            playerobj* po = dynamic_cast<playerobj*>(i);
            if(po->isCreator()) {
                po->SendToEntity(msg.formatted.str());
            }
        }
    }
}

void my_sink::log(const spd::details::log_msg& msg)
{
    // Your code here.
    // details::log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
    // msg.formatted contains the formatted log.
    // msg.raw contains pre formatted log
    // std::cout << msg.formatted.str();
    if(msg.level == spdlog::level::err) {
        script_entity* se = security_context::Instance().GetCurrentEntity();
        SendToEntity(msg, se);
    }

    if(m_events.size() < 1000) {
        m_events.pop_front();
    }
    m_events.push_back(msg.formatted.str());
}

void my_sink::flush()
{
    // std::cout << std::flush;
}
