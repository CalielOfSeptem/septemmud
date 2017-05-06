#include "script_entities/livingentity.h"
#include "entity_manager.h"

roomobj* living_entity::GetRoom()
{
    assert(GetEnvironment() != NULL);
    return dynamic_cast<roomobj*>(GetEnvironment());
}
/*

void lua_stacktrace(lua_State* L)
{


    lua_Debug info;
    int level = 0;
    while (lua_getstack(L, level, &info)) {
       lua_getinfo(L, "nSlf", &info);
      // const char *lua_getupvalue(L, 0, 0);

       // fprintf(stderr, "  [%d] %s:%d -- %s [%s]\n",
       //     level, info.short_src, info.currentline,
       //     (info.name ? info.name : "<unknown>"), info.what);
        //std::cout << info.source << std::endl;

        lua_getuservalue(L, 1);
        sol::environment en = sol::environment(L, 1);
        if( en.valid() )
        {
            sol::optional<std::string> s = en["_INTERNAL_SCRIPT_PATH_"];
            if( s )
            {
                std::cout << s.value() << std::endl;
            }
        }


        ///auto e = getfenv(1);

        ++level;
    }
    
    lua_Debug entry;
    int depth = 0; 

    while (lua_getstack(L, depth, &entry))
    {
        int status = lua_getinfo(L, "Sln", &entry);
        assert(status);

        printf("%s(%d): %s\n", entry.short_src, entry.currentline, entry.name ? entry.name : "?");
        printf("%s", entry.source);
        getfenv ()
        depth++;
    }
     
}
*/
bool living_entity::DoCommand(sol::this_state ts, const std::string& cmd)
{
 /*   
    lua_State* L = ts;
    lua_stacktrace(L);
    sol::object selfobj = sol::object(L, 1);
    script_entity& self = selfobj.as<script_entity>();
*/

    //return NULL;
    entity_manager::Instance().do_command( this, cmd );
     
}
