#include "PreMatchMenu.h"


PreMatchMenu::PreMatchMenu()
{

}


PreMatchMenu::~PreMatchMenu()
{
}

bool PreMatchMenu::Initialize(const sf::Vector2f &i_Scale)
{
    if (!MenuTexture.loadFromFile("Data/PreMatchSprite.png")) {};
    MenuSprite.setTexture(MenuTexture);
    MenuSprite.setPosition(sf::Vector2f(270, 120));

    if (!TheBowFormationTexture.loadFromFile("Data/Formations/TheBow_Formation.png")) {};
    if (!TheBlitzFormationTexture.loadFromFile("Data/Formations/TheBlitz_Formation.png")) {};
    if (!TheBoxFormationTexture.loadFromFile("Data/Formations/TheBox_Formation.png")) {};
    if (!TheSpearFormationTexture.loadFromFile("Data/Formations/TheSpear_Formation.png")) {};
    if (!TheTriangleFormation.loadFromFile("Data/Formations/TheTriangle_Formation.png")) {};
    FormationSprite.setTexture(TheBowFormationTexture);
    FormationSprite.setPosition(sf::Vector2f(470, 125));


    StartMatchButton.ButtonRect.setSize(sf::Vector2f(396, 76));
    StartMatchButton.ButtonRect.setPosition(sf::Vector2f(1194, 909));
    StartMatchButton.ButtonRect.setFillColor(sf::Color::Transparent);
    StartMatchButton.ButtonRect.setOutlineColor(sf::Color::Red);
    StartMatchButton.ButtonRect.setOutlineThickness(2);
    StartMatchButton.EventOnClick = EventType::MatchStarted;
    ButtonsContainer.push_back(StartMatchButton);


    NextFormationButton.ButtonRect.setSize(sf::Vector2f(110, 52));
    NextFormationButton.ButtonRect.setPosition(sf::Vector2f(1360, 135));
    NextFormationButton.ButtonRect.setFillColor(sf::Color::Transparent);
    NextFormationButton.ButtonRect.setOutlineColor(sf::Color::Red);
    NextFormationButton.ButtonRect.setOutlineThickness(2);
    NextFormationButton.EventOnClick = EventType::NextFormation;
    ButtonsContainer.push_back(NextFormationButton);

    PreviousFormationButton.ButtonRect.setSize(sf::Vector2f(110, 52));
    PreviousFormationButton.ButtonRect.setPosition(sf::Vector2f(470, 135));
    PreviousFormationButton.ButtonRect.setFillColor(sf::Color::Transparent);
    PreviousFormationButton.ButtonRect.setOutlineColor(sf::Color::Red);
    PreviousFormationButton.ButtonRect.setOutlineThickness(2);
    PreviousFormationButton.EventOnClick = EventType::PreviousFormation;
    ButtonsContainer.push_back(PreviousFormationButton);

    SetMenuScale(i_Scale);
    return true;

}

void PreMatchMenu::DrawMenu(sf::RenderWindow & MainWindow)
{
    MainWindow.draw(MenuSprite);
    for (auto i = 0; i < ButtonsContainer.size(); i++)
    {
        if (ButtonsContainer.at(i).isHoverd == true)
        {
            MainWindow.draw(ButtonsContainer.at(i).ButtonRect);
        }
    }
    MainWindow.draw(FormationSprite);
    if (GrabbedPlayerCard)
    {
        GrabbedPlayerCard->setPlayerCardHighlighted(true);
    }
    m_GameEntityManager->DrawUserTeamPlayerCards(MainWindow, sf::Vector2f(380, 626), 70);

}

void PreMatchMenu::NextFormation()
{
    CurrentFormation++;
    if (CurrentFormation > TotalFormations)
    {
        CurrentFormation = 1;
    }
    SelectFormation();
}

void PreMatchMenu::PreviousFormation()
{
    CurrentFormation--;
    if (CurrentFormation <= 0)
    {
        CurrentFormation = TotalFormations;
    }
    SelectFormation();
}

void PreMatchMenu::SetCurrentFormation()
{
    switch (m_GameEntityManager->GetUserTeam()->GetFormation())
    {
    case Team::Formation::TheBow:
        CurrentFormation = 1;
        break;
    case Team::Formation::TheBlitz:
        CurrentFormation = 2;
        break;
    case Team::Formation::TheBox:
        CurrentFormation = 3;
        break;
    case Team::Formation::TheSpear:
        CurrentFormation = 4;
        break;
    case Team::Formation::TheTriangle:
        CurrentFormation = 5;
        break;
    }
    SelectFormation();
    m_GameEntityManager->GetUserTeam()->SetPlayerPositions();
}

void PreMatchMenu::LeftClickEvent()
{
    for (auto i = 0; i < ButtonsContainer.size(); i++)
    {
        if (ButtonsContainer.at(i).ButtonRect.getGlobalBounds().contains(SavedMousePosition))
        {
            HandleEvent_CallBack(ButtonsContainer.at(i).EventOnClick, ButtonsContainer.at(i).PassedArgumentValue);
        }
    }

    //if a player is already grabbed, swap a player or deslect current player
    if (GrabbedPlayerCard)
    {
        std::shared_ptr<Player> SwapingPlayer = m_GameEntityManager->CheckforUserTeamPlayerCardSelection(SavedMousePosition);
        if (SwapingPlayer)
        {
            auto Team = m_GameEntityManager->GetUserTeam()->TeamRoster;
            std::iter_swap(std::find(Team->begin(), Team->end(), SwapingPlayer), std::find(Team->begin(), Team->end(), GrabbedPlayerCard));
            SwapingPlayer->setPlayerCardHighlighted(false);
        }

        GrabbedPlayerCard->setPlayerCardHighlighted(false);
        GrabbedPlayerCard = nullptr;
        m_GameEntityManager->GetUserTeam()->SetPlayerPositions();

    }
    else // if no player is grabbed then check to see if one has been selected
    {
        GrabbedPlayerCard = m_GameEntityManager->CheckforUserTeamPlayerCardSelection(SavedMousePosition);
    }
}

void PreMatchMenu::UpdateMenu(const sf::Vector2f &MousePosition)
{
    SavedMousePosition = MousePosition;
    LookForButtonHovering();

    m_GameEntityManager->CheckforUserTeamPlayerCardSelection(MousePosition);
}

void PreMatchMenu::SetMenuScale(const sf::Vector2f &i_scale)
{
    MenuSprite.setScale(i_scale);
    MenuSprite.setPosition(MenuSprite.getPosition().x * i_scale.x, MenuSprite.getPosition().y * i_scale.y);
    for (auto i = 0; i < ButtonsContainer.size(); i++)
    {
        ButtonsContainer.at(i).ButtonRect.setScale(i_scale);
        ButtonsContainer.at(i).ButtonRect.setPosition(ButtonsContainer.at(i).ButtonRect.getPosition().x * i_scale.x, ButtonsContainer.at(i).ButtonRect.getPosition().y * i_scale.y);
    }

    FormationSprite.setScale(i_scale);
    FormationSprite.setPosition(FormationSprite.getPosition().x * i_scale.x, FormationSprite.getPosition().y * i_scale.y);

}

void PreMatchMenu::SelectFormation()
{
    switch (CurrentFormation)
    {
    case 1:
        FormationSprite.setTexture(TheBowFormationTexture);
        m_GameEntityManager->GetUserTeam()->SetFormation(Team::Formation::TheBow);
        break;
    case 2:
        FormationSprite.setTexture(TheBlitzFormationTexture);
        m_GameEntityManager->GetUserTeam()->SetFormation(Team::Formation::TheBlitz);
        break;
    case 3:
        FormationSprite.setTexture(TheBoxFormationTexture);
        m_GameEntityManager->GetUserTeam()->SetFormation(Team::Formation::TheBox);
        break;
    case 4:
        FormationSprite.setTexture(TheSpearFormationTexture);
        m_GameEntityManager->GetUserTeam()->SetFormation(Team::Formation::TheSpear);
        break;
    case 5:
        FormationSprite.setTexture(TheTriangleFormation);
        m_GameEntityManager->GetUserTeam()->SetFormation(Team::Formation::TheTriangle);
        break;
    }
}
