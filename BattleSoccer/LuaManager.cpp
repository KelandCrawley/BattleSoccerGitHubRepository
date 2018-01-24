#include "LuaManager.h"
#include "MatchSession.h"

LuaManager::LuaManager()
{
}

LuaManager::~LuaManager()
{
}


void LuaManager::Initialize()
{
    //lua initialization
    L = luabridge::luaL_newstate();
    luaL_openlibs(L);
    lua_pcall(L, 0, 0, 0);

    // player class bindings
    luabridge::getGlobalNamespace(L)
        .beginClass<Player>("Player")
        .addConstructor<void(*)(void)>()
        .addFunction("GetPlayerID", &Player::GetPlayerID)
        .addFunction("GetTeamNumber", &Player::GetTeamNumber)
        .addFunction("HoldPosition", &Player::HoldPosition)
        .endClass();
   
    // match session class bindings
    luabridge::getGlobalNamespace(L)
        .beginClass<MatchSession>("MatchSession")
        .addConstructor<void(*)(void)>()
        .addFunction("GetBallPositionX", &MatchSession::GetBallPositionX)
        .addFunction("GetBallPositionY", &MatchSession::GetBallPositionY)
        .addFunction("isClosestToBall", &MatchSession::isClosestToBall)
        .addFunction("HavePlayerSupportBall", &MatchSession::HavePlayerSupportBall)
        .addFunction("IsBallLoose", &MatchSession::IsBallLoose)
        .addFunction("GetInDefensivePosition", &MatchSession::GetInDefensivePosition)
        .addFunction("CheckIsBallCarrier", &MatchSession::CheckIsBallCarrier)
        .addFunction("AiShootBall", &MatchSession::AiShootBall)
        .addFunction("DoesBallCarrierHaveOpenShot", &MatchSession::DoesBallCarrierHaveOpenShot)
        .addFunction("DoesTeammateHaveShot", &MatchSession::DoesTeammateHaveShot)
        .addFunction("PassToTeamMateInScoringPosition", &MatchSession::PassToTeamMateInScoringPosition)
        .addFunction("isBallCarrierMarked", &MatchSession::isBallCarrierMarked)
        .addFunction("isBallCarrierAGoalie", &MatchSession::isBallCarrierAGoalie)
        .addFunction("DoesBallCarrierHaveAnOpenPass", &MatchSession::DoesBallCarrierHaveAnOpenPass)
        .addFunction("AiPassBall", &MatchSession::AiPassBall)
        .addFunction("SendPlayerToAttackingPosition", &MatchSession::SendPlayerToAttackingPosition)
        .addFunction("GetDistanceToBall", &MatchSession::GetDistanceToBall)
        .addFunction("HavePlayerChaseLooseBall", &MatchSession::HavePlayerChaseLooseBall)
        .addFunction("AITackle", &MatchSession::AITackle)
        .addFunction("AdvanceBall", &MatchSession::AdvanceBall)
        .addData("TeamWithBall", &MatchSession::TeamWithBall)
        .endClass(); 

    //RNG Class Bindings
    luabridge::getGlobalNamespace(L)
        .beginClass<RNGClass>("RNGClass")
        .addConstructor<void(*)(void)>()
        .addStaticFunction("GetRandomNumber", &RNGClass::GetRandomInteger)
        .endClass();

    // sfml Class Bindings
    luabridge::getGlobalNamespace(L)
        .beginClass<sf::FloatRect>("FloatRect")
        .addConstructor<void(*)(void)>()
        .endClass();
    
    // Player Manager script initializations
    if (luaL_dofile(L, "PlayerManager.lua") == 0)
    { // script has opened
        luabridge::LuaRef table = luabridge::getGlobal(L, "PlayerManager");
        LuaPlayerManager = std::make_shared<luabridge::LuaRef>(table);
        if ((*LuaPlayerManager).isTable())
        {
            if ((*LuaPlayerManager)["Initiate"].isFunction())
            {
                try
                {
                    (*LuaPlayerManager)["Initiate"]((*LuaPlayerManager));
                    std::cout << "LuaPlayerManager Set" << std::endl;
                }
                catch (luabridge::LuaException const& e)
                {
                    std::cout << "LuaException: " << e.what() << std::endl;
                }
    
            }
           
        }
        else
        {
            std::cout << "LuaPlayerManager not Set" << std::endl;
        }
    }


}


void LuaManager::UpdateLuaPlayerManager( MatchSession *i_Session)
{
    if ((*LuaPlayerManager).isTable())
    {
        if ((*LuaPlayerManager)["ChooseBestStates"].isFunction())
        {
            try
            {
                (*LuaPlayerManager)["ChooseBestStates"]((*LuaPlayerManager), i_Session);
            }
            catch (luabridge::LuaException const& e)
            {
                std::cout << "LuaException: " << e.what() << std::endl;
            }

        }

    }
    else
    {
        std::cout << "luaPlayerManager not Set" << std::endl;
    }
}

void LuaManager::RegisterPlayerWithLua(std::shared_ptr<Player> i_Player)
{
    if ((*LuaPlayerManager).isTable())
    {
        if ((*LuaPlayerManager)["CreateNewPlayer"].isFunction())
        {
            try
            {
                (*LuaPlayerManager)["CreateNewPlayer"]((*LuaPlayerManager), i_Player.get());

            }
            catch (luabridge::LuaException const& e)
            {
                std::cout << "LuaException: " << e.what() << std::endl;
            }
        }

    }
}