#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "RNGClass.h"

#include "Buff.h"
#include "PlayerCard.h"

class Player
    : public std::enable_shared_from_this<Player>

{
public:
    enum SpecialAbility {None = 0, Blink = 1, BlazingSpeed = 2, FrostNova = 3, VoidChain = 4, CurveShot = 5, PowerShot = 6};
    enum class PlayerPosition { CenterDefender, UpperDefender, LowerDefender, MidFielder, UpperWing, LowerWing, CenterAttacker, Goalie, ForwardGoalie };

    Player(std::string i_PlayerName, float i_PlayerStrength, float i_PlayerSpeed, float i_PlayerShotPower, float i_PlayerCatch, PlayerPosition i_Position, SpecialAbility i_SpecialAbility ,bool isUserTeam);
    Player();
    ~Player();

    //Returns a special ability based on a passed in string value
    //used for loading players from a saved file
    static SpecialAbility GetAbilityByName(std::string &i_string);

    //Returns a PlayerPosition based on a passed in string value
    //used for loading players from a saved file
    static PlayerPosition GetPositionByName(std::string &i_string);


    //Sets the scale of the player sprite and controlled indication ring
    void SetScale(const sf::Vector2f &i_Scale);

    //Initializes the player for the next match
    //@param i_TeamNumber sets a players teamnumber variable and changes the color of the target location rectangle to the correct color
    // Note: Target Location Rectangles are only used for debugging
    void InitializeForMatch(int i_TeamNumber, sf::Vector2f StartingPosition);

    //Draws the player sprite and any player visual information. Including ability icon and controlled player indicator
    void DrawPlayerSprite(sf::RenderWindow &MainWindow);

    //Updates the cooldown/buff timers and moves player based on time elapsed
    void UpdatePlayer(float DeltaTimeAsSeconds);

    // resets playres flags and abilities. also sets new ai strategy based on current position
    void ResetPlayer(const sf::RectangleShape &DefensiveThird, const sf::RectangleShape &MiddleThird);
    void ResetCooldowns();

    // cancels all current movement commands
    void CancelActions();

    bool MovingUp;
    bool MovingDown;
    bool MovingRight;
    bool MovingLeft;

    /////////////////getters////////////

    float GetPlayerStrength();
    float GetPlayerSpeed();
    float GetPlayerCatch();
    float GetPlayerShotPower();
    std::string GetPlayerName();
    std::string GetPlayerAbilityString();
    sf::FloatRect GetGlobalBounds();
    sf::Vector2f GetPosition();
    float GetPredictionLocationX(float TotalDistance, float BallVelocity);
    float GetPredictionLocationY(float TotalDistance, float BallVelocity);
    float GetRotaion();
    SpecialAbility GetPlayerSpeicalAbility();
    PlayerPosition GetPlayerPosition();
    std::string GetPlayerPositionString();
    bool GetIsStunned();
    bool GetIsSlowed();
    int GetPlayerID();
    int GetTeamNumber();


    ////////////////Setters/////////////////

    void SetRotation(const sf::Vector2f &FactingLocation);
    void SetRotation(float i_Rotation);
    void SetSpritePosition(const sf::Vector2f &i_Position);
    void SetPlayerPosition(Player::PlayerPosition i_Position);
    void SetPlayerID(int i_PlayerID);
    void SetTeamNumber(int i_TeamNumber);
    void SetIsPlayerControlled(bool i_isPlayerControlled);



    /////////buffs interface///////////

    void AddBuff(float i_Duration, float i_SpeedChangePercentage, bool i_DoesStun);
    void AddBuff(Buff i_Buff);


    sf::Vector2f PlayerSavedPosition;



    //abilities

    bool CheckAbilityCooldown();
    void UseAbility();


    /////////AI //////////



    void HoldPosition();
    void MarkAPlayer(const sf::Vector2f &i_PlayerPosition);
    void SetTargetLocation(const sf::Vector2f &i_location, const sf::Vector2f &TargetSize);
    sf::RectangleShape GetInDefensivePosition(const sf::RectangleShape &DefensiveThird, const sf::RectangleShape &MiddleThird);
    void AdvanceBall(const sf::RectangleShape &MiddleThird, const sf::RectangleShape &AttackingThird);
    void GetInOffenseivePosition(const sf::RectangleShape &MiddleThird);
    void GetInMidFieldPosition(const sf::RectangleShape &MiddleThird);


    //flag used to override AI strategy to go for the pass
    bool isBallBeingPassedToYou;

    //Changes a players color to blue if on user team and Red if not on User Team
    void ChangeToDifferentColor(bool isUserTeam);


    /////// Playercard interface//////////

    void UpdatePlayerCard();
    void DrawPlayerCard(sf::RenderWindow &MainWindow, const sf::Vector2f &CardPosition, const sf::Vector2f &i_Scale);
    bool CheckForPlayerCardSelection(const sf::Vector2f &i_Location);
    void setPlayerCardHighlighted(bool i_isHighlighted);
    int CalculateWeeklySalary();
    int CalculateHireBonus();

private:

    bool isPlayerControlled;

    // Variable that determines which player sprite variant to load from file
    int PlayerSpriteType;

    sf::Sprite PlayerSprite;
    sf::Texture PlayerTexure;

    SpecialAbility m_PlayerSpecialAbility;
    PlayerPosition m_PlayerPosition;

    sf::Vector2f MoveVector;

    ///////////////Buffs////////

    std::vector<Buff> BuffsContainer;
    void TickBuffs(float DeltaTimeAsSeconds);
    void MovePlayer(float DeltaTimeAsSeconds);
    float GetMoveModification();


    //////////ablities/////////

    void TickCooldowns(float DeltaTimeAsSeconds);
    float AbilityCooldownDuration;
    float AbilityCooldownTimer;


    //lua stuff

    //playerid only used for lua and is  always between 1-10, eventually may switch to be unique for the game and used outside of lua, if so then lua will need be changes to earse players
    int PlayerID;
    int TeamNumber; // 1 for user, 2 for not user

    // player indication ring
    sf::CircleShape PlayerIndicationRing;

    //ai implementation
    sf::RectangleShape TargetPositionRectangle;
    void AiMove(float DeltaTimeAsSeconds);
    int CurrentCornerStrafe = 0;



    //stats
    float PlayerStrength;
    float PlayerCatch;
    float PlayerSpeed;
    float PlayerShotPower;
    std::string PlayerName;

    //playercard
    PlayerCard m_PlayerCard;

    //player scale
    float ScaleX(float xInput);
    float ScaleY(float yInput);
    sf::Vector2f Scale(float xInput, float yInput);
    sf::Vector2f Scale(const sf::Vector2f &i_Input);


};

