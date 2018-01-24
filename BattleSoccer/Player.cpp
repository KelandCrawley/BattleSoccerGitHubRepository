#include "Player.h"



Player::SpecialAbility Player::GetAbilityByName(std::string &i_string)
{

    if (i_string == "BlazingSpeed" || i_string == "Blazing Speed")
    {
        return Player::SpecialAbility::BlazingSpeed;
    }
    else if (i_string == "Blink")
    {
        return Player::SpecialAbility::Blink;
    }
    else if (i_string == "VoidChain" || i_string == "Void Chain")
    {
        return Player::SpecialAbility::VoidChain;
    }
    else if (i_string == "CurveShot" || i_string == "Curve Shot")
    {
        return Player::SpecialAbility::CurveShot;
    }
    else if (i_string == "Power Shot" || i_string == "PowerShot")
    {
        return Player::SpecialAbility::PowerShot;
    }
    else if (i_string == "Frost Nova" || i_string == "FrostNova")
    {
        return Player::SpecialAbility::FrostNova;
    }

    return Player::SpecialAbility::None;
}

Player::PlayerPosition Player::GetPositionByName(std::string & i_string)
{
    //enum class PlayerPosition { CenterDefender, UpperDefender, LowerDefender, MidFielder, UpperWing, LowerWing, CenterAttacker, Goalie, ForwardGoalie };
    if (i_string == "CenterDefender" || i_string == "Center Defender" || i_string == "CD")
    {
        return Player::PlayerPosition::CenterDefender;
    }
    else if (i_string == "UpperDefender" || i_string == "Upper Defender" || i_string == "UD")
    {
        return Player::PlayerPosition::UpperDefender;
    }
    else if (i_string == "LowerDefender" || i_string == "Lower Defender" || i_string == "LD")
    {
        return Player::PlayerPosition::LowerDefender;
    }
    else if (i_string == "MidFielder" || i_string == "Mid Fielder" || i_string == "MF")
    {
        return Player::PlayerPosition::MidFielder;
    }
    else if (i_string == "UpperWing" || i_string == "Upper Wing" || i_string == "UW")
    {
        return Player::PlayerPosition::UpperWing;
    }
    else if (i_string == "LowerWing" || i_string == "Lower Wing" || i_string == "LW")
    {
        return Player::PlayerPosition::LowerWing;
    }
    else if (i_string == "Center Attacker" || i_string == "CenterAttacker" || i_string == "CA")
    {
        return Player::PlayerPosition::CenterAttacker;
    }
    else if (i_string == "Goalie" || i_string == "GL")
    {
        return Player::PlayerPosition::Goalie;
    }
    else if (i_string == "ForwardGoalie" || i_string == "Forward Goalie" || i_string == "FG")
    {
        return Player::PlayerPosition::ForwardGoalie;
    }

    return Player::PlayerPosition::ForwardGoalie;
}

Player::Player(std::string i_PlayerName, float i_PlayerStrength, float i_PlayerSpeed, float i_PlayerShotPower, float i_PlayerCatch, PlayerPosition i_Position, SpecialAbility i_SpecialAbility, bool isUserTeam)
    :TargetPositionRectangle(sf::Vector2f(25,25))
    , m_PlayerCard()
    ,PlayerName(i_PlayerName)
{


    PlayerSpeed = i_PlayerSpeed;
    PlayerShotPower = i_PlayerShotPower;
    PlayerCatch = i_PlayerCatch;
    PlayerStrength = i_PlayerStrength;
    MovingUp = false;
    MovingLeft = false;
    MovingDown = false;
    MovingRight = false;

    AbilityCooldownDuration = 35.0f;
    AbilityCooldownTimer = AbilityCooldownDuration;

    isPlayerControlled = false;

    TargetPositionRectangle.setOrigin(12.5, 12.5);


    TargetPositionRectangle.setFillColor(sf::Color::Transparent);

    CurrentCornerStrafe = 0;
    isBallBeingPassedToYou = false;

    m_PlayerPosition = i_Position;
    m_PlayerSpecialAbility = i_SpecialAbility;


    PlayerSpriteType = RNGClass::GetRandomInteger(0, 3);

    if (isUserTeam)
    {
        TeamNumber = 1;
        switch (PlayerSpriteType)
        {
        case 0:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Blue.png"));
            break;
        case 1:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Blue1.png"));
            break;
        case 2:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Blue2.png"));
            break;
        case 3:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Blue3.png"));
            break;

        }

    }
    else
    {
        TeamNumber = 2;
        switch (PlayerSpriteType)
        {
        case 0:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Red.png"));

            break;
        case 1:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Red1.png"));
            break;
        case 2:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Red2.png"));
            break;
        case 3:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Red3.png"));
            break;
        }

        PlayerSprite.setRotation(180);

    }
    PlayerSprite.setOrigin(15, 22);
    PlayerSprite.setTexture(PlayerTexure);

    PlayerIndicationRing.setRadius(PlayerSprite.getGlobalBounds().width - 2);
    PlayerIndicationRing.setFillColor(sf::Color::Transparent);
    PlayerIndicationRing.setOrigin(PlayerIndicationRing.getRadius(), PlayerIndicationRing.getRadius());
    PlayerIndicationRing.setOutlineColor(sf::Color::Blue);
    PlayerIndicationRing.setOutlineThickness(2);

    m_PlayerCard.Initialize(PlayerSpriteType, isUserTeam);
    m_PlayerCard.UpdatePlayerCardStats(PlayerShotPower, PlayerStrength, PlayerSpeed, PlayerCatch, PlayerName, GetPlayerPositionString(), m_PlayerSpecialAbility, CalculateWeeklySalary(), TeamNumber);
}

Player::Player()
    :TargetPositionRectangle(sf::Vector2f(25, 25))
    , m_PlayerCard()
{
    PlayerName = "Dummy";

    PlayerSpeed = 350;
    PlayerShotPower = 80;
    PlayerCatch = 60;
    PlayerStrength = 60;
    MovingUp = false;
    MovingLeft = false;
    MovingDown = false;
    MovingRight = false;

    AbilityCooldownDuration = 20.0f;
    AbilityCooldownTimer = AbilityCooldownDuration;


    isPlayerControlled = false;

    TargetPositionRectangle.setOrigin(12.5, 12.5);


    TargetPositionRectangle.setFillColor(sf::Color::Transparent);

    CurrentCornerStrafe = 0;
    isBallBeingPassedToYou = false;

    m_PlayerPosition = PlayerPosition::MidFielder;
    m_PlayerSpecialAbility = SpecialAbility::None;


    PlayerIndicationRing.setRadius(PlayerSprite.getGlobalBounds().width - 2);
    PlayerIndicationRing.setFillColor(sf::Color::Transparent);
    PlayerIndicationRing.setOrigin(PlayerIndicationRing.getRadius(), PlayerIndicationRing.getRadius());
    PlayerIndicationRing.setOutlineColor(sf::Color::Blue);
    PlayerIndicationRing.setOutlineThickness(2);

    m_PlayerCard.Initialize(1, false);
    m_PlayerCard.UpdatePlayerCardStats(PlayerShotPower, PlayerStrength, PlayerSpeed, PlayerCatch, PlayerName, GetPlayerPositionString(), m_PlayerSpecialAbility, CalculateWeeklySalary(), TeamNumber);

}

Player::~Player()
{
}

void Player::SetScale(const sf::Vector2f &i_Scale)
{
    PlayerIndicationRing.setScale(i_Scale);
    PlayerSprite.setScale(i_Scale);
}

void Player::InitializeForMatch(int i_TeamNumber, sf::Vector2f StartingPosition)
{
    //
    TeamNumber = i_TeamNumber;
    PlayerSprite.setPosition(StartingPosition);
    


    if (TeamNumber == 1)
    {
        TargetPositionRectangle.setOutlineColor(sf::Color::Blue);
        TargetPositionRectangle.setOutlineThickness(2);
    }

    if (TeamNumber == 2)
    {
        TargetPositionRectangle.setOutlineColor(sf::Color::Red);
        TargetPositionRectangle.setOutlineThickness(2);
    }
}

void Player::DrawPlayerSprite(sf::RenderWindow & MainWindow)
{
    if (isPlayerControlled)
    {
        PlayerIndicationRing.setPosition(PlayerSprite.getPosition());
        MainWindow.draw(PlayerIndicationRing);
    }

   // MainWindow.draw(TargetPositionRectangle);

    MainWindow.draw(PlayerSprite);

    // if ability is avaliabe, draw a mini icon in the bottom left corner of the player sprite
    if (CheckAbilityCooldown())
    {
         m_PlayerCard.DrawPlayerAbilityMiniIcon(MainWindow, sf::Vector2f(PlayerSprite.getPosition().x - PlayerSprite.getOrigin().x, PlayerSprite.getPosition().y + PlayerSprite.getOrigin().y));
    }
}

void Player::TickBuffs(float DeltaTimeAsSeconds)
{
    for (auto i = 0; i < BuffsContainer.size(); i++)
    {
        if (BuffsContainer.at(i).TickBuff(DeltaTimeAsSeconds))
        {
            BuffsContainer.erase(BuffsContainer.erase(BuffsContainer.begin() + i, BuffsContainer.begin() + i));
        }
    }
}

void Player::MovePlayer(float DeltaTimeAsSeconds)
{
    if (GetIsStunned())
    {
        return;
    }
    if (isPlayerControlled == false)
    {
        AiMove(DeltaTimeAsSeconds);
        return;
    }

    sf::Vector2f MoveVector = sf::Vector2f(0, 0);
    if (MovingUp == true)
    {
        MoveVector.y -= 1.0f;
    }
    if (MovingDown == true)
    {
        MoveVector.y += 1.0f;
    }
    if (MovingRight == true)
    {
        MoveVector.x += 1.0f;
        if (MovingUp || MovingDown)
        {
            MoveVector.y *= (sqrt(2) / 2);
            MoveVector.x = (sqrt(2) / 2);
        }
        
    }
    if (MovingLeft == true)
    {
        MoveVector.x -= 1.0f;
        if (MovingUp || MovingDown)
        {
            MoveVector.y *= (sqrt(2) / 2);
            MoveVector.x = -(sqrt(2) / 2);
        }
    }

    float MoveAmount = (DeltaTimeAsSeconds * PlayerSpeed) / 2 * GetMoveModification();
    PlayerSprite.move(MoveVector.x * MoveAmount * PlayerSprite.getScale().x, MoveVector.y * MoveAmount * PlayerSprite.getScale().y);
}

float Player::GetMoveModification()
{
    float LargetSPeedDecrase = 0.0f;
    float LargestSpeedIncrease = 0.0f;
    float TotalSpeedChange= 0.0f;

    for (auto i = 0; i < BuffsContainer.size(); i++)
    {
        if (BuffsContainer.at(i).GetSpeedChangePercentage() > LargestSpeedIncrease)
        {
            LargestSpeedIncrease = BuffsContainer.at(i).GetSpeedChangePercentage();

        }
        else if (BuffsContainer.at(i).GetSpeedChangePercentage() < LargetSPeedDecrase)
        {
            LargetSPeedDecrase = BuffsContainer.at(i).GetSpeedChangePercentage();
        }
    }
    TotalSpeedChange = 1.0f + (LargestSpeedIncrease + LargetSPeedDecrase);
    return TotalSpeedChange;
}

bool Player::GetIsStunned()
{
    for (auto i = 0; i < BuffsContainer.size(); i++)
    {
        if (BuffsContainer.at(i).GetDoesStun())
        {
            return true;
        }
    }
    return false;
}

bool Player::GetIsSlowed()
{
    if (GetMoveModification() < 1.0f)
    {
        return true;
    }
    return false;
}

void Player::TickCooldowns(float DeltaTimeAsSeconds)
{
    AbilityCooldownTimer += DeltaTimeAsSeconds;
    if (AbilityCooldownTimer >= AbilityCooldownDuration)
    {
        AbilityCooldownTimer = AbilityCooldownDuration;
    }

}

void Player::AiMove(float DeltaTimeAsSeconds)
{
    sf::Vector2f StrafePosition;
     SetRotation(TargetPositionRectangle.getPosition());
    if (!TargetPositionRectangle.getGlobalBounds().intersects(PlayerSprite.getGlobalBounds()))
    {
        float MoveAmount = (DeltaTimeAsSeconds * PlayerSpeed) / 2 * GetMoveModification();
        MoveVector = sf::Vector2f(cos(PlayerSprite.getRotation() * 3.1415 / 180.0), sin(PlayerSprite.getRotation() * 3.1415 / 180.0));
        PlayerSprite.move(sf::Vector2f(cos(PlayerSprite.getRotation() * 3.1415 / 180.0) * MoveAmount * PlayerSprite.getScale().x, sin(PlayerSprite.getRotation() * 3.1415 / 180.0) * MoveAmount * PlayerSprite.getScale().y));
        return;
    }
    else
    {
        //strafe
        switch (CurrentCornerStrafe)
        { 
        case 0:
            // top left corner
            StrafePosition.x = TargetPositionRectangle.getPosition().x - (TargetPositionRectangle.getSize().x / 2);
            StrafePosition.y = TargetPositionRectangle.getPosition().y - (TargetPositionRectangle.getSize().y / 2);
            break;
        case 1:
            // top right corner
            StrafePosition.x = TargetPositionRectangle.getPosition().x + (TargetPositionRectangle.getSize().x / 2);
            StrafePosition.y = TargetPositionRectangle.getPosition().y - (TargetPositionRectangle.getSize().y / 2);
            break;
        case 2:
            // top left corner
            StrafePosition.x = TargetPositionRectangle.getPosition().x - (TargetPositionRectangle.getSize().x / 2);
            StrafePosition.y = TargetPositionRectangle.getPosition().y + (TargetPositionRectangle.getSize().y / 2);
            break;
        case 3:
            // bottom left corner
            StrafePosition.x = TargetPositionRectangle.getPosition().x + (TargetPositionRectangle.getSize().x / 2);
            StrafePosition.y = TargetPositionRectangle.getPosition().y + (TargetPositionRectangle.getSize().y / 2);
            break;
        default:
            // top left corner
            StrafePosition.x = TargetPositionRectangle.getPosition().x - (TargetPositionRectangle.getSize().x / 2);
            StrafePosition.y = TargetPositionRectangle.getPosition().y - (TargetPositionRectangle.getSize().y / 2);
            break;

        }

        if (PlayerSprite.getGlobalBounds().contains(StrafePosition))
        {
           CurrentCornerStrafe =  RNGClass::GetRandomInteger(0, 3);
        }
        else
        {
            SetRotation(sf::Vector2f(StrafePosition));
            float MoveAmount = (DeltaTimeAsSeconds * PlayerSpeed) / 2 * GetMoveModification();
            MoveVector = sf::Vector2f(cos(PlayerSprite.getRotation() * 3.1415 / 180.0), sin(PlayerSprite.getRotation() * 3.1415 / 180.0));
            PlayerSprite.move(sf::Vector2f(cos(PlayerSprite.getRotation() * 3.1415 / 180.0) * MoveAmount * PlayerSprite.getScale().x, sin(PlayerSprite.getRotation() * 3.1415 / 180.0) * MoveAmount * PlayerSprite.getScale().y));
        }
    }
}

float Player::ScaleX(float xInput)
{
    return (xInput * PlayerSprite.getScale().x);
}

float Player::ScaleY(float yInput)
{
    return (yInput * PlayerSprite.getScale().y);
}

sf::Vector2f Player::Scale(float xInput, float yInput)
{
    return sf::Vector2f((xInput * PlayerSprite.getScale().x), (yInput, PlayerSprite.getScale().y));
}

sf::Vector2f Player::Scale(const sf::Vector2f &i_Input)
{
    return sf::Vector2f((i_Input.x * PlayerSprite.getScale().x), (i_Input.y, PlayerSprite.getScale().y));;
}

void Player::DrawPlayerCard(sf::RenderWindow &MainWindow, const sf::Vector2f &CardPosition, const sf::Vector2f &i_Scale)
{
    m_PlayerCard.SetPlayerCardScale(i_Scale);
    m_PlayerCard.DrawPlayerCard(MainWindow, CardPosition);
}

bool Player::CheckForPlayerCardSelection(const sf::Vector2f &i_Location)
{
    if (m_PlayerCard.PlayerCardSprite.getGlobalBounds().contains(i_Location))
    {
        return true;
    }
    return false;
}

void Player::setPlayerCardHighlighted(bool i_isHighlighted)
{
    m_PlayerCard.SetIsHighlighted(i_isHighlighted);
}

//changes palyer to another team, #param isUserTeam boolean
void Player::ChangeToDifferentColor(bool isUserTeam)
{
    if (isUserTeam)
    {
        TeamNumber = 1;
        switch (PlayerSpriteType)
        {
        case 0:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Blue.png"));
            break;
        case 1:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Blue1.png"));
            break;
        case 2:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Blue2.png"));
            break;
        case 3:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Blue3.png"));
            break;

        }
        PlayerSprite.setRotation(0);
    }
    else
    {
        TeamNumber = 2;
        switch (PlayerSpriteType)
        {
        case 0:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Red.png"));

            break;
        case 1:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Red1.png"));
            break;
        case 2:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Red2.png"));
            break;
        case 3:
            if (!PlayerTexure.loadFromFile("Data/Players/Player_Red3.png"));
            break;
        }

        PlayerSprite.setRotation(180);

    }

    PlayerSprite.setTexture(PlayerTexure);
}

void Player::MarkAPlayer(const sf::Vector2f &i_PlayerPosition)
{
    TargetPositionRectangle.setSize(sf::Vector2f(50, 50));
    TargetPositionRectangle.setOrigin(TargetPositionRectangle.getGlobalBounds().width / 2, TargetPositionRectangle.getGlobalBounds().height / 2);
    TargetPositionRectangle.setPosition(i_PlayerPosition);

}

void Player::SetTargetLocation(const sf::Vector2f &i_location, const sf::Vector2f &TargetSize)
{
    TargetPositionRectangle.setSize(TargetSize);
    TargetPositionRectangle.setOrigin(TargetPositionRectangle.getGlobalBounds().width / 2, TargetPositionRectangle.getGlobalBounds().height / 2);
    TargetPositionRectangle.setPosition(i_location);
}

sf::RectangleShape Player::GetInDefensivePosition(const sf::RectangleShape &DefensiveThird, const sf::RectangleShape &MiddleThird)
{
    switch (m_PlayerPosition)
    {
    case Player::PlayerPosition::CenterDefender:
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(DefensiveThird.getPosition().x - ScaleX(125), DefensiveThird.getPosition().y), sf::Vector2f(75, 125));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(DefensiveThird.getPosition().x + ScaleX(125), DefensiveThird.getPosition().y), sf::Vector2f(75, 125));
        }
        break;
    case Player::PlayerPosition::LowerDefender:
        // go to lower defensive third
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(DefensiveThird.getPosition().x - ScaleX(125), DefensiveThird.getPosition().y + ScaleY(150)), sf::Vector2f(75, 125));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(DefensiveThird.getPosition().x + ScaleX(125), DefensiveThird.getPosition().y + ScaleY(150)), sf::Vector2f(75, 125));
        }
        break;
    case Player::PlayerPosition::UpperDefender:
        // go to Upper defensive third
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(DefensiveThird.getPosition().x - ScaleX(125), DefensiveThird.getPosition().y - ScaleY(150)), sf::Vector2f(75, 125));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(DefensiveThird.getPosition().x + ScaleX(125), DefensiveThird.getPosition().y - ScaleY(150)), sf::Vector2f(75, 125));
        }
        break;
    case Player::PlayerPosition::MidFielder:
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(DefensiveThird.getPosition().x + ScaleX(50) , MiddleThird.getPosition().y), sf::Vector2f(125, 150));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(DefensiveThird.getPosition().x - ScaleX(50), MiddleThird.getPosition().y), sf::Vector2f(125, 150));
        }

        break;
    case Player::PlayerPosition::LowerWing:
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x - ScaleX(300), MiddleThird.getPosition().y + ScaleY(150)), sf::Vector2f(100, 100));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x + ScaleX(300), MiddleThird.getPosition().y + ScaleY(150)), sf::Vector2f(100, 100));
        }
        break;
    case Player::PlayerPosition::UpperWing:
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x - ScaleX(300), MiddleThird.getPosition().y - ScaleY(150)), sf::Vector2f(100, 100));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x + ScaleX(300), MiddleThird.getPosition().y - ScaleY(150)), sf::Vector2f(100, 100));
        }
        break;
    case Player::PlayerPosition::CenterAttacker:
        SetTargetLocation(sf::Vector2f((MiddleThird.getPosition().x + DefensiveThird.getPosition().x) / 2, MiddleThird.getPosition().y), sf::Vector2f(100, 100));
        break;
    default:
        HoldPosition();
        break;
    }

    return TargetPositionRectangle;
}

void Player::AdvanceBall(const sf::RectangleShape &MiddleThird, const sf::RectangleShape &AttackingThird)
{
    GetInOffenseivePosition(MiddleThird);
    return;

    //testing above method to allow playesr to advance ball towards thier normal offensive position

    if (TeamNumber == 1)
    {
        // offensivly moves right
        if (AttackingThird.getGlobalBounds().contains(GetPosition()))
        {
            //move towards middle of attacking third
            SetTargetLocation(AttackingThird.getPosition(), sf::Vector2f(50, 200));

        }
        else
        {
            // just move towards offensive side
            SetTargetLocation(sf::Vector2f(PlayerSprite.getPosition().x + ScaleX(200), PlayerSprite.getPosition().y), sf::Vector2f(100, 100));
        }

    }
    else
    {
        // offensivly moves left
        if (AttackingThird.getGlobalBounds().contains(GetPosition()))
        {
            //move towards middle of attacking third
            SetTargetLocation(AttackingThird.getPosition(), sf::Vector2f(50, 200));

        }
        else
        {
            // just move towards offensive side
            SetTargetLocation(sf::Vector2f(PlayerSprite.getPosition().x - ScaleX(200), PlayerSprite.getPosition().y), sf::Vector2f(100, 100));
        }
    }
}

void Player::GetInOffenseivePosition(const sf::RectangleShape &MiddleThird)
{
    sf::RectangleShape OffensiveThird;
    if (TeamNumber == 1)
    {
        OffensiveThird.setPosition(sf::Vector2f(MiddleThird.getPosition().x + MiddleThird.getGlobalBounds().width, MiddleThird.getPosition().y));
    }
    else
    {
        OffensiveThird.setPosition(sf::Vector2f(MiddleThird.getPosition().x - MiddleThird.getGlobalBounds().width, MiddleThird.getPosition().y));
    }


    sf::RectangleShape AttackingThird(MiddleThird);
    if (TeamNumber == 1)
    {
        // attacks right
        AttackingThird.setPosition(MiddleThird.getPosition().x + MiddleThird.getGlobalBounds().width, MiddleThird.getPosition().y);
    }
    else
    {
        //atacks left
        AttackingThird.setPosition(MiddleThird.getPosition().x - MiddleThird.getGlobalBounds().width, MiddleThird.getPosition().y);

    }
    switch (m_PlayerPosition)
    {
    case Player::PlayerPosition::Goalie:
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x - ScaleX(725), MiddleThird.getPosition().y), sf::Vector2f(50, 125));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x + ScaleX(725), MiddleThird.getPosition().y), sf::Vector2f(50, 125));

        }
        break;
    case Player::PlayerPosition::ForwardGoalie:
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x - ScaleX(450), MiddleThird.getPosition().y), sf::Vector2f(50, 125));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x + ScaleX(450), MiddleThird.getPosition().y), sf::Vector2f(50, 125));

        }
        break;
        // go to middle third
    case Player::PlayerPosition::CenterDefender:
        if (TeamNumber == 1)
        {
        SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x + ScaleX(200), MiddleThird.getPosition().y), sf::Vector2f(125, 100));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x - ScaleX(200), MiddleThird.getPosition().y), sf::Vector2f(125, 100));

        }
        break;
    case Player::PlayerPosition::LowerDefender:
        // go to lower MiddleThird third
        if (TeamNumber == 1)
        {
             SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x + ScaleX(200), MiddleThird.getPosition().y + ScaleY(175)), sf::Vector2f(125, 100));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x - ScaleX(200), MiddleThird.getPosition().y + ScaleY(175)), sf::Vector2f(125, 100));
        }
        break;
    case Player::PlayerPosition::UpperDefender:
        // go to Upper MiddleThird third
        if (TeamNumber == 1)
        {
        SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x + ScaleX(200), MiddleThird.getPosition().y - ScaleY(175)), sf::Vector2f(125, 100));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x - ScaleX(200), MiddleThird.getPosition().y - ScaleY(175)), sf::Vector2f(125, 100));

        }
        break;
    case Player::PlayerPosition::MidFielder:
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(AttackingThird.getPosition().x - ScaleX(150), MiddleThird.getPosition().y), sf::Vector2f(75, 50));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(AttackingThird.getPosition().x + ScaleX(150), MiddleThird.getPosition().y), sf::Vector2f(75, 50));

        }
        break;
    case Player::PlayerPosition::LowerWing:
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(AttackingThird.getPosition().x + ScaleX(50), MiddleThird.getPosition().y + ScaleY(180)), sf::Vector2f(125, 50));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(AttackingThird.getPosition().x - ScaleX(50), MiddleThird.getPosition().y + ScaleY(180)), sf::Vector2f(125, 50));

        }
        break;
    case Player::PlayerPosition::UpperWing:
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(AttackingThird.getPosition().x + ScaleX(50), MiddleThird.getPosition().y - ScaleY(180)), sf::Vector2f(125, 50));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(AttackingThird.getPosition().x - ScaleX(50), MiddleThird.getPosition().y - ScaleY(180)), sf::Vector2f(125, 50));

        }
        break;
    case Player::PlayerPosition::CenterAttacker:
        // go to Attacking third 
        SetTargetLocation(AttackingThird.getPosition(), sf::Vector2f(50, 175));
        break;
    default:
        HoldPosition();
        break;
    }

}

void Player::GetInMidFieldPosition(const sf::RectangleShape &MiddleThird)
{
    switch (m_PlayerPosition)
    {
    case Player::PlayerPosition::CenterDefender:
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x - ScaleX(350), MiddleThird.getPosition().y), sf::Vector2f(150, 125));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x + ScaleX(350), MiddleThird.getPosition().y), sf::Vector2f(150, 125));

        }

        break;
    case Player::PlayerPosition::LowerDefender:
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x - ScaleX(250), MiddleThird.getPosition().y + ScaleY(150)), sf::Vector2f(125, 125));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x + ScaleX(250), MiddleThird.getPosition().y + ScaleY(150)), sf::Vector2f(125, 125));
        }

        break;
    case Player::PlayerPosition::UpperDefender:
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x - ScaleX(250), MiddleThird.getPosition().y - ScaleY(150)), sf::Vector2f(125, 125));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x + ScaleX(250), MiddleThird.getPosition().y - ScaleY(150)), sf::Vector2f(125, 125));
        }

        break;
    case Player::PlayerPosition::MidFielder:
        // go to middle of defensive and middle thrid
        SetTargetLocation(sf::Vector2f(MiddleThird.getPosition()), sf::Vector2f(150, 150));
        break;
    case Player::PlayerPosition::LowerWing:
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x - ScaleX(75), MiddleThird.getPosition().y + ScaleY(150)), sf::Vector2f(150, 150));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x + ScaleX(75), MiddleThird.getPosition().y + ScaleY(150)), sf::Vector2f(150, 150));
        }

        break;
    case Player::PlayerPosition::UpperWing:
        if (TeamNumber == 1)
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x - ScaleX(75), MiddleThird.getPosition().y - ScaleY(150)), sf::Vector2f(150, 150));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x + ScaleX(75), MiddleThird.getPosition().y - ScaleY(150)), sf::Vector2f(150, 150));

        }
        break;
    case Player::PlayerPosition::CenterAttacker:
        if (TeamNumber == 1)
        {
        SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x  + ScaleX(50), MiddleThird.getPosition().y), sf::Vector2f(150, 150));
        }
        else
        {
            SetTargetLocation(sf::Vector2f(MiddleThird.getPosition().x - ScaleX(50), MiddleThird.getPosition().y), sf::Vector2f(150, 150));

        }
        break;
    default:
        HoldPosition();
        break;
    }
}

void Player::ResetPlayer(const sf::RectangleShape &DefensiveThird, const sf::RectangleShape &MiddleThird)
{
    isPlayerControlled = false;
    BuffsContainer.clear();
    CancelActions();
    MoveVector = sf::Vector2f(0, 0);
    isBallBeingPassedToYou = false;

    switch (m_PlayerPosition)
    {
    case Player::PlayerPosition::Goalie:
        if (TeamNumber == 1)
        {
            SetSpritePosition(sf::Vector2f(DefensiveThird.getPosition().x - (DefensiveThird.getSize().x * 0.40), DefensiveThird.getPosition().y));
        }
        else
        {
            SetSpritePosition(sf::Vector2f(DefensiveThird.getPosition().x + (DefensiveThird.getSize().x * 0.40), DefensiveThird.getPosition().y));
        }
        break;
    case Player::PlayerPosition::ForwardGoalie:
        if (TeamNumber == 1)
        {
            SetSpritePosition(sf::Vector2f(DefensiveThird.getPosition().x - (DefensiveThird.getSize().x * 0.33), DefensiveThird.getPosition().y));

        }
        else
        {
            SetSpritePosition(sf::Vector2f(DefensiveThird.getPosition().x + (DefensiveThird.getSize().x * 0.33), DefensiveThird.getPosition().y));
        }
        break;
    case Player::PlayerPosition::CenterDefender:
        // go to lower defensive third
        SetSpritePosition(sf::Vector2f(DefensiveThird.getPosition().x, DefensiveThird.getPosition().y));
        break;
    case Player::PlayerPosition::LowerDefender:
        // go to lower defensive third
        SetSpritePosition(sf::Vector2f(DefensiveThird.getPosition().x, DefensiveThird.getPosition().y + ScaleY(150)));
        break;
    case Player::PlayerPosition::UpperDefender:
        // go to Upper defensive third
        SetSpritePosition(sf::Vector2f(DefensiveThird.getPosition().x, DefensiveThird.getPosition().y - ScaleY(150)));
        break;
    case Player::PlayerPosition::MidFielder:
        if (TeamNumber == 1)
        {
            SetSpritePosition(sf::Vector2f((MiddleThird.getPosition().x + DefensiveThird.getPosition().x) / 2 - ScaleX(40), MiddleThird.getPosition().y));

        }
        else
        {
            SetSpritePosition(sf::Vector2f((MiddleThird.getPosition().x + DefensiveThird.getPosition().x) / 2 + ScaleX(40), MiddleThird.getPosition().y));
        }
        break;
    case Player::PlayerPosition::LowerWing:
        if (TeamNumber == 1)
        {
            SetSpritePosition(sf::Vector2f((MiddleThird.getPosition().x + DefensiveThird.getPosition().x) / 2 + ScaleX(40), MiddleThird.getPosition().y + ScaleY(150)));
        }
        else
        {
            SetSpritePosition(sf::Vector2f((MiddleThird.getPosition().x + DefensiveThird.getPosition().x) / 2 - ScaleX(40), MiddleThird.getPosition().y + ScaleY(150)));
        }
        break;
    case Player::PlayerPosition::UpperWing:
        if (TeamNumber == 1)
        {
            SetSpritePosition(sf::Vector2f((MiddleThird.getPosition().x + DefensiveThird.getPosition().x) / 2 + ScaleX(40), MiddleThird.getPosition().y - ScaleY(150)));
        }
        else
        {
            SetSpritePosition(sf::Vector2f((MiddleThird.getPosition().x + DefensiveThird.getPosition().x) / 2 - ScaleX(40), MiddleThird.getPosition().y - ScaleY(150)));
        }
        break;
    case Player::PlayerPosition::CenterAttacker:
        // go to middle third
        if (TeamNumber == 1)
        {
            SetSpritePosition(sf::Vector2f(MiddleThird.getPosition().x - (MiddleThird.getSize().x * 0.3), MiddleThird.getPosition().y));
        }
        else
        {
            SetSpritePosition(sf::Vector2f(MiddleThird.getPosition().x + (MiddleThird.getSize().x * 0.3), MiddleThird.getPosition().y));
        }

        break;

    }
}

void Player::ResetCooldowns()
{
    AbilityCooldownTimer = AbilityCooldownDuration;
}

void Player::UpdatePlayer(float DeltaTimeAsSeconds)
{
    TickBuffs(DeltaTimeAsSeconds);
    TickCooldowns(DeltaTimeAsSeconds);
    MovePlayer(DeltaTimeAsSeconds);
}

void Player::CancelActions()
{
    MovingUp = false;
    MovingLeft = false;
    MovingDown = false;
    MovingRight = false;

}

float Player::GetPlayerStrength()
{
    return PlayerStrength;
}

float Player::GetPlayerSpeed()
{
    return PlayerSpeed;
}

float Player::GetPlayerCatch()
{
    return PlayerCatch;
}

float Player::GetPlayerShotPower()
{
    return PlayerShotPower;
}

std::string Player::GetPlayerName()
{
    return PlayerName;
}

std::string Player::GetPlayerAbilityString()
{
    std::string AbilityString = "None";
    switch (m_PlayerSpecialAbility)
    {
    case Player::FrostNova:
        AbilityString = "FrostNova";
        break;
    case Player::VoidChain:
        AbilityString = "VoidChain";
        break;
    case Player::BlazingSpeed:
        AbilityString = "BlazingSpeed";
        break;
    case Player::Blink:
        AbilityString = "Blink";
        break;
    case Player::PowerShot:
        AbilityString = "PowerShot";
        break;
    case Player::CurveShot:
        AbilityString = "CurveShot";
        break;
    }
    return AbilityString;
}

void Player::SetRotation(const sf::Vector2f &FactingLocation)
{
    float xDistance = FactingLocation.x - GetPosition().x;
    float yDistance = FactingLocation.y - GetPosition().y;
    float TotalDistance = (sqrt(xDistance * xDistance + yDistance * yDistance));
    float Rotation = 0.0f;
    if (!(TotalDistance == 0))
    {
        Rotation = (acos(xDistance / TotalDistance) * 180.0 / 3.14159);
    }
    if (GetPosition().y > FactingLocation.y) // below target
    {
        Rotation = (360 - Rotation);
    }
    PlayerSprite.setRotation(Rotation);
}

void Player::SetRotation(float i_Rotation)
{
    PlayerSprite.setRotation(i_Rotation);
}

float Player::GetRotaion()
{
    return PlayerSprite.getRotation();
}

sf::FloatRect Player::GetGlobalBounds()
{
    return PlayerSprite.getGlobalBounds();
}

sf::Vector2f Player::GetPosition()
{
    return PlayerSprite.getPosition();
}

float Player::GetPredictionLocationX(float TotalDistance, float BallVelocity)
{
    //non-calculus approximation
    float BallTravelTime = TotalDistance / BallVelocity;
    float PlayerXVelocity = MoveVector.x * PlayerSpeed / 2;

    return (GetPosition().x + (PlayerXVelocity * BallTravelTime));
}

float Player::GetPredictionLocationY(float TotalDistance, float BallVelocity)
{
    //non-calculus approximation
    float BallTravelTime = TotalDistance / BallVelocity;
    float PlayerYVelocity = MoveVector.y * PlayerSpeed / 2;

    return (GetPosition().y + (PlayerYVelocity * BallTravelTime));
}

void Player::SetSpritePosition(const sf::Vector2f &i_Position)
{

    PlayerSprite.setPosition(i_Position);
}

Player::SpecialAbility Player::GetPlayerSpeicalAbility()
{
    return m_PlayerSpecialAbility;
}

Player::PlayerPosition Player::GetPlayerPosition()
{
    return m_PlayerPosition;
}

std::string Player::GetPlayerPositionString()
{
    switch (m_PlayerPosition)
    {
    case Player::PlayerPosition::Goalie:
        return "GL";
        break;
    case Player::PlayerPosition::ForwardGoalie:
        return "FG";
        break;
    case Player::PlayerPosition::LowerDefender:
        return "LD";
        break;
    case Player::PlayerPosition::CenterDefender:
        return "CD";
        break;
    case Player::PlayerPosition::UpperDefender:
        return "UD";
        break;
    case Player::PlayerPosition::MidFielder:
        return "MF";
        break;
    case Player::PlayerPosition::UpperWing:
        return "UW";
        break;
    case Player::PlayerPosition::LowerWing:
        return "LW";
        break;
    case Player::PlayerPosition::CenterAttacker:
        return "CA";
        break;
    }
    return "";
}

void Player::SetPlayerPosition(Player::PlayerPosition i_Position)
{
    m_PlayerPosition = i_Position;
    m_PlayerCard.UpdatePlayerCardStats(PlayerShotPower, PlayerStrength, PlayerSpeed, PlayerCatch, PlayerName, GetPlayerPositionString(), m_PlayerSpecialAbility, CalculateWeeklySalary(), TeamNumber);
}

void Player::AddBuff(float i_Duration, float i_SpeedChangePercentage, bool i_DoesStun)
{
    AddBuff(Buff(i_Duration, i_SpeedChangePercentage, i_DoesStun));
}

void Player::AddBuff(Buff i_Buff)
{
    if (BuffsContainer.empty())
    {
        BuffsContainer.push_back(i_Buff);
        return;
    }
    for (auto i = 0; i < BuffsContainer.size(); i++)
    {
        //only add buffs that are better then some other buff
        if (i_Buff.GetSpeedChangePercentage() > 0.0f && i_Buff.GetSpeedChangePercentage() > BuffsContainer.at(i).GetSpeedChangePercentage())
        {
            BuffsContainer.push_back(i_Buff);
            return;

        }
        else if (i_Buff.GetSpeedChangePercentage() < 0.0f && i_Buff.GetSpeedChangePercentage() < BuffsContainer.at(i).GetSpeedChangePercentage())
        {
            BuffsContainer.push_back(i_Buff);
            return;
        }
    }
}

bool Player::CheckAbilityCooldown()
{
    if (AbilityCooldownTimer >= AbilityCooldownDuration)
    {
        return true;
    }
    return false;
}


void Player::UseAbility()
{
    AbilityCooldownTimer = 0.0f;
}


int Player::GetPlayerID()
{
    return PlayerID;
}

void Player::SetPlayerID(int i_PlayerID)
{
    PlayerID = i_PlayerID;
}

int Player::GetTeamNumber()
{
    return TeamNumber;
}

void Player::SetTeamNumber(int i_TeamNumber)
{
    TeamNumber = i_TeamNumber;
}

void Player::SetIsPlayerControlled(bool i_isPlayerControlled)
{
    isPlayerControlled = i_isPlayerControlled;
    setPlayerCardHighlighted(i_isPlayerControlled);
}

void Player::UpdatePlayerCard()
{
    m_PlayerCard.UpdatePlayerCardStats(PlayerShotPower, PlayerStrength, PlayerSpeed, PlayerCatch, PlayerName, GetPlayerPositionString(), m_PlayerSpecialAbility, CalculateWeeklySalary(), TeamNumber);
}

void Player::HoldPosition()
{
    TargetPositionRectangle.setPosition(GetPosition());
}

int Player::CalculateWeeklySalary()
{
    int TotalWeeklySalary = 50;
    TotalWeeklySalary += (PlayerShotPower - 60) * 2;
    TotalWeeklySalary += (PlayerCatch - 60) * 2;
    TotalWeeklySalary += (PlayerStrength - 60) * 2;
    TotalWeeklySalary += ((PlayerSpeed - 275) / 5) * 2;
    TotalWeeklySalary = (TotalWeeklySalary - (TotalWeeklySalary % 5));

    return TotalWeeklySalary;
}

int Player::CalculateHireBonus()
{
    return CalculateWeeklySalary() * 5;
}