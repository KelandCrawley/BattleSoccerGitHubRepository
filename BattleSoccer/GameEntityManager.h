#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <functional>
#include <memory>

#include "Team.h"
#include "EventTypes.h"
#include "FileManager.h"

#include "MathHelper.hpp"

class GameEntityManager
    : public std::enable_shared_from_this<GameEntityManager>
{
public:
    GameEntityManager();
    ~GameEntityManager();

    bool Initialize(sf::Vector2f i_Scale);

         //gui
    void ListTeamsWithLeagueRecord(sf::RenderWindow &MainwWindow, sf::Vector2f i_Position);
    void UpdateLeagueStandings();
    void DrawGameInfo(sf::RenderWindow &MainwWindow, sf::Vector2f i_Position);
    void DrawEndOfSeasonInfo(sf::RenderWindow &MainwWindow, const sf::Vector2f &i_Position);

    //Returns team based on the position of the team in the main team container, Note: this container is not sorted by wins, it remains in the same order throught each season
    std::shared_ptr<Team> GetTeam(int i_TeamNumber);

        //User Team Actions
    std::shared_ptr<Team> GetUserTeam();
    void AddPlayerToUserTeam(std::shared_ptr<Player> i_Player);
    void DeletePlayerOnUserTeam(std::shared_ptr<Player> i_Player);

    //checks if user team has 5 players to play a match
    bool isUserTeamValid();
    void LoadUserTeam(std::shared_ptr<Team> i_UserTeam);


         //player cards
    void DrawUserTeamPlayerCards(sf::RenderWindow &MainWindow,  sf::Vector2f i_Position, float BufferBetweenCards);
    void DrawFreeAgentPlayerCard(sf::RenderWindow &MainwWindow,  sf::Vector2f i_Position);
    std::shared_ptr<Player> CheckforUserTeamPlayerCardSelection(const sf::Vector2f &MouseLocation);
    void DrawSelectedPlayerCard(sf::RenderWindow &MainWindow, sf::Vector2f i_Position);
    void DrawSelectedPlayerSigningBonus(sf::RenderWindow &MainWindow, const sf::Vector2f &i_Position);
    void CheckForFreeAgentPLayerCardSelection(const sf::Vector2f &MouseLocation);
    std::shared_ptr<Player> SelectedPlayer;


        //Season simulation
    void ProgressSeason(int i_WhichTeamWon);
    std::shared_ptr<Team> GetUsersNextOpposingTeam();
    void StartNewSeason();
    int GetUserPlaceInLeague();

         // Save & Load
    void SaveFreeAgents();
    void LoadFreeAgents();
    void SaveGameState();
    void LoadGameState();
    void ResetData();

          //events
    std::function<void(EventType, int)> HandleEvent_CallBack;

private:

         //season simulation
    std::vector<std::shared_ptr<Team>> RegionalLeagueTeamContainer;
    std::shared_ptr<Team> UserTeam;
    int WeekNumber;
    std::map<std::shared_ptr<Team>, std::shared_ptr<Team>> WeeklyPairings;
    int CurrentLeagueLevel;

    int SimulateMatch(std::shared_ptr<Team> i_Team1, std::shared_ptr<Team> i_Team2);
    void ResetLeague();
    void SimulateWeeklyMatches();
    void SimulatePlayoffMatches(int i_WhichTeamWon);
    void SetPlayoffTeams(int NumberOfTeams);
    std::vector<std::shared_ptr<Team>> CurrentLeagueTeamContainer;
    static bool FindGreaterWins(std::shared_ptr<Team> i_Team1, std::shared_ptr<Team> i_Team2);
    void SetWeeklyPairings();

         // League Intiliazation
    void InitializeCityLeague();
    void InitializeRegionalLeague();
    void InitializeNationalLeague();
    void InitializeWorldLeague();



            //Free Agents
    std::vector<std::shared_ptr<Player>> FreeAgentContainer;

         //text
    sf::Font MainFont;
    sf::Text TeamRankInfoText[8][4];
    sf::Text UserGameInfoText[2];
    sf::Text PlayerCostStatsText[1];
    sf::Text CurrentLeagueNameText[1];
    sf::Text EndOfSeasonInfoText[4];


         //user specific stuff
   int UserCurrentCash;
   int CalculateUserteamSalaryCost();
   int UserPlaceInLeague;


         //scale 
   sf::Vector2f CurrentScale;

         // save current teams
   void SaveCurrentLeague();



};
