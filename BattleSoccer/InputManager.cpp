#include "InputManager.h"



InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::Initialize()
{
    FileManager::GameSettingsDataPack SavedSettings = FileManager::LoadSettings();

    //initial key bidnding load
    KeyBindingsMap.emplace(EventType::MoveUpToggled, static_cast<sf::Keyboard::Key>(SavedSettings.MoveUp_KeyCode));
    KeyBindingsMap.emplace(EventType::MoveDownToggled, static_cast<sf::Keyboard::Key>(SavedSettings.MoveDown_KeyCode));
    KeyBindingsMap.emplace(EventType::MoveRightToggled, static_cast<sf::Keyboard::Key>(SavedSettings.MoveRight_KeyCode));
    KeyBindingsMap.emplace(EventType::MoveLeftToggled, static_cast<sf::Keyboard::Key>(SavedSettings.MoveLeft_KeyCode));
    KeyBindingsMap.emplace(EventType::SwitchToClosestPlayer, static_cast<sf::Keyboard::Key>(SavedSettings.SwitchToClosestPlayer_KeyCode));
    KeyBindingsMap.emplace(EventType::AbilityUsed, static_cast<sf::Keyboard::Key>(SavedSettings.UseAbility_KeyCode));
}

void InputManager::HandleInput(sf::RenderWindow & MainWindow, int i_CurrentGameState)
{
    // feed out to GameState specific functions
    switch (i_CurrentGameState)
    {
    case GameState::InMatch:
        HandleMatchSessionInput(MainWindow);
        break;
    case GameState::InSettingsMenu:
    case GameState::inMainMenu:
    case GameState::PreMatch:
        HandleMenuInput(MainWindow, i_CurrentGameState);
        break;
    }


    GlobalMousePosition = MainWindow.mapPixelToCoords(sf::Mouse::getPosition(MainWindow));


}

sf::Vector2f InputManager::GetGlobalMousePosition()
{
    return GlobalMousePosition;
}

void InputManager::CaptureEventForKeyBinding(EventType i_Type)
{
    SavedEventType = i_Type;
    //set flag for future capture in main input loop (HandleInput function)
    doCaptureKeyForBinding = true;
}

std::vector<int> InputManager::GetKeyBinding_KeyCodes()
{
    //create vector
    std::vector<int> KeyCodesContainer;

    auto iterator = KeyBindingsMap.begin();
    //cycle through key bidnigns map
    while (iterator != KeyBindingsMap.end())
    {
        //insert key codes from each mapping
        KeyCodesContainer.push_back(iterator->second);
        iterator++;
    }
    return KeyCodesContainer;
}


void InputManager::HandleMenuInput(sf::RenderWindow & MainWindow, int i_CurrentGameState)
{
    sf::Event event;

    while (MainWindow.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button)
            {
            case 0: // left click
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                {
                }
                else
                {
                    if (i_CurrentGameState == GameState::inMainMenu)
                    {
                        HandleEvent_CallBack(EventType::MainMenu_LeftClickPressed, 0);

                    }
                    else if (i_CurrentGameState == GameState::PreMatch)
                    {
                        HandleEvent_CallBack(EventType::PreMatchMenu_LeftClickPressed, 0);
                    }
                    else if (i_CurrentGameState == GameState::InSettingsMenu)
                    {
                        HandleEvent_CallBack(EventType::SettingsMenu_LeftClickPressed, 0);
                    }

                }
                break;
            case 1: // rick click
                if (i_CurrentGameState == GameState::inMainMenu)
                {
                    HandleEvent_CallBack(EventType::MainMenu_RightClickPressed, 0);

                }
                else if (i_CurrentGameState == GameState::PreMatch)
                {
                    HandleEvent_CallBack(EventType::PreMatchMenu_RightClickPressed, 0);
                }
                break;
            }
            break;
        case sf::Event::KeyPressed:
            // if capture key binding flag is true then capture current input and pair it with savedEventType (from CaptureEventForKeyBinding function)
            if (doCaptureKeyForBinding == true && i_CurrentGameState == GameState::InSettingsMenu)
            {
                ChangeKeyBinding(SavedEventType, event.key.code);
            }
            //only specific key that is pressed in the menu is the escape key, check for it
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
                HandleEvent_CallBack(EventType::EscapePressed, 0);
                break;
            }
            break;
        case sf::Event::Closed:
            HandleEvent_CallBack(EventType::WindowClosed, 0);
            break;
        }

    }
}

void InputManager::HandleMatchSessionInput(sf::RenderWindow & MainWindow)
{
    sf::Event event;

    while (MainWindow.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button)
            {
            case 0:
                HandleEvent_CallBack(EventType::MatchSession_LeftClickPressed, 0);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                {
                    HandleEvent_CallBack(EventType::SpecialShootBall, 0);
                }
                else
                {
                    HandleEvent_CallBack(EventType::ShootBall, 0);
                }
                break;
            case 1:
                HandleEvent_CallBack(EventType::PassBall, 0);
                break;
            }
            break;
        case sf::Event::KeyPressed:

            if (LookForKeyBinding(event))
            {
                return;
            }
            else
            {
                return;
            }
            break;
        case sf::Event::KeyReleased:

            if (LookForKeyBinding(event))
            {
                return;
            }
            else
            {
                switch (event.key.code)
                {

                case sf::Keyboard::Num1:
                    HandleEvent_CallBack(EventType::SwitchedPlayer, 0);
                    break;
                case sf::Keyboard::Num2:
                    HandleEvent_CallBack(EventType::SwitchedPlayer, 1);
                    break;
                case sf::Keyboard::Num3:
                    HandleEvent_CallBack(EventType::SwitchedPlayer, 2);
                    break;
                case sf::Keyboard::Num4:
                    HandleEvent_CallBack(EventType::SwitchedPlayer, 3);
                    break;
                case sf::Keyboard::Num5:
                    HandleEvent_CallBack(EventType::SwitchedPlayer, 4);
                    break;
                case sf::Keyboard::Escape:
                    HandleEvent_CallBack(EventType::EscapePressed, 0);
                    break;
                }
            }

            break;
        case sf::Event::Closed:
            HandleEvent_CallBack(EventType::WindowClosed, 0);
            break;
        }
    }
}

bool InputManager::LookForKeyBinding(sf::Event i_event)
{
    auto iterator = KeyBindingsMap.begin();

    //key released 
    if (i_event.type == sf::Event::KeyReleased)
    {
        while (iterator != KeyBindingsMap.end())
        {
            if (i_event.key.code == iterator->second)
            {
                HandleEvent_CallBack(iterator->first, 0);
                return true;
            }
            iterator++;
        }
    } //key pressed
    else if( i_event.type == sf::Event::KeyPressed)
    {
        while (iterator != KeyBindingsMap.end())
        {
            if (i_event.key.code == iterator->second)
            {
                HandleEvent_CallBack(iterator->first, 1);
                return true;
            }
            iterator++;
        }
    }

    return false;
}

void InputManager::ChangeKeyBinding(EventType i_Type, sf::Keyboard::Key i_Key)
{
    doCaptureKeyForBinding = false;
    auto iterator = KeyBindingsMap.begin();
    //list of approved keys
    if (i_Key <= 24 || i_Key == sf::Keyboard::Tab || i_Key == sf::Keyboard::Space || i_Key == sf::Keyboard::Up || i_Key == sf::Keyboard::Down || i_Key == sf::Keyboard::Right || i_Key == sf::Keyboard::Left || i_Key == sf::Keyboard::RControl || i_Key == sf::Keyboard::RShift)
    {
        while (iterator != KeyBindingsMap.end())
        {
            if (iterator->first == i_Type)
            {
                if (CheckForDuplicateKeyBinding(i_Key))
                {
                    // key already has a binding, cannot bind again
                    return;
                }
                else
                {
                    iterator->second = i_Key;
                }
                HandleEvent_CallBack(EventType::BindingChanged, 0);
                return;
            }
            iterator++;
        }
    }

   

}

bool InputManager::CheckForDuplicateKeyBinding(sf::Keyboard::Key i_Key)
{
    auto iterator = KeyBindingsMap.begin();

    while (iterator != KeyBindingsMap.end())
    {
        if (iterator->second == i_Key)
        {
            return true;
        }
        iterator++;
    }
    return false;
}
