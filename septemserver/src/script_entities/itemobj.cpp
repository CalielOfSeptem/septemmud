#include "stdafx.h"
#include "script_entities/itemobj.h"
#include "entity_manager.h"
#include "string_utils.h"
#include "fs/fs_manager.h"


#include "json.hpp"

namespace fs = boost::filesystem;

using json = nlohmann::json;

itemobj::itemobj(sol::this_state ts, sol::this_environment te, std::string name, ItemType itype)
    : script_entity(ts, te, EntityType::ITEM, name)
{
    SetName(name);
    entity_manager::Instance().register_item(this);
}

void itemobj::on_environment_change(EnvironmentChangeEvent evt, script_entity* env)
{
    if(evt == EnvironmentChangeEvent::REMOVED) {
        try {
            this->remove_settings_file();

            // if this is a container, remove their files as well..

            for(auto it : this->GetItems()) {
                it->remove_settings_file();
                std::string l = "";
                it->set_entityStorageLocation(l);
            }

        } catch(std::exception& ex) {
            debug(ex.what());
        }
    } else if(evt == EnvironmentChangeEvent::ADDED) {
        std::string location = env->get_entityStorageLocation();
        this->set_entityStorageLocation(location);

        if(!location.empty()) {
            do_load();
            set_isInitialized(true);

            for(auto it : this->GetItems()) {
                it->set_entityStorageLocation(location);
                it->do_load();
            }
        }

        // do_save();
    }
	script_entity::on_environment_change(evt, env);
}

bool itemobj::do_load()
{
    if(this->get_entityStorageLocation().empty())
        return false;

    try {
        if(!fs::exists(this->get_entityStorageLocation() + "/" + this->uid)) {
            this->do_save();
            return true;
        }
        std::ifstream i(this->get_entityStorageLocation() + "/" + this->uid);
        json j;
        i >> j;
        _load_from_json_(j);

    } catch(std::exception& ex) {
        this->debug(ex.what());
    }
    return true;
}

bool itemobj::_load_from_json_(json& j)
{
    SetLook(j["look"]);
    SetName(j["name"]);

    set_weight(j["weight"]);
    set_size(j["size"]);
    set_itemType(j["item_type"]);

    set_defaultStackSize(j["defaultStackSize"]);
    set_currentStackCount(j["currentStackCount"]);
    // set_isWearable(j["isWearable"]);
    // set_isStackable(j["isStackable"]);
    set_isContainer(j["isContainer"]);
    set_pluralName(j["pluralName"]);

    set_isOpen(j["isOpen"]);
    set_isLocked(j["isLocked"]);
    set_isLockable(j["isLockable"]);

    const json& inventory = j["inventory"]; //<<<< this bit was hard to figure out
    for(auto& element : json::iterator_wrapper(inventory)) {
        if(element.key().size() > 0 && element.value() != NULL) {
            // std::cout << element.key() << " maps to " << element.value() << std::endl;
            std::string s1 = element.key();
            std::string s2 = element.value();
            if(entity_manager::Instance().clone_item(s2, dynamic_cast<script_entity*>(this), s1) == NULL) {
                std::string err = "Error while attempting to clone item.";
                this->debug(err);
            }
        }
    }
    const json& uprops = j["userProps"]; //<<<< this bit was hard to figure out
    for(auto& element : json::iterator_wrapper(uprops)) {
        if(element.key().size() > 0 && element.value() != NULL) {
            std::string s1 = element.key();
            std::string s2 = element.value();
            // SaveProperty(s1, )
            this->userProps[s1] = s2;
        }
    }
    return true;
}

bool itemobj::do_save()
{
    if(this->get_entityStorageLocation().size() == 0)
        return false;

    try {
        std::string j = Serialize();
        std::ofstream o(this->get_entityStorageLocation() + "/" + this->get_uid());
        // o << std::setw(4) << j << std::endl;
        o << j << std::endl;
        // std::cout << j << std::endl;
    } catch(std::exception& ex) {
        std::stringstream ss;
        ss << "Error during save: " << ex.what();
        this->debug(ss.str());
        return false;
    }

    return true;
}

bool itemobj::do_json_load(std::string& j)
{
    if(j.empty())
        return false;
    json js = json::parse(j);
    return _load_from_json_(js);
}

bool itemobj::AddEntityToInventory(script_entity* se)
{
    bool b = container_base::AddEntityToInventory(se);
    this->do_save();
    return b;
}

bool itemobj::RemoveEntityFromInventory(script_entity* se)
{
    bool b = container_base::RemoveEntityFromInventory(se);
    this->do_save();
    return b;
}

std::vector<itemobj*> itemobj::GetItems()
{
    std::vector<script_entity*> ents = GetInventory();
    std::vector<itemobj*> t_items;
    for(auto i : ents) {
        t_items.push_back(dynamic_cast<itemobj*>(i));
    }
    return t_items;
}

bool itemobj::remove_settings_file()
{
    if(this->get_entityStorageLocation().empty())
        return false;
    try {
        if(fs::exists(this->get_entityStorageLocation() + "/" + this->get_uid()))
            fs::remove(this->get_entityStorageLocation() + "/" + this->get_uid());
    } catch(std::exception& ex) {
        this->debug(ex.what());
        return false;
    }
    return true;
}

std::string itemobj::Serialize()
{
    json j;
    j["uid"] = this->get_uid();
    j["physical_script_path"] = GetPhysicalScriptPath();
    j["virtual_script_path"] = GetVirtualScriptPath();
    j["look"] = GetLook();
    j["name"] = script_entity::GetName();

    j["weight"] = get_weight();
    j["size"] = get_size();
    j["item_type"] = get_itemType();
    j["defaultStackSize"] = get_defaultStackSize();
    j["currentStackCount"] = get_currentStackCount();
    j["isWearable"] = get_isWearable();
    j["isStackable"] = get_isStackable();
    j["isContainer"] = get_isContainer();
    j["pluralName"] = get_pluralName();

    j["isOpen"] = get_isOpen();
    j["isLocked"] = get_isLocked();
    j["isLockable"] = get_isLockable();

    std::vector<script_entity*> items = this->GetInventory();
    std::map<std::string, std::string> item_objs;
    for(auto i : items) {
        itemobj* obj = dynamic_cast<itemobj*>(i);
        item_objs[obj->get_uid()] = i->GetPhysicalScriptPath();
    }

    j["inventory"] = item_objs;
    j["userProps"] = userProps;
    return j.dump(4);
}

ItemType itemobj::get_itemType()
{
    return m_type;
}

void itemobj::set_itemType(ItemType t)
{
    m_type = t;
    do_save();
}

bool itemobj::get_isWearable()
{
    return (m_slot_mask == InventorySlot::NONE) ? false : true;
}

bool itemobj::get_isStackable()
{
    return (m_defaultStackSize == 0) ? false : true;
}

itemobj::itemobj()
{
}

script_entity* itemobj::GetOwner()
{
    return this;
}

double itemobj::get_weight()
{
    return m_weight;
}

void itemobj::set_weight(double d)
{
    m_weight = d;
    do_save();
}

ItemSize itemobj::get_size()
{
    return m_size;
}

void itemobj::set_size(ItemSize is)
{
    m_size = is;
    do_save();
}

bool itemobj::get_isContainer()
{
    return bisContainer;
}

void itemobj::set_isContainer(bool b)
{
    bisContainer = b;
    do_save();
}

bool itemobj::get_isLockable()
{
    return bisLockable;
}

void itemobj::set_isLockable(bool b)
{
    bisLockable = b;
    do_save();
}

bool itemobj::get_isCloseable()
{
    return bisCloseable;
}

void itemobj::set_isCloseable(bool b)
{
    bisCloseable = b;
    do_save();
}

bool itemobj::get_isOpen()
{
    return bisOpen;
}

void itemobj::set_isOpen(bool b)
{
    bisOpen = b;
    do_save();
}

void itemobj::set_isLocked(bool b)
{
    bisLocked = b;
    do_save();
}

bool itemobj::get_isInitialized()
{
    return bisInitialized;
}

void itemobj::set_isInitialized(bool b)
{
    bisInitialized = b;
    do_save();
}

std::string itemobj::get_itemArticle()
{
    return itemArticle;
}

void itemobj::set_itemArticle(std::string article)
{
    itemArticle = article;
    do_save();
}

std::string itemobj::get_itemNoun()
{
    return itemNoun;
}

void itemobj::set_itemNoun(std::string noun)
{
    itemNoun = noun;
    do_save();
}

std::string itemobj::get_itemAdjectives()
{
    return itemAdjectives;
}

void itemobj::set_itemAdjectives(std::string adjectives)
{
    itemAdjectives = adjectives;
    do_save();
}

const std::string& itemobj::get_pluralName()
{
    return pluralName;
}

void itemobj::set_pluralName(std::string pluralName)
{
    this->pluralName = pluralName;
    do_save();
}

std::string itemobj::get_itemPluralNoun()
{
    std::vector<std::string> parts = split(pluralName, ' ');
    if(parts.size() == 0)
        return "";
    return parts[parts.size() - 1];
}

void itemobj::SetName(const std::string& name)
{
    std::vector<std::string> parts = split(name, ' ');
    assert(parts.size() != 0);

    // the first part should be 'a' or 'an'
    std::string article = boost::to_lower_copy(parts[0]);
    if(article != "a" && article != "an") {
        // we're going to just add an article, it may have been omitted on accident..
        this->set_itemArticle("a");
    } else {
        this->set_itemArticle(article);
    }
    // now we need to know if there are adjectives..
    // adjectives always follow the article
    if(parts.size() == 2) {
        // no adjectives.. pull the noun out
        this->set_itemNoun(parts[1]);
    } else {
        for(unsigned int x = 1; x < parts.size() - 1; x++) {
            this->itemAdjectives = this->itemAdjectives + parts[x] + " ";
        }
        trim(this->itemAdjectives); // remove extra white spaces..
        this->set_itemNoun(parts[parts.size() - 1]);
    }
    script_entity::SetName(name);
    do_save();
}

const std::string& itemobj::GetName()
{
    if(this->get_isStackable() && this->get_currentStackCount() > 1) {
        return this->get_pluralName();
    } else {
        return script_entity::GetName();
    }
}

int itemobj::get_defaultStackSize()
{
    return this->m_defaultStackSize;
}

void itemobj::set_defaultStackSize(int stackSize)
{
    m_defaultStackSize = stackSize;
    do_save();
}

int itemobj::get_currentStackCount()
{
    return this->m_currentStackCount;
}

void itemobj::set_currentStackCount(int stackCount)
{
    m_currentStackCount = stackCount;
    do_save();
}

int itemobj::get_inventorySlot()
{
    return this->m_slot_mask;
}

void itemobj::set_inventorySlot(int slot)
{
    m_slot_mask = slot;
    do_save();
}

void itemobj::incrementStackCount()
{
    m_currentStackCount++;
    do_save();
}

bool itemobj::SaveProperty(const std::string& name, sol::object ob)
{
    auto maybe_string = ob.as<sol::optional<std::string> >();
    if(maybe_string) {
        userProps[name] = *maybe_string;
        return true;
    }

    auto maybe_int = ob.as<sol::optional<int> >();
    if(maybe_int) {
        std::string s = std::to_string(*maybe_int);
        userProps[name] = s;
        return true;
    }
    return false;
}

void itemobj::decrementStackCount()
{
    if(m_currentStackCount == 1) {
        return;
    } else {
        m_currentStackCount--;
        do_save();
    }
}

sol::object itemobj::GetProperty(const std::string& name, sol::this_state L)
{
    for(auto kvp : this->userProps) {
        if(kvp.first == boost::to_lower_copy(name)) {
            return sol::object(L, sol::in_place, kvp.second); // kvp.second;
        }
    }
    return sol::object(L, sol::in_place, sol::lua_nil);
}

void itemobj::debug(const std::string& msg)
{
    if(this->GetEnvironment()) {
        this->GetEnvironment()->debug(msg);
    }
}

bool itemobj::get_isLocked()
{
    return bisLocked;
}
