#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "EventHandler.h"
#include "MatchSession.h"
#include "InputManager.h"
#include "EventTypes.h"
#include "ProcessManager.h"
#include "MainMenu.h"
#include "SettingsMenu.h"
#include "PreMatchMenu.h"
#include "SoundManager.h"
#include "GameEntityManager.h"
#include "FileManager.h"

class Game
{
public:
    enum GameState { inMainMenu ,PreMatch, LoadingMatch, InMatch, InSettingsMenu };

    Game();
    ~Game();

    bool Initialize();
    bool Run();

    // Main Window stuff
    sf::RenderWindow MainWindow;

    //
    void NewMatch();
    void EndMatch(int i_WhichTeamWon);
    void PreMatchStarted();
    void LoadMatch();

    int GetGameState();

    static sf::Vector2f WindowScale;

private:

    
    // Delta Time
    sf::Time DeltaTime;
    sf::Clock clock;

    // Game Entity Manager
    std::shared_ptr<GameEntityManager> m_GameEntityManager;

    //Process Manager 
    ProcessManager m_ProcessManager;

    //Event Manager
    EventHandler m_EventHandler;

    //view
    sf::View MainView;


    //Input Manager
    InputManager m_InputManager;

    // SOund Manager
    SoundManager m_SoundManager;

    // File Manager
    FileManager m_FileManager;

    // MatchSession
    MatchSession CurrentSession;

    //Main Menu
    MainMenu m_MainMenu;

    //PreMatch Menu
    PreMatchMenu m_PreMatchMenu;

    //Settings Menu
    SettingsMenu m_SettingsMenu;
    void EnterSettingsMenu();

    //loading menu sprite
    sf::Sprite LoadingMenuSprite;
    sf::Texture LoadingMenuTexture;


    //Draw 
    void DrawGame();

    //Game State
    GameState CurrentGameState;
    
    //SHutdown
    void ShutDown();

    //
    void EscapeEvent();
    void AbortMatch();
    void ResetData();


};

