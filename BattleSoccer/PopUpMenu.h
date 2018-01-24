#pragma once

#include "GameEntityManager.h"
#include "Menu.h"

class PopUpMenu : public Menu
{
    friend class MainMenu;
public:
    PopUpMenu();
    ~PopUpMenu();

    bool Initialize(const sf::Vector2f &i_Scale) override;
    bool Initialize(sf::String FileName);
    void DrawMenu(sf::RenderWindow &MainWindow) override;

    void SetDoDrawMenu(bool i_Setter);
    bool GetDoDrawMenu();

    sf::Vector2f GetMenuPosition();
    void SetMenuPosition(sf::Vector2f i_Position);
private:

    bool doDrawMenu;
};

