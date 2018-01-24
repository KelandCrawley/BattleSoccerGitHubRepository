#include "PopUpMenu.h"



PopUpMenu::PopUpMenu()
{
}


PopUpMenu::~PopUpMenu()
{
}

bool PopUpMenu::Initialize(const sf::Vector2f &i_Scale)
{
    return false;
}

bool PopUpMenu::Initialize(sf::String FileName)
{
    if (!MenuTexture.loadFromFile(FileName)) {};
    MenuSprite.setTexture(MenuTexture);

    MenuSprite.setOrigin(sf::Vector2f(MenuSprite.getGlobalBounds().width / 2, MenuSprite.getGlobalBounds().height / 2));
    return false;
}

void PopUpMenu::DrawMenu(sf::RenderWindow & MainWindow)
{
    MainWindow.draw(MenuSprite);
    for (auto i = 0; i < ButtonsContainer.size(); i++)
    {
        if (ButtonsContainer.at(i).isHoverd == true)
        {
            MainWindow.draw(ButtonsContainer.at(i).ButtonRect);
        }
    }
}

void PopUpMenu::SetDoDrawMenu(bool i_Setter)
{
    doDrawMenu = i_Setter;
}

bool PopUpMenu::GetDoDrawMenu()
{
    return doDrawMenu;
}

sf::Vector2f PopUpMenu::GetMenuPosition()
{
    return MenuSprite.getPosition();
}

void PopUpMenu::SetMenuPosition(sf::Vector2f i_Position)
{
    MenuSprite.setPosition(i_Position);
}
