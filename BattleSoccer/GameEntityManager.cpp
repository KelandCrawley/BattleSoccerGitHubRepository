#include "GameEntityManager.h"

GameEntityManager::GameEntityManager()
    :CurrentScale(1.0f, 1.0f)
{
}


GameEntityManager::~GameEntityManager()
{
}

bool GameEntityManager::Initialize(sf::Vector2f i_Scale)
{
    if (!MainFont.loadFromFile("Data/ArialFont/arial.ttf"))
    {
    }
    //set text settings
    for (auto i = 0; i < 8; i++)
    {
        for (auto j = 0; j < 4; j++)
        {
            TeamRankInfoText[i][j].setFillColor(sf::Color::Black);
            TeamRankInfoText[i][j].setFont(MainFont);
            TeamRankInfoText[i][j].setCharacterSize(32);
        }

    }
    for (auto i = 0; i < 4; i++)
    {
        UserGameInfoText[i].setFillColor(sf::Color::White);
        UserGameInfoText[i].setOutlineColor(sf::Color::Black);
        UserGameInfoText[i].setOutlineThickness(2);
        UserGameInfoText[i].setFont(MainFont);
        UserGameInfoText[i].setCharacterSize(22);
    }
    PlayerCostStatsText[0].setFillColor(sf::Color::Green);
    PlayerCostStatsText[0].setFont(MainFont);
    PlayerCostStatsText[0].setCharacterSize(26);

    CurrentLeagueNameText[0].setFillColor(sf::Color::Black);
    CurrentLeagueNameText[0].setFont(MainFont);
    CurrentLeagueNameText[0].setCharacterSize(48);
    CurrentLeagueNameText[0].setOutlineThickness(0);
    for (auto i = 0; i < 4; i++)
    {
        EndOfSeasonInfoText[i].setFillColor(sf::Color::Black);
        EndOfSeasonInfoText[i].setFont(MainFont);
        EndOfSeasonInfoText[i].setCharacterSize(32);
    }
    //initalize user team
    UserTeam = std::make_shared<Team>();
    LoadUserTeam(FileManager::LoadUserTeam());
    CurrentLeagueTeamContainer.push_back(UserTeam);

    //load game state
    LoadGameState();
    switch (CurrentLeagueLevel)
    {
    case 1:
        InitializeCityLeague();
        break;
    case 2:
        InitializeRegionalLeague();
        break;
    case 3:
        InitializeNationalLeague();
        break;
    case 4:
        InitializeWorldLeague();
        break;
    }

    //setup game
    SetWeeklyPairings();
    UpdateLeagueStandings();

    CurrentScale = i_Scale;
    return false;
}

void GameEntityManager::ListTeamsWithLeagueRecord(sf::RenderWindow &MainwWindow, sf::Vector2f i_Position)
{
    //scaling
    i_Position.x *= CurrentScale.x;
    i_Position.y *= CurrentScale.y;

    //set league text
    switch (CurrentLeagueLevel)
    {
    case 1:
        CurrentLeagueNameText[0].setString("All City Open");
        break;
    case 2:
        CurrentLeagueNameText[0].setString("Regional Invitational");
        break;
    case 3:
        CurrentLeagueNameText[0].setString("National ChampionShip");
        break;
    case 4:
        CurrentLeagueNameText[0].setString("World Cup");
        break;
    }
    CurrentLeagueNameText[0].setScale(CurrentScale);
    CurrentLeagueNameText[0].setPosition(sf::Vector2f((i_Position.x) + 75, i_Position.y));
    MainwWindow.draw(CurrentLeagueNameText[0]);

    //set team text info
    for (auto i = 0; i < CurrentLeagueTeamContainer.size(); i++)
    {
        TeamRankInfoText[i][0].setScale(CurrentScale);
        TeamRankInfoText[i][1].setScale(CurrentScale);
        TeamRankInfoText[i][2].setScale(CurrentScale);
        TeamRankInfoText[i][3].setScale(CurrentScale);

        TeamRankInfoText[i][0].setPosition(sf::Vector2f(i_Position.x, (i_Position.y + 60 + (40 * i))));
        TeamRankInfoText[i][1].setPosition(sf::Vector2f((i_Position.x + (75 * CurrentScale.x)), (i_Position.y + 60 + (40 * i))));
        TeamRankInfoText[i][2].setPosition(sf::Vector2f((i_Position.x + (375 * CurrentScale.x)), (i_Position.y + 60 + (40 * i))));
        TeamRankInfoText[i][3].setPosition(sf::Vector2f((i_Position.x + (450 * CurrentScale.x)), (i_Position.y + 60 + (40 * i))));
        for (auto j = 0; j < 4; j++)
        {
            MainwWindow.draw(TeamRankInfoText[i][j]);
        }

    }



}

void GameEntityManager::UpdateLeagueStandings()
{
    // sort by wins
    std::vector<std::shared_ptr<Team>> TeamsBuffer = CurrentLeagueTeamContainer;
    std::sort(TeamsBuffer.begin(), TeamsBuffer.end(), GameEntityManager::FindGreaterWins);

    //set team info text
    for (auto i = 0; i < TeamsBuffer.size(); i++)
    {
        TeamRankInfoText[i][0].setString(std::to_string(i + 1) + ".");
        TeamRankInfoText[i][1].setString(TeamsBuffer.at(i)->GetTeamName());
        TeamRankInfoText[i][2].setString(std::to_string(TeamsBuffer.at(i)->GetLeagueWins()));
        TeamRankInfoText[i][3].setString(std::to_string(TeamsBuffer.at(i)->GetLeagueLosses()));

        if (TeamsBuffer.at(i) == UserTeam)
        {
            UserPlaceInLeague = i + 1;
            TeamRankInfoText[i][0].setFillColor(sf::Color::Blue);
            TeamRankInfoText[i][1].setFillColor(sf::Color::Blue);
            TeamRankInfoText[i][2].setFillColor(sf::Color::Blue);
            TeamRankInfoText[i][3].setFillColor(sf::Color::Blue);
        }
        else
        {
            TeamRankInfoText[i][0].setFillColor(sf::Color::Black);
            TeamRankInfoText[i][1].setFillColor(sf::Color::Black);
            TeamRankInfoText[i][2].setFillColor(sf::Color::Black);
            TeamRankInfoText[i][3].setFillColor(sf::Color::Black);
        }
    }
}

void GameEntityManager::DrawGameInfo(sf::RenderWindow & MainwWindow, sf::Vector2f i_Position)
{
    i_Position.x *= CurrentScale.x;
    i_Position.y *= CurrentScale.y;

    UserGameInfoText[0].setString("$" + std::to_string(UserCurrentCash));
    UserGameInfoText[1].setString("Week: " + std::to_string(WeekNumber));

    UserGameInfoText[0].setPosition(i_Position);
    UserGameInfoText[1].setPosition(sf::Vector2f(i_Position.x + 100, i_Position.y));

    for (auto i = 0; i < 2; i++)
    {
        MainwWindow.draw(UserGameInfoText[i]);
    }
}

void GameEntityManager::DrawEndOfSeasonInfo(sf::RenderWindow & MainwWindow, const sf::Vector2f &i_Position)
{
    EndOfSeasonInfoText[0].setPosition(sf::Vector2f(i_Position.x - 125, i_Position.y - 80));
    EndOfSeasonInfoText[1].setPosition(sf::Vector2f(i_Position.x - 110, i_Position.y));
    EndOfSeasonInfoText[2].setPosition(sf::Vector2f(i_Position.x - 140, i_Position.y + 80));
    EndOfSeasonInfoText[3].setPosition(sf::Vector2f(i_Position.x - 120, i_Position.y + 110));

    EndOfSeasonInfoText[0].setString(CurrentLeagueNameText[0].getString());
    EndOfSeasonInfoText[1].setString("You Placed " + std::to_string(UserPlaceInLeague));

    //logic to decide end of season messsage
    // based on current league and user teams place in league

    if (UserPlaceInLeague == 1 && CurrentLeagueLevel != 4)
    {
        EndOfSeasonInfoText[2].setString("You've Been Promoted to");
        switch (CurrentLeagueLevel)
        {
        case 2:
            EndOfSeasonInfoText[3].setString("Regional Invitational");
            break;
        case 3:
            EndOfSeasonInfoText[3].setString("National Championship");
            break;
        case 4:
            EndOfSeasonInfoText[3].setString("World Cup");
            break;
        }
    }
    else if (UserPlaceInLeague > 4 && CurrentLeagueLevel > 1)
    {
        EndOfSeasonInfoText[2].setString("You've Been Demoted to");
        switch (CurrentLeagueLevel)
        {
        case 1:
            EndOfSeasonInfoText[3].setString("All City Open");
            break;
        case 2:
            EndOfSeasonInfoText[3].setString("Regional Invitational");
            break;
        case 3:
            EndOfSeasonInfoText[3].setString("National Championship");
            break;
        }
    }
    else
    {
        //this shouldnt happen but if some case isnt coverd this will set text to empty
        EndOfSeasonInfoText[2].setString("");
        EndOfSeasonInfoText[3].setString("");
    }
    
    //draw text
    for (auto i = 0; i < 4; i++)
    {
        MainwWindow.draw(EndOfSeasonInfoText[i]);
    }
}


//gets team based on position in current league team container
std::shared_ptr<Team> GameEntityManager::GetTeam(int i_TeamNumber)
{
    try
    {
        if (CurrentLeagueTeamContainer.size() > i_TeamNumber)
        {
            return CurrentLeagueTeamContainer.at(i_TeamNumber);
        }
        else
        {
            return CurrentLeagueTeamContainer.at(0);
        }

    }
    catch (const std::out_of_range& oor)
    {
        std::cout << "Out of Range error: " << oor.what() << '\n';
    }
}

std::shared_ptr<Team> GameEntityManager::GetUserTeam()
{
    return UserTeam;
}

void GameEntityManager::AddPlayerToUserTeam(std::shared_ptr<Player> i_Player)
{
    //check if user roster has an open spot
    if (UserTeam->TeamRoster->size() < 5)
    {
        //tell the player to switch colors
        i_Player->ChangeToDifferentColor(true);

        //add player to user team
        UserTeam->AddPlayer(i_Player);

    }
    else
    {
        return;
    }
    // find the player in the free agent container and erase them
    auto Result = std::find(std::begin(FreeAgentContainer), std::end(FreeAgentContainer), i_Player);
    if (Result != std::end(FreeAgentContainer))
    {
        FreeAgentContainer.erase(Result);
    }

    //reduce user team money by amount equal to players hire bonus
    UserCurrentCash -= i_Player->CalculateHireBonus();

    // tell event maanger that to signel an event
    HandleEvent_CallBack(EventType::UserTeamChanged, 0);
}

void GameEntityManager::DeletePlayerOnUserTeam(std::shared_ptr<Player> i_Player)
{
    //try and delete plyaer on user team
    if (UserTeam->DeletePlayer(i_Player))
    {
        //change player to not user team
        i_Player->ChangeToDifferentColor(false);

        // insert player into free agent container
        FreeAgentContainer.push_back(i_Player);

        // tell event manager to push an event
        HandleEvent_CallBack(EventType::UserTeamChanged, 0);
    }
}

bool GameEntityManager::isUserTeamValid()
{
    //check if team has 5 players
    if (UserTeam->TeamRoster->size() < 5)
    {
        return false;
    }
    return true;
}

void GameEntityManager::LoadUserTeam(std::shared_ptr<Team> i_UserTeam)
{
    UserTeam->TeamRoster = i_UserTeam->TeamRoster;
    UserTeam->SetTeamName(i_UserTeam->GetTeamName());
    UserTeam->LeagueLossTotal = i_UserTeam->LeagueLossTotal;
    UserTeam->LeagueWinTotal = i_UserTeam->LeagueWinTotal;
    UserTeam->LifetimeWinTotal = i_UserTeam->LifetimeWinTotal;
    UserTeam->LifetimeLossTotal = i_UserTeam->LifetimeLossTotal;
    UpdateLeagueStandings();
}

void GameEntityManager::DrawUserTeamPlayerCards(sf::RenderWindow & MainWindow, sf::Vector2f i_Position, float BufferBetweenCards)
{
    i_Position.x *= CurrentScale.x;
    i_Position.y *= CurrentScale.y;
    for (auto i = 0; i < UserTeam->TeamRoster->size(); i++)
    {
        UserTeam->TeamRoster->at(i)->DrawPlayerCard(MainWindow, sf::Vector2f(i_Position.x + ((180 + BufferBetweenCards)* CurrentScale.x* i), i_Position.y), CurrentScale);
    }
}

void GameEntityManager::DrawFreeAgentPlayerCard(sf::RenderWindow & MainwWindow, sf::Vector2f i_Position)
{
    i_Position.x *= CurrentScale.x;
    i_Position.y *= CurrentScale.y;

    for (auto i = 0; i < FreeAgentContainer.size(); i++)
    {
        if (i < 4)
        {
            FreeAgentContainer.at(i)->DrawPlayerCard(MainwWindow, sf::Vector2f(i_Position.x + (i * (200 * CurrentScale.x)) + (40 * CurrentScale.x), i_Position.y), CurrentScale);
        }
        else if (i < 8)
        {
            FreeAgentContainer.at(i)->DrawPlayerCard(MainwWindow, sf::Vector2f(i_Position.x + ((i - 4) * (200 * CurrentScale.x)) + (40 * CurrentScale.x), i_Position.y + (275 * CurrentScale.y)), CurrentScale);
        }
        else
        {
            return;
        }
    }
}

std::shared_ptr<Player> GameEntityManager::CheckforUserTeamPlayerCardSelection(const sf::Vector2f &MouseLocation)
{
    SelectedPlayer = nullptr;

    //cycle through each user player
    for (auto i = 0; i < UserTeam->TeamRoster->size(); i++)
    {
        //check if player is already selected
        if (UserTeam->TeamRoster->at(i) != SelectedPlayer)
        {
            //set card highlight to false if not selcted
            UserTeam->TeamRoster->at(i)->setPlayerCardHighlighted(false);
        }
        if (UserTeam->TeamRoster->at(i)->CheckForPlayerCardSelection(MouseLocation))
        {
            //player is selcted, set player as selected and set highlight to true
            SelectedPlayer = UserTeam->TeamRoster->at(i);
            SelectedPlayer->setPlayerCardHighlighted(true);
            return SelectedPlayer;
        }
    }
    return nullptr;

}

void GameEntityManager::DrawSelectedPlayerCard(sf::RenderWindow & MainWindow, sf::Vector2f i_Position)
{
    i_Position.x *= CurrentScale.x;
    i_Position.y *= CurrentScale.y;
    if (SelectedPlayer)
    {
        SelectedPlayer->DrawPlayerCard(MainWindow, i_Position, CurrentScale);
    }
}

void GameEntityManager::DrawSelectedPlayerSigningBonus(sf::RenderWindow & MainWindow, const sf::Vector2f &i_Position)
{
    if (SelectedPlayer)
    {
        PlayerCostStatsText[0].setString("$" + std::to_string(SelectedPlayer->CalculateHireBonus()));
        PlayerCostStatsText[0].setPosition(i_Position);
        MainWindow.draw(PlayerCostStatsText[0]);
    }
}

void GameEntityManager::CheckForFreeAgentPLayerCardSelection(const sf::Vector2f &MouseLocation)
{

    SelectedPlayer = nullptr;
    for (auto i = 0; i < FreeAgentContainer.size(); i++)
    {
        FreeAgentContainer.at(i)->setPlayerCardHighlighted(false);
        if (FreeAgentContainer.at(i)->CheckForPlayerCardSelection(MouseLocation))
        {
            SelectedPlayer = FreeAgentContainer.at(i);
            SelectedPlayer->setPlayerCardHighlighted(true);
        }
    }
}

void GameEntityManager::ProgressSeason(int i_WhichTeamWon)
{
    auto OpposingTeam = GetUsersNextOpposingTeam();
    if (OpposingTeam)
    {
        if (i_WhichTeamWon == 0)
        {
            UserTeam->AddWin();
            OpposingTeam->AddLoss();
            UserCurrentCash += (500 * CurrentLeagueLevel);
        }
        else if (i_WhichTeamWon == 1)
        {
            OpposingTeam->AddWin();
            UserTeam->AddLoss();
            UserCurrentCash += 250 ;

        }
    }

    // hard way, find better soloution
    // first simulate matches that are in que
    //
    //then sets matchups based on what week it currently is
    switch (WeekNumber)
    {
        case 1:
        SimulateWeeklyMatches();
        WeeklyPairings.clear();
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(2));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(3), CurrentLeagueTeamContainer.at(1));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(4), CurrentLeagueTeamContainer.at(6));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(5), CurrentLeagueTeamContainer.at(7));

        break;
         case 2:
        SimulateWeeklyMatches();
        WeeklyPairings.clear();
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(3));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(2), CurrentLeagueTeamContainer.at(1));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(4), CurrentLeagueTeamContainer.at(7));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(5), CurrentLeagueTeamContainer.at(6));
        break;
    case 3:
        SimulateWeeklyMatches();
        WeeklyPairings.clear();
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(4));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(2), CurrentLeagueTeamContainer.at(5));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(3), CurrentLeagueTeamContainer.at(6));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(1), CurrentLeagueTeamContainer.at(7));
        break;
    case 4:
        SimulateWeeklyMatches();
        WeeklyPairings.clear();
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(5));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(2), CurrentLeagueTeamContainer.at(6));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(3), CurrentLeagueTeamContainer.at(7));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(1), CurrentLeagueTeamContainer.at(4));
        break;
    case 5:
        SimulateWeeklyMatches();
        WeeklyPairings.clear();
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(6));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(2), CurrentLeagueTeamContainer.at(7));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(3), CurrentLeagueTeamContainer.at(4));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(1), CurrentLeagueTeamContainer.at(5));
        break;
    case 6:
        SimulateWeeklyMatches();
        WeeklyPairings.clear();
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(7));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(2), CurrentLeagueTeamContainer.at(4));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(3), CurrentLeagueTeamContainer.at(5));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(1), CurrentLeagueTeamContainer.at(6));
        break;
    case 7:
        SimulateWeeklyMatches();
        WeeklyPairings.clear();
        SetPlayoffTeams(4);
        for (auto i = 0; i < CurrentLeagueTeamContainer.size(); i++)
        {
            if (CurrentLeagueTeamContainer.at(i) == UserTeam)
            {
                break;
            }
        }
        // user team not found, they didnt make playoffs go down a league
        if (CurrentLeagueLevel > 1)
        {
            CurrentLeagueLevel--;
        }
        break;
    case 8:
        SimulatePlayoffMatches(i_WhichTeamWon);
        break;
    case 9:
        SimulatePlayoffMatches(i_WhichTeamWon);
        break;
    case 10:
        // reset league
        if (CurrentLeagueTeamContainer.at(0) == UserTeam)
        {
            CurrentLeagueLevel++;
            if (CurrentLeagueLevel > 4)
            {
                CurrentLeagueLevel = 4;
            }
        }
        HandleEvent_CallBack(EventType::SeasonComplete, 0);
        break;
   


    }
    // now progress week and update standings
    WeekNumber++;
    UpdateLeagueStandings();

    //pay out player salaries
    UserCurrentCash -= CalculateUserteamSalaryCost();

    //save
    FileManager::SaveTeam(UserTeam, "UserTeam");
    SaveCurrentLeague();
    SaveGameState();

}

std::shared_ptr<Team> GameEntityManager::GetUsersNextOpposingTeam()
{
    //start iterator
    auto Iterator = WeeklyPairings.begin();
    while (Iterator != WeeklyPairings.end())
    {
        //look for userteam in first position
        if (Iterator->first == UserTeam)
        {
            return Iterator->second;
        } // else look for useteam in second positin
        else if (Iterator->second == UserTeam)
        {
            return Iterator->first;
        }

        Iterator++;
    }

    // userteam not found, return null
    //this can happen if user team is not in the playoffs
    return nullptr;

}

void GameEntityManager::SaveFreeAgents()
{
    //sves free agents based on current league
    switch (CurrentLeagueLevel)
    {
    case 1:
        FileManager::SaveFreeAgents("FreeAgents_StarterLeague", FreeAgentContainer);
        break;
    case 2:
        FileManager::SaveFreeAgents("FreeAgents_RegionalLeauge", FreeAgentContainer);
        break;
    case 3:
        FileManager::SaveFreeAgents("FreeAgents_NationalLeague", FreeAgentContainer);
        break;
    case 4:
        FileManager::SaveFreeAgents("FreeAgents_WorldLeague", FreeAgentContainer);
        break;
    }

}

void GameEntityManager::LoadFreeAgents()
{
    switch (CurrentLeagueLevel)
    {
    case 1:
        FreeAgentContainer = FileManager::LoadFreeAgents("FreeAgents_StarterLeague");
        break;
    case 2:
        FreeAgentContainer = FileManager::LoadFreeAgents("FreeAgents_RegionalLeauge");
        break;
    case 3:
        FreeAgentContainer = FileManager::LoadFreeAgents("FreeAgents_NationalLeague");
        break;
    case 4:
        FreeAgentContainer = FileManager::LoadFreeAgents("FreeAgents_WorldLeague");
        break;
    }
}

void GameEntityManager::SaveGameState()
{
    std::vector<int> GameStateNumbers;
    GameStateNumbers.push_back(WeekNumber);
    GameStateNumbers.push_back(CurrentLeagueLevel);
    GameStateNumbers.push_back(UserCurrentCash);
    FileManager::SaveGameState(GameStateNumbers);
}

void GameEntityManager::LoadGameState()
{
    std::vector<int> GameStateNumbers = FileManager::LoadGameState();

    try
    {
        WeekNumber = GameStateNumbers.at(0);
        CurrentLeagueLevel = GameStateNumbers.at(1);
        UserCurrentCash = GameStateNumbers.at(2);
    }
    catch (std::out_of_range e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void GameEntityManager::StartNewSeason()
{
    ResetLeague();
}

int GameEntityManager::GetUserPlaceInLeague()
{
    return UserPlaceInLeague;
}

void GameEntityManager::ResetData()
{
    // Data to be Reset
        //All teams win/loas

    InitializeRegionalLeague();
    ResetLeague();
    SaveCurrentLeague();

    InitializeNationalLeague();
    ResetLeague();
    SaveCurrentLeague();

    InitializeWorldLeague();
    ResetLeague();
    SaveCurrentLeague();

       // user team
    UserTeam = FileManager::ResetUserTeam();

       //free agents
    FileManager::ResetFreeAgents("FreeAgents_StarterLeague");
    FileManager::ResetFreeAgents("FreeAgents_RegionalLeauge");
    FileManager::ResetFreeAgents("FreeAgents_NationalLeague");
    FileManager::ResetFreeAgents("FreeAgents_WorldLeague");


   //GameState, currentweek, current league level, User Cash
    FileManager::ResetGameState();
    LoadGameState();

    //reset city league and initialize it
    InitializeCityLeague();
    ResetLeague();
    SaveCurrentLeague();

    SetWeeklyPairings();
    UpdateLeagueStandings();

}

int GameEntityManager::SimulateMatch(std::shared_ptr<Team> i_Team1, std::shared_ptr<Team> i_Team2)
{
    //currently simple coin flip simulation
    std::cout << i_Team1->GetTeamName() << " vs " << i_Team2->GetTeamName() << std::endl;
    int RandomNumber = RNGClass::GetRandomInteger(0, 1);
    if (RandomNumber == 0)
    {
        i_Team1->AddWin();
        i_Team2->AddLoss();
        return 1;
    }
    else
    {
        i_Team1->AddLoss();
        i_Team2->AddWin();
        return 2;
    }

}

void GameEntityManager::ResetLeague()
{
    //clear current league
    CurrentLeagueTeamContainer.clear();

    // re0initialize league based on current league level
    switch (CurrentLeagueLevel)
    {
    case 1:
        InitializeCityLeague();
        break;
    case 2:
        InitializeRegionalLeague();
        break;
    case 3:
        InitializeNationalLeague();
        break;
    case 4:
        InitializeWorldLeague();
        break;
    }

    //clear weekly pairings and find the new ones
    WeeklyPairings.clear();
    WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(1));
    WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(2), CurrentLeagueTeamContainer.at(3));
    WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(4), CurrentLeagueTeamContainer.at(5));
    WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(6), CurrentLeagueTeamContainer.at(7));

    //clear each teams current league win/loss
    for (auto i = 0; i < CurrentLeagueTeamContainer.size(); i++)
    {
        CurrentLeagueTeamContainer.at(i)->ClearLeagueRecord();
    }
    UpdateLeagueStandings();
}

void GameEntityManager::SimulateWeeklyMatches()
{
    auto Iterator = WeeklyPairings.begin();
    while (Iterator != WeeklyPairings.end())
    {
        // if user team is not in match, simulate it, if user team is in match, skip it (userteam plays matches before simulation takes place)
        if (Iterator->first != UserTeam && Iterator->second != UserTeam)
        {
            SimulateMatch(Iterator->first, Iterator->second);
        }
        Iterator++;
    }
}

void GameEntityManager::SimulatePlayoffMatches(int i_WhichTeamWon)
{

    auto Iterator = WeeklyPairings.begin();
    auto LosingTeam = std::shared_ptr<Team>(nullptr);
    if (Iterator != WeeklyPairings.end())
    {
        // if weekly pairings is nt empty, initialzie losing team to first position of first match
        auto LosingTeam = WeeklyPairings.begin()->first;
    }
    else
    {
        // error, pairings was empty
        return;
    }

    // cycle through matches
    while (Iterator != WeeklyPairings.end())
    {
        // if match does not container user team, simulate it
        if (Iterator->first != UserTeam && Iterator->second != UserTeam)
        {
            if (SimulateMatch(Iterator->first, Iterator->second) == 1)
            {
                LosingTeam = Iterator->second;
            }
            else
            {
                LosingTeam = Iterator->first;

            }

            auto secondIterator = std::find(CurrentLeagueTeamContainer.begin(), CurrentLeagueTeamContainer.end(), LosingTeam);
            if (secondIterator != CurrentLeagueTeamContainer.end())
            {
                //erase losing team from the standings in order to show only the teams still in playoffs
                std::cout << "Erasing Team: " << secondIterator->get()->GetTeamName() << std::endl;
                CurrentLeagueTeamContainer.erase(secondIterator);
            }
        } // else user team is in match, take param for which team won
        else
        {
            if (i_WhichTeamWon == 0)
            {
                LosingTeam = Iterator->second;
            }
            else
            {
                LosingTeam = UserTeam;
            }
            auto secondIterator = std::find(CurrentLeagueTeamContainer.begin(), CurrentLeagueTeamContainer.end(), LosingTeam);
            if (secondIterator != CurrentLeagueTeamContainer.end())
            {
                std::cout << "Erasing Team: " << secondIterator->get()->GetTeamName() << std::endl;
                CurrentLeagueTeamContainer.erase(secondIterator);
            }
        }

        Iterator++;
    }
    // clear parings, and set next week parings
    WeeklyPairings.clear();
    SetPlayoffTeams(CurrentLeagueTeamContainer.size());

}

void GameEntityManager::SetPlayoffTeams(int NumberOfTeams)
{
    //clear parirings
    UpdateLeagueStandings();
    WeeklyPairings.clear();

    // sort teams
    std::sort(CurrentLeagueTeamContainer.begin(), CurrentLeagueTeamContainer.end(), GameEntityManager::FindGreaterWins);

    // if param is 4 teams then we need to set the semi-finals
    if (NumberOfTeams == 4)
    {
        CurrentLeagueTeamContainer.erase(CurrentLeagueTeamContainer.begin() + 4, CurrentLeagueTeamContainer.end());
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(3));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(1), CurrentLeagueTeamContainer.at(2));
    } // if parm is 2 teams, we set the finals
    else if (NumberOfTeams == 2)
    {
        CurrentLeagueTeamContainer.erase(CurrentLeagueTeamContainer.begin() + 2, CurrentLeagueTeamContainer.end());
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(1));
    }
    else
    {
        return;
    }

}

void GameEntityManager::InitializeCityLeague()
{
    CurrentLeagueTeamContainer.clear();
    CurrentLeagueTeamContainer.push_back(UserTeam);



    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Rangers"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Sharks"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Comets"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Giants"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Titans"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Gorillias"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Bowlers"));



    FreeAgentContainer.clear();
    LoadFreeAgents();



}

void GameEntityManager::InitializeRegionalLeague()
{
    CurrentLeagueTeamContainer.clear();
    CurrentLeagueTeamContainer.push_back(UserTeam);

    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("BobCats"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Camels"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Hornets"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Jumping Beans"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Storm"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Team 99"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Tricksters"));
  

    FreeAgentContainer.clear();
    LoadFreeAgents();


}

void GameEntityManager::InitializeNationalLeague()
{
    CurrentLeagueTeamContainer.clear();
    CurrentLeagueTeamContainer.push_back(UserTeam);


    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Thunder"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Monsters"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Cyborgs"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("DeadShots"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Hilltoppers"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Red Shirts"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Gnomes"));


    FreeAgentContainer.clear();
    LoadFreeAgents();

  
  
}

void GameEntityManager::InitializeWorldLeague()
{
    CurrentLeagueTeamContainer.clear();
    CurrentLeagueTeamContainer.push_back(UserTeam);


    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Tenacity"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Skyhawks"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Team Strafe"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Defiance"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Golden"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Pristine"));
    CurrentLeagueTeamContainer.push_back(FileManager::LoadTeam("Dragons"));



    FreeAgentContainer.clear();
    LoadFreeAgents();

 
   
}


bool GameEntityManager::FindGreaterWins(std::shared_ptr<Team> i_Team1, std::shared_ptr<Team> i_Team2)
{
    return (i_Team1->GetLeagueWins() > i_Team2->GetLeagueWins());
}

int GameEntityManager::CalculateUserteamSalaryCost()
{
    int TotalSalary = 0;
    for (auto i = 0; i < UserTeam->TeamRoster->size(); i++)
    {
        TotalSalary += UserTeam->TeamRoster->at(i)->CalculateWeeklySalary();
    }

    return TotalSalary;
}

void GameEntityManager::SaveCurrentLeague()
{
    for (auto i = 0; i < CurrentLeagueTeamContainer.size(); i++)
    {
        if (CurrentLeagueTeamContainer.at(i) != UserTeam)
        {
            FileManager::SaveTeam(CurrentLeagueTeamContainer.at(i), CurrentLeagueTeamContainer.at(i)->GetTeamName());
        }
    }
}

void GameEntityManager::SetWeeklyPairings()
{
    WeeklyPairings.clear();
    switch (WeekNumber)
    {
    case 1:
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(1));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(2), CurrentLeagueTeamContainer.at(3));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(4), CurrentLeagueTeamContainer.at(5));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(6), CurrentLeagueTeamContainer.at(7));
        break;
    case 2:
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(2));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(3), CurrentLeagueTeamContainer.at(1));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(4), CurrentLeagueTeamContainer.at(6));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(5), CurrentLeagueTeamContainer.at(7));
        break;
    case 3:
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(3));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(2), CurrentLeagueTeamContainer.at(1));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(4), CurrentLeagueTeamContainer.at(7));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(5), CurrentLeagueTeamContainer.at(6));
        break;
    case 4:
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(4));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(2), CurrentLeagueTeamContainer.at(5));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(3), CurrentLeagueTeamContainer.at(6));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(1), CurrentLeagueTeamContainer.at(7));
        break;
    case 5:
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(5));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(2), CurrentLeagueTeamContainer.at(6));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(3), CurrentLeagueTeamContainer.at(7));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(1), CurrentLeagueTeamContainer.at(4));
        break;
    case 6:
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(6));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(2), CurrentLeagueTeamContainer.at(7));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(3), CurrentLeagueTeamContainer.at(4));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(1), CurrentLeagueTeamContainer.at(5));
        break;
    case 7:
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(0), CurrentLeagueTeamContainer.at(7));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(2), CurrentLeagueTeamContainer.at(4));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(3), CurrentLeagueTeamContainer.at(5));
        WeeklyPairings.emplace(CurrentLeagueTeamContainer.at(1), CurrentLeagueTeamContainer.at(6));
        break;
    case 8:
        SetPlayoffTeams(4);
        for (auto i = 0; i < CurrentLeagueTeamContainer.size(); i++)
        {
            if (CurrentLeagueTeamContainer.at(i) == UserTeam)
            {
                break;
            }
        }
        break;
    case 9:
        SetPlayoffTeams(2);
        break;
    case 10:
        ResetLeague();
        break;
    }
}
