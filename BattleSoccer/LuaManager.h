#pragma once
#include <iostream>
#include <LuaBridge.h>
#include <memory>
#include <vector>

#include "RNGClass.h"
#include "Player.h"

class MatchSession;
class LuaManager
{
    friend class MatchSession;

public:
    LuaManager();
    ~LuaManager();

    void Initialize();

    // Shared Pointer to lua table that manages players
    std::shared_ptr<luabridge::LuaRef> LuaPlayerManager;

    // called every loop
    // Class lua state manager for each player in the match
    void UpdateLuaPlayerManager( MatchSession *i_Session);

    // registers player into lua player manager table.
    void RegisterPlayerWithLua(std::shared_ptr<Player> i_Player);

private:
    luabridge::lua_State* L;

};

