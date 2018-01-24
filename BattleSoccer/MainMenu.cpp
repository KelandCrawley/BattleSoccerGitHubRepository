#include "MainMenu.h"


MainMenu::MainMenu()
    :HirePlayerPopUpMenu()
    , ReleasePlayerPopUpMenu()
    ,EndOfSeasonPopUpMenu()
{
    PlayNextRoundButton.ButtonRect.setSize(sf::Vector2f(396, 76));
    PlayNextRoundButton.ButtonRect.setPosition(sf::Vector2f(1370, 932));
    PlayNextRoundButton.ButtonRect.setFillColor(sf::Color::Transparent);
    PlayNextRoundButton.ButtonRect.setOutlineColor(sf::Color::Red);
    PlayNextRoundButton.ButtonRect.setOutlineThickness(2);
    PlayNextRoundButton.EventOnClick = EventType::PreMatchStarted;
    ButtonsContainer.push_back(PlayNextRoundButton);

    SettingsButton.ButtonRect.setSize(sf::Vector2f(60, 60));
    SettingsButton.ButtonRect.setPosition(sf::Vector2f(1790, 17));
    SettingsButton.ButtonRect.setFillColor(sf::Color::Transparent);
    SettingsButton.ButtonRect.setOutlineColor(sf::Color::Transparent);
    SettingsButton.ButtonRect.setOutlineThickness(2);
    SettingsButton.EventOnClick = EventType::SettingsButtonPressed;
    ButtonsContainer.push_back(SettingsButton);


}


MainMenu::~MainMenu()
{
}

bool MainMenu::Initialize(const sf::Vector2f &i_Scale)
{
    if (!MenuTexture.loadFromFile("Data/MainMenuSprite.png")) {};
    MenuSprite.setTexture(MenuTexture);

    HirePlayerPopUpMenu.Initialize(sf::String("Data/HirePlayerPopUpSprite.png"));
    ReleasePlayerPopUpMenu.Initialize(sf::String("Data/ReleasePlayerPopUpSprite.png"));
    EndOfSeasonPopUpMenu.Initialize(sf::String("Data/EndOfSeasonPopUpSprite.png"));

    HirePlayerPopUpMenu.SetMenuPosition(sf::Vector2f(MenuSprite.getGlobalBounds().width / 2, MenuSprite.getGlobalBounds().height / 2));
    ReleasePlayerPopUpMenu.SetMenuPosition(sf::Vector2f(MenuSprite.getGlobalBounds().width / 2, MenuSprite.getGlobalBounds().height / 2));
    EndOfSeasonPopUpMenu.SetMenuPosition(sf::Vector2f(MenuSprite.getGlobalBounds().width / 2, MenuSprite.getGlobalBounds().height / 2));


    HirePlayerPopUpMenu.AddButton(sf::RectangleShape(sf::Vector2f(196, 46)), sf::Vector2f(MenuSprite.getGlobalBounds().width / 2 + 19 , MenuSprite.getGlobalBounds().height / 2 + 159),0, EventType::NoEvent, std::bind(&MainMenu::HirePlayer, this));
    HirePlayerPopUpMenu.AddButton(sf::RectangleShape(sf::Vector2f(196, 46)), sf::Vector2f(MenuSprite.getGlobalBounds().width / 2 - 215, MenuSprite.getGlobalBounds().height / 2 + 159), 0, EventType::NoEvent, std::bind(&MainMenu::Cancel, this));


    ReleasePlayerPopUpMenu.AddButton(sf::RectangleShape(sf::Vector2f(196, 46)), sf::Vector2f(MenuSprite.getGlobalBounds().width / 2 + 19, MenuSprite.getGlobalBounds().height / 2 + 159), 0, EventType::NoEvent, std::bind(&MainMenu::ReleasePlayer, this));
    ReleasePlayerPopUpMenu.AddButton(sf::RectangleShape(sf::Vector2f(196, 46)), sf::Vector2f(MenuSprite.getGlobalBounds().width / 2 - 215, MenuSprite.getGlobalBounds().height / 2 + 159), 0, EventType::NoEvent, std::bind(&MainMenu::Cancel, this));
  

    EndOfSeasonPopUpMenu.AddButton(sf::RectangleShape(sf::Vector2f(396, 56)), sf::Vector2f(MenuSprite.getGlobalBounds().width / 2 - 198, MenuSprite.getGlobalBounds().height / 2 + 159), 0, EventType::NoEvent, std::bind(&MainMenu::AdvanceSeason, this));


    SetMenuScale(i_Scale);

    return true;
}

void MainMenu::DrawMenu(sf::RenderWindow & MainWindow)
{
    MainWindow.draw(MenuSprite);
    for (auto i = 0; i < ButtonsContainer.size(); i++)
    {
        if (ButtonsContainer.at(i).isHoverd == true)
        {
            MainWindow.draw(ButtonsContainer.at(i).ButtonRect);
        }
    }
    if (GrabbedPlayerCard)
    {
        GrabbedPlayerCard->setPlayerCardHighlighted(true);
    }
    m_GameEntityManager->DrawUserTeamPlayerCards(MainWindow, sf::Vector2f(100, 50), 10);
    m_GameEntityManager->DrawFreeAgentPlayerCard(MainWindow, sf::Vector2f(100, 520));
    m_GameEntityManager->ListTeamsWithLeagueRecord(MainWindow, sf::Vector2f(1325, 100));
    m_GameEntityManager->DrawGameInfo(MainWindow, sf::Vector2f(1475, 25));

    if (HirePlayerPopUpMenu.GetDoDrawMenu())
    {
        HirePlayerPopUpMenu.DrawMenu(MainWindow);
        m_GameEntityManager->DrawSelectedPlayerCard(MainWindow, sf::Vector2f(MenuSprite.getGlobalBounds().width / 2 - 212, MenuSprite.getGlobalBounds().height / 2 - 200));
        m_GameEntityManager->DrawSelectedPlayerSigningBonus(MainWindow, sf::Vector2f(MenuSprite.getGlobalBounds().width / 2 + 47, MenuSprite.getGlobalBounds().height / 2 + 110));
    }
    else if (ReleasePlayerPopUpMenu.GetDoDrawMenu())
    {
        ReleasePlayerPopUpMenu.DrawMenu(MainWindow);
        m_GameEntityManager->DrawSelectedPlayerCard(MainWindow, sf::Vector2f(MenuSprite.getGlobalBounds().width / 2 - 212, MenuSprite.getGlobalBounds().height / 2 - 200));
    }
    else if (EndOfSeasonPopUpMenu.GetDoDrawMenu())
    {
        EndOfSeasonPopUpMenu.DrawMenu(MainWindow);
        m_GameEntityManager->DrawEndOfSeasonInfo(MainWindow, sf::Vector2f(MenuSprite.getGlobalBounds().width / 2, MenuSprite.getGlobalBounds().height / 2));
    }
}


void MainMenu::UpdateMenu(const sf::Vector2f &MousePosition)
{
    SavedMousePosition = MousePosition;
    if (HirePlayerPopUpMenu.GetDoDrawMenu())
    {
        HirePlayerPopUpMenu.SavedMousePosition = this->SavedMousePosition;
        HirePlayerPopUpMenu.LookForButtonHovering();
        return;
    }
    else if (ReleasePlayerPopUpMenu.GetDoDrawMenu())
    {
        ReleasePlayerPopUpMenu.SavedMousePosition = this->SavedMousePosition;
        ReleasePlayerPopUpMenu.LookForButtonHovering();
        return;
    }
    else if (EndOfSeasonPopUpMenu.GetDoDrawMenu())
    {
        EndOfSeasonPopUpMenu.SavedMousePosition = this->SavedMousePosition;
        EndOfSeasonPopUpMenu.LookForButtonHovering();
        return;
    }

    LookForButtonHovering();

    m_GameEntityManager->CheckforUserTeamPlayerCardSelection(MousePosition);
    m_GameEntityManager->CheckForFreeAgentPLayerCardSelection(MousePosition);

}

void MainMenu::RightClickEvent()
{
    if (HirePlayerPopUpMenu.GetDoDrawMenu())
    {
        return;
    }
    else if (ReleasePlayerPopUpMenu.GetDoDrawMenu())
    {
        return;
    }
    else if (EndOfSeasonPopUpMenu.GetDoDrawMenu())
    {
        return;
    }
    ReleasePlayerPopUpMenu.SetDoDrawMenu(false);
    HirePlayerPopUpMenu.SetDoDrawMenu(false);
    m_GameEntityManager->CheckforUserTeamPlayerCardSelection(SavedMousePosition);
    if (m_GameEntityManager->SelectedPlayer)
    {
        ReleasePlayerPopUpMenu.SetDoDrawMenu(true);
        return;
    }

    m_GameEntityManager->CheckForFreeAgentPLayerCardSelection(SavedMousePosition);
    if (m_GameEntityManager->SelectedPlayer)
    {
        HirePlayerPopUpMenu.SetDoDrawMenu(true);
        return;
    }
}

void MainMenu::LeftClickEvent()
{
    if (HirePlayerPopUpMenu.GetDoDrawMenu())
    {
        HirePlayerPopUpMenu.LeftClickEvent();

        return;
    }
    else if (ReleasePlayerPopUpMenu.GetDoDrawMenu())
    {
        ReleasePlayerPopUpMenu.LeftClickEvent();
        return;
    }
    else if (EndOfSeasonPopUpMenu.GetDoDrawMenu())
    {
        EndOfSeasonPopUpMenu.LeftClickEvent();
        return;
    }

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
        }
        GrabbedPlayerCard->setPlayerCardHighlighted(false);
        SwapingPlayer->setPlayerCardHighlighted(false);
        GrabbedPlayerCard = nullptr;
        m_GameEntityManager->GetUserTeam()->SetPlayerPositions();

    }
    else // if no player is grabbed then check to see if one has been selected
    {
        GrabbedPlayerCard = m_GameEntityManager->CheckforUserTeamPlayerCardSelection(SavedMousePosition);
    }
}

void MainMenu::HirePlayer()
{
    HirePlayerPopUpMenu.SetDoDrawMenu(false);
    ReleasePlayerPopUpMenu.SetDoDrawMenu(false);

    if (m_GameEntityManager->SelectedPlayer)
    {
        m_GameEntityManager->AddPlayerToUserTeam(m_GameEntityManager->SelectedPlayer);
        m_GameEntityManager->SelectedPlayer = nullptr;
    }

}

void MainMenu::ReleasePlayer()
{
    HirePlayerPopUpMenu.SetDoDrawMenu(false);
    ReleasePlayerPopUpMenu.SetDoDrawMenu(false);
    if (m_GameEntityManager->SelectedPlayer)
    {
        m_GameEntityManager->DeletePlayerOnUserTeam(m_GameEntityManager->SelectedPlayer);
        m_GameEntityManager->SelectedPlayer->SetTeamNumber(2);
        m_GameEntityManager->SelectedPlayer->UpdatePlayerCard();
        m_GameEntityManager->SelectedPlayer = nullptr;
    }
}

void MainMenu::Cancel()
{
    HirePlayerPopUpMenu.SetDoDrawMenu(false);
    ReleasePlayerPopUpMenu.SetDoDrawMenu(false);
    m_GameEntityManager->SelectedPlayer = nullptr;

}

void MainMenu::PopUpEndOfSeason()
{
    EndOfSeasonPopUpMenu.SetDoDrawMenu(true);
}

void MainMenu::SaveUserTeam()
{
    FileManager::SaveTeam(m_GameEntityManager->GetUserTeam(), "UserTeam");
    m_GameEntityManager->SaveFreeAgents();
    m_GameEntityManager->SaveGameState();

}

void MainMenu::AdvanceSeason()
{
    m_GameEntityManager->StartNewSeason();
    EndOfSeasonPopUpMenu.SetDoDrawMenu(false);

}
