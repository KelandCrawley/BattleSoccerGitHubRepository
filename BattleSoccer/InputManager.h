#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <map>
#include <unordered_map>

#include "EventTypes.h"
#include "FileManager.h"

class InputManager
{
public:
    enum GameState { inMainMenu, PreMatch, LoadingMatch, InMatch, InSettingsMenu };

    InputManager();
    ~InputManager();

    void Initialize();

    //handles user input, @parm &MainWindow is the state of the main game window, @param i_CurrentGameState is the current GameState the the game is in (See Enum GameState above)
    void HandleInput(sf::RenderWindow &MainWindow, int i_CurrentGameState);

    //Pushes an event to the Event Handler
    std::function<void(EventType, int)> HandleEvent_CallBack;


    sf::Vector2f GetGlobalMousePosition();

    //captures event on the stack and flags it to be saved
    void CaptureEventForKeyBinding(EventType i_Type);

    //returns the integer value of each keybinding, key codes are SFML::keyboard::keycodes
    std::vector<int> GetKeyBinding_KeyCodes();

private:

    sf::Vector2f GlobalMousePosition;

    // individual handle input functions
    void HandleMenuInput(sf::RenderWindow &MainWindow,  int i_CurrentGameState);
    void HandleMatchSessionInput(sf::RenderWindow &MainWindow);

    //maps events to SFML::keyboard::keycodes
    std::unordered_map<EventType, sf::Keyboard::Key> KeyBindingsMap;

    // returns true if an keycode is associated with an event
    bool LookForKeyBinding(sf::Event i_event);

    bool doCaptureKeyForBinding;
    EventType SavedEventType;
    
    //sets a keyboard::key to an event
    void ChangeKeyBinding(EventType i_Type, sf::Keyboard::Key i_Key);

    // returns true if a key::code is alredy in use
    bool CheckForDuplicateKeyBinding(sf::Keyboard::Key i_Key);

};

