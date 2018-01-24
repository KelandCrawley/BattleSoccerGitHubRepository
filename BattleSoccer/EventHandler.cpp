#include "EventHandler.h"



EventHandler::EventHandler()
{
}


EventHandler::~EventHandler()
{
}

void EventHandler::Initialize()
{
 /*
    Initialization of events
    Each event gets an empty vector to hold its registered delegates
 */

    std::vector<std::function<void(int)>> Container;
    EventsMap.emplace(EventType::WindowClosed, Container);
    EventsMap.emplace(EventType::MainMenu_LeftClickPressed, Container);
    EventsMap.emplace(EventType::MainMenu_RightClickPressed, Container);
    EventsMap.emplace(EventType::PreMatchMenu_LeftClickPressed, Container);
    EventsMap.emplace(EventType::PreMatchMenu_RightClickPressed, Container);
    EventsMap.emplace(EventType::SettingsMenu_LeftClickPressed, Container);
    EventsMap.emplace(EventType::EscapePressed, Container);
    EventsMap.emplace(EventType::SettingsButtonPressed, Container);
    EventsMap.emplace(EventType::GameVolumeDown, Container);
    EventsMap.emplace(EventType::GameVolumeUp, Container);
    EventsMap.emplace(EventType::MusicVolumeUp, Container);
    EventsMap.emplace(EventType::MusicVolumeDown, Container);
    EventsMap.emplace(EventType::BindingChanged, Container);
    EventsMap.emplace(EventType::SwitchedPlayer, Container);
    EventsMap.emplace(EventType::SpecialShootBall, Container);
    EventsMap.emplace(EventType::AbilityUsed, Container);
    EventsMap.emplace(EventType::SwitchToClosestPlayer, Container);
    EventsMap.emplace(EventType::MoveUpToggled, Container);
    EventsMap.emplace(EventType::MoveDownToggled, Container);
    EventsMap.emplace(EventType::MoveRightToggled, Container);
    EventsMap.emplace(EventType::MoveLeftToggled, Container);
    EventsMap.emplace(EventType::ShootBall, Container);
    EventsMap.emplace(EventType::PassBall, Container);
    EventsMap.emplace(EventType::MatchSession_LeftClickPressed, Container);
    EventsMap.emplace(EventType::GoalScored, Container);
    EventsMap.emplace(EventType::BallShot, Container);
    EventsMap.emplace(EventType::PlayerTackled, Container);
    EventsMap.emplace(EventType::BallPassed, Container);
    EventsMap.emplace(EventType::TackleMissed, Container);
    EventsMap.emplace(EventType::BallHitWall, Container);
    EventsMap.emplace(EventType::CountDownStarted , Container);
    EventsMap.emplace(EventType::NextFormation, Container);
    EventsMap.emplace(EventType::PreviousFormation, Container);
    EventsMap.emplace(EventType::UserTeamChanged, Container);
    EventsMap.emplace(EventType::ResetData, Container);
    EventsMap.emplace(EventType::MatchStarted, Container);
    EventsMap.emplace(EventType::MatchEnded, Container);
    EventsMap.emplace(EventType::AbortMatch, Container);
    EventsMap.emplace(EventType::PreMatchStarted, Container);
    EventsMap.emplace(EventType::SeasonComplete, Container);
    EventsMap.emplace(EventType::NoEvent, Container);
    EventsMap.emplace(EventType::QuitGame, Container);





}
void EventHandler::HandleEvent(EventType i_Type, int Arg)
{
    // all events that can be passed, should be in the map therefore no exceptions should be thrown
    auto Iterator = EventsMap.find(i_Type);
    if (Iterator != EventsMap.end())
    {
        for (auto i = 0; i < Iterator->second.size(); i++)
        {
            Iterator->second.at(i)(Arg);
        }
    }
    return;
}

void EventHandler::RegisterDelegate(std::function<void(int)> i_Delegate, EventType i_Type)
{
    // all events that can be passed, should be in the map therefore no exceptions should be thrown
    auto Iterator = EventsMap.find(i_Type);
    Iterator->second.push_back(i_Delegate);

}