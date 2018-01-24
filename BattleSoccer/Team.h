#pragma once
#include <memory>

#include "Player.h"

class Team
{
    friend class FileManager;
    friend class GameEntityManager;
public:
    Team();
    ~Team();

    //formation options
    enum class Formation{TheBow, TheBox, TheBlitz, TheSpear, TheTriangle};


    ////////////Player Roster data and functions/////////////

    std::shared_ptr<std::vector<std::shared_ptr<Player>>> TeamRoster;
    void AddPlayer(std::shared_ptr<Player> i_Player);
    bool DeletePlayer(std::shared_ptr<Player> i_Player);


    /////////// League Data and functions /////////

    void AddWin();
    void AddLoss();
    void ClearLeagueRecord();
    int GetLeagueWins();
    int GetLeagueLosses();

    ///////// Getter/Setters /////////

    std::string GetTeamName();
    void SetTeamName(std::string i_TeamName);

    void SetFormation(Formation i_Formation);
    Formation GetFormation();


    // Sets player formation positions based on thier position in the TeamRoster vector and which formation the team is using
    // User can arange his players however they like to be in the correct order before a match starts
    // CPU teams always maintain their rosters in a certian order to ensure thier players are in the right positions
    void SetPlayerPositions();

private:

    ///////// Team Data /////////
    //League win/loss total is used to calcualte team rankings and is reset every season
    //Lifetime win/loss is recorded for each team but currently is not used

    std::string TeamName;
    int LeagueWinTotal;
    int LeagueLossTotal;
    int LifetimeWinTotal;
    int LifetimeLossTotal;
    Formation TeamFormation;
};

