#include "Game.h"

sf::Vector2f Game::WindowScale(1.0f, 1.0f);

Game::Game()
    :MainWindow(sf::VideoMode::getDesktopMode(), "Main Window", sf::Style::Close | sf::Style::Resize)
    , m_EventHandler()
    , MainView()
    , CurrentSession()
    , m_InputManager()
    , m_ProcessManager()
    , m_MainMenu()
    , m_PreMatchMenu()
    , m_SettingsMenu()
    , m_SoundManager()
    , m_FileManager()
    , m_GameEntityManager(nullptr)
{
    WindowScale = sf::Vector2f(MainWindow.getSize().x / 1920.0f, MainWindow.getSize().y / 1080.0f);
}


Game::~Game()
{

}

bool Game::Initialize()
{

 
    // Intialize classes
    CurrentSession.Initialize();
    CurrentSession.AddProcess_Callback = std::bind(&ProcessManager::AttachProcess, &m_ProcessManager, std::placeholders::_1);
    CurrentSession.HandleEvent_CallBack = std::bind(&EventHandler::HandleEvent, &m_EventHandler, std::placeholders::_1, std::placeholders::_2);


    m_EventHandler.Initialize();


    m_InputManager.Initialize();
    m_InputManager.HandleEvent_CallBack = std::bind(&EventHandler::HandleEvent, &m_EventHandler, std::placeholders::_1, std::placeholders::_2);

    //Delta Time
    DeltaTime = clock.restart();

    // default video settings
    MainWindow.setVerticalSyncEnabled(false);
    MainWindow.setFramerateLimit(60);

    //game delegates
    m_EventHandler.RegisterDelegate(std::bind(&Game::ShutDown, this), EventType::WindowClosed);
    m_EventHandler.RegisterDelegate(std::bind(&Game::PreMatchStarted, this), EventType::PreMatchStarted);
    m_EventHandler.RegisterDelegate(std::bind(&Game::NewMatch, this), EventType::MatchStarted);
    m_EventHandler.RegisterDelegate(std::bind(&Game::EndMatch, this, std::placeholders::_1), EventType::MatchEnded);
    m_EventHandler.RegisterDelegate(std::bind(&Game::AbortMatch, this), EventType::AbortMatch);
    m_EventHandler.RegisterDelegate(std::bind(&Game::EscapeEvent, this), EventType::EscapePressed);
    m_EventHandler.RegisterDelegate(std::bind(&Game::EnterSettingsMenu, this), EventType::SettingsButtonPressed);
    m_EventHandler.RegisterDelegate(std::bind(&Game::ResetData, this), EventType::ResetData);
    m_EventHandler.RegisterDelegate(std::bind(&Game::ShutDown, this), EventType::QuitGame);

    // match session delegates
    m_EventHandler.RegisterDelegate(std::bind(&MatchSession::ToggleMovePlayerUp, &CurrentSession, std::placeholders::_1), EventType::MoveUpToggled);
    m_EventHandler.RegisterDelegate(std::bind(&MatchSession::ToggleMovePlayerDown, &CurrentSession, std::placeholders::_1), EventType::MoveDownToggled);
    m_EventHandler.RegisterDelegate(std::bind(&MatchSession::ToggleMovePlayerRight, &CurrentSession, std::placeholders::_1), EventType::MoveRightToggled);
    m_EventHandler.RegisterDelegate(std::bind(&MatchSession::ToggleMovePlayerLeft, &CurrentSession, std::placeholders::_1), EventType::MoveLeftToggled);
    m_EventHandler.RegisterDelegate(std::bind(&MatchSession::HandleLeftClickInput, &CurrentSession), EventType::MatchSession_LeftClickPressed);

    m_EventHandler.RegisterDelegate(std::bind(&MatchSession::SwitchPlayer, &CurrentSession, std::placeholders::_1), EventType::SwitchedPlayer);
    m_EventHandler.RegisterDelegate(std::bind(&MatchSession::ShootBallEvent, &CurrentSession), EventType::ShootBall);
    m_EventHandler.RegisterDelegate(std::bind(&MatchSession::SpecialShootBallEvent, &CurrentSession), EventType::SpecialShootBall);
    m_EventHandler.RegisterDelegate(std::bind(&MatchSession::UseAbilityEvent, &CurrentSession), EventType::AbilityUsed);
    m_EventHandler.RegisterDelegate(std::bind(&MatchSession::SwitchToClosestPlayer, &CurrentSession), EventType::SwitchToClosestPlayer);
    m_EventHandler.RegisterDelegate(std::bind(&MatchSession::RightClickEvent, &CurrentSession), EventType::PassBall);


    // sound manager delegates
    m_EventHandler.RegisterDelegate(std::bind(&SoundManager::PlayGoalSounds, &m_SoundManager), EventType::GoalScored);
    m_EventHandler.RegisterDelegate(std::bind(&SoundManager::PlayBallShotSound, &m_SoundManager), EventType::BallShot);
    m_EventHandler.RegisterDelegate(std::bind(&SoundManager::PlayRandomHitSound, &m_SoundManager), EventType::PlayerTackled);
    m_EventHandler.RegisterDelegate(std::bind(&SoundManager::PlayPassSound, &m_SoundManager), EventType::BallPassed);
    m_EventHandler.RegisterDelegate(std::bind(&SoundManager::PlayTackleMissedSound, &m_SoundManager), EventType::TackleMissed);
    m_EventHandler.RegisterDelegate(std::bind(&SoundManager::PlayBallHitWallSound, &m_SoundManager), EventType::BallHitWall);
    m_EventHandler.RegisterDelegate(std::bind(&SoundManager::PlayCountdownSound, &m_SoundManager), EventType::CountDownStarted);
    m_EventHandler.RegisterDelegate(std::bind(&SoundManager::GameVolumeUp, &m_SoundManager), EventType::GameVolumeUp);
    m_EventHandler.RegisterDelegate(std::bind(&SoundManager::GameVolumeDown, &m_SoundManager), EventType::GameVolumeDown);
    m_EventHandler.RegisterDelegate(std::bind(&SoundManager::MusicVolumeUp, &m_SoundManager), EventType::MusicVolumeUp);
    m_EventHandler.RegisterDelegate(std::bind(&SoundManager::MusicVolumeDown, &m_SoundManager), EventType::MusicVolumeDown);
    m_EventHandler.RegisterDelegate(std::bind(&SoundManager::AbortMatchSounds, &m_SoundManager), EventType::AbortMatch);


    // Main Menu Delegates
    m_EventHandler.RegisterDelegate(std::bind(&MainMenu::LeftClickEvent, &m_MainMenu), EventType::MainMenu_LeftClickPressed);
    m_EventHandler.RegisterDelegate(std::bind(&MainMenu::RightClickEvent, &m_MainMenu), EventType::MainMenu_RightClickPressed);
    m_EventHandler.RegisterDelegate(std::bind(&MainMenu::PopUpEndOfSeason, &m_MainMenu), EventType::SeasonComplete);
    m_EventHandler.RegisterDelegate(std::bind(&MainMenu::SaveUserTeam, &m_MainMenu), EventType::UserTeamChanged);

    // Settings Menu Delegates
    m_EventHandler.RegisterDelegate(std::bind(&SettingsMenu::LeftClickEvent, &m_SettingsMenu), EventType::SettingsMenu_LeftClickPressed);
    m_EventHandler.RegisterDelegate(std::bind(&SettingsMenu::UpdateSettings, &m_SettingsMenu), EventType::SettingsMenu_LeftClickPressed);
    m_EventHandler.RegisterDelegate(std::bind(&SettingsMenu::UpdateSettings, &m_SettingsMenu), EventType::SettingsButtonPressed);
    m_EventHandler.RegisterDelegate(std::bind(&SettingsMenu::UpdateSettings, &m_SettingsMenu), EventType::BindingChanged);
    m_SettingsMenu.ChangeBinding_CallBack = std::bind(&InputManager::CaptureEventForKeyBinding, &m_InputManager, std::placeholders::_1);
    m_SettingsMenu.GetKeyBindings_CallBack = std::bind(&InputManager::GetKeyBinding_KeyCodes, &m_InputManager);
    
    // PreMatch delegate
    m_EventHandler.RegisterDelegate(std::bind(&PreMatchMenu::LeftClickEvent, &m_PreMatchMenu), EventType::PreMatchMenu_LeftClickPressed);
    m_EventHandler.RegisterDelegate(std::bind(&PreMatchMenu::RightClickEvent, &m_PreMatchMenu), EventType::PreMatchMenu_RightClickPressed);
    m_EventHandler.RegisterDelegate(std::bind(&PreMatchMenu::NextFormation, &m_PreMatchMenu), EventType::NextFormation);
    m_EventHandler.RegisterDelegate(std::bind(&PreMatchMenu::PreviousFormation, &m_PreMatchMenu), EventType::PreviousFormation);


    // game entity manager delegates
    m_GameEntityManager = std::make_shared<GameEntityManager>();
    m_GameEntityManager->Initialize(WindowScale);
    m_GameEntityManager->HandleEvent_CallBack = std::bind(&EventHandler::HandleEvent, &m_EventHandler, std::placeholders::_1, std::placeholders::_2);



    // initializations
    m_PreMatchMenu.Initialize(WindowScale);
    m_PreMatchMenu.HandleEvent_CallBack = std::bind(&EventHandler::HandleEvent, &m_EventHandler, std::placeholders::_1, std::placeholders::_2);
    m_PreMatchMenu.m_GameEntityManager = m_GameEntityManager;

    m_MainMenu.Initialize(WindowScale);
    m_MainMenu.HandleEvent_CallBack = std::bind(&EventHandler::HandleEvent, &m_EventHandler, std::placeholders::_1, std::placeholders::_2);
    m_MainMenu.m_GameEntityManager = m_GameEntityManager;

    m_SoundManager.LoadMenuSounds();

    m_SettingsMenu.Initialize(WindowScale);
    m_SettingsMenu.HandleEvent_CallBack = std::bind(&EventHandler::HandleEvent, &m_EventHandler, std::placeholders::_1, std::placeholders::_2);

    // loading menu sprite
    if (!LoadingMenuTexture.loadFromFile("Data/LoadingMenuSprite.png")) {};
    LoadingMenuSprite.setTexture(LoadingMenuTexture);



    CurrentGameState = GameState::inMainMenu;

    //scale
    CurrentSession.SetMatchSessionScale(WindowScale);
    return true;
}

bool Game::Run()
{

    //Main Game Loop
    while (MainWindow.isOpen())
    {

        DeltaTime = clock.restart();
        //if (MainWindow.hasFocus())
        m_InputManager.HandleInput(MainWindow, CurrentGameState);
        m_ProcessManager.UpdateProcesses(DeltaTime.asSeconds());
        m_SoundManager.UpdateSoundManager();

        switch (CurrentGameState)
        {
        case GameState::inMainMenu:
            m_MainMenu.UpdateMenu(m_InputManager.GetGlobalMousePosition());
            break;
        case GameState::PreMatch:
            m_PreMatchMenu.UpdateMenu(m_InputManager.GetGlobalMousePosition());
            break;
        case GameState::LoadingMatch:
            LoadMatch();
            break;
        case GameState::InMatch:
            CurrentSession.MatchSessionLoop(DeltaTime.asSeconds(), m_InputManager.GetGlobalMousePosition());
            break;
        case GameState::InSettingsMenu:
            m_SettingsMenu.UpdateMenu(m_InputManager.GetGlobalMousePosition());
            break;
        }

        DrawGame();

    }
    // game loop exited, return to main,cpp and exit program
    std::cout << "Ending Run" << std::endl;
    return 0;
}

void Game::NewMatch()
{
    if (!m_GameEntityManager->isUserTeamValid())
    {
        return;
    }
    CurrentGameState = GameState::LoadingMatch;
    m_SoundManager.SetDoPlayerMenuMusic(false);
    m_SoundManager.LoadMatchSessionSounds();

}

void Game::EndMatch(int i_WhichTeamWon)
{
    CurrentGameState = GameState::inMainMenu;
    m_SoundManager.SetDoPlayerMenuMusic(true);
    m_GameEntityManager->ProgressSeason(i_WhichTeamWon);

}

void Game::PreMatchStarted()
{
    std::shared_ptr<Team> NextOpponet = m_GameEntityManager->GetUsersNextOpposingTeam();
    if (NextOpponet)
    {
        CurrentGameState = GameState::PreMatch;
        m_PreMatchMenu.SetCurrentFormation();
    }
    else
    {
        EndMatch(2);
    }

}

void Game::LoadMatch()
{
    std::shared_ptr<Team> NextOpponet = m_GameEntityManager->GetUsersNextOpposingTeam();
    if (NextOpponet)
    {
        clock.restart(); // need to drop the load time for the sound manager
        CurrentSession.NewMatch(m_GameEntityManager->GetTeam(0), NextOpponet);
    }
    else
    {
        EndMatch(2);
    }
    CurrentGameState = InMatch;
}

int Game::GetGameState()
{
    return CurrentGameState;
}


void Game::EnterSettingsMenu()
{
    CurrentGameState = InSettingsMenu;
}

void Game::DrawGame()
{
    MainWindow.clear(sf::Color::Black);
    MainWindow.setMouseCursorVisible(true);

    switch (CurrentGameState)
    {
    case GameState::inMainMenu:
        m_MainMenu.DrawMenu(MainWindow);
        break;
    case GameState::PreMatch:
        m_MainMenu.DrawMenu(MainWindow);
        m_PreMatchMenu.DrawMenu(MainWindow);
        break;
    case GameState::LoadingMatch:
        MainWindow.draw(LoadingMenuSprite);
        break;
    case GameState::InMatch:
        CurrentSession.DrawMatchSession(MainWindow);
        m_ProcessManager.DrawAnimations(MainWindow);
        break;
    case GameState::InSettingsMenu:
        m_MainMenu.DrawMenu(MainWindow);
        m_SettingsMenu.DrawMenu(MainWindow);
        break;
    }

    MainWindow.display();
}

void Game::ShutDown()
{
    MainWindow.close();
}

void Game::EscapeEvent()
{
    switch (CurrentGameState)
    {
    case inMainMenu:
        CurrentGameState = InSettingsMenu;
        break;
    case PreMatch:
        CurrentGameState = inMainMenu;
        break;
    case InMatch:
        CurrentSession.ToggleMatchSessionMenu();
        break;
    case LoadingMatch:
        // do nothing while loading match
        break;
    case InSettingsMenu:
        CurrentGameState = inMainMenu;
        break;
    }
}

void Game::AbortMatch()
{
    CurrentGameState = GameState::inMainMenu;
    m_SoundManager.SetDoPlayerMenuMusic(true);
}

void Game::ResetData()
{
    m_GameEntityManager->ResetData();
    CurrentGameState = inMainMenu;
}
