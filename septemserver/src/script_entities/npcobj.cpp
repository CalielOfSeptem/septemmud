#include "stdafx.h"
#include "script_entities/npcobj.h"
#include "entity_manager.h"

npcobj::npcobj(sol::this_state ts, sol::this_environment te, std::string name)
    : living_entity(ts, te, EntityType::NPC, name)
{
    SetName(name);
    entity_manager::Instance().register_npc(this);
}

npcobj::~npcobj()
{
    unload_inventory_from_game();
}


void npcobj::debug(const std::string& msg)
{

}

void npcobj::SendToEntity(const std::string& msg)
{
    // if( GetName() == "caliel" )
    //    std::cout << msg << std::endl;
    if(this->onOutput)
        this->onOutput(msg);
    living_entity::SendToEntity(msg);
}

void npcobj::SendToEnvironment(const std::string& msg)
{
    living_entity::SendToEnvironment(msg);
}


void npcobj::on_environment_change(EnvironmentChangeEvent evt, script_entity* env)
{
	script_entity::on_environment_change(evt, env);
}

npcobj::npcobj()
{
}


std::string npcobj::get_npcArticle()
{
    return npcArticle;
}

void npcobj::set_npcArticle(std::string article)
{
    npcArticle = article;
}

std::string npcobj::get_npcNoun()
{
    return npcNoun;
}

void npcobj::set_npcNoun(std::string noun)
{
    npcNoun = noun;
}

std::string npcobj::get_npcAdjectives()
{
    return npcAdjectives;
}

void npcobj::set_npcAdjectives(std::string adjectives)
{
    npcAdjectives = adjectives;
}

void npcobj::SetName(const std::string& name)
{
    std::vector<std::string> parts = split(name, ' ');
    assert(parts.size() != 0);

    // the first part should be 'a' or 'an'
    std::string article = boost::to_lower_copy(parts[0]);
    if(article != "a" && article != "an") {
        // we're going to just add an article, it may have been omitted on accident..
        this->set_npcArticle("a");
    } else {
        this->set_npcArticle(article);
    }
    // now we need to know if there are adjectives..
    // adjectives always follow the article
    if(parts.size() == 2) {
        // no adjectives.. pull the noun out
        this->set_npcNoun(parts[1]);
    } else {
        for(unsigned int x = 1; x < parts.size() - 1; x++) {
            this->npcAdjectives = this->npcAdjectives + parts[x] + " ";
        }
        trim(this->npcAdjectives); // remove extra white spaces..
        this->set_npcNoun(parts[parts.size() - 1]);
    }
    script_entity::SetName(name);
}

const std::string& npcobj::GetName()
{

    return script_entity::GetName();
    
}