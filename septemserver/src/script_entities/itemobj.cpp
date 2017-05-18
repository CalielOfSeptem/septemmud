#include "script_entities/itemobj.h"
#include <random>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

namespace {
std::string const default_chars = 
    "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
}

std::string random_string(size_t len = 15, std::string const &allowed_chars = default_chars) {
    std::mt19937_64 gen { std::random_device()() };

    std::uniform_int_distribution<size_t> dist { 0, allowed_chars.length()-1 };

    std::string ret;

    std::generate_n(std::back_inserter(ret), len, [&] { return allowed_chars[dist(gen)]; });
    return ret;
}

itemobj::itemobj(sol::this_state ts, sol::this_environment te, std::string name) :
    script_entity(ts, te, EntityType::ITEM, name)
{
    std::cout << random_string() << "\n";
   // std::cout << random_string(10, "abcdefghijklmnopqrstuvwxyz");
}
     

itemobj::itemobj(sol::this_state ts, sol::this_environment te) :
    script_entity(ts, te, EntityType::ITEM, "")
{
    std::cout << random_string() << "\n";
   // std::cout << random_string(10, "abcdefghijklmnopqrstuvwxyz");
}
     