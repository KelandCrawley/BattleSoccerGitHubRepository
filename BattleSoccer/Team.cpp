#include "Team.h"



Team::Team()
{
    LeagueWinTotal = 0;
    LeagueLossTotal = 0;
    LifetimeWinTotal = 0;
    LifetimeLossTotal = 0;

    // initialize vector
    TeamRoster = std::make_shared<std::vector<std::shared_ptr<Player>>>();


    // pick a random formation, "TheBlitz" formation is not an option as the ai does not execute it well
    int RandomNumber = RNGClass::GetRandomInteger(1, 4);
    switch (RandomNumber)
    {
    case 1:
        TeamFormation = Team::Formation::TheBow;
        break;
    case 2:
        TeamFormation = Team::Formation::TheBox;
        break;
    case 3:
        TeamFormation = Team::Formation::TheSpear;
        break;
    case 4:
        TeamFormation = Team::Formation::TheTriangle;
        break;
    }
}


Team::~Team()
{
}

void Team::AddPlayer(std::shared_ptr<Player> i_Player)
{
    if (TeamRoster->size() < 5)
    {
        TeamRoster->push_back(i_Player);
    }
    SetPlayerPositions();
}

bool Team::DeletePlayer(std::shared_ptr<Player> i_Player)
{
    for (auto i = 0; i < TeamRoster->size(); i++)
    {
        if (TeamRoster->at(i) == i_Player)
        {
            TeamRoster->erase(TeamRoster->begin() + i);
            return true;
        }
    }
    return false;
}

void Team::AddWin()
{
    LeagueWinTotal++;
    LifetimeWinTotal++;
}

void Team::AddLoss()
{
    LeagueLossTotal++;
    LifetimeLossTotal++;
}

void Team::ClearLeagueRecord()
{
    LeagueWinTotal = 0;
    LeagueLossTotal = 0;
}

int Team::GetLeagueWins()
{
    return LeagueWinTotal;
}

int Team::GetLeagueLosses()
{
    return LeagueLossTotal;
}

std::string Team::GetTeamName()
{
    return TeamName;
}

void Team::SetTeamName(std::string i_TeamName)
{
    TeamName = i_TeamName;
}

void Team::SetFormation(Formation i_Formation)
{
    TeamFormation = i_Formation; 
    SetPlayerPositions();
}

Team::Formation Team::GetFormation()
{
    return TeamFormation;
}

void Team::SetPlayerPositions()
{

    switch (TeamFormation)
    {
    case Team::Formation::TheBox:
        if (TeamRoster->size() > 0) { TeamRoster->at(0)->SetPlayerPosition(Player::PlayerPosition::Goalie); };
        if (TeamRoster->size() > 1) { TeamRoster->at(1)->SetPlayerPosition(Player::PlayerPosition::UpperDefender); };
        if (TeamRoster->size() > 2) { TeamRoster->at(2)->SetPlayerPosition(Player::PlayerPosition::LowerDefender); };
        if (TeamRoster->size() > 3) { TeamRoster->at(3)->SetPlayerPosition(Player::PlayerPosition::UpperWing); };
        if (TeamRoster->size() > 4) { TeamRoster->at(4)->SetPlayerPosition(Player::PlayerPosition::LowerWing); };

        break;
    case Team::Formation::TheBow:
        if (TeamRoster->size() > 0) { TeamRoster->at(0)->SetPlayerPosition(Player::PlayerPosition::Goalie); };
        if (TeamRoster->size() > 1) { TeamRoster->at(1)->SetPlayerPosition(Player::PlayerPosition::UpperDefender); };
        if (TeamRoster->size() > 2) { TeamRoster->at(2)->SetPlayerPosition(Player::PlayerPosition::LowerDefender); };
        if (TeamRoster->size() > 3) { TeamRoster->at(3)->SetPlayerPosition(Player::PlayerPosition::MidFielder); };
        if (TeamRoster->size() > 4) { TeamRoster->at(4)->SetPlayerPosition(Player::PlayerPosition::CenterAttacker); };
        break;
    case Team::Formation::TheBlitz:
        if (TeamRoster->size() > 0) { TeamRoster->at(0)->SetPlayerPosition(Player::PlayerPosition::ForwardGoalie); };
        if (TeamRoster->size() > 1) { TeamRoster->at(1)->SetPlayerPosition(Player::PlayerPosition::CenterDefender); };
        if (TeamRoster->size() > 2) { TeamRoster->at(2)->SetPlayerPosition(Player::PlayerPosition::LowerWing); };
        if (TeamRoster->size() > 3) { TeamRoster->at(3)->SetPlayerPosition(Player::PlayerPosition::UpperWing); };
        if (TeamRoster->size() > 4) { TeamRoster->at(4)->SetPlayerPosition(Player::PlayerPosition::CenterAttacker); };
        break;
    case Team::Formation::TheSpear:
        if (TeamRoster->size() > 0) { TeamRoster->at(0)->SetPlayerPosition(Player::PlayerPosition::Goalie); };
        if (TeamRoster->size() > 1) { TeamRoster->at(1)->SetPlayerPosition(Player::PlayerPosition::CenterDefender); };
        if (TeamRoster->size() > 2) { TeamRoster->at(2)->SetPlayerPosition(Player::PlayerPosition::MidFielder); };
        if (TeamRoster->size() > 3) { TeamRoster->at(3)->SetPlayerPosition(Player::PlayerPosition::UpperWing); };
        if (TeamRoster->size() > 4) { TeamRoster->at(4)->SetPlayerPosition(Player::PlayerPosition::LowerWing); };
        break;
    case Team::Formation::TheTriangle:
        if (TeamRoster->size() > 0) { TeamRoster->at(0)->SetPlayerPosition(Player::PlayerPosition::Goalie); };
        if (TeamRoster->size() > 1) { TeamRoster->at(1)->SetPlayerPosition(Player::PlayerPosition::CenterDefender); };
        if (TeamRoster->size() > 2) { TeamRoster->at(2)->SetPlayerPosition(Player::PlayerPosition::MidFielder); };
        if (TeamRoster->size() > 3) { TeamRoster->at(3)->SetPlayerPosition(Player::PlayerPosition::UpperWing); };
        if (TeamRoster->size() > 4) { TeamRoster->at(4)->SetPlayerPosition(Player::PlayerPosition::CenterAttacker); };
        break;
    }
}

