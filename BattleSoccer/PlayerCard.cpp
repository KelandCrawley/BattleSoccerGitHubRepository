#include "PlayerCard.h"
#include "Player.h"


PlayerCard::PlayerCard()
    :CardHighLight(sf::Vector2f(180,250))
{
}


PlayerCard::~PlayerCard()
{
}

void PlayerCard::Initialize(int i_PlayerSpriteType, bool isUserTeam)
{
    PlayerSpriteType = i_PlayerSpriteType;
    if (isUserTeam)
    {
        CurrentTeamNumber = 1;
        switch (PlayerSpriteType)
        {
        case 0:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Blue.png"));
            break;
        case 1:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Blue1.png"));
            break;
        case 2:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Blue2.png"));
            break;
        case 3:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Blue3.png"));
            break;

        }

    }
    else
    {
        CurrentTeamNumber = 2;
        switch (PlayerSpriteType)
        {
        case 0:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Red.png"));

            break;
        case 1:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Red1.png"));
            break;
        case 2:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Red2.png"));
            break;
        case 3:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Red3.png"));
            break;
        }

        PlayerSprite.setRotation(180);

    }
    PlayerSprite.setOrigin(15, 22);
    PlayerSprite.setTexture(PlayerTexture);
    PlayerSprite.setRotation(90);

    if (!PlayerCardTexture.loadFromFile("Data/PlayerCard.png")) {};

    PlayerCardSprite.setTexture(PlayerCardTexture);

    if (!MainFont.loadFromFile("Data/ArialFont/arial.ttf"))
    {

    }
    for (auto i = 0; i < 6; i++)
    {
        CardText[i][0].setFillColor(sf::Color::Black);
        CardText[i][0].setFont(MainFont);
        CardText[i][0].setOrigin(sf::Vector2f(10, 2));
        CardText[i][0].setCharacterSize(24);

        CardText[i][1].setFillColor(sf::Color::White);
        CardText[i][1].setFont(MainFont);
        CardText[i][1].setOrigin(sf::Vector2f(10, 2));
        CardText[i][1].setCharacterSize(24);
    }
    CardText[0][0].setCharacterSize(32);


    CardText[1][0].setString("Sht");
    CardText[2][0].setString("Cth");
    CardText[3][0].setString("Str");
    CardText[4][0].setString("Spd");
    CardText[5][0].setString("$");
    CardText[5][0].setOutlineColor(sf::Color::Black);
    CardText[5][0].setOutlineThickness(2);
    CardText[5][0].setFillColor(sf::Color::Green);

    CardHighLight.setSize(sf::Vector2f(PlayerCardSprite.getGlobalBounds().width - 4, PlayerCardSprite.getGlobalBounds().height - 4));
    CardHighLight.setOutlineColor(sf::Color::Transparent);
    CardHighLight.setOutlineThickness(2);
    CardHighLight.setFillColor(sf::Color::Transparent);


}

void PlayerCard::DrawPlayerCard(sf::RenderWindow& MainWindow, const sf::Vector2f &CardPosition)
{
    float xScale = PlayerCardSprite.getScale().x;
    float yScale = PlayerCardSprite.getScale().y;

    PlayerCardSprite.setPosition(CardPosition);
    CardHighLight.setPosition(sf::Vector2f(CardPosition.x + 2, CardPosition.y + 2));

    CardText[0][0].setPosition(sf::Vector2f(CardPosition.x + (20 * xScale), CardPosition.y + (5 * yScale)));
    CardText[1][0].setPosition(sf::Vector2f(CardPosition.x + (15 * xScale), CardPosition.y + (120 * yScale)));
    CardText[2][0].setPosition(sf::Vector2f(CardPosition.x + (90 * xScale), CardPosition.y + (120 * yScale)));
    CardText[3][0].setPosition(sf::Vector2f(CardPosition.x + (15 * xScale), CardPosition.y + (180 * yScale)));
    CardText[4][0].setPosition(sf::Vector2f(CardPosition.x + (90 * xScale), CardPosition.y + (180 * yScale)));
    CardText[5][0].setPosition(sf::Vector2f(CardPosition.x + (30 * xScale), CardPosition.y + (220 * yScale)));

    CardText[0][1].setPosition(sf::Vector2f(CardPosition.x + (145 * xScale), CardText[0][0].getPosition().y + 5));
    CardText[1][1].setPosition(sf::Vector2f(CardText[1][0].getPosition().x + CardText[1][0].getGlobalBounds().width + 5, CardText[1][0].getPosition().y));
    CardText[2][1].setPosition(sf::Vector2f(CardText[2][0].getPosition().x + CardText[2][0].getGlobalBounds().width + 5, CardText[2][0].getPosition().y));
    CardText[3][1].setPosition(sf::Vector2f(CardText[3][0].getPosition().x + CardText[3][0].getGlobalBounds().width + 5, CardText[3][0].getPosition().y));
    CardText[4][1].setPosition(sf::Vector2f(CardText[4][0].getPosition().x + CardText[4][0].getGlobalBounds().width + 5, CardText[4][0].getPosition().y));
    CardText[5][1].setPosition(sf::Vector2f(CardText[5][0].getPosition().x + CardText[5][0].getGlobalBounds().width + 5, CardText[5][0].getPosition().y));

    PlayerSprite.setPosition(CardPosition.x + (50 * xScale), CardPosition.y + (72 * yScale));
    PlayerAbilityIconSprite.setPosition(CardPosition.x + (115 * xScale), CardPosition.y + (50 * yScale));

    MainWindow.draw(PlayerCardSprite);
    for (auto i = 0; i < 6; i++)
    {
        MainWindow.draw(CardText[i][0]);
        MainWindow.draw(CardText[i][1]);

    }
    MainWindow.draw(PlayerAbilityIconSprite);
    MainWindow.draw(PlayerSprite);
    MainWindow.draw(CardHighLight);

}

void PlayerCard::DrawPlayerAbilityMiniIcon(sf::RenderWindow & MainWindow, const sf::Vector2f &i_Position)
{
    PlayerAbilityMiniIconSprite.setPosition(i_Position);
    MainWindow.draw(PlayerAbilityMiniIconSprite);
}

void PlayerCard::SetIsHighlighted(bool i_isHighlighted)
{
    
    if (i_isHighlighted)
    {
        CardHighLight.setOutlineColor(sf::Color::Blue);
    }
    else
    {
        CardHighLight.setOutlineColor(sf::Color::Transparent);
    }
    
}

void PlayerCard::SetPlayerCardScale(const sf::Vector2f &i_Scale)
{
    for (auto i = 0; i < 6; i++)
    {
        CardText[i][0].setScale(i_Scale);
        CardText[i][1].setScale(i_Scale);
    }
    PlayerCardSprite.setScale(i_Scale);
    PlayerAbilityIconSprite.setScale(i_Scale);
    PlayerSprite.setScale(i_Scale);
    CardHighLight.setScale(i_Scale);
}

void PlayerCard::UpdatePlayerSprite()
{
    if (CurrentTeamNumber)
    {
        switch (PlayerSpriteType)
        {
        case 0:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Blue.png"));
            break;
        case 1:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Blue1.png"));
            break;
        case 2:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Blue2.png"));
            break;
        case 3:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Blue3.png"));
            break;

        }

    }
    else
    {
        switch (PlayerSpriteType)
        {
        case 0:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Red.png"));

            break;
        case 1:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Red1.png"));
            break;
        case 2:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Red2.png"));
            break;
        case 3:
            if (!PlayerTexture.loadFromFile("Data/Players/Player_Red3.png"));
            break;
        }

        PlayerSprite.setRotation(180);

    }
}

void PlayerCard::UpdatePlayerCardStats(int i_sht, int i_str, int i_spd, int i_cat, std::string i_PlayerName, std::string i_PlayerPosition, int i_SpecialAbility, int i_WeeklySalary, int i_TeamNumber)
{
    switch (i_SpecialAbility)
    {
    case Player::SpecialAbility::Blink:
        if (!PlayerAbilityIconTexture.loadFromFile("Data/AbilityIcons/BlinkSpellIcon.png")) {};
        break;
    case Player::SpecialAbility::BlazingSpeed:
        if (!PlayerAbilityIconTexture.loadFromFile("Data/AbilityIcons/BlazingSpeedIcon.png")) {};
        break;
    case Player::SpecialAbility::FrostNova:
        if (!PlayerAbilityIconTexture.loadFromFile("Data/AbilityIcons/FrostNovaIcon.png")) {};
        break;
    case Player::SpecialAbility::VoidChain:
        if (!PlayerAbilityIconTexture.loadFromFile("Data/AbilityIcons/VoidChainIcon.png")) {};
        break;
    case Player::SpecialAbility::CurveShot:
        if (!PlayerAbilityIconTexture.loadFromFile("Data/AbilityIcons/CurveShotIcon.png")) {};
        break;
    case Player::SpecialAbility::PowerShot:
        if (!PlayerAbilityIconTexture.loadFromFile("Data/AbilityIcons/PowerSHotIcon.png")) {};
        break;
    }
    PlayerAbilityIconSprite.setTexture(PlayerAbilityIconTexture);
    PlayerAbilityMiniIconSprite.setTexture(PlayerAbilityIconTexture);
    PlayerAbilityMiniIconSprite.setScale(sf::Vector2f(0.30f, 0.30f));

    CardText[0][0].setString(i_PlayerName);
    CardText[1][1].setString(std::to_string(i_sht));
    CardText[2][1].setString(std::to_string(i_cat));
    CardText[3][1].setString(std::to_string(i_str));
    CardText[4][1].setString(std::to_string(i_spd));


    CardText[5][0].setString("$" + std::to_string(i_WeeklySalary));
    CardText[5][1].setString("/ Week");

    CardText[0][1].setString(i_PlayerPosition);

    if (i_TeamNumber != CurrentTeamNumber)
    {
        CurrentTeamNumber = i_TeamNumber;
        UpdatePlayerSprite();
    }
}
