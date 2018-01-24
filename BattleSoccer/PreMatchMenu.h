#pragma once

#include "Menu.h"
#include "GameEntityManager.h"

class PreMatchMenu : public Menu
{

public:
    PreMatchMenu();
    ~PreMatchMenu();


    bool Initialize(const sf::Vector2f &i_Scale) override;
    void DrawMenu(sf::RenderWindow &MainWindow);

    std::shared_ptr<GameEntityManager> m_GameEntityManager;

    void NextFormation();
    void PreviousFormation();

    void SetCurrentFormation();

    void LeftClickEvent() override;

    void UpdateMenu(const sf::Vector2f &MousePosition) override;

    void SetMenuScale(const sf::Vector2f &i_scale) override;


private:

    Button StartMatchButton;
    Button NextFormationButton;
    Button PreviousFormationButton;

    sf::Sprite FormationSprite;
    sf::Texture TheBowFormationTexture;
    sf::Texture TheBlitzFormationTexture;
    sf::Texture TheBoxFormationTexture;
    sf::Texture TheSpearFormationTexture;
    sf::Texture TheTriangleFormation;
   
    int CurrentFormation = 1;
    int TotalFormations = 5;

    void SelectFormation();

    std::shared_ptr<Player> GrabbedPlayerCard;


};

