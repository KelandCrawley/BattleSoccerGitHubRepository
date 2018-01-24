#pragma once



#include "GameEntityManager.h"
#include "Menu.h"
#include "PopUpMenu.h"

class MainMenu : public Menu
{
public:
    MainMenu();
    ~MainMenu();

    bool Initialize(const sf::Vector2f &i_Scale) override;
    void DrawMenu(sf::RenderWindow &MainWindow) override;

    void UpdateMenu(const sf::Vector2f &MousePosition) override;

    void RightClickEvent() override;
    void LeftClickEvent() override;


    std::shared_ptr<GameEntityManager> m_GameEntityManager;


    // on event functions
    void HirePlayer();
    void ReleasePlayer();
    void Cancel();
    void PopUpEndOfSeason();
    void SaveUserTeam();

private:

    Button PlayNextRoundButton;
    Button SettingsButton;
    std::shared_ptr<Player> GrabbedPlayerCard;

    PopUpMenu HirePlayerPopUpMenu;
    PopUpMenu ReleasePlayerPopUpMenu;
    PopUpMenu EndOfSeasonPopUpMenu;

    void AdvanceSeason();
};

