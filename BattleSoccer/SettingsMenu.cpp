#include "SettingsMenu.h"
#include "Game.h"


SettingsMenu::SettingsMenu()
    :ConfirmResetDataPopUP()
{
    ExitMenuButton.ButtonRect.setSize(sf::Vector2f(396, 76));
    ExitMenuButton.ButtonRect.setPosition(sf::Vector2f(770, 851));
    ExitMenuButton.ButtonRect.setFillColor(sf::Color::Transparent);
    ExitMenuButton.ButtonRect.setOutlineColor(sf::Color::Red);
    ExitMenuButton.ButtonRect.setOutlineThickness(2);
    ExitMenuButton.EventOnClick = EventType::EscapePressed;
    ExitMenuButton.PassedArgumentValue = 0;
    ExitMenuButton.Button_CallBack = std::bind(&SettingsMenu::SaveSettings, this);
    ButtonsContainer.push_back(ExitMenuButton);

    QuitGameButton.ButtonRect.setSize(sf::Vector2f(196, 38));
    QuitGameButton.ButtonRect.setPosition(sf::Vector2f(870, 948));
    QuitGameButton.ButtonRect.setFillColor(sf::Color::Transparent);
    QuitGameButton.ButtonRect.setOutlineColor(sf::Color::Red);
    QuitGameButton.ButtonRect.setOutlineThickness(2);
    QuitGameButton.EventOnClick = EventType::QuitGame;
    QuitGameButton.PassedArgumentValue = 0;
    ButtonsContainer.push_back(QuitGameButton);

    GameVolumeDownButton.ButtonRect.setSize(sf::Vector2f(97, 36));
    GameVolumeDownButton.ButtonRect.setPosition(sf::Vector2f(629, 360));
    GameVolumeDownButton.ButtonRect.setFillColor(sf::Color::Transparent);
    GameVolumeDownButton.ButtonRect.setOutlineColor(sf::Color::Red);
    GameVolumeDownButton.ButtonRect.setOutlineThickness(2);
    GameVolumeDownButton.EventOnClick = EventType::GameVolumeDown;
    GameVolumeDownButton.PassedArgumentValue = 0;
    ButtonsContainer.push_back(GameVolumeDownButton);

    GameVolumeUpButton.ButtonRect.setSize(sf::Vector2f(97, 36));
    GameVolumeUpButton.ButtonRect.setPosition(sf::Vector2f(849, 360));
    GameVolumeUpButton.ButtonRect.setFillColor(sf::Color::Transparent);
    GameVolumeUpButton.ButtonRect.setOutlineColor(sf::Color::Red);
    GameVolumeUpButton.ButtonRect.setOutlineThickness(2);
    GameVolumeUpButton.EventOnClick = EventType::GameVolumeUp;
    GameVolumeUpButton.PassedArgumentValue = 0;
    ButtonsContainer.push_back(GameVolumeUpButton);

    MusicVolumeDownButton.ButtonRect.setSize(sf::Vector2f(97, 36));
    MusicVolumeDownButton.ButtonRect.setPosition(sf::Vector2f(629, 410));
    MusicVolumeDownButton.ButtonRect.setFillColor(sf::Color::Transparent);
    MusicVolumeDownButton.ButtonRect.setOutlineColor(sf::Color::Red);
    MusicVolumeDownButton.ButtonRect.setOutlineThickness(2);
    MusicVolumeDownButton.EventOnClick = EventType::MusicVolumeDown;
    MusicVolumeUpButton.PassedArgumentValue = 0;
    ButtonsContainer.push_back(MusicVolumeDownButton);

    MusicVolumeUpButton.ButtonRect.setSize(sf::Vector2f(97, 36));
    MusicVolumeUpButton.ButtonRect.setPosition(sf::Vector2f(849, 410));
    MusicVolumeUpButton.ButtonRect.setFillColor(sf::Color::Transparent);
    MusicVolumeUpButton.ButtonRect.setOutlineColor(sf::Color::Red);
    MusicVolumeUpButton.ButtonRect.setOutlineThickness(2);
    MusicVolumeUpButton.EventOnClick = EventType::MusicVolumeUp;
    MusicVolumeUpButton.PassedArgumentValue = 0;
    ButtonsContainer.push_back(MusicVolumeUpButton);

    MoveUpBindingButton.ButtonRect.setSize(sf::Vector2f(196, 38));
    MoveUpBindingButton.ButtonRect.setPosition(sf::Vector2f(1454, 368));
    MoveUpBindingButton.ButtonRect.setFillColor(sf::Color::Transparent);
    MoveUpBindingButton.ButtonRect.setOutlineColor(sf::Color::Red);
    MoveUpBindingButton.ButtonRect.setOutlineThickness(2);
    MoveUpBindingButton.EventOnClick = EventType::NoEvent;
    MoveUpBindingButton.PassedArgumentValue = 0;
    MoveUpBindingButton.Button_CallBack = std::bind(&SettingsMenu::KeyBindingButtonPressed, this, std::placeholders::_1);
    ButtonsContainer.push_back(MoveUpBindingButton);

    MoveDownBindingButton.ButtonRect.setSize(sf::Vector2f(196, 38));
    MoveDownBindingButton.ButtonRect.setPosition(sf::Vector2f(1454, 409));
    MoveDownBindingButton.ButtonRect.setFillColor(sf::Color::Transparent);
    MoveDownBindingButton.ButtonRect.setOutlineColor(sf::Color::Red);
    MoveDownBindingButton.ButtonRect.setOutlineThickness(2);
    MoveDownBindingButton.EventOnClick = EventType::NoEvent;
    MoveDownBindingButton.PassedArgumentValue = 1;
    MoveDownBindingButton.Button_CallBack = std::bind(&SettingsMenu::KeyBindingButtonPressed, this, std::placeholders::_1);
    ButtonsContainer.push_back(MoveDownBindingButton);

    MoveRightBindingButton.ButtonRect.setSize(sf::Vector2f(196, 38));
    MoveRightBindingButton.ButtonRect.setPosition(sf::Vector2f(1454, 450));
    MoveRightBindingButton.ButtonRect.setFillColor(sf::Color::Transparent);
    MoveRightBindingButton.ButtonRect.setOutlineColor(sf::Color::Red);
    MoveRightBindingButton.ButtonRect.setOutlineThickness(2);
    MoveRightBindingButton.EventOnClick = EventType::NoEvent;
    MoveRightBindingButton.PassedArgumentValue = 2;
    MoveRightBindingButton.Button_CallBack = std::bind(&SettingsMenu::KeyBindingButtonPressed, this, std::placeholders::_1);
    ButtonsContainer.push_back(MoveRightBindingButton);

    MoveLeftBindingButton.ButtonRect.setSize(sf::Vector2f(196, 38));
    MoveLeftBindingButton.ButtonRect.setPosition(sf::Vector2f(1454, 491));
    MoveLeftBindingButton.ButtonRect.setFillColor(sf::Color::Transparent);
    MoveLeftBindingButton.ButtonRect.setOutlineColor(sf::Color::Red);
    MoveLeftBindingButton.ButtonRect.setOutlineThickness(2);
    MoveLeftBindingButton.EventOnClick = EventType::NoEvent;
    MoveLeftBindingButton.PassedArgumentValue = 3;
    MoveLeftBindingButton.Button_CallBack = std::bind(&SettingsMenu::KeyBindingButtonPressed, this, std::placeholders::_1);
    ButtonsContainer.push_back(MoveLeftBindingButton);

    SwitchToClosestPlayerBindingButton.ButtonRect.setSize(sf::Vector2f(196, 38));
    SwitchToClosestPlayerBindingButton.ButtonRect.setPosition(sf::Vector2f(1454, 532));
    SwitchToClosestPlayerBindingButton.ButtonRect.setFillColor(sf::Color::Transparent);
    SwitchToClosestPlayerBindingButton.ButtonRect.setOutlineColor(sf::Color::Red);
    SwitchToClosestPlayerBindingButton.ButtonRect.setOutlineThickness(2);
    SwitchToClosestPlayerBindingButton.EventOnClick = EventType::NoEvent;
    SwitchToClosestPlayerBindingButton.PassedArgumentValue = 4;
    SwitchToClosestPlayerBindingButton.Button_CallBack = std::bind(&SettingsMenu::KeyBindingButtonPressed, this, std::placeholders::_1);
    ButtonsContainer.push_back(SwitchToClosestPlayerBindingButton);

    UseAbilityBindingButton.ButtonRect.setSize(sf::Vector2f(196, 38));
    UseAbilityBindingButton.ButtonRect.setPosition(sf::Vector2f(1454, 573));
    UseAbilityBindingButton.ButtonRect.setFillColor(sf::Color::Transparent);
    UseAbilityBindingButton.ButtonRect.setOutlineColor(sf::Color::Red);
    UseAbilityBindingButton.ButtonRect.setOutlineThickness(2);
    UseAbilityBindingButton.EventOnClick = EventType::NoEvent;
    UseAbilityBindingButton.PassedArgumentValue = 5;
    UseAbilityBindingButton.Button_CallBack = std::bind(&SettingsMenu::KeyBindingButtonPressed, this, std::placeholders::_1);
    ButtonsContainer.push_back(UseAbilityBindingButton);

    ResetDataButton.ButtonRect.setSize(sf::Vector2f(196, 36));
    ResetDataButton.ButtonRect.setPosition(sf::Vector2f(495, 650));
    ResetDataButton.ButtonRect.setFillColor(sf::Color::Transparent);
    ResetDataButton.ButtonRect.setOutlineColor(sf::Color::Red);
    ResetDataButton.ButtonRect.setOutlineThickness(2);
    ResetDataButton.EventOnClick = EventType::NoEvent;
    ResetDataButton.PassedArgumentValue = 5;
    ResetDataButton.Button_CallBack = std::bind(&SettingsMenu::ToggleConfrimResetDataPopUP, this);
    ButtonsContainer.push_back(ResetDataButton);

    if (!MainFont.loadFromFile("Data/ArialFont/arial.ttf")){ }

    for (auto i = 0; i < 8; i++)
    {
        SettingsText[i].setFillColor(sf::Color::Black);
        SettingsText[i].setFont(MainFont);
        SettingsText[i].setCharacterSize(24);
    }
    SettingsText[0].setString(std::to_string(SoundManager::EffectsVolume / 5));
    SettingsText[1].setString(std::to_string(SoundManager::MusicVolume / 5));

    SettingsText[0].setPosition(sf::Vector2f(780, 364));
    SettingsText[1].setPosition(sf::Vector2f(780, 414));

    SettingsText[2].setPosition(sf::Vector2f(1545, 384));
    SettingsText[3].setPosition(sf::Vector2f(1545, 425));
    SettingsText[4].setPosition(sf::Vector2f(1545, 466));
    SettingsText[5].setPosition(sf::Vector2f(1545, 507));
    SettingsText[6].setPosition(sf::Vector2f(1545, 546));
    SettingsText[7].setPosition(sf::Vector2f(1545, 589));



    //key mapping
    KeyToStringMap.emplace(sf::Keyboard::Unknown, "Invalid");
    KeyToStringMap.emplace(sf::Keyboard::A, "A");
    KeyToStringMap.emplace(sf::Keyboard::B, "B");
    KeyToStringMap.emplace(sf::Keyboard::C, "C");
    KeyToStringMap.emplace(sf::Keyboard::D, "D");
    KeyToStringMap.emplace(sf::Keyboard::E, "E");
    KeyToStringMap.emplace(sf::Keyboard::F, "F");
    KeyToStringMap.emplace(sf::Keyboard::G, "G");
    KeyToStringMap.emplace(sf::Keyboard::H, "H");
    KeyToStringMap.emplace(sf::Keyboard::I, "I");
    KeyToStringMap.emplace(sf::Keyboard::J, "J");
    KeyToStringMap.emplace(sf::Keyboard::K, "K");
    KeyToStringMap.emplace(sf::Keyboard::L, "L");
    KeyToStringMap.emplace(sf::Keyboard::M, "M");
    KeyToStringMap.emplace(sf::Keyboard::N, "N");
    KeyToStringMap.emplace(sf::Keyboard::O, "O");
    KeyToStringMap.emplace(sf::Keyboard::P, "P");
    KeyToStringMap.emplace(sf::Keyboard::Q, "Q");
    KeyToStringMap.emplace(sf::Keyboard::R, "R");
    KeyToStringMap.emplace(sf::Keyboard::S, "S");
    KeyToStringMap.emplace(sf::Keyboard::T, "T");
    KeyToStringMap.emplace(sf::Keyboard::U, "U");
    KeyToStringMap.emplace(sf::Keyboard::V, "V");
    KeyToStringMap.emplace(sf::Keyboard::W, "W");
    KeyToStringMap.emplace(sf::Keyboard::X, "X");
    KeyToStringMap.emplace(sf::Keyboard::Y, "Y");
    KeyToStringMap.emplace(sf::Keyboard::Z, "Z");
    KeyToStringMap.emplace(sf::Keyboard::Tab, "Tab");
    KeyToStringMap.emplace(sf::Keyboard::Space, "Space");
    KeyToStringMap.emplace(sf::Keyboard::Up, "Up Key");
    KeyToStringMap.emplace(sf::Keyboard::Down, "Down Key");
    KeyToStringMap.emplace(sf::Keyboard::Right, "Right Key");
    KeyToStringMap.emplace(sf::Keyboard::Left, "Left Key");
    KeyToStringMap.emplace(sf::Keyboard::RControl, "Right Control");
    KeyToStringMap.emplace(sf::Keyboard::RShift, "Right Shift");

}


SettingsMenu::~SettingsMenu()
{
}

bool SettingsMenu::Initialize(const sf::Vector2f &i_scale)
{
    if (!MenuTexture.loadFromFile("Data/SettingsMenuScreenSprite.png")) {};
    MenuSprite.setTexture(MenuTexture);
    MenuSprite.setPosition(sf::Vector2f(270, 120));

    std::vector<int> KeyCodesCotainer = GetKeyBindings_CallBack();
    if (KeyCodesCotainer.size() == 6)
    {
        SettingsText[2].setString(GetKeyString(KeyCodesCotainer.at(0)));
        SettingsText[3].setString(GetKeyString(KeyCodesCotainer.at(1)));
        SettingsText[4].setString(GetKeyString(KeyCodesCotainer.at(2)));
        SettingsText[5].setString(GetKeyString(KeyCodesCotainer.at(3)));
        SettingsText[6].setString(GetKeyString(KeyCodesCotainer.at(4)));
        SettingsText[7].setString(GetKeyString(KeyCodesCotainer.at(5)));


        SettingsText[2].setOrigin(sf::Vector2f((SettingsText[2].getString().getSize() * 24 / 4), 12));
        SettingsText[3].setOrigin(sf::Vector2f((SettingsText[2].getString().getSize() * 24 / 4), 12));
        SettingsText[4].setOrigin(sf::Vector2f((SettingsText[2].getString().getSize() * 24 / 4), 12));
        SettingsText[5].setOrigin(sf::Vector2f((SettingsText[2].getString().getSize() * 24 / 4), 12));
        SettingsText[6].setOrigin(sf::Vector2f((SettingsText[2].getString().getSize() * 24 / 4), 12));
        SettingsText[7].setOrigin(sf::Vector2f((SettingsText[2].getString().getSize() * 24 / 4), 12));
    }

    ConfirmResetDataPopUP.Initialize("Data/ConfirmResetDataPopUP.png");
    ConfirmResetDataPopUP.SetMenuPosition(sf::Vector2f(MenuSprite.getPosition().x + MenuSprite.getGlobalBounds().width / 2, MenuSprite.getPosition().y + MenuSprite.getGlobalBounds().height / 2));
    ConfirmResetDataPopUP.AddButton(sf::RectangleShape(sf::Vector2f(198, 50)), sf::Vector2f(ConfirmResetDataPopUP.GetMenuPosition().x -217, ConfirmResetDataPopUP.GetMenuPosition().y), 0, EventType::NoEvent, std::bind(&SettingsMenu::ToggleConfrimResetDataPopUP, this));
    ConfirmResetDataPopUP.AddButton(sf::RectangleShape(sf::Vector2f(198, 50)), sf::Vector2f(ConfirmResetDataPopUP.GetMenuPosition().x + 19, ConfirmResetDataPopUP.GetMenuPosition().y), 0, EventType::NoEvent, std::bind(&SettingsMenu::TriggerResetDataEvent, this));


    SetMenuScale(i_scale);
    return false;
}

void SettingsMenu::DrawMenu(sf::RenderWindow & MainWindow)
{
    MainWindow.draw(MenuSprite);
    for (auto i = 0; i < ButtonsContainer.size(); i++)
    {
        if (ButtonsContainer.at(i).isHoverd == true)
        {
            MainWindow.draw(ButtonsContainer.at(i).ButtonRect);
        }
    }
    for (auto i = 0; i < 8; i++)
    {
        MainWindow.draw(SettingsText[i]);
    }
    if (ConfirmResetDataPopUP.GetDoDrawMenu())
    {
        ConfirmResetDataPopUP.DrawMenu(MainWindow);
    }
}

void SettingsMenu::LeftClickEvent()
{
    if(ConfirmResetDataPopUP.GetDoDrawMenu())
    {
        ConfirmResetDataPopUP.SavedMousePosition = SavedMousePosition;
        ConfirmResetDataPopUP.LeftClickEvent();
        return;
    }
    for (auto i = 0; i < ButtonsContainer.size(); i++)
    {
        if (ButtonsContainer.at(i).ButtonRect.getGlobalBounds().contains(SavedMousePosition))
        {
            HandleEvent_CallBack(ButtonsContainer.at(i).EventOnClick, ButtonsContainer.at(i).PassedArgumentValue);
            if (ButtonsContainer.at(i).Button_CallBack)
            {
                ButtonsContainer.at(i).Button_CallBack(ButtonsContainer.at(i).PassedArgumentValue);
            }

        }
    }
}

void SettingsMenu::UpdateSettings()
{
    UpdateSoundSettingsText();
    UpdateKeyBindingText();
}

void SettingsMenu::SetMenuScale(const sf::Vector2f &i_scale)
{
    MenuSprite.setScale(i_scale);
    MenuSprite.setPosition(MenuSprite.getPosition().x * i_scale.x, MenuSprite.getPosition().y * i_scale.y);
    for (auto i = 0; i < ButtonsContainer.size(); i++)
    {
        ButtonsContainer.at(i).ButtonRect.setScale(i_scale);
        ButtonsContainer.at(i).ButtonRect.setPosition(ButtonsContainer.at(i).ButtonRect.getPosition().x * i_scale.x, ButtonsContainer.at(i).ButtonRect.getPosition().y * i_scale.y);
    }

    SettingsText[0].setCharacterSize(24 * MenuSprite.getScale().y);
    SettingsText[1].setCharacterSize(24 * MenuSprite.getScale().y);
    SettingsText[2].setCharacterSize(24 * MenuSprite.getScale().y);
    SettingsText[3].setCharacterSize(24 * MenuSprite.getScale().y);
    SettingsText[4].setCharacterSize(24 * MenuSprite.getScale().y);
    SettingsText[5].setCharacterSize(24 * MenuSprite.getScale().y);
    SettingsText[6].setCharacterSize(24 * MenuSprite.getScale().y);
    SettingsText[7].setCharacterSize(24 * MenuSprite.getScale().y);


    SettingsText[0].setPosition(sf::Vector2f(780 * MenuSprite.getScale().x, 364 * MenuSprite.getScale().y ));
    SettingsText[1].setPosition(sf::Vector2f(780 * MenuSprite.getScale().x, 414 * MenuSprite.getScale().y));
    SettingsText[2].setPosition(sf::Vector2f(1545 * MenuSprite.getScale().x, 384 * MenuSprite.getScale().y));
    SettingsText[3].setPosition(sf::Vector2f(1545 * MenuSprite.getScale().x, 425 * MenuSprite.getScale().y));
    SettingsText[4].setPosition(sf::Vector2f(1545 * MenuSprite.getScale().x, 466 * MenuSprite.getScale().y));
    SettingsText[5].setPosition(sf::Vector2f(1545 * MenuSprite.getScale().x, 507 * MenuSprite.getScale().y));
    SettingsText[6].setPosition(sf::Vector2f(1545 * MenuSprite.getScale().x, 546 * MenuSprite.getScale().y));
    SettingsText[7].setPosition(sf::Vector2f(1545 * MenuSprite.getScale().x, 589 * MenuSprite.getScale().y));
}


void SettingsMenu::KeyBindingButtonPressed(int i_BindingNumber)
{
    EventType i_Type = EventType::NoEvent;
    switch (i_BindingNumber)
    {
    case 0:
        i_Type = EventType::MoveUpToggled;
        break;
    case 1:
        i_Type = EventType::MoveDownToggled;
        break;
    case 2:
        i_Type = EventType::MoveRightToggled;
        break;
    case 3:
        i_Type = EventType::MoveLeftToggled;
        break;
    case 4:
        i_Type = EventType::SwitchToClosestPlayer;
        break;
    case 5:
        i_Type = EventType::AbilityUsed;
        break;
    }
    ChangeBinding_CallBack(i_Type);
}

void SettingsMenu::UpdateSoundSettingsText()
{
    //sound volume is set from 0-50 so i divide by 5 in order to achieve 0-10
    SettingsText[0].setString(std::to_string(SoundManager::EffectsVolume / 5));
    SettingsText[1].setString(std::to_string(SoundManager::MusicVolume / 5));
}

void SettingsMenu::UpdateKeyBindingText()
{
    std::vector<int> KeyCodesCotainer = GetKeyBindings_CallBack();
    if (KeyCodesCotainer.size() == 6)
    {
        SettingsText[2].setString(GetKeyString(KeyCodesCotainer.at(0)));
        SettingsText[3].setString(GetKeyString(KeyCodesCotainer.at(1)));
        SettingsText[4].setString(GetKeyString(KeyCodesCotainer.at(2)));
        SettingsText[5].setString(GetKeyString(KeyCodesCotainer.at(3)));
        SettingsText[6].setString(GetKeyString(KeyCodesCotainer.at(4)));
        SettingsText[7].setString(GetKeyString(KeyCodesCotainer.at(5)));

        SettingsText[2].setOrigin(sf::Vector2f((SettingsText[2].getString().getSize() * SettingsText[2].getCharacterSize() / 4), SettingsText[2].getCharacterSize() / 2));
        SettingsText[3].setOrigin(sf::Vector2f((SettingsText[3].getString().getSize() * SettingsText[2].getCharacterSize() / 4), SettingsText[2].getCharacterSize() / 2));
        SettingsText[4].setOrigin(sf::Vector2f((SettingsText[4].getString().getSize() * SettingsText[2].getCharacterSize() / 4), SettingsText[2].getCharacterSize() / 2));
        SettingsText[5].setOrigin(sf::Vector2f((SettingsText[5].getString().getSize() * SettingsText[2].getCharacterSize() / 4), SettingsText[2].getCharacterSize() / 2));
        SettingsText[6].setOrigin(sf::Vector2f((SettingsText[6].getString().getSize() * SettingsText[2].getCharacterSize() / 4), SettingsText[2].getCharacterSize() / 2));
        SettingsText[7].setOrigin(sf::Vector2f((SettingsText[7].getString().getSize() * SettingsText[2].getCharacterSize() / 4), SettingsText[2].getCharacterSize() / 2));
    }
}

void SettingsMenu::ToggleConfrimResetDataPopUP()
{
    ConfirmResetDataPopUP.SetDoDrawMenu(!ConfirmResetDataPopUP.GetDoDrawMenu());
}

void SettingsMenu::TriggerResetDataEvent()
{
    ConfirmResetDataPopUP.SetDoDrawMenu(false);
    HandleEvent_CallBack(EventType::ResetData, 0);
}

void SettingsMenu::SaveSettings()
{
    FileManager::GameSettingsDataPack SavedSettingsPack;
    SavedSettingsPack.SavedGameVolume = SoundManager::EffectsVolume;
    SavedSettingsPack.SavedMusicVolume = SoundManager::MusicVolume;

    std::vector<int> KeyCodesCotainer = GetKeyBindings_CallBack();
    if (KeyCodesCotainer.size() == 6)
    {
        SavedSettingsPack.MoveUp_KeyCode = KeyCodesCotainer.at(0);
        SavedSettingsPack.MoveDown_KeyCode = KeyCodesCotainer.at(1);
        SavedSettingsPack.MoveRight_KeyCode = KeyCodesCotainer.at(2);
        SavedSettingsPack.MoveLeft_KeyCode = KeyCodesCotainer.at(3);
        SavedSettingsPack.SwitchToClosestPlayer_KeyCode = KeyCodesCotainer.at(4);
        SavedSettingsPack.UseAbility_KeyCode = KeyCodesCotainer.at(5);
    }


    FileManager::SaveSettings(SavedSettingsPack);
}

std::string SettingsMenu::GetKeyString(int i_keyCode)
{
    return GetKeyString(static_cast<sf::Keyboard::Key>(i_keyCode));
}

std::string SettingsMenu::GetKeyString(sf::Keyboard::Key i_key)
{
    auto iterator = KeyToStringMap.begin();
    while (iterator != KeyToStringMap.end())
    {
        if (iterator->first == i_key)
        {
            return iterator->second;
        }
        iterator++;
    }
    return "Not Found";
}

