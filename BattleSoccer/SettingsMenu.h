#pragma once
#include "GameEntityManager.h"
#include "Menu.h"
#include "PopUpMenu.h"

#include "FileManager.h"
#include "SoundManager.h"
#include "InputManager.h"

class SettingsMenu : public Menu
{
public:
    SettingsMenu();
    ~SettingsMenu();

    bool Initialize(const sf::Vector2f &i_scale) override;
    void DrawMenu(sf::RenderWindow &MainWindow) override;

    void LeftClickEvent() override;

    void UpdateSettings();

    void SetMenuScale(const sf::Vector2f &i_scale) override;

    std::shared_ptr<GameEntityManager> m_GameEntityManager;

    std::function<void(EventType)> ChangeBinding_CallBack;
    std::function<std::vector<int>()> GetKeyBindings_CallBack;

private:

    Button ExitMenuButton;
    Button QuitGameButton;
    Button GameVolumeUpButton;
    Button GameVolumeDownButton;
    Button MusicVolumeUpButton;
    Button MusicVolumeDownButton;

    Button ResetDataButton;

    Button MoveUpBindingButton;
    Button MoveDownBindingButton;
    Button MoveRightBindingButton;
    Button MoveLeftBindingButton;
    Button SwitchToClosestPlayerBindingButton;
    Button UseAbilityBindingButton;
    void KeyBindingButtonPressed(int i_BindingNumber);

    sf::Font MainFont;
    sf::Text SettingsText[8];



    //loading
    void SaveSettings();


    std::string GetKeyString(int i_keyCode);
    std::string GetKeyString(sf::Keyboard::Key i_key);
    std::map<sf::Keyboard::Key, std::string> KeyToStringMap;



    void UpdateSoundSettingsText();
    void UpdateKeyBindingText();

    //popupMenu
    PopUpMenu ConfirmResetDataPopUP;
    void ToggleConfrimResetDataPopUP();
    void TriggerResetDataEvent();

};

