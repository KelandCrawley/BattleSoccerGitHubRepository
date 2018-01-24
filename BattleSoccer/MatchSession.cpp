#include "MatchSession.h"



MatchSession::MatchSession()
    :ControlledPlayer(nullptr)
    , NorthWall(sf::Vector2f(1756, 22))
    , SouthWall(sf::Vector2f(1756, 22))
    , WestWallNorthHalf(sf::Vector2f(22, 250))
    , WestWallSouthHalf(sf::Vector2f(22, 250))
    , EastWallNorthHalf(sf::Vector2f(22, 250))
    , EastWallSouthHalf(sf::Vector2f(22, 250))
    , LeftGoal(sf::Vector2f(22, 200))
    , RightGoal(sf::Vector2f(22, 200))
    , EndOfMatchPopUpMenu()
    , MatchSessionMenuPopUp()
    , ShootingPlayer(nullptr)
    , AISavedPlayerPointer(nullptr)
    , m_LuaManager()
    , MiddleThird()
    , MouseOverPlayerRectangle(sf::Vector2f(50, 50))
    , MouseHoveredPlayer(nullptr)
    , StopPlay(false)
    , isBallBeingShot(false)
    , isSuddenDeath(false)
    , ShotRayCastBuffer()
{

    BallVelocityDropOffPerSecond = 0.6f;
    DoShowBall = true;
    TimeBetweenUpdates = 0.1f;
    TimeSinceLastUpdate = TimeBetweenUpdates;
    MouseOverPlayerRectangle.setFillColor(sf::Color::Transparent);
    MouseOverPlayerRectangle.setOutlineColor(sf::Color::Cyan);
    MouseOverPlayerRectangle.setOutlineThickness(2);
    MouseOverPlayerRectangle.setOrigin(MouseOverPlayerRectangle.getSize().x / 2, MouseOverPlayerRectangle.getSize().y / 2);

    ShotRayCastBuffer.setFillColor(sf::Color::Black);

    EndOfMatchPopUpMenu.Initialize(sf::String("Data/EndOfMatchPopUpSprite.png"));
    MatchSessionMenuPopUp.Initialize(sf::String("Data/MatchSessionMenuPopUp.png"));

}


MatchSession::~MatchSession()
{
}

bool MatchSession::NewMatch(std::shared_ptr<Team> i_Team1, std::shared_ptr<Team> i_Team2)
{
    Team1Name = i_Team1->GetTeamName();
    Team2Name = i_Team2->GetTeamName();

    EndOfMatchPopUpMenu.SetDoDrawMenu(false);
    MatchSessionMenuPopUp.SetDoDrawMenu(false);
    isSuddenDeath = false;

    Team1PlayersContainer = (*i_Team1->TeamRoster);
    Team2PlayersContainer = (*i_Team2->TeamRoster);

    i_Team1->SetPlayerPositions();
    i_Team2->SetPlayerPositions();

    EndOfMatchText[1].setString(i_Team1->GetTeamName());
    EndOfMatchText[2].setString(i_Team2->GetTeamName());

    
    for (auto i = 0; i < Team1PlayersContainer.size(); i++)
    {
        Team1PlayersContainer.at(i)->InitializeForMatch(1, sf::Vector2f(LeftGoal.getPosition().x + LeftGoal.getSize().x + (200 * MatchSessionScale.x), (100 + (200 * i) * MatchSessionScale.y)));
        Team1PlayersContainer.at(i)->SetPlayerID(i + 1);
        Team1PlayersContainer.at(i)->SetTeamNumber(1);
        Team1PlayersContainer.at(i)->ResetCooldowns();
        m_LuaManager.RegisterPlayerWithLua(Team1PlayersContainer.at(i));
    }
    for (auto i = 0; i < Team2PlayersContainer.size(); i++)
    {
        Team2PlayersContainer.at(i)->InitializeForMatch(2, sf::Vector2f(RightGoal.getPosition().x + RightGoal.getSize().x + (200 * MatchSessionScale.x), (100 + (200 * i) * MatchSessionScale.y)));
        Team2PlayersContainer.at(i)->SetPlayerID(i + 6);
        Team2PlayersContainer.at(i)->SetTeamNumber(2);
        Team2PlayersContainer.at(i)->ResetCooldowns();
        m_LuaManager.RegisterPlayerWithLua(Team2PlayersContainer.at(i));

    }



    TeamWithBall = 0;
    StopPlay = true;
    ResetPlayers();

    if (Team1PlayersContainer.size() > 0)
    {
        ControlledPlayer = Team1PlayersContainer.at(0);
        ControlledPlayer->SetIsPlayerControlled(true);
    }




    if (ControlledPlayer == nullptr)
    {
        return false;
    }

    SetMatchSessionScale(MatchSessionScale);
    return true;
}

bool MatchSession::Initialize()
{


    if (!MapTexture.loadFromFile("Data/Field1.png")) { return false; };
    MapSprite.setTexture(MapTexture);
    MapSprite.setOrigin(900, 372);
    MapSprite.setPosition(sf::Vector2f(960, 420));

    EndOfMatchPopUpMenu.SetMenuPosition(MapSprite.getPosition());
    EndOfMatchPopUpMenu.AddButton(sf::RectangleShape(sf::Vector2f(396, 56)), sf::Vector2f(MapSprite.getPosition().x - 200, MapSprite.getPosition().y + 157), 0, EventType::NoEvent, std::bind(&MatchSession::EndMatch, this));

    MatchSessionMenuPopUp.SetMenuPosition(MapSprite.getPosition());
    MatchSessionMenuPopUp.AddButton(sf::RectangleShape(sf::Vector2f(300, 50)), sf::Vector2f(MapSprite.getPosition().x - 150, MapSprite.getPosition().y + -25), 0, EventType::NoEvent, std::bind(&MatchSession::AbortMatch, this));

    MiddleThird.setSize(sf::Vector2f(MapSprite.getGlobalBounds().width / 3, MapSprite.getGlobalBounds().height));
    MiddleThird.setOrigin(MiddleThird.getSize().x / 2, MiddleThird.getSize().y / 2);
    MiddleThird.setPosition(MapSprite.getPosition());

    NorthWall.setPosition(MapSprite.getPosition().x - MapSprite.getOrigin().x + 22, MapSprite.getPosition().y - MapSprite.getOrigin().y);
    SouthWall.setPosition(MapSprite.getPosition().x - MapSprite.getOrigin().x + 22, MapSprite.getPosition().y + MapSprite.getOrigin().y - 22);

    WestWallNorthHalf.setPosition(MapSprite.getPosition().x - MapSprite.getOrigin().x, MapSprite.getPosition().y - MapSprite.getOrigin().y + 22);
    EastWallNorthHalf.setPosition(MapSprite.getPosition().x + MapSprite.getOrigin().x - 22, MapSprite.getPosition().y - MapSprite.getOrigin().y + 22);

    LeftGoal.setPosition(WestWallNorthHalf.getPosition().x, WestWallNorthHalf.getPosition().y + 250);
    RightGoal.setPosition(EastWallNorthHalf.getPosition().x, EastWallNorthHalf.getPosition().y + 250);

    WestWallSouthHalf.setPosition(LeftGoal.getPosition().x, LeftGoal.getPosition().y + 200);
    EastWallSouthHalf.setPosition(RightGoal.getPosition().x, RightGoal.getPosition().y + 200);

    MapOutline.setSize(sf::Vector2f(MapSprite.getGlobalBounds().width, MapSprite.getGlobalBounds().height));
    MapOutline.setOrigin(MapSprite.getOrigin());
    MapOutline.setPosition(MapSprite.getPosition());
    MapOutline.setFillColor(sf::Color::Transparent);
    MapOutline.setOutlineColor(sf::Color::Transparent);
    MapOutline.setOutlineThickness(4);


    if (!BallTexture.loadFromFile("Data/GameBall.png")) { return false; };
    BallSprite.setTexture(BallTexture);
    BallSprite.setOrigin(11, 10);
    BallSprite.setPosition(MapSprite.getPosition());

    if (!MainFont.loadFromFile("Data/ArialFont/arial.ttf"))
    {

    }
    ShotVelocityText[0].setFillColor(sf::Color::White);
    ShotVelocityText[0].setFont(MainFont);
    ShotVelocityText[0].setOrigin(sf::Vector2f(10, 2));
    ShotVelocityText[0].setPosition(sf::Vector2f(BallSprite.getPosition().x, BallSprite.getPosition().y + BallSprite.getOrigin().y));
    ShotVelocityText[0].setOutlineColor(sf::Color::Black);
    ShotVelocityText[0].setOutlineThickness(1);
    ShotVelocityText[0].setCharacterSize(24);
    ShotVelocityText[0].setString("00");


    for (auto i = 0; i < 5; i++)
    {
        EndOfMatchText[i].setFillColor(sf::Color::Black);
        EndOfMatchText[i].setFont(MainFont);
        EndOfMatchText[i].setCharacterSize(32);
    }
    EndOfMatchText[0].setPosition(sf::Vector2f(MapSprite.getPosition().x - 45, MapSprite.getPosition().y - 240));
    EndOfMatchText[1].setPosition(sf::Vector2f(MapSprite.getPosition().x - 190, MapSprite.getPosition().y - 180));
    EndOfMatchText[2].setPosition(sf::Vector2f(MapSprite.getPosition().x + 70, MapSprite.getPosition().y - 180));
    EndOfMatchText[3].setPosition(sf::Vector2f(MapSprite.getPosition().x - 19, MapSprite.getPosition().y - 180));
    EndOfMatchText[4].setPosition(sf::Vector2f(MapSprite.getPosition().x + 17, MapSprite.getPosition().y - 180));

    //lua
    m_LuaManager.Initialize();



    for (auto i = 0; i < 3; i++)
    {
        MatchInfoText[i].setFillColor(sf::Color::White);
        MatchInfoText[i].setFont(MainFont);
        MatchInfoText[i].setOutlineColor(sf::Color::Black);
        MatchInfoText[i].setOutlineThickness(1);
        MatchInfoText[i].setCharacterSize(36);
    }

    MatchInfoText[0].setString("0");
    MatchInfoText[0].setPosition(sf::Vector2f(120, 10));
    MatchInfoText[1].setString("0");
    MatchInfoText[1].setPosition(sf::Vector2f(1600, 10));
    MatchInfoText[2].setString("0:00");
    MatchInfoText[2].setPosition(sf::Vector2f(920, 10));

    Team1Goals = 0;
    Team2Goals = 0;
    MatchDuration = 90.0f;
    MatchRemainingTime = MatchDuration;



    return true;


}

void MatchSession::DrawMatchSession(sf::RenderWindow & MainWindow)
{
    MainWindow.draw(MapSprite);
    MainWindow.draw(MapOutline);
    for (auto i = 0; i < Team1PlayersContainer.size(); i++)
    {
        Team1PlayersContainer.at(i)->DrawPlayerSprite(MainWindow);
    }

    for (auto i = 0; i < Team2PlayersContainer.size(); i++)
    {
        Team2PlayersContainer.at(i)->DrawPlayerSprite(MainWindow);
    }
    for (auto i = 0; i < AreaOfEffectContainer.size(); i++)
    {
        MainWindow.draw(AreaOfEffectContainer.at(i).AreaEffected);
    }

    if (DoShowBall)
    {
        MainWindow.draw(BallSprite);
    }
    if (MouseHoveredPlayer)
    {
        MouseOverPlayerRectangle.setPosition(MouseHoveredPlayer->GetPosition());
        MainWindow.draw(MouseOverPlayerRectangle);
    }

    ShotVelocityText[0].setPosition(sf::Vector2f(BallSprite.getPosition().x, BallSprite.getPosition().y  + BallSprite.getOrigin().y));
    ShotVelocityText[0].setString(std::to_string((int)(BallVelocity / 10.0f)));
    MainWindow.draw(ShotVelocityText[0]);
    for (auto i = 0; i < 3; i++)
    {
        MainWindow.draw(MatchInfoText[i]);
    }
   DrawInMatchUI(MainWindow);

    if (EndOfMatchPopUpMenu.GetDoDrawMenu())
    {
        EndOfMatchPopUpMenu.DrawMenu(MainWindow);
        for (auto i = 0; i < 5; i++)
        {
            MainWindow.draw(EndOfMatchText[i]);
        }
    }

    if (MatchSessionMenuPopUp.GetDoDrawMenu())
    {
        MatchSessionMenuPopUp.DrawMenu(MainWindow);
    }

    // used to test the ray casts
   // ShotRayCastBuffer.setFillColor(sf::Color::Transparent);
   // ShotRayCastBuffer.setOutlineThickness(2);
   // MainWindow.draw(ShotRayCastBuffer);
}

void MatchSession::EndMatch()
{
    int WhichTeamWon = 0;
    if (Team1Goals < Team2Goals)
    {
        WhichTeamWon = 1;
    }
    Team1Goals = 0;
    Team2Goals = 0;
    MatchRemainingTime = MatchDuration;
    HandleEvent_CallBack(EventType::MatchEnded, WhichTeamWon);

}

void MatchSession::StartEndOfMatchPopUP()
{
    StopPlay = true;
    EndOfMatchPopUpMenu.SetDoDrawMenu(true);
    
    if (Team1Goals > Team2Goals)
    {
        EndOfMatchText[0].setString("You Win!");
    }
    else
    {
        EndOfMatchText[0].setString("You Lose!");
    }

    EndOfMatchText[3].setString(std::to_string(Team1Goals));
    EndOfMatchText[4].setString(std::to_string(Team2Goals));
}

void MatchSession::StartSuddenDeath()
{
    CreateFloatingText(1.5f, "Sudden Death!", sf::Vector2f(MiddleThird.getPosition().x, MiddleThird.getPosition().y - 300), sf::Color::Black, 108);
    isSuddenDeath = true;
}

void MatchSession::MatchSessionLoop(float DeltaTimeAsSeconds, const sf::Vector2f &i_MouseLocation)
{
    if (StopPlay)
    {
        UpdateMatchInfo(0.0f);
        MouseLocation = i_MouseLocation;
        return;
    }

    UpdateMatchInfo(DeltaTimeAsSeconds);

    //lua manger ai
    TimeSinceLastUpdate += DeltaTimeAsSeconds;
    if (TimeSinceLastUpdate > TimeBetweenUpdates)
    {
        m_LuaManager.UpdateLuaPlayerManager(this);
        TimeSinceLastUpdate = 0.0f;
    }

    //stored for calculating shooting/passing vectors
    MouseLocation = i_MouseLocation;

    CheckForMouseHovering();

    for (auto i = 0; i < Team1PlayersContainer.size(); i++)
    {
        if (Team1PlayersContainer.at(i) != ControlledPlayer)
        {
            CheckIfPlayerShouldUseAbility(Team1PlayersContainer.at(i));
        }
        Team1PlayersContainer.at(i)->PlayerSavedPosition = Team1PlayersContainer.at(i)->GetPosition();
        if (Team1PlayersContainer.at(i)->isBallBeingPassedToYou == true)
        {
            HavePlayerChaseLooseBall(Team1PlayersContainer.at(i)->GetPlayerID());
        }
        Team1PlayersContainer.at(i)->UpdatePlayer(DeltaTimeAsSeconds);
        if (CheckForWallCollision(Team1PlayersContainer.at(i)) || CheckIfOffMap(Team1PlayersContainer.at(i)->GetGlobalBounds()))
        {

        }
        
    }
    for (auto i = 0; i < Team2PlayersContainer.size(); i++)
    {
        if (Team2PlayersContainer.at(i) != ControlledPlayer)
        {
            CheckIfPlayerShouldUseAbility(Team2PlayersContainer.at(i));
        }
        Team2PlayersContainer.at(i)->PlayerSavedPosition = Team2PlayersContainer.at(i)->GetPosition();
        if (Team2PlayersContainer.at(i)->isBallBeingPassedToYou == true)
        {
            HavePlayerChaseLooseBall(Team2PlayersContainer.at(i)->GetPlayerID());
        }
        Team2PlayersContainer.at(i)->UpdatePlayer(DeltaTimeAsSeconds);
        if (CheckForWallCollision(Team2PlayersContainer.at(i)) || CheckIfOffMap(Team2PlayersContainer.at(i)->GetGlobalBounds()))
        {
        }
    }

    if (ControlledPlayer)
    {
        ControlledPlayer->SetRotation(MouseLocation);
        if (CheckForWallCollision(ControlledPlayer))
        {
        }
    }

    MoveBall(DeltaTimeAsSeconds);
    
    CheckForAreaOfEffectCollsion();
    CheckForBallWallCollision();
    CheckForGoal();
    CheckForLooseBallPickup();
}

void MatchSession::ToggleMovePlayerUp(int ToggleNumber)
{
    if (ControlledPlayer)
    {
        if (ToggleNumber)
        {
            ControlledPlayer->MovingUp = true;
            isMoveUpPressed = true;
        }
        else
        {
            ControlledPlayer->MovingUp = false;
            isMoveUpPressed = false;
        }
    }
}

void MatchSession::ToggleMovePlayerDown(int ToggleNumber)
{
    if (ControlledPlayer)
    {
        if (ToggleNumber)
        {
            ControlledPlayer->MovingDown = true;
            isMoveDownPressed = true;
        }
        else
        {
            ControlledPlayer->MovingDown = false;
            isMoveDownPressed = false;

        }
    }
}

void MatchSession::ToggleMovePlayerRight(int ToggleNumber)
{
    if (ControlledPlayer)
    {
        if (ToggleNumber)
        {
            ControlledPlayer->MovingRight = true;
            isMoveRightPressed = true;
        }
        else
        {
            ControlledPlayer->MovingRight = false;
            isMoveRightPressed = false;
        }
    }
}

void MatchSession::ToggleMovePlayerLeft(int ToggleNumber)
{
    if (ControlledPlayer)
    {
        if (ToggleNumber)
        {
             ControlledPlayer->MovingLeft = true;
             isMoveLeftPressed = true;

        }
        else
        {
            ControlledPlayer->MovingLeft = false;
            isMoveLeftPressed = false;

        }
    }
}

void MatchSession::SwitchPlayer(int PlayerNumber)
{
    if (ControlledPlayer)
    {
        ControlledPlayer->SetIsPlayerControlled(false);
        ControlledPlayer->CancelActions();
    }
    if (Team1PlayersContainer.size() > PlayerNumber)
    {

        ControlledPlayer = Team1PlayersContainer.at(PlayerNumber);
        ControlledPlayer->SetIsPlayerControlled(true);

        ControlledPlayer->MovingUp = isMoveUpPressed;
        ControlledPlayer->MovingDown = isMoveDownPressed;
        ControlledPlayer->MovingRight = isMoveRightPressed;
        ControlledPlayer->MovingLeft = isMoveLeftPressed;

    }
}
void MatchSession::SwitchToPlayerByPointer(std::shared_ptr<Player> i_Player)
{
    if (ControlledPlayer)
    {
        ControlledPlayer->SetIsPlayerControlled(false);
        ControlledPlayer->CancelActions();
    }

    ControlledPlayer = i_Player;
    ControlledPlayer->SetIsPlayerControlled(true);

    ControlledPlayer->MovingUp = isMoveUpPressed;
    ControlledPlayer->MovingDown = isMoveDownPressed;
    ControlledPlayer->MovingRight = isMoveRightPressed;
    ControlledPlayer->MovingLeft = isMoveLeftPressed;
}
void MatchSession::SwitchToClosestPlayer()
{
    float LongestDistance = 10000.0f;
    int ClosestTeamMemberPositionNumber = 0;

    float CheckDistance = 0.0f;
    for (auto i = 0; i < Team1PlayersContainer.size(); i++)
    {
        CheckDistance = sqrt(pow((Team1PlayersContainer.at(i)->GetPosition().x - GetBallPositionX()), 2) + pow(Team1PlayersContainer.at(i)->GetPosition().y - GetBallPositionY(), 2));
        if (CheckDistance < LongestDistance)
        {
            LongestDistance = CheckDistance;
            ClosestTeamMemberPositionNumber = i;
        }
    }
    SwitchPlayer(ClosestTeamMemberPositionNumber);
}

void MatchSession::ShootBallEvent()
{
    if (ControlledPlayer && BallCarrierPointer)
    {
        if (ControlledPlayer == BallCarrierPointer)
        {
            ShootBall(MouseLocation);
        }
        else
        {
            if (ControlledPlayer->GetGlobalBounds().intersects(BallSprite.getGlobalBounds()))
            {
                ShootBall(MouseLocation);
            }
        }
    }
}

void MatchSession::RightClickEvent()
{
    if (ControlledPlayer)
    {
        // tackle
        if (ControlledPlayer->GetTeamNumber() != TeamWithBall || IsBallLoose())
        {
            //Tackle
            Tackle(ControlledPlayer);
        }
        else
        {

            // pass
            if (BallCarrierPointer)
            {

                if (ControlledPlayer == BallCarrierPointer)
                {
                
                    PassBallToLocation(MouseLocation);

                    if (MouseHoveredPlayer)
                    {
                        if (MouseHoveredPlayer->GetTeamNumber() == ControlledPlayer->GetTeamNumber())
                        {
                            SwitchToPlayerByPointer(MouseHoveredPlayer);
                        }
                    }
                    else
                    {
                        float LongestDistance = 10000.0f;
                        int ClosestTeamMemberPositionNumber = 0;

                        float CheckDistance = 0.0f;
                        for (auto i = 0; i < Team1PlayersContainer.size(); i++)
                        {
                            CheckDistance = sqrt(pow((Team1PlayersContainer.at(i)->GetPosition().x - MouseLocation.x), 2) + pow(Team1PlayersContainer.at(i)->GetPosition().y - MouseLocation.y, 2));
                            if (CheckDistance < LongestDistance)
                            {
                                LongestDistance = CheckDistance;
                                ClosestTeamMemberPositionNumber = i;
                            }
                        }
                        SwitchPlayer(ClosestTeamMemberPositionNumber);
                    }

                }
                else
                {
                    if (ControlledPlayer->GetGlobalBounds().intersects(BallSprite.getGlobalBounds()))
                    {
                        PassBallToLocation(MouseLocation);
                    }
                }
            }
        }

    }
    BallCarrierPointer = nullptr;
}

void MatchSession::SpecialShootBallEvent()
{
    if (ControlledPlayer && BallCarrierPointer)
    {
        if (ControlledPlayer == BallCarrierPointer)
        {
            SpecialShootBall();
        }
    }
}

void MatchSession::ShootBall(const sf::Vector2f &ShootTarget)
{

    HandleEvent_CallBack(EventType::BallShot, 0);
    if (BallCarrierPointer)
    {
        if (!BallCarrierPointer->GetGlobalBounds().contains(BallSprite.getPosition()))
        {
            return;
        }
        ShootingPlayer = BallCarrierPointer;
    }
    else
    {
        return;
    }
    isBallBeingShot = true;
    BallVelocity = BallCarrierPointer->GetPlayerShotPower() * 10;

    //calculate moveVector
    float xDistance = ShootTarget.x - BallCarrierPointer->GetPosition().x;
    float yDistance = ShootTarget.y - BallCarrierPointer->GetPosition().y;
    float TotalDistance = (sqrt(xDistance * xDistance + yDistance * yDistance));
    float Rotation = 0.0f;
    if (!(TotalDistance == 0))
    {
        Rotation = (acos(xDistance / TotalDistance) * 180.0 / 3.14159);
    }
    if (BallCarrierPointer->GetPosition().y > ShootTarget.y) // below target
    {
        Rotation = (360 - Rotation);
    }

    BallMoveDirection = Rotation;

    BallSprite.move(sf::Vector2f(cos(BallMoveDirection * 3.1415 / 180.0) * 50, sin(BallMoveDirection * 3.1415 / 180.0) * 50));
}

void MatchSession::BallCarrierDropBall()
{
    if (BallCarrierPointer)
    {
        BallMoveDirection = BallCarrierPointer->GetRotaion();
        BallSprite.move(Scale(sf::Vector2f(cos(BallMoveDirection * 3.1415 / 180.0) * 40, sin(BallMoveDirection * 3.1415 / 180.0) * 40)));
        BallVelocity = 20.0f;
        TeamWithBall = 0;
        BallCarrierPointer = nullptr;
    }

}

void MatchSession::GoalScored(int ScoringTeam)
{
    CreateFloatingText(2.0f, "Goal!", sf::Vector2f(MiddleThird.getPosition().x, MiddleThird.getPosition().y - 300), sf::Color::Black, 108);

    if (ScoringTeam == 1)
    {
        Team1Goals++;
    }
    else if (ScoringTeam == 2)
    {
        Team2Goals++;
    }
    if (isSuddenDeath)
    {
        StartEndOfMatchPopUP();
        return;
    }

    std::shared_ptr<TimerProcess> NewProcess = std::make_shared<TimerProcess>();
    NewProcess->SetTimer(2.0f);
    NewProcess->SetSuccessFunction(std::bind(&MatchSession::ResetPlayers, this));
    AddProcess_Callback(NewProcess);

    StopPlay = true;

    TeamWithBall = 0;
    StopBall();

    HandleEvent_CallBack(EventType::GoalScored, 0);


}

void MatchSession::UpdateMatchInfo(float DeltaTimeAsSeconds)
{
    MatchRemainingTime -= DeltaTimeAsSeconds;
    if (MatchRemainingTime <= 0.0f)
    {
        MatchRemainingTime = 0.0f;
    }

    //timer stuff
    int MinutesRemaining = (MatchRemainingTime / 60);
    int SecondsRemaining = (MatchRemainingTime - (MinutesRemaining * 60));

    std::string SecondsString;
    if (SecondsRemaining < 10.0f)
    {
        SecondsString = ("0" + std::to_string(SecondsRemaining));
    }
    else
    {
        SecondsString = std::to_string(SecondsRemaining);
    }
    MatchInfoText[0].setString(Team1Name + "  " +  std::to_string(Team1Goals));
    MatchInfoText[1].setString(Team2Name + "  " +  std::to_string(Team2Goals));
    MatchInfoText[2].setString(std::to_string(MinutesRemaining) + ":" + SecondsString);

    if (MatchRemainingTime <= 0.0f && isSuddenDeath == false)
    {
        if (Team2Goals == Team1Goals)
        {

            StartSuddenDeath();
        }
        else
        {
            StartEndOfMatchPopUP();
        }
    }

}

void MatchSession::StartCountdown()
{
    CreateFloatingText(3.0f, "3", sf::Vector2f(MiddleThird.getPosition().x, MiddleThird.getPosition().y - 300), sf::Color::Black, 108, true);

    HandleEvent_CallBack(EventType::CountDownStarted, 0);

    std::shared_ptr<TimerProcess> NewProcess = std::make_shared<TimerProcess>();
    NewProcess->SetTimer(3.0f);
    NewProcess->SetSuccessFunction(std::bind(&MatchSession::BeginPlay, this));
    AddProcess_Callback(NewProcess);
}

void MatchSession::BeginPlay()
{
    StopPlay = false;
}

void MatchSession::ResetPlayers()
{
    isMoveUpPressed = false;
    isMoveDownPressed = false;
    isMoveLeftPressed = false;
    isMoveRightPressed = false;

    BallSprite.setPosition(MapSprite.getPosition());
    StopBall();
    for (auto i = 0; i < Team1PlayersContainer.size(); i++)
    {
        Team1PlayersContainer.at(i)->ResetPlayer(GetDefensiveThird(Team1PlayersContainer.at(i)->GetTeamNumber()), MiddleThird);

    }
    for (auto i = 0; i < Team2PlayersContainer.size(); i++)
    {
        Team2PlayersContainer.at(i)->ResetPlayer(GetDefensiveThird(Team2PlayersContainer.at(i)->GetTeamNumber()), MiddleThird);
    }
    StartCountdown();
}

void MatchSession::CheckForMouseHovering()
{
    sf::RectangleShape MousePointerEnlargerBox(Scale(sf::Vector2f(150, 150)));
    MousePointerEnlargerBox.setOrigin(MousePointerEnlargerBox.getSize().x / 2, MousePointerEnlargerBox.getSize().y / 2);
    MousePointerEnlargerBox.setPosition(MouseLocation);

    MouseHoveredPlayer = nullptr;
    for (auto i = 0; i < Team1PlayersContainer.size(); i++)
    {
        if (Team1PlayersContainer.at(i)->GetGlobalBounds().intersects(MousePointerEnlargerBox.getGlobalBounds()))
        {
            MouseHoveredPlayer = Team1PlayersContainer.at(i);
            return;
        }
    }
    for (auto i = 0; i < Team2PlayersContainer.size(); i++)
    {
        if (Team2PlayersContainer.at(i)->GetGlobalBounds().intersects(MousePointerEnlargerBox.getGlobalBounds()))
        {
            MouseHoveredPlayer = Team2PlayersContainer.at(i);
            return;
        }
    }
}

void MatchSession::PassBallToLocation(const sf::Vector2f &TargetLocation)
{

    if (BallCarrierPointer)
    {
        if (!BallCarrierPointer->GetGlobalBounds().contains(BallSprite.getPosition()))
        {
            return;
        }
        ShootingPlayer = BallCarrierPointer;
    }
    else
    {
        return;
    }

    //calculate moveVector
    float xDistance = TargetLocation.x - BallCarrierPointer->GetPosition().x;
    float yDistance = TargetLocation.y - BallCarrierPointer->GetPosition().y;
    float TotalDistance = (sqrt(xDistance * xDistance + yDistance * yDistance));
    float Rotation = 0.0f;
    if (!(TotalDistance == 0))
    {
        Rotation = (acos(xDistance / TotalDistance) * 180.0 / 3.14159);
    }
    if (BallCarrierPointer->GetPosition().y > TargetLocation.y) // below target
    {
        Rotation = (360 - Rotation);
    }

    if (TotalDistance < Scale(350))
    {
        BallVelocity = 450;
    }
    else if (TotalDistance < Scale(700))
    {
        BallVelocity = TotalDistance + 100;

    }
    else
    {
        BallVelocity = 800;
    }
    BallMoveDirection = Rotation;

    BallSprite.move(Scale(sf::Vector2f(cos(BallMoveDirection * 3.1415 / 180.0) * 50, sin(BallMoveDirection * 3.1415 / 180.0) * 50)));

    HandleEvent_CallBack(EventType::BallPassed, 0);

    return;
}

void MatchSession::SpecialShootBall()
{
    if (BallCarrierPointer)
    {
        if (!BallCarrierPointer->CheckAbilityCooldown())
        {
            // Shot not ready
            return;
        }
        ShootingPlayer = BallCarrierPointer;


        switch (BallCarrierPointer->GetPlayerSpeicalAbility())
        {
        case Player::SpecialAbility::CurveShot:
            HandleEvent_CallBack(EventType::BallShot, 0);
            CurveShot(MouseLocation);
            BallCarrierPointer->UseAbility();
            break;
        case Player::SpecialAbility::PowerShot:
            HandleEvent_CallBack(EventType::BallShot, 0);
            PowerShot(MouseLocation);
            BallCarrierPointer->UseAbility();
            break;
        default:
            ShootBall(MouseLocation);
            break;
        }
    }
    else
    {
        return;
    }
}

void MatchSession::UseAbilityEvent()
{
    if (StopPlay)
    {
        return;
    }
    if (ControlledPlayer)
    {
        if (!ControlledPlayer->CheckAbilityCooldown())
        {
            // Ability not ready
            return;
        }



        switch (ControlledPlayer->GetPlayerSpeicalAbility())
        {
        case Player::SpecialAbility::Blink:
            Blink(ControlledPlayer);
            ControlledPlayer->UseAbility();
            break;
        case Player::SpecialAbility::BlazingSpeed:
            ControlledPlayer->UseAbility();
            Blaze(ControlledPlayer);
            break;
        case Player::SpecialAbility::FrostNova:
            ControlledPlayer->UseAbility();
            CreateIcePatch(MouseLocation);
            break;
        case Player::SpecialAbility::VoidChain:
            VoidChain(ControlledPlayer, MouseLocation);
            ControlledPlayer->UseAbility();
            //Void Chains
            break;
            // dont use ability when they have a shooting special and they are not shooting
        case Player::SpecialAbility::CurveShot:
        case Player::SpecialAbility::PowerShot:
            break;

        }
    }
}

void MatchSession::CreateIcePatch(const sf::Vector2f &i_TargetLocation)
{
    if (!MapSprite.getGlobalBounds().contains(i_TargetLocation))
    {
        return;
    }
    AreaEffect NewEffect;
    NewEffect.AreaEffected.setRadius(75 * MatchSessionScale.x);
    NewEffect.AreaEffected.setOrigin(NewEffect.AreaEffected.getRadius(), NewEffect.AreaEffected.getRadius());
    NewEffect.AreaEffected.setPosition(i_TargetLocation);
    NewEffect.AreaEffected.setFillColor(sf::Color::Transparent);
    NewEffect.AreaEffected.setOutlineColor(sf::Color::Blue);
    NewEffect.AreaEffected.setOutlineThickness(2);
    NewEffect.AppliedBuff = Buff(0.5f, -0.5f, false);
    AreaOfEffectContainer.push_back(NewEffect);


    std::shared_ptr<TimerProcess> NewProcess = std::make_shared<TimerProcess>();
    NewProcess->SetTimer(2.0f);
    NewProcess->SetSuccessFunction(std::bind(&MatchSession::EraseLastAreaOfEffect, this));
    AddProcess_Callback(NewProcess);
}

void MatchSession::Blink(std::shared_ptr<Player> i_Player)
{
    bool HasBallPosession = false;


    if (i_Player)
    {
        sf::Vector2f SavedPosition = i_Player->GetPosition();
        if (i_Player->GetGlobalBounds().contains(BallSprite.getPosition()))
        {
            HasBallPosession = true;
        }
        float Rotation = i_Player->GetRotaion();
        i_Player->SetSpritePosition(sf::Vector2f(i_Player->GetPosition().x + cos(Rotation * 3.1415 / 180.0) * (150 * MatchSessionScale.x), i_Player->GetPosition().y + sin(Rotation * 3.1415 / 180.0) * (150 * MatchSessionScale.x)));
   

        if (CheckForWallCollision(i_Player->GetGlobalBounds()) || !MapSprite.getGlobalBounds().intersects(i_Player->GetGlobalBounds()))
        {
            i_Player->SetSpritePosition(SavedPosition);
        }
        else
        {
            if (HasBallPosession)
            {
                BallSprite.setPosition(i_Player->GetPosition());
            }
        }

        i_Player->AddBuff(0.15f, 0.0f, true);
    }

}

void MatchSession::Blaze(std::shared_ptr<Player> i_Player)
{
    if (i_Player)
    {
        i_Player->AddBuff(Buff(1.5f, 0.5f, false));
    }
}


void MatchSession::VoidChain(std::shared_ptr<Player> i_Player, const sf::Vector2f &i_TargetLocation)
{
    if (!i_Player)
    {
        return;
    }
    if (!MapSprite.getGlobalBounds().contains(MouseLocation))
    {
        return;
    }
    std::shared_ptr<ProjectileProcess> NewProjectile = std::make_shared<ProjectileProcess>(10.0f, MatchSessionScale);
    i_Player->SetRotation(i_TargetLocation);
    NewProjectile->AppliedBuff = Buff(2.0f, 0.0f, true);
    NewProjectile->CheckForCollison_CallBack = std::bind(&MatchSession::CheckForProjectileCollison, this, std::placeholders::_1, std::placeholders::_2);
    NewProjectile->SetAnimationMoveSpeed(800.0f);
    NewProjectile->SetRotation(i_Player->GetRotaion());
    NewProjectile->SetAnimationPosition(sf::Vector2f(i_Player->GetPosition().x +  cos(i_Player->GetRotaion() * 3.1415 / 180.0) * (50 * MatchSessionScale.x), i_Player->GetPosition().y + sin(i_Player->GetRotaion() * 3.1415 / 180.0) * (50 * MatchSessionScale.y)));

    AddProcess_Callback(NewProjectile);
}

void MatchSession::ClearBallMods()
{
    isBallInvurnerable = false;
    DoShowBall = true;
}

bool MatchSession::CheckForProjectileCollison(const sf::FloatRect &ProjectileBounds, const Buff &i_AppliedBuff)
{
    if (!MapSprite.getGlobalBounds().intersects(ProjectileBounds))
    {
        return true;
    }
    if (CheckForWallCollision(ProjectileBounds))
    {
        return true;
    }

    for (auto i = 0; i < Team2PlayersContainer.size(); i++)
    {
        if (ProjectileBounds.intersects(Team2PlayersContainer.at(i)->GetGlobalBounds()))
        {
            Team2PlayersContainer.at(i)->AddBuff(i_AppliedBuff);
            return true;
        }
    }
    for (auto i = 0; i < Team1PlayersContainer.size(); i++)
    {
        if (ProjectileBounds.intersects(Team1PlayersContainer.at(i)->GetGlobalBounds()))
        {
            Team1PlayersContainer.at(i)->AddBuff(i_AppliedBuff);
            return true;
        }
    }

    return false;
}

float MatchSession::GetBallPositionX()
{
    return BallSprite.getPosition().x;
}

float MatchSession::GetBallPositionY()
{
    return BallSprite.getPosition().y;
}

bool MatchSession::isClosestToBall(int i_PlayerID, int i_TeamNumber)
{
    float LongestDistance = 10000.0f;
    int ClosestTeamMemberID = 0;

    float CheckDistance = 0.0f;
    if (i_TeamNumber == 1)
    {
        for (auto i = 0; i < Team1PlayersContainer.size(); i++)
        {
            CheckDistance = sqrt(pow((Team1PlayersContainer.at(i)->GetPosition().x - GetBallPositionX()), 2) + pow(Team1PlayersContainer.at(i)->GetPosition().x - GetBallPositionX(), 2));
            if (CheckDistance < LongestDistance)
            {
                LongestDistance = CheckDistance;
                ClosestTeamMemberID = Team1PlayersContainer.at(i)->GetPlayerID();
            }
        }
    }
    else if (i_TeamNumber == 2)
    {
        for (auto i = 0; i < Team2PlayersContainer.size(); i++)
        {
            CheckDistance = sqrt(pow((Team2PlayersContainer.at(i)->GetPosition().x - GetBallPositionX()), 2) + pow(Team2PlayersContainer.at(i)->GetPosition().x - GetBallPositionX(), 2));
            if (CheckDistance < LongestDistance)
            {
                LongestDistance = CheckDistance;
                ClosestTeamMemberID = Team2PlayersContainer.at(i)->GetPlayerID();
            }
        }
    }

    if (ClosestTeamMemberID == i_PlayerID)
    {
        return true;
    }
    return false;
}

void MatchSession::HavePlayerChaseLooseBall(int i_PlayerID)
{
    std::shared_ptr<Player> i_Player = GetPlayerByID(i_PlayerID);
    if (i_Player)
    {
        sf::Vector2f TargetLocaiton = BallSprite.getPosition();
        i_Player->SetRotation(BallSprite.getPosition());
        //
        if (TargetLocaiton.x < WestWallNorthHalf.getPosition().x + WestWallNorthHalf.getGlobalBounds().width + (BallSprite.getOrigin().x * 2))
        {
            TargetLocaiton.x += (BallSprite.getOrigin().x * 1.5);
        }
        else if (TargetLocaiton.x > EastWallNorthHalf.getPosition().x - EastWallNorthHalf.getGlobalBounds().width - (BallSprite.getOrigin().x * 2))
        {
            TargetLocaiton.x -= (BallSprite.getOrigin().x * 1.5);
        }

        if (TargetLocaiton.y < NorthWall.getPosition().y + NorthWall.getGlobalBounds().height + (BallSprite.getOrigin().y * 2))
        {
            TargetLocaiton.y += (BallSprite.getOrigin().y * 1.5);
        }
        else if (TargetLocaiton.y < SouthWall.getPosition().y - SouthWall.getGlobalBounds().height - (BallSprite.getOrigin().y * 2))
        {
            TargetLocaiton.y -= (BallSprite.getOrigin().y * 1.5);
        }

        i_Player->SetTargetLocation(TargetLocaiton, Scale(sf::Vector2f(25, 25)));
    }


}

void MatchSession::HavePlayerSupportBall(int i_PlayerID, int i_TeamNumber)
{
    std::shared_ptr<Player> i_Player = GetPlayerByID(i_PlayerID);


    if (TeamWithBall == 0)
    {

        // true loose ball conditions
        if (i_Player)
        {
            switch (i_Player->GetPlayerPosition())
            {
            case Player::PlayerPosition::CenterDefender:
            case Player::PlayerPosition::UpperDefender:
                //Pass through
            case Player::PlayerPosition::LowerDefender:
                GetInDefensivePosition(i_Player->GetPlayerID(), i_Player->GetTeamNumber());
                break;

            case Player::PlayerPosition::MidFielder:
                GetOpenInMidField(i_Player);
                break;
            case Player::PlayerPosition::UpperWing:
            case Player::PlayerPosition::LowerWing:
            case Player::PlayerPosition::CenterAttacker:
                GetOpenInMidField(i_Player);
                break;
            case Player::PlayerPosition::ForwardGoalie:
            case Player::PlayerPosition::Goalie:
                GetInDefensivePosition(i_Player->GetPlayerID(), i_Player->GetTeamNumber());
                break;
            default:
                i_Player->HoldPosition();
                break;
            }
        }
    }
    else if (i_TeamNumber == TeamWithBall)
    {
        SendPlayerToAttackingPosition(i_PlayerID, i_TeamNumber);
    }
    else
    {
        GetInDefensivePosition(i_PlayerID, i_TeamNumber);
    }

   
}

bool MatchSession::IsBallLoose()
{
    if (isBallInvurnerable)
    {
        return true;
    }
    for (auto i = 0; i < Team2PlayersContainer.size(); i++)
    {
        if (BallSprite.getGlobalBounds().intersects(Team2PlayersContainer.at(i)->GetGlobalBounds()))
        {
            TeamWithBall = 2;
            return false;
        }
    }
    for (auto i = 0; i < Team1PlayersContainer.size(); i++)
    {
        if (BallSprite.getGlobalBounds().intersects(Team1PlayersContainer.at(i)->GetGlobalBounds()))
        {
            TeamWithBall = 1;
            return false;
        }
    }
    return true;
}

bool MatchSession::CheckIfShouldMarkPlayerInScoringPosition()
{
    sf::RectangleShape DefensiveThrid = MiddleThird; 

    if (TeamWithBall == 1)
    {
        DefensiveThrid = GetDefensiveThird(2);
        for (auto i = 0; i < Team1PlayersContainer.size(); i++)
        {
            if (Team1PlayersContainer.at(i)->GetGlobalBounds().intersects(DefensiveThrid.getGlobalBounds()))
            {
                if (!CheckIfPlayerIsMarked(Team1PlayersContainer.at(i), sf::Vector2f(200,200)))
                {
                    // player is open

                    return true;
                }
            }
        }
    }
    else
    {
        DefensiveThrid = GetDefensiveThird(1);
        for (auto i = 0; i < Team2PlayersContainer.size(); i++)
        {
            if (Team2PlayersContainer.at(i)->GetGlobalBounds().intersects(DefensiveThrid.getGlobalBounds()))
            {
                if (!CheckIfPlayerIsMarked(Team2PlayersContainer.at(i), sf::Vector2f(200,200)))
                {
                    // player is open
                    return true;
                }
            }
        }
    }
    return false;
}

bool MatchSession::DoesPlayerHaveShot(std::shared_ptr<Player> i_Player, int ShotRange)
{
    sf::Vector2f TargetLocation = sf::Vector2f(RightGoal.getPosition().x, RightGoal.getPosition().y + 55);
    if (i_Player->GetTeamNumber() == 1)
    {

        for (auto i = 0; i < 3; i++)
        {
            TargetLocation = sf::Vector2f(RightGoal.getPosition().x, RightGoal.getPosition().y + (25 + (75*i)));
            if (RayShotCheck(i_Player, ShotRange, TargetLocation))
            {
                AIShotTargetLocation = TargetLocation;
              
                return true;
            }
        }
    }
    else
    {
        for (auto i = 0; i < 3; i++)
        {
            TargetLocation = sf::Vector2f(LeftGoal.getPosition().x, LeftGoal.getPosition().y + (25 + (75 * i)));
            if (RayShotCheck(i_Player, ShotRange, TargetLocation))
            {
                AIShotTargetLocation = TargetLocation;
                return true;
            }
        }
    }
    return false;
}

void MatchSession::PassBallToPlayer(std::shared_ptr<Player> i_Player)
{
    i_Player->isBallBeingPassedToYou = true;
    float Distance, XDistance, YDistance;
    float Rotation;


    if (BallCarrierPointer && i_Player)
    {
        XDistance = (i_Player->GetPosition().x - BallCarrierPointer->GetPosition().x);
        YDistance = (i_Player->GetPosition().y - BallCarrierPointer->GetPosition().y);

        Distance = sqrt(pow(XDistance, 2) + pow(YDistance, 2));

        if (Distance < Scale(200))
        {
            BallVelocity = 250;
        }
        else if (Distance < Scale(450))
        {
            BallVelocity = Distance + 50;

        }
        else if (Distance < Scale(700))
        {
            BallVelocity = Distance + 50;

        }
        else
        {
            BallVelocity = 800;
        }

        // recalculate distance by predicing where to lead Pass
        XDistance = (i_Player->GetPredictionLocationX(Distance, BallVelocity) - BallCarrierPointer->GetPosition().x);
        YDistance = (i_Player->GetPredictionLocationY(Distance, BallVelocity) - BallCarrierPointer->GetPosition().y);
        Distance = sqrt(pow(XDistance, 2) + pow(YDistance, 2));


        if (!(Distance == 0))
        {
            Rotation = (acos(XDistance / Distance) * 180.0 / 3.14159);
        }
        else
        {
            return;
        }
        if (BallCarrierPointer->GetPosition().y > i_Player->GetPosition().y + 75) // below target
        {
            Rotation = (360 - Rotation);
        }

        BallCarrierPointer->SetRotation(Rotation);


        BallMoveDirection = Rotation;

        BallSprite.move(Scale(sf::Vector2f(cos(BallMoveDirection * 3.1415 / 180.0) * 50, sin(BallMoveDirection * 3.1415 / 180.0) * 50)));

        HandleEvent_CallBack(EventType::BallPassed, 0);

    }
    else return;
}

void MatchSession::SetGoaliePosition(std::shared_ptr<Player> i_Goalie)
{
    sf::Vector2f TargetPositon;
    sf::Vector2f PositionSize(25,25);

    if (i_Goalie->GetTeamNumber() == 1)
    {
        TargetPositon = (sf::Vector2f(LeftGoal.getPosition().x + 50, LeftGoal.getPosition().y + 100));

    }
    else
    {
       TargetPositon = (sf::Vector2f(RightGoal.getPosition().x - 50, RightGoal.getPosition().y + 100));

    }

    if (BallCarrierPointer)
    {
        if (BallCarrierPointer->GetPosition().y < RightGoal.getPosition().y + Scale(50))
        {
            TargetPositon.y = RightGoal.getPosition().y + 50;
        }
        else if (BallCarrierPointer->GetPosition().y > RightGoal.getPosition().y + Scale(50) && BallCarrierPointer->GetPosition().y < RightGoal.getPosition().y + Scale(150))
        {
            TargetPositon.y = BallCarrierPointer->GetPosition().y;
        }
        else
        {
            TargetPositon.y = RightGoal.getPosition().y + Scale(150);
        }

    }
    else
    {
        //difficulty, if easy this can be set to not calculate the ball location and to instead chase after where it is at this momentw
        if (isBallBeingShot)
        {
            TargetPositon = GetBlockShotLocation(i_Goalie);
        }
    }

    i_Goalie->SetTargetLocation(TargetPositon, Scale(PositionSize));
    return;
}

sf::Vector2f MatchSession::GetBlockShotLocation(std::shared_ptr<Player> i_Player)
{
    sf::Vector2f TargetPosition = BallSprite.getPosition();

    float XDistance, YDistance, TotalDistance;

    YDistance = (MiddleThird.getPosition().y - BallSprite.getPosition().y); // middle third has orgin in the middle of the field

    // Determine where ball will hit the goal
    if (i_Player->GetTeamNumber() == 2)
    {
        TargetPosition.x = RightGoal.getPosition().x - Scale(30);
        XDistance = (RightGoal.getPosition().x - BallSprite.getPosition().x);
        TotalDistance = sqrt(pow(XDistance, 2) + pow(YDistance, 2));
    }
    else
    {
        TargetPosition.x = LeftGoal.getPosition().x + Scale(30) + LeftGoal.getGlobalBounds().width;

        XDistance = (LeftGoal.getPosition().x - BallSprite.getPosition().x);
        TotalDistance = sqrt(pow(XDistance, 2) + pow(YDistance, 2));

    }
    TargetPosition.y = BallSprite.getPosition().y + (TotalDistance * sin(BallMoveDirection * 3.1415926 / 180));

    if (TargetPosition.x < LeftGoal.getPosition().x + 2)
    {
        TargetPosition.x = LeftGoal.getPosition().x + 2;
    }
    else if (TargetPosition.x > RightGoal.getPosition().x - 2)
    {
        TargetPosition.x = RightGoal.getPosition().x - 2;
    }
    return TargetPosition;
}

void MatchSession::Tackle(std::shared_ptr<Player> i_Player)
{
    if (i_Player->GetIsStunned() == true)
    {
        return;
    }

    std::vector<std::shared_ptr<Player>> OpposingTeam;

    if (i_Player->GetTeamNumber() == 1)
    {
        OpposingTeam = Team2PlayersContainer;
    }
    else
    {
        OpposingTeam = Team1PlayersContainer;
    }

    float Rotation = i_Player->GetRotaion();
    i_Player->SetSpritePosition(sf::Vector2f(i_Player->GetPosition().x + cos(Rotation * 3.1415 / 180.0) * (35 * MatchSessionScale.x), i_Player->GetPosition().y + sin(Rotation * 3.1415 / 180.0) * (35 * MatchSessionScale.y)));

    for (auto i = 0; i < OpposingTeam.size(); i++)
    {
        if (i_Player->GetGlobalBounds().intersects(OpposingTeam.at(i)->GetGlobalBounds()))
        {
            HandleEvent_CallBack(EventType::PlayerTackled, 0);
            if (OpposingTeam.at(i) != BallCarrierPointer)
            {
                FoulCommited(i_Player);
            }

            // see who wins tackle, tackling player gets a 10 stat bonus
            int TacklingPlayersStrength = i_Player->GetPlayerStrength() + 10 + (RNGClass::GetRandomInteger(0, (int)i_Player->GetPlayerStrength()));
            int BallCarrierStrength = OpposingTeam.at(i)->GetPlayerStrength() + (RNGClass::GetRandomInteger(0, (int)OpposingTeam.at(i)->GetPlayerStrength()));

            if (TacklingPlayersStrength >= BallCarrierStrength)
            {
                //Tackling player won, opposing player drops ball and gets stunned
                OpposingTeam.at(i)->AddBuff(1.25f, 0.0f, true);
                i_Player->AddBuff(0.25f, 0.0f, true);
                if (OpposingTeam.at(i) == BallCarrierPointer)
                {
                    BallCarrierDropBall();
                }
                return;
            }
            else
            {
                //None Tackler won, tackler is stunned and ball carrer is slowed
                OpposingTeam.at(i)->AddBuff(0.5f, -0.5f, false);
                i_Player->AddBuff(1.25f, 0.0f, true);
                return;
            }

            return;
        }
    }
    HandleEvent_CallBack(EventType::TackleMissed, 0);;
    i_Player->AddBuff(0.25f, 0.0f, true);
}

void MatchSession::FoulCommited(std::shared_ptr<Player> i_Player)
{
    MapOutline.setOutlineColor(sf::Color::Red);

    std::shared_ptr<TimerProcess> FoulAlertProcess = std::make_shared<TimerProcess>();
    FoulAlertProcess->SetSuccessFunction(std::bind(&MatchSession::ClearMapOutlineAlerts, this));
    FoulAlertProcess->SetTimer(0.4f);
    if (AddProcess_Callback)
    {
        AddProcess_Callback(FoulAlertProcess);
    }
}

void MatchSession::ClearMapOutlineAlerts()
{
    MapOutline.setOutlineColor(sf::Color::Transparent);
}

int MatchSession::PlayerStrengthBattle(std::shared_ptr<Player> i_Player1, std::shared_ptr<Player> i_Player2)
{
    int Player1Strength = i_Player1->GetPlayerStrength() + (RNGClass::GetRandomInteger(0, (int)i_Player1->GetPlayerStrength()));
    int Player2Strength = i_Player2->GetPlayerStrength() + (RNGClass::GetRandomInteger(0, (int)i_Player2->GetPlayerStrength()));

    if (Player1Strength >= Player2Strength)
    {
        i_Player2->AddBuff(1.0f, 0.0f, true);
        i_Player1->AddBuff(0.25f, 0.0f, true);
        return 1;
    }
    else
    {
        i_Player2->AddBuff(0.25f, 0.0f, true);
        i_Player1->AddBuff(1.0f, 0.0f, true);
        return 2;
    }
}

void MatchSession::CreateFloatingText(float i_AnimationDuration, std::string i_AnimatedText, const sf::Vector2f &i_Position, const sf::Color &i_Color, int i_CharSize)
{
    CreateFloatingText(i_AnimationDuration, i_AnimatedText, i_Position, i_Color, i_CharSize, false);
}

void MatchSession::CreateFloatingText(float i_AnimationDuration, std::string i_AnimatedText, const sf::Vector2f &i_Position, const sf::Color &i_Color, int i_CharSize, bool i_isTimerText)
{
    std::shared_ptr<FloatingTextProcess> NewFloatingTextProcess = std::make_shared<FloatingTextProcess>(i_AnimationDuration, i_AnimatedText, i_Position, i_Color, i_CharSize, i_isTimerText);
    AddProcess_Callback(NewFloatingTextProcess);
}

void MatchSession::ResetIsBallBeingPassedToYou()
{
    for (auto i = 0; i < Team1PlayersContainer.size(); i++)
    {
        Team1PlayersContainer.at(i)->isBallBeingPassedToYou = false;
    }
    for (auto i = 0; i < Team2PlayersContainer.size(); i++)
    {
        Team2PlayersContainer.at(i)->isBallBeingPassedToYou = false;
    }
}

bool MatchSession::RayShotCheck(std::shared_ptr<Player> i_Player, int ShotRange, const sf::Vector2f &TargetLocation)
{

    sf::RectangleShape ShotRayCast(sf::Vector2f(1, 25));

    float Distance, XDistance, YDistance;
    float Rotation;

    if (i_Player)
    {
        XDistance = (TargetLocation.x - i_Player->GetPosition().x);
        YDistance = (TargetLocation.y- i_Player->GetPosition().y);

        Distance = sqrt(pow(XDistance, 2) + pow(YDistance, 2));
        if (Distance > ShotRange)
        {
            return false;
        }
        else if (Distance < 200) //always shoot point blank
        {
            return true;
        }
        float RayCastWidth = (100 / (ShotRange / Distance));
        ShotRayCast.setSize(sf::Vector2f(Distance, RayCastWidth));
        ShotRayCast.setPosition(i_Player->GetPosition());

        if (!(Distance == 0))
        {
            Rotation = (acos(XDistance / Distance) * 180.0 / 3.14159);
        }
        else
        {
            return false;
        }
        if (i_Player->GetPosition().y >TargetLocation.y ) // below target
        {
            Rotation = (360 - Rotation);
        }


        ShotRayCast.setOrigin(sf::Vector2f(0, RayCastWidth / 2));
        ShotRayCast.setPosition(i_Player->GetPosition());
        ShotRayCast.setRotation(Rotation);

        for (auto i = 0; i < Team1PlayersContainer.size(); i++)
        {
            if (i_Player != Team1PlayersContainer.at(i))
            {
                if (ShotRayCast.getGlobalBounds().intersects(Team1PlayersContainer.at(i)->GetGlobalBounds()))
                {
                    return false;
                }
            }
        }

        for (auto i = 0; i < Team2PlayersContainer.size(); i++)
        {
            if (i_Player != Team2PlayersContainer.at(i))
            {
                if (ShotRayCast.getGlobalBounds().intersects(Team2PlayersContainer.at(i)->GetGlobalBounds()))
                {
                    return false;
                }
            }
        }

    }
    else
    {
        return false;
    }
    return true;
}

bool MatchSession::CheckForBetterPlayerPriroity(std::shared_ptr<Player> Contender, std::shared_ptr<Player> Champion)
{
    int ContenderPriorityValue = 0;
    int ChampionPriorityValue = 0;
    switch (Contender->GetPlayerPosition())
    {
    case Player::PlayerPosition::ForwardGoalie:
    case Player::PlayerPosition::Goalie:
        ContenderPriorityValue += 1;
        break;
    case Player::PlayerPosition::CenterDefender:
    case Player::PlayerPosition::UpperDefender:
    case Player::PlayerPosition::LowerDefender:
        ContenderPriorityValue += 2;
        break;
    case Player::PlayerPosition::MidFielder:
        ContenderPriorityValue += 3;
        break;
    case Player::PlayerPosition::UpperWing:
    case Player::PlayerPosition::LowerWing:
        ContenderPriorityValue += 4;
        break;
    case Player::PlayerPosition::CenterAttacker:
        ContenderPriorityValue += 5;
        break;
    }
    switch (Champion->GetPlayerPosition())
    {
    case Player::PlayerPosition::ForwardGoalie:
    case Player::PlayerPosition::Goalie:
        ChampionPriorityValue += 1;
        break;
    case Player::PlayerPosition::CenterDefender:
    case Player::PlayerPosition::UpperDefender:
    case Player::PlayerPosition::LowerDefender:
        ChampionPriorityValue += 2;
        break;
    case Player::PlayerPosition::MidFielder:
        ChampionPriorityValue += 3;
        break;
    case Player::PlayerPosition::UpperWing:
    case Player::PlayerPosition::LowerWing:
        ChampionPriorityValue += 4;
        break;
    case Player::PlayerPosition::CenterAttacker:
        ContenderPriorityValue += 5;
        break;
    }

    if (ContenderPriorityValue > ChampionPriorityValue)
    {
        return true;
    }
    return false;
}

void MatchSession::DrawInMatchUI(sf::RenderWindow & MainWindow)
{
    sf::Vector2f Scale = MapSprite.getScale();
    
    for (auto i = 0; i < Team1PlayersContainer.size(); i++)
    {
        Team1PlayersContainer.at(i)->DrawPlayerCard(MainWindow, sfh::ScalarProduct(sf::Vector2f((20 + (180 * i)), 820), Scale), Scale);
    }
    for (auto i = 0; i < Team2PlayersContainer.size(); i++)
    {
        Team2PlayersContainer.at(i)->DrawPlayerCard(MainWindow, sfh::ScalarProduct(sf::Vector2f((1000 + (180 * i)), 820), Scale), Scale);
    }
}

void MatchSession::AbortMatch()
{
    Team1Goals = 0;
    Team2Goals = 0;
    MatchRemainingTime = MatchDuration;
    HandleEvent_CallBack(EventType::AbortMatch, 0);
}

sf::Vector2f MatchSession::Scale(const sf::Vector2f &i_Input)
{
    return sf::Vector2f(i_Input.x * MatchSessionScale.x, i_Input.y * MatchSessionScale.y);
}

sf::Vector2f MatchSession::Scale(float xInput, float yInput)
{
    return sf::Vector2f(xInput * MatchSessionScale.x, yInput * MatchSessionScale.y);
}

float MatchSession::Scale(float Input)
{
    float Scale = ((MatchSessionScale.x + MatchSessionScale.y) / 2);
    return (Input * Scale);
}

void MatchSession::CheckForPlayerInZone(std::shared_ptr<Player> i_Player, const sf::RectangleShape &DefensiveZone)
{
    auto OpposingTeam = Team1PlayersContainer;
    if (i_Player->GetTeamNumber() == 1)
    {
        OpposingTeam = Team2PlayersContainer;
    }

    for (auto i = 0; i < OpposingTeam.size(); i++)
    {
        if (DefensiveZone.getGlobalBounds().contains(OpposingTeam.at(i)->GetPosition()))
        {
            if (!CheckIfPlayerIsMarked(OpposingTeam.at(i), sf::Vector2f(100, 100)))
            {
                i_Player->SetTargetLocation(OpposingTeam.at(i)->GetPosition(), sf::Vector2f(50, 50)); 
                return;
            }
        }
    }
}

bool MatchSession::DoesBallCarrierHaveOpenShot()
{
    if (BallCarrierPointer)
    {
        if (BallCarrierPointer->CheckAbilityCooldown())
        {
            switch (BallCarrierPointer->GetPlayerSpeicalAbility())
            {
            case Player::SpecialAbility::CurveShot:
                return DoesPlayerHaveShot(BallCarrierPointer, BallCarrierPointer->GetPlayerShotPower() * 7.5f);
                break;
            case Player::SpecialAbility::PowerShot:
                return DoesPlayerHaveShot(BallCarrierPointer, BallCarrierPointer->GetPlayerShotPower() * 8.5f);
                break;
            default:

                break;
            }


        }
       return DoesPlayerHaveShot(BallCarrierPointer, (BallCarrierPointer->GetPlayerShotPower() * 6.5f));
    }
    return false;
}

bool MatchSession::DoesTeammateHaveShot()
{


    AISavedPlayerPointer = nullptr;
    if (TeamWithBall == 1)
    {
        for (auto i = 0; i < Team1PlayersContainer.size(); i++)
        {
            if (Team1PlayersContainer.at(i) == BallCarrierPointer)
            {
                continue;
            }
            if (CheckIfPlayerIsMarked(Team1PlayersContainer.at(i), sf::Vector2f(50,50)))
            {
                continue;
            }
            if (DoesPlayerHaveShot(Team1PlayersContainer.at(i), Team1PlayersContainer.at(i)->GetPlayerShotPower() * 7.0f))
            {
                if (ClearPassToTeammate(Team1PlayersContainer.at(i)))
                {
                    AISavedPlayerPointer = Team1PlayersContainer.at(i);
                    return true;
                }

            }
        }
    }
    else if(TeamWithBall == 2)
    {
         for (auto i = 0; i < Team2PlayersContainer.size(); i++)
         {
             if (Team2PlayersContainer.at(i) == BallCarrierPointer)
             {
                 continue;
             }
             if (CheckIfPlayerIsMarked(Team2PlayersContainer.at(i), sf::Vector2f(50,50)))
             {
                 continue;
             }
              if (DoesPlayerHaveShot(Team2PlayersContainer.at(i), Team2PlayersContainer.at(i)->GetPlayerShotPower() * 7.0f))
              {
                  AISavedPlayerPointer = Team2PlayersContainer.at(i);
                  return true;
              }
         }
    }

    return false;
}

void MatchSession::PassToTeamMateInScoringPosition()
{
    if (BallCarrierPointer && AISavedPlayerPointer)
    {
        if (BallCarrierPointer != ControlledPlayer)
        {
            PassBallToPlayer(AISavedPlayerPointer);

        }
    }
}

bool MatchSession::isBallCarrierMarked()
{
    if (BallCarrierPointer)
    {
        if (CheckIfPlayerIsMarked(BallCarrierPointer, sf::Vector2f(200,200)))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    throw std::exception("Bug: is Ball Carrier Marked function called when ball carrier pointer is nullptr");


    return false;
}

bool MatchSession::isBallCarrierAGoalie()
{
    if (BallCarrierPointer)
    {
        if (BallCarrierPointer->GetPlayerPosition() == Player::PlayerPosition::Goalie || BallCarrierPointer->GetPlayerPosition() == Player::PlayerPosition::ForwardGoalie)

        {
            return true;
        }
    }
    return false;
}

bool MatchSession::DoesBallCarrierHaveAnOpenPass()
{

    sf::RectangleShape ShotRayCast(sf::Vector2f(1, 25));
    std::vector<std::shared_ptr<Player>> AlliedTeamVector;
    std::vector<std::shared_ptr<Player>> OpposingTeamVector;
    bool FoundPass = true;
    AISavedPlayerPointer = nullptr;

    float Distance, XDistance, YDistance;
    float Rotation;

    if (BallCarrierPointer)
    {
        ShotRayCast.setPosition(BallCarrierPointer->GetPosition());

        if (TeamWithBall == 1)
        {
            AlliedTeamVector = Team1PlayersContainer;
            OpposingTeamVector = Team2PlayersContainer;
        }
        else
        {
            AlliedTeamVector = Team2PlayersContainer;
            OpposingTeamVector = Team1PlayersContainer;
        }

        for (auto i = 0; i < AlliedTeamVector.size(); i++)
        {
            if (CheckIfPlayerIsMarked(AlliedTeamVector.at(i), sf::Vector2f(50, 50)))
            {
                continue;
            }

            FoundPass = true;
            if (AlliedTeamVector.at(i) == BallCarrierPointer)
            {
                FoundPass = false;
                continue;
            }
            //shoots toward east gaol
            XDistance = (AlliedTeamVector.at(i)->GetPosition().x - BallCarrierPointer->GetPosition().x);
            YDistance = (AlliedTeamVector.at(i)->GetPosition().y + Scale(75) - BallCarrierPointer->GetPosition().y);

            Distance = sqrt(pow(XDistance, 2) + pow(YDistance, 2));
            if (Distance > 1000)
            {
                FoundPass = false;
                continue;
            }
            float RayCastWidth = (90 / (500 / Distance));
            ShotRayCast.setSize(sf::Vector2f(Distance, RayCastWidth));
            ShotRayCast.setOrigin(sf::Vector2f(0, RayCastWidth / 2));
            ShotRayCast.setPosition(BallCarrierPointer->GetPosition());

            if (!(Distance == 0))
            {
                Rotation = (acos(XDistance / Distance) * 180.0 / 3.14159);
            }
            else
            {
                return false;
            }
            if (BallCarrierPointer->GetPosition().y > AlliedTeamVector.at(i)->GetPosition().y + Scale(75)) // below target
            {
                Rotation = (360 - Rotation);
            }

            ShotRayCast.setRotation(Rotation);


            for (auto j = 0; j < OpposingTeamVector.size(); j++)
            {
             
                if (ShotRayCast.getGlobalBounds().intersects(OpposingTeamVector.at(j)->GetGlobalBounds()))
                {

                    FoundPass = false;
                    break;
                }
            }

            if (FoundPass)
            {

                if (AISavedPlayerPointer != nullptr)
                {
                    if (CheckForBetterPlayerPriroity(AlliedTeamVector.at(i), AISavedPlayerPointer))
                    {
                        AISavedPlayerPointer = AlliedTeamVector.at(i);

                    }
                }
                else
                {
                    AISavedPlayerPointer = AlliedTeamVector.at(i);
                }


            }

        }

    }
    if (AISavedPlayerPointer)
    {
        ShotRayCastBuffer.setOutlineColor(sf::Color::Magenta);
        ShotRayCastBuffer = ShotRayCast;
        return true;

    }
        return false;
}

void MatchSession::AiPassBall()
{
    if (BallCarrierPointer && AISavedPlayerPointer)
    {
        if (BallCarrierPointer != ControlledPlayer)
        {
            PassBallToPlayer(AISavedPlayerPointer);
        }
    }
}

void MatchSession::SendPlayerToAttackingPosition(int i_PlayerID, int i_TeamNumber)
{
    std::shared_ptr<Player> i_Player = GetPlayerByID(i_PlayerID);

    if (i_Player)
    {
        if (i_Player->GetPlayerPosition() == Player::PlayerPosition::Goalie)
        {
            i_Player->SetTargetLocation(GetAreaInFrontOfDefensiveGoal(i_TeamNumber).getPosition(), Scale(sf::Vector2f(5, 5)));
            return;
        }
        else
        {
            if (GetDefensiveThird(i_TeamNumber).getGlobalBounds().contains(BallSprite.getPosition()))
            {
                GetOpenInMidField(i_Player);
                std::cout << "MID"  << " " << i_TeamNumber << std::endl;
                return;
            }
            else
            {
                i_Player->GetInOffenseivePosition(MiddleThird);
            }

        }

    }
}

float MatchSession::GetDistanceToBall(int PlayerID)
{
    std::shared_ptr<Player> i_Player = GetPlayerByID(PlayerID);
    if (i_Player)
    {
        float XDistance = (i_Player->GetPosition().x - BallSprite.getPosition().x);
        float YDistance = (i_Player->GetPosition().y - BallSprite.getPosition().y);
        float Distance = sqrt(pow(XDistance, 2) + pow(YDistance, 2));

        return Distance;
    }

    return 9999.0;
}

void MatchSession::AITackle(int i_PlayerID)
{
    std::shared_ptr<Player> i_Player = GetPlayerByID(i_PlayerID);
    if (i_Player)
    {
        if (i_Player->GetIsStunned() == true || i_Player == ControlledPlayer)
        {
            return;
        }
        float Distance, XDistance, YDistance;
        float Rotation;

        if (BallCarrierPointer)
        {
            XDistance = (BallCarrierPointer->GetPosition().x - i_Player->GetPosition().x);
            YDistance = (BallCarrierPointer->GetPosition().y - i_Player->GetPosition().y);
            Distance = sqrt(pow(XDistance, 2) + pow(YDistance, 2));

            if (!(Distance == 0))
            {
                Rotation = (acos(XDistance / Distance) * 180.0 / 3.14159);
            }
            else
            {
                return;
            }
            if (i_Player->GetPosition().y > BallCarrierPointer->GetPosition().y) // below target
            {
                Rotation = (360 - Rotation);
            }

            i_Player->SetRotation(Rotation);
            Tackle(i_Player);
        }
    }
}

void MatchSession::AdvanceBall()
{
    if (BallCarrierPointer)
    {
        BallCarrierPointer->AdvanceBall(MiddleThird, GetAttackingThird(BallCarrierPointer->GetTeamNumber()));
    }
}

void MatchSession::SetMatchSessionScale(const sf::Vector2f &i_Scale)
{
    MatchSessionScale = i_Scale;

    BallSprite.setScale(i_Scale);
    BallSprite.setOrigin(10  * i_Scale.x, 10 * i_Scale.y);

    for (auto i = 0; i < Team1PlayersContainer.size(); i++)
    {
        Team1PlayersContainer.at(i)->SetScale(i_Scale);
    }
    for (auto i = 0; i < Team2PlayersContainer.size(); i++)
    {
        Team2PlayersContainer.at(i)->SetScale(i_Scale);
    }

    MapSprite.setScale(i_Scale);
    MapSprite.setPosition(Scale(sf::Vector2f(960, 420)));

    EndOfMatchPopUpMenu.SetMenuPosition(Scale(MapSprite.getPosition()));

    MiddleThird.setSize(sf::Vector2f(MapSprite.getGlobalBounds().width / 3, MapSprite.getGlobalBounds().height));
    MiddleThird.setOrigin(MiddleThird.getSize().x / 2, MiddleThird.getSize().y / 2);
    MiddleThird.setPosition(MapSprite.getPosition());

    NorthWall.setScale(i_Scale);
    SouthWall.setScale(i_Scale);
    NorthWall.setPosition(MapSprite.getPosition().x - (MapSprite.getOrigin().x - 22) * i_Scale.x, MapSprite.getPosition().y - (MapSprite.getOrigin().y * i_Scale.y));
    SouthWall.setPosition(MapSprite.getPosition().x - (MapSprite.getOrigin().x - 22) * i_Scale.x, MapSprite.getPosition().y + (MapSprite.getOrigin().y - 22) * i_Scale.y);

    WestWallNorthHalf.setScale(i_Scale);
    EastWallNorthHalf.setScale(i_Scale);
    WestWallNorthHalf.setPosition(MapSprite.getPosition().x - (MapSprite.getOrigin().x * i_Scale.x), MapSprite.getPosition().y - (MapSprite.getOrigin().y - 22) * i_Scale.y);
    EastWallNorthHalf.setPosition(MapSprite.getPosition().x + (MapSprite.getOrigin().x - 22) * i_Scale.x, MapSprite.getPosition().y - (MapSprite.getOrigin().y - 22) * i_Scale.y);

    LeftGoal.setScale(i_Scale);
    RightGoal.setScale(i_Scale);
    LeftGoal.setPosition(WestWallNorthHalf.getPosition().x, WestWallNorthHalf.getPosition().y + (250 * i_Scale.y));
    RightGoal.setPosition(EastWallNorthHalf.getPosition().x, EastWallNorthHalf.getPosition().y + (250 * i_Scale.y));

    WestWallSouthHalf.setScale(i_Scale);
    EastWallSouthHalf.setScale(i_Scale);
    WestWallSouthHalf.setPosition(LeftGoal.getPosition().x, LeftGoal.getPosition().y + (200 * i_Scale.y));
    EastWallSouthHalf.setPosition(RightGoal.getPosition().x, RightGoal.getPosition().y + (200 * i_Scale.y));

    MapOutline.setSize(sf::Vector2f(MapSprite.getGlobalBounds().width, MapSprite.getGlobalBounds().height));
    MapOutline.setOrigin(Scale(MapSprite.getOrigin()));
    MapOutline.setPosition(MapSprite.getPosition());


    BallSprite.setOrigin(Scale(11, 10));
    BallSprite.setPosition(MapSprite.getPosition());

    ShotVelocityText[0].setOrigin(Scale(sf::Vector2f(10, 2)));
    ShotVelocityText[0].setCharacterSize(24 * i_Scale.x);

    for (auto i = 0; i < 5; i++)
    {
        EndOfMatchText[i].setCharacterSize(32);
    }
    EndOfMatchText[0].setPosition(sf::Vector2f(MapSprite.getPosition().x - (45 * i_Scale.x), MapSprite.getPosition().y - (240 * i_Scale.y)));
    EndOfMatchText[1].setPosition(sf::Vector2f(MapSprite.getPosition().x - (190 * i_Scale.x), MapSprite.getPosition().y - (180 * i_Scale.y)));
    EndOfMatchText[2].setPosition(sf::Vector2f(MapSprite.getPosition().x + (70 * i_Scale.x), MapSprite.getPosition().y - (180 * i_Scale.y)));
    EndOfMatchText[3].setPosition(sf::Vector2f(MapSprite.getPosition().x - (19 * i_Scale.x), MapSprite.getPosition().y - (180 * i_Scale.y)));
    EndOfMatchText[4].setPosition(sf::Vector2f(MapSprite.getPosition().x + (17 * i_Scale.x), MapSprite.getPosition().y - (180 * i_Scale.y)));



    for (auto i = 0; i < 3; i++)
    {
        MatchInfoText[i].setCharacterSize(36 * i_Scale.x);
    }
    MatchInfoText[0].setPosition(Scale(sf::Vector2f(120, 10)));
    MatchInfoText[1].setPosition(Scale(sf::Vector2f(1600, 10)));
    MatchInfoText[2].setPosition(Scale(sf::Vector2f(920, 10)));

    

}

bool MatchSession::ClearPassToTeammate(std::shared_ptr<Player> i_Teammate)
{
    sf::RectangleShape ShotRayCast(sf::Vector2f(1, 25));
    std::vector<std::shared_ptr<Player>> OpposingTeamVector;

    float Distance, XDistance, YDistance;
    float Rotation;

    if (BallCarrierPointer)
    {
        ShotRayCast.setPosition(BallCarrierPointer->GetPosition());

        if (TeamWithBall == 1)
        {
            OpposingTeamVector = Team2PlayersContainer;
        }
        else
        {
            OpposingTeamVector = Team1PlayersContainer;
        }


        if (i_Teammate == BallCarrierPointer || i_Teammate->GetTeamNumber() != BallCarrierPointer->GetTeamNumber())
        {
            return false;;
        }


        XDistance = (i_Teammate->GetPosition().x - BallCarrierPointer->GetPosition().x);
        YDistance = (i_Teammate->GetPosition().y + Scale(75) - BallCarrierPointer->GetPosition().y);
        Distance = sqrt(pow(XDistance, 2) + pow(YDistance, 2));

        if (Distance > 1000)
        {
            return false;;
        }
        float RayCastWidth = (90 / (500 / Distance));
        ShotRayCast.setSize(sf::Vector2f(Distance, RayCastWidth));
        ShotRayCast.setOrigin(sf::Vector2f(0, RayCastWidth / 2));
        ShotRayCast.setPosition(BallCarrierPointer->GetPosition());

        if (!(Distance == 0))
        {
            Rotation = (acos(XDistance / Distance) * 180.0 / 3.14159);
        }
        else
        {
            return false;
        }
        if (BallCarrierPointer->GetPosition().y > i_Teammate->GetPosition().y + Scale(75)) // below target
        {
            Rotation = (360 - Rotation);
        }

        ShotRayCast.setRotation(Rotation);


        for (auto j = 0; j < OpposingTeamVector.size(); j++)
        {
            if (ShotRayCast.getGlobalBounds().intersects(OpposingTeamVector.at(j)->GetGlobalBounds()))
            {
                return false;
            }
        }


    }
    else
    {
        return false;
    }
    return true;
}

void MatchSession::HandleLeftClickInput()
{
    if (EndOfMatchPopUpMenu.GetDoDrawMenu())
    {
        EndOfMatchPopUpMenu.SavedMousePosition = MouseLocation;
        EndOfMatchPopUpMenu.LeftClickEvent();
    }
    if (MatchSessionMenuPopUp.GetDoDrawMenu())
    {
        MatchSessionMenuPopUp.SavedMousePosition = MouseLocation;
        MatchSessionMenuPopUp.LeftClickEvent();
    }
}

void MatchSession::ToggleMatchSessionMenu()
{
    MatchSessionMenuPopUp.SetDoDrawMenu(!MatchSessionMenuPopUp.GetDoDrawMenu());
    if (MatchSessionMenuPopUp.GetDoDrawMenu())
    {
        StopPlay = true;
    }
    else
    {
        StopPlay = false;
    }
}

void MatchSession::CheckIfPlayerShouldUseAbility(std::shared_ptr<Player> i_Player)
{
    if (i_Player)
    {
        if (!i_Player->CheckAbilityCooldown())
        {
            return;
        }
        switch (i_Player->GetPlayerSpeicalAbility())
        {
        case Player::SpecialAbility::Blink:
            if (i_Player == BallCarrierPointer)
            {
                if (CheckIfPlayerIsMarked(i_Player, sf::Vector2f(100, 100)))
                {
                    BallCarrierPointer->UseAbility();
                    Blink(i_Player);
                }

            }
            break;
        case Player::SpecialAbility::BlazingSpeed:
            if (i_Player == BallCarrierPointer)
            {
                if (!CheckIfPlayerIsMarked(i_Player, sf::Vector2f(200, 200)))
                {
                    BallCarrierPointer->UseAbility();
                    Blaze(i_Player);
                }

            }
            break;
        case Player::SpecialAbility::FrostNova:
            if (BallCarrierPointer)
            {
                //ball carrier must not be marked, stunned, or slowed
                if (BallCarrierPointer->GetTeamNumber() != i_Player->GetTeamNumber() && BallCarrierPointer->GetIsStunned() == false && BallCarrierPointer->GetIsSlowed() == false && !CheckIfPlayerIsMarked(BallCarrierPointer, sf::Vector2f(150,150)))
                {
                    i_Player->UseAbility();
                    CreateIcePatch(BallCarrierPointer->GetPosition());
                }
            }
            break;
        case Player::SpecialAbility::VoidChain:
            if (BallCarrierPointer)
            {
                if (BallCarrierPointer->GetTeamNumber() != i_Player->GetTeamNumber() && GetDistanceToBall(i_Player->GetPlayerID()) < Scale(300.0f))
                {
                    i_Player->UseAbility();
                    VoidChain(i_Player, BallCarrierPointer->GetPosition());
                }
            }
            break;
        }
    }
}

void MatchSession::GetInDefensivePosition(int i_PlayerID, int i_TeamNumber)
{
    std::shared_ptr<Player> i_Player = GetPlayerByID(i_PlayerID);
    sf::RectangleShape PlayerDefensiveRectangle;
    if(i_Player)
    {
        if (i_Player->GetPlayerPosition() == Player::PlayerPosition::Goalie || i_Player->GetPlayerPosition() == Player::PlayerPosition::ForwardGoalie)
        {   
            // if ball is in defensive thrid
            if (GetDefensiveThird(i_TeamNumber).getGlobalBounds().intersects(BallSprite.getGlobalBounds()))
            {
                // get in between ball and goal
                SetGoaliePosition(i_Player);
            }
            else
            {
                i_Player->SetTargetLocation(GetAreaInFrontOfDefensiveGoal(i_TeamNumber).getPosition(), Scale(sf::Vector2f(5, 5)));
            }
            return;
        }
        else
        {
            PlayerDefensiveRectangle = i_Player->GetInDefensivePosition(GetDefensiveThird(i_TeamNumber), MiddleThird);
            CheckForPlayerInZone(i_Player, PlayerDefensiveRectangle);
        }

    }
}

bool MatchSession::CheckIsBallCarrier(int i_PlayerID)
{
    if (BallCarrierPointer)
    {  
        if (BallCarrierPointer->GetPlayerID() == i_PlayerID)
        {
            return true;
        }
    }
    return false;
}

void MatchSession::AiShootBall()
{
    float Distance, XDistance, YDistance;
    float Rotation;
    sf::Vector2f ShotTarget(0, 0);
    ShotTarget = AIShotTargetLocation;

    if (BallCarrierPointer)
    {
        if (ControlledPlayer)
        {
            if (ControlledPlayer == BallCarrierPointer)
            {
                return;
            }
        }
        XDistance = (AIShotTargetLocation.x - BallCarrierPointer->GetPosition().x);
        YDistance = (AIShotTargetLocation.y - BallCarrierPointer->GetPosition().y);
        Distance = sqrt(pow(XDistance, 2) + pow(YDistance, 2));


        if (BallCarrierPointer->CheckAbilityCooldown())
        {
            switch (BallCarrierPointer->GetPlayerSpeicalAbility())
            {
            case::Player::SpecialAbility::PowerShot:
                BallCarrierPointer->UseAbility();
                HandleEvent_CallBack(EventType::BallShot, 0);
                PowerShot(ShotTarget);

                break;
            case::Player::SpecialAbility::CurveShot:
                //dont shoot curves super close
                if (Distance < 200.0f)
                {
                    break;
                }
                BallCarrierPointer->UseAbility();
                HandleEvent_CallBack(EventType::BallShot, 0);
                ShotTarget.y = MiddleThird.getPosition().y;
         
                CurveShot(ShotTarget);
                return;
                break;
            default:
                break;
            }
        }



        if (!(Distance == 0))
        {
            Rotation = (acos(XDistance / Distance) * 180.0 / 3.14159);
        }
        else
        {
            return;
        }
        if (BallCarrierPointer->GetPosition().y > AIShotTargetLocation.y) // below target
        {
            Rotation = (360 - Rotation);
        }

        BallCarrierPointer->SetRotation(Rotation);
        ShootBall(ShotTarget);
    }
}

void MatchSession::CurveShot(const sf::Vector2f &i_TargetLocation)
{
    sf::Vector2f TargetLocation = i_TargetLocation;
    if (!BallCarrierPointer)
    {
        return;
    }
    BallVelocity = BallCarrierPointer->GetPlayerShotPower() * 12;

   
    isBallBeingShot = true;
    //calculate moveVector
    float xDistance = TargetLocation.x - BallCarrierPointer->GetPosition().x;
    float yDistance = TargetLocation.y - BallCarrierPointer->GetPosition().y;
    float TotalDistance = (sqrt(xDistance * xDistance + yDistance * yDistance));
    float Rotation = 0.0f;
    if (!(TotalDistance == 0))
    {
        Rotation = (acos(xDistance / TotalDistance) * 180.0 / 3.14159);
    }
    if (BallCarrierPointer->GetPosition().y > TargetLocation.y) // below target
    {
        Rotation = (360 - Rotation);
    }

    //facing right
    if (xDistance > 0)
    {
        if (yDistance > 0)
        {
            CurveIntensity = 2.8f * (Scale(500) / TotalDistance);
        }
        else
        {
            CurveIntensity = -2.8f * (Scale(500) / TotalDistance);

        }
    }
    else
    {
        if (yDistance < 0)
        {
            CurveIntensity = 2.8f * (Scale(500) / TotalDistance);
        }
        else
        {
            CurveIntensity = -2.8f * (Scale(500) / TotalDistance);

        }
    }



    //estimate travel time with a BallVelocityDropOffPerSecond% velocity reduction per second
    float EstimatedTravelTimeInSeconds = (TotalDistance / (BallVelocity));

    // account for -20.0f Rotation persecond, 
    BallMoveDirection = Rotation + (EstimatedTravelTimeInSeconds * (16 * CurveIntensity));

    BallSprite.move(Scale(sf::Vector2f(cos(BallMoveDirection * 3.1415 / 180.0) * 50, sin(BallMoveDirection * 3.1415 / 180.0) * 50)));
}

void MatchSession::PowerShot(const sf::Vector2f &i_TargetLocation)
{
    if (!BallCarrierPointer)
    {
        return;
    }

    BallVelocity = BallCarrierPointer->GetPlayerShotPower() * 14;

    isBallBeingShot = true;
    //calculate moveVector
    float xDistance = i_TargetLocation.x - BallCarrierPointer->GetPosition().x;
    float yDistance = i_TargetLocation.y - BallCarrierPointer->GetPosition().y;
    float TotalDistance = (sqrt(xDistance * xDistance + yDistance * yDistance));
    float Rotation = 0.0f;
    if (!(TotalDistance == 0))
    {
        Rotation = (acos(xDistance / TotalDistance) * 180.0 / 3.14159);
    }
    if (BallCarrierPointer->GetPosition().y > i_TargetLocation.y) // below target
    {
        Rotation = (360 - Rotation);
    }

    BallMoveDirection = Rotation;

    BallSprite.move(Scale(sf::Vector2f(cos(BallMoveDirection * 3.1415 / 180.0) * 50, sin(BallMoveDirection * 3.1415 / 180.0) * 50)));
}


void MatchSession::EraseLastAreaOfEffect()
{
    if (AreaOfEffectContainer.size() > 0)
    {
        AreaOfEffectContainer.erase(AreaOfEffectContainer.begin());
    }
}

void MatchSession::CheckForAreaOfEffectCollsion()
{
    for (auto i = 0; i < Team1PlayersContainer.size(); i++)
    {
        for (auto j = 0; j < AreaOfEffectContainer.size(); j++)
        {
            if (AreaOfEffectContainer.at(j).AreaEffected.getGlobalBounds().contains(Team1PlayersContainer.at(i)->GetPosition()))
            {
                Team1PlayersContainer.at(i)->AddBuff(AreaOfEffectContainer.at(j).AppliedBuff);

            }
        }
    }

    for (auto i = 0; i < Team2PlayersContainer.size(); i++)
    {
        for (auto j = 0; j < AreaOfEffectContainer.size(); j++)
        {
            if (AreaOfEffectContainer.at(j).AreaEffected.getGlobalBounds().contains(Team2PlayersContainer.at(i)->GetPosition()))
            {
                Team2PlayersContainer.at(i)->AddBuff(AreaOfEffectContainer.at(j).AppliedBuff);

            }
        }
    }
}

void MatchSession::CheckForLooseBallPickup()
{

    if (isBallInvurnerable)
    {
        BallCarrierPointer = nullptr;
        return;
    }

    //drop ball when stunned
    if (BallCarrierPointer)
    {
        if (BallCarrierPointer->GetIsStunned())
        {
            if (BallCarrierPointer->GetGlobalBounds().intersects(BallSprite.getGlobalBounds()))
            {
                BallCarrierDropBall();
            }
        }

    }
    

    for (auto i = 0; i < Team2PlayersContainer.size(); i++) 
    {
        if (Team2PlayersContainer.at(i)->GetIsStunned())
        {
            continue;
        }
        if (BallSprite.getGlobalBounds().intersects(Team2PlayersContainer.at(i)->GetGlobalBounds()))
        {
            if (BallCarrierPointer != nullptr)
            {
                if (BallCarrierPointer->GetTeamNumber() != Team2PlayersContainer.at(i)->GetTeamNumber())
                {

                    if ((PlayerStrengthBattle(Team2PlayersContainer.at(i), BallCarrierPointer) == 2))
                    {
                        //ball carrier won
                        return;

                    }
                }
            }

            ResetIsBallBeingPassedToYou();

            (BallVelocity -= (Team2PlayersContainer.at(i)->GetPlayerCatch() * 10));
            if (BallCarrierPointer != Team2PlayersContainer.at(i) && isBallBeingShot == true)
            {
                CreateFloatingText(2.0f, std::to_string((int)(-1.0f * Team2PlayersContainer.at(i)->GetPlayerCatch())), Team2PlayersContainer.at(i)->GetPosition(), sf::Color::Red, 24);
            }
            if (BallVelocity <= 0)
            {

                BallCarrierPointer = Team2PlayersContainer.at(i);
                BallSprite.setPosition(Team2PlayersContainer.at(i)->GetPosition());
                isBallBeingShot = false;
                StopBall();
                return;
            }
            else
            {
                Team2PlayersContainer.at(i)->AddBuff(0.25f, 0.0f, true);
                BallSprite.move(Scale(sf::Vector2f(cos(BallMoveDirection * 3.1415 / 180.0) * 60, sin(BallMoveDirection * 3.1415 / 180.0) * 60)));
            }
        }
    }
    for (auto i = 0; i < Team1PlayersContainer.size(); i++)
    {
        if (Team1PlayersContainer.at(i)->GetIsStunned())
        {
            continue;
        }
        if (BallSprite.getGlobalBounds().intersects(Team1PlayersContainer.at(i)->GetGlobalBounds()))
        {
            if (BallCarrierPointer != nullptr)
            {
                if (BallCarrierPointer->GetTeamNumber() != Team1PlayersContainer.at(i)->GetTeamNumber())
                {
                   
                    if ((PlayerStrengthBattle(Team1PlayersContainer.at(i), BallCarrierPointer) == 2))
                    {
                        //ball carrier won
                        return;

                    }
                }
            }


            ResetIsBallBeingPassedToYou();

            if (ControlledPlayer != Team1PlayersContainer.at(i))
            {
                SwitchPlayer(i);
            }

            (BallVelocity -= Team1PlayersContainer.at(i)->GetPlayerCatch() * 10);
            if (BallCarrierPointer != Team1PlayersContainer.at(i) && isBallBeingShot == true)
            {
                CreateFloatingText(2.0f, std::to_string((int)(-1.0f * Team1PlayersContainer.at(i)->GetPlayerCatch())), Team1PlayersContainer.at(i)->GetPosition(), sf::Color::Black, 24);
            }
            if (BallVelocity <= 0)
            {
                BallCarrierPointer = Team1PlayersContainer.at(i);
                BallSprite.setPosition(Team1PlayersContainer.at(i)->GetPosition());
                isBallBeingShot = false;
                StopBall();
            }
            else
            {
                Team1PlayersContainer.at(i)->AddBuff(0.25f, 0.0f, true);
                BallSprite.move(Scale(sf::Vector2f(cos(BallMoveDirection * 3.1415 / 180.0) * 60, sin(BallMoveDirection * 3.1415 / 180.0) * 60)));
            }
            return;
        }
    }
    BallCarrierPointer = nullptr;
}

void MatchSession::StopBall()
{
    BallVelocity = 0.0f;
    CurveIntensity = 0.0f;

}

void MatchSession::MoveBall(float DeltaTimeAsSeconds)
{
    if (BallVelocity > 1.0f)
    {
        if (CurveIntensity != 0.0f)
        {
            BallMoveDirection -= ((28.0f * CurveIntensity)* DeltaTimeAsSeconds);
            CurveIntensity = (CurveIntensity * (1.0f - (0.088f * DeltaTimeAsSeconds)));
        }
        BallSprite.rotate(3000 * (BallVelocity / 1000) * DeltaTimeAsSeconds); 
        BallSprite.move(Scale(sf::Vector2f(cos(BallMoveDirection * 3.1415 / 180.0) * DeltaTimeAsSeconds * BallVelocity, sin(BallMoveDirection * 3.1415 / 180.0) * DeltaTimeAsSeconds * BallVelocity)));
        BallVelocity = (BallVelocity * (1.0f - (BallVelocityDropOffPerSecond * DeltaTimeAsSeconds)));
    }
    else
    {
        StopBall();
    }


}

void MatchSession::CheckForGoal()
{
    if (LeftGoal.getGlobalBounds().contains(BallSprite.getPosition()))
    {
        GoalScored(2);
    }
    if (RightGoal.getGlobalBounds().contains(BallSprite.getPosition()))
    {
        GoalScored(1);

    }

}

void MatchSession::CheckForBallWallCollision()
{
    if (BallVelocity < 5)
    {
        return;
    }
    float BallVelocityReduction = 0.80f;
    if (NorthWall.getGlobalBounds().intersects(BallSprite.getGlobalBounds()) || SouthWall.getGlobalBounds().intersects(BallSprite.getGlobalBounds()))
    {
        BallMoveDirection = -BallMoveDirection;
        BallVelocity = (BallVelocity * BallVelocityReduction);
        CurveIntensity = 0.0f;
        HandleEvent_CallBack(EventType::BallHitWall, 0);
        return;
    }

    if (EastWallNorthHalf.getGlobalBounds().intersects(BallSprite.getGlobalBounds()) || EastWallSouthHalf.getGlobalBounds().intersects(BallSprite.getGlobalBounds()) || WestWallSouthHalf.getGlobalBounds().intersects(BallSprite.getGlobalBounds()) || WestWallNorthHalf.getGlobalBounds().intersects(BallSprite.getGlobalBounds()))
    {
        BallMoveDirection = -BallMoveDirection;
        BallMoveDirection -= 180;
        BallVelocity = (BallVelocity * BallVelocityReduction);
        CurveIntensity = 0.0f;
        HandleEvent_CallBack(EventType::BallHitWall, 0);

        return;
    }



    // somehow ball got out of stadium, put it back in
    if (!MapSprite.getGlobalBounds().contains(BallSprite.getPosition()))
    {
        BallVelocity = 0;
        BallSprite.setPosition(MapSprite.getPosition());
    }
}

bool MatchSession::CheckForWallCollision(const sf::FloatRect &i_Entity)
{
    if (NorthWall.getGlobalBounds().intersects(i_Entity) || SouthWall.getGlobalBounds().intersects(i_Entity))
    {
        return true;
    }

    if (EastWallNorthHalf.getGlobalBounds().intersects(i_Entity) || EastWallSouthHalf.getGlobalBounds().intersects(i_Entity) || WestWallSouthHalf.getGlobalBounds().intersects(i_Entity) || WestWallNorthHalf.getGlobalBounds().intersects(i_Entity))

    {
        return true;;
    }
    if (LeftGoal.getGlobalBounds().intersects(i_Entity) || RightGoal.getGlobalBounds().intersects(i_Entity))
    {
        return true;
    }
    return false;
}

bool MatchSession::CheckForWallCollision(std::shared_ptr<Player> i_Player)
{
    if (NorthWall.getGlobalBounds().intersects(i_Player->GetGlobalBounds()))
    {
        i_Player->SetSpritePosition(sf::Vector2f(i_Player->GetPosition().x, NorthWall.getPosition().y + NorthWall.getGlobalBounds().height + (i_Player->GetGlobalBounds().height / 2)));
        return true;
    }
    if (SouthWall.getGlobalBounds().intersects(i_Player->GetGlobalBounds()))
    {
        i_Player->SetSpritePosition(sf::Vector2f(i_Player->GetPosition().x, SouthWall.getPosition().y - (i_Player->GetGlobalBounds().height / 2)));
        return true;
    }
    if (WestWallNorthHalf.getGlobalBounds().intersects(i_Player->GetGlobalBounds()) || WestWallSouthHalf.getGlobalBounds().intersects(i_Player->GetGlobalBounds()) || LeftGoal.getGlobalBounds().intersects(i_Player->GetGlobalBounds()))
    {
        i_Player->SetSpritePosition(sf::Vector2f((i_Player->GetGlobalBounds().width / 2) + WestWallNorthHalf.getPosition().x + WestWallNorthHalf.getGlobalBounds().width, i_Player->GetPosition().y));

        return true;;
    }
    if (EastWallNorthHalf.getGlobalBounds().intersects(i_Player->GetGlobalBounds()) || EastWallSouthHalf.getGlobalBounds().intersects(i_Player->GetGlobalBounds()) || RightGoal.getGlobalBounds().intersects(i_Player->GetGlobalBounds()))
    {
        i_Player->SetSpritePosition(sf::Vector2f(EastWallNorthHalf.getPosition().x - (i_Player->GetGlobalBounds().width / 2), i_Player->GetPosition().y));

        return true;
    }
    return false;
}

bool MatchSession::CheckIfOffMap(const sf::FloatRect &i_Entity)
{
    if (MapSprite.getGlobalBounds().intersects(i_Entity))
    {
        return false;
    }
    return true;
}

sf::RectangleShape MatchSession::GetDefensiveThird(int i_TeamNumber)
{
    sf::RectangleShape DefensiveThird = MiddleThird;
    
    if (i_TeamNumber == 1)
    {
        //defending west goal
        DefensiveThird.setPosition(MiddleThird.getPosition().x - MiddleThird.getSize().x, MiddleThird.getPosition().y);
    }
    else
    {
        //defending east goal
        DefensiveThird.setPosition(MiddleThird.getPosition().x + MiddleThird.getSize().x, MiddleThird.getPosition().y);
    }

    return DefensiveThird;
}

sf::RectangleShape MatchSession::GetAttackingThird(int i_TeamNumber)
{
    sf::RectangleShape AttackingThird(sf::Vector2f(MapSprite.getGlobalBounds().width / 3, MapSprite.getGlobalBounds().height));
    AttackingThird.setOrigin(AttackingThird.getSize().x / 2, AttackingThird.getSize().y / 2);


    if (i_TeamNumber == 1)
    {
        // on user teanm, shoots to the east
        AttackingThird.setPosition(MapSprite.getPosition().x + AttackingThird.getSize().x, MapSprite.getPosition().y);
    }
    else
    {
        // on computer teanm, shoots to the west
        AttackingThird.setPosition(MapSprite.getPosition().x - AttackingThird.getSize().x, MapSprite.getPosition().y);
    }
    return AttackingThird;
}

sf::RectangleShape MatchSession::GetAreaInFrontOfDefensiveGoal(int i_TeamNumber)
{
    sf::RectangleShape AreaInFrontofGoal(sf::Vector2f(5, 5));
    AreaInFrontofGoal.setOrigin(12.5, 12.5);
    if (i_TeamNumber == 1)
    {
        AreaInFrontofGoal.setPosition(LeftGoal.getPosition().x + Scale(50), LeftGoal.getPosition().y + Scale(100));
    }
    else
    {
        AreaInFrontofGoal.setPosition(RightGoal.getPosition().x - Scale(50), RightGoal.getPosition().y + Scale(100));
    }
    return AreaInFrontofGoal;
}

std::shared_ptr<Player> MatchSession::GetPlayerByID(int i_ID)
{
    for (auto i = 0; i < Team1PlayersContainer.size(); i++)
    {
        if (Team1PlayersContainer.at(i)->GetPlayerID() == i_ID)
        {
            return Team1PlayersContainer.at(i);
        }
    }

    for (auto i = 0; i < Team2PlayersContainer.size(); i++)
    {
        if (Team2PlayersContainer.at(i)->GetPlayerID() == i_ID)
        {
            return Team2PlayersContainer.at(i);
        }
    }

    return nullptr;
}

bool MatchSession::CheckIfPlayerIsMarked(std::shared_ptr<Player> i_Player, sf::Vector2f MarkedBoxSize)
{
    MarkedBoxSize = Scale(MarkedBoxSize);
    float CheckDistance = 0.0f;
    sf::RectangleShape MarkedRange(MarkedBoxSize);
    MarkedRange.setOrigin(MarkedBoxSize.x / 2, MarkedBoxSize.y / 2);
    MarkedRange.setPosition(i_Player->GetPosition());

    if (i_Player->GetTeamNumber() == 1)
    {
        for (auto i = 0; i < Team2PlayersContainer.size(); i++)
        {
            if (Team2PlayersContainer.at(i)->GetGlobalBounds().intersects(MarkedRange.getGlobalBounds()))
            {
                return true;
            }
        }
    }
    else
    {
        for (auto i = 0; i < Team1PlayersContainer.size(); i++)
        {
            if (Team1PlayersContainer.at(i)->GetGlobalBounds().intersects(MarkedRange.getGlobalBounds()))
            {
                return true;
            }
        }
    }

    return false;
}

void MatchSession::MarkAPlayer(std::shared_ptr<Player> i_Player)
{
    bool FoundAMark = false;
    float LongestDistance = 10000.0f;
    sf::Vector2f MarkPlayerPosition = sf::Vector2f(0, 0);

    float CheckDistance = 0.0f;
    if (i_Player->GetTeamNumber() == 1)
    {
        MarkPlayerPosition = Team2PlayersContainer.at(0)->GetPosition();
        for (auto i = 0; i < Team2PlayersContainer.size(); i++)
        {
            if (Team2PlayersContainer.at(i)->GetPlayerPosition() == Player::PlayerPosition::Goalie || Team2PlayersContainer.at(i)->GetPlayerPosition() == Player::PlayerPosition::ForwardGoalie)
            {
                continue;
            }
            CheckDistance = sqrt(pow((Team2PlayersContainer.at(i)->GetPosition().x - i_Player->GetPosition().x), 2) + pow(Team2PlayersContainer.at(i)->GetPosition().y - i_Player->GetPosition().y, 2));
            if (CheckDistance < LongestDistance && !CheckIfPlayerIsMarked(Team2PlayersContainer.at(i), sf::Vector2f(150, 150)))
            {
                LongestDistance = CheckDistance;
                MarkPlayerPosition = Team2PlayersContainer.at(i)->GetPosition();
                FoundAMark = true;
            }
        }
    }
    else
    {
        MarkPlayerPosition = Team1PlayersContainer.at(0)->GetPosition();
        for (auto i = 0; i < Team1PlayersContainer.size(); i++)
        {
            if (Team1PlayersContainer.at(i)->GetPlayerPosition() == Player::PlayerPosition::Goalie || Team1PlayersContainer.at(i)->GetPlayerPosition() == Player::PlayerPosition::ForwardGoalie)
            {
                continue;
            }
            CheckDistance = sqrt(pow((Team1PlayersContainer.at(i)->GetPosition().x - i_Player->GetPosition().x), 2) + pow(Team1PlayersContainer.at(i)->GetPosition().y - i_Player->GetPosition().y, 2));
            if (CheckDistance < LongestDistance && !CheckIfPlayerIsMarked(Team1PlayersContainer.at(i), sf::Vector2f(150, 150)))
            {
                LongestDistance = CheckDistance;
                MarkPlayerPosition = Team1PlayersContainer.at(i)->GetPosition();
                FoundAMark = true;

            }
        }
    }

    if (FoundAMark)
    {
        i_Player->MarkAPlayer(MarkPlayerPosition);

    }
    else
    {
        if (TeamWithBall == 0)
        {
            i_Player->HoldPosition();

        }
        else
        {
            i_Player->GetInDefensivePosition(GetDefensiveThird(i_Player->GetTeamNumber()), MiddleThird);
        }
    }
}

void MatchSession::GetOpenInMidField(std::shared_ptr<Player> i_Player)
{
    i_Player->GetInMidFieldPosition(MiddleThird);
}

void MatchSession::GetOpenInAttackingThird(std::shared_ptr<Player> i_Player)
{
    sf::RectangleShape AttackingThird = GetAttackingThird(i_Player->GetTeamNumber());

    if (i_Player->GetGlobalBounds().intersects(AttackingThird.getGlobalBounds()))
    {
        // player is in attacking third, check if they are marked
        if (CheckIfPlayerIsMarked(i_Player, sf::Vector2f(100,100)))
        {
           // not open
        }
        else
        {
            // open, hold position
            i_Player->HoldPosition();
        }
    }
    else
    {
        i_Player->SetTargetLocation(AttackingThird.getPosition(), AttackingThird.getSize());
    }
}
