#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <math.h>

#include "LuaManager.h"
#include "Player.h"
#include "TimerProcess.h"
#include "ProjectileProcess.h"
#include "FloatingTextProcess.h"
#include "Buff.h"
#include "EventTypes.h"
#include "Team.h"
#include "PopUpMenu.h"


#include "MathHelper.hpp"


class MatchSession
    : public std::enable_shared_from_this<MatchSession>
{
    friend class LuaManager;

public:

    //maybe move to a process

    //Custom structure that combines a circle shape with buff applied with any player that comes in contact with the area of the circle
    struct AreaEffect
    {
        sf::CircleShape AreaEffected;
        Buff AppliedBuff;
    };


    MatchSession();
    ~MatchSession();

    //starts a new match using two teams, one of which should be the user team
    bool NewMatch(std::shared_ptr<Team> i_Team1, std::shared_ptr<Team> i_Team2);

    //Initialize the Match Session class instance, Text, Map etc
    bool Initialize();

    //Main Draw function, draws all game componets or calls componets draw function, called every loop
    void DrawMatchSession(sf::RenderWindow &MainWindow);

    // Controls game cycle for match session
    void MatchSessionLoop(float DeltaTimeAsSeconds, const sf::Vector2f &i_MouseLocation);

    //Main control movement control functions
    //@param ToggleNumber, 0 passed if move XXX key is relased, 1 is passed if move XXX key is pressed

    void ToggleMovePlayerUp(int ToggleNumber);
    void ToggleMovePlayerDown(int ToggleNumber);
    void ToggleMovePlayerRight(int ToggleNumber);
    void ToggleMovePlayerLeft(int ToggleNumber);

    //Switches the user controlled player to the position correlated to parameter
    //@param PlayerNumber is the roster position of the player the user wants to control
    void SwitchPlayer(int PlayerNumber);

    //Switches to closet player to the ball
    void SwitchToClosestPlayer();

    //Shots the ball using the playres Speical shot ability, if player does not have speical shot then it does nothing
    void SpecialShootBallEvent();

    //Called when the user tells the game to shoot the ball
    // if user controlled player has the ball it shoots the ball
    void ShootBallEvent();

    //Handles "the right click event" this is the passing function
    // if a user player is near the mouse pointer location then it will try and pass to the player (leads the player towards thier velocity vector) and also switches to that player as the controlled player
    // if no user player is near the mouse pointer then it passes it to that location exactly
    void RightClickEvent();

    //Handles the use ability event
    // Uses the controlled players ability if avaliable
    void UseAbilityEvent();

    //Smart pointer that points to the current user controlled player
    std::shared_ptr<Player> ControlledPlayer;

    //call back function to add process to the main process manager list
    std::function<void(StrongProcessPtr)> AddProcess_Callback;

    // Clears the ball of isVisiable and isInvernerable flags
    void ClearBallMods();

    //cheks if a projectile has collided with any player or map object
    //@param ProjectileBounds is passed from the calling function and is the bounds of the projectile in quesiton
    //@param i_AppliedBuff is the buff that is applied to any player that is struck with the projectile
    //@Return returns a boolean value to indicate if the projectile has made contact with a player or object
    bool CheckForProjectileCollison(const sf::FloatRect &ProjectileBounds, const Buff &i_AppliedBuff);

    //callback that pushes events to the main event handler
    std::function<void(EventType, int)> HandleEvent_CallBack;

    //used for menu left click eventhandling during in match menu
    void HandleLeftClickInput();

    //Toggle funciton for in match menu
    void ToggleMatchSessionMenu();

    // Sets the size/scale of the maps text, map, objects and players based on the windows scale porportion based on a baseline 1920x1080 window
    //@param i_Scale is the passed in scale, in most cases the scale will the saved MatchSessionScale variable
    void SetMatchSessionScale(const sf::Vector2f &i_Scale);

    bool ClearPassToTeammate(std::shared_ptr<Player> i_Teammate);

    ////////////////// ai lua calls/////////////////////////////
    // The following functions are called from Lua and the paramters are passed in from lua
    
    float GetBallPositionX();
    float GetBallPositionY();
    bool isClosestToBall(int i_PlayerID, int i_TeamNumber);
    void HavePlayerChaseLooseBall(int i_PlayerID);
    void HavePlayerSupportBall(int i_PlayerID, int i_TeamNumber);
    bool IsBallLoose();
    int TeamWithBall;
    void GetInDefensivePosition(int i_PlayerID, int i_TeamNumber);
    bool CheckIsBallCarrier(int i_PlayerID);
    void AiShootBall();
    bool DoesBallCarrierHaveOpenShot();
    bool DoesTeammateHaveShot();
    void PassToTeamMateInScoringPosition();
    bool isBallCarrierMarked();
    bool isBallCarrierAGoalie();
    bool DoesBallCarrierHaveAnOpenPass();
    void AiPassBall();
    void SendPlayerToAttackingPosition(int i_PlayerID, int i_TeamNumber);
    float GetDistanceToBall(int PlayerID);
    void AITackle(int i_PlayerID);
    void AdvanceBall();






private:

    // Switches the user controlled player to the player cloest to the mouse pointer
    // called after the ball is passed near another user player
    void SwitchToPlayerByPointer(std::shared_ptr<Player> i_Player);

             //Player containers
    std::vector<std::shared_ptr<Player>> Team1PlayersContainer;
    std::vector<std::shared_ptr<Player>> Team2PlayersContainer;


             // Ball stuff
    sf::Sprite BallSprite;
    sf::Texture BallTexture;
    bool isBallInvurnerable;
    bool DoShowBall;

    //This function moves the ball away from the ball carrier and tells the game that the player is not longer in controll of the ball
    // Used when a player is tackled or hit with an ability that stops them from holding the ball
    void BallCarrierDropBall();

    //Field stuff
    sf::Sprite MapSprite;
    sf::Texture MapTexture;
    sf::RectangleShape MapOutline;

    //Saved Move Keys, allows the user to start moving a player immediatly after switching to them by remembering what the current move button status is

    bool isMoveUpPressed;
    bool isMoveDownPressed;
    bool isMoveRightPressed;
    bool isMoveLeftPressed;

         //goals
    bool StopPlay;
    void GoalScored(int ScoringTeam);


    /////////////// These functions still need documentation /////////////

    void EndMatch();
    void StartEndOfMatchPopUP();
    void StartSuddenDeath();
    bool isSuddenDeath;
    void UpdateMatchInfo(float DeltaTimeAsSeconds);
    sf::Text MatchInfoText[3];
    sf::String Team1Name;
    sf::String Team2Name;
    int Team1Goals;
    int Team2Goals;
    float MatchDuration;
    float MatchRemainingTime;
    void StartCountdown();
    void BeginPlay();
    void ResetPlayers();
    void CheckForMouseHovering();
    sf::Vector2f MouseLocation;
    void PassBallToLocation(const sf::Vector2f &TargetLocation);
    sf::RectangleShape MouseOverPlayerRectangle;
    std::shared_ptr<Player> MouseHoveredPlayer;
    bool isBallBeingShot;
    std::shared_ptr<Player> ShootingPlayer;
    void SpecialShootBall();
    void ShootBall(const sf::Vector2f &ShootTarget);
    void CurveShot(const sf::Vector2f &i_TargetLocation);
    void PowerShot(const sf::Vector2f &i_TargetLocation);
    void CreateIcePatch(const sf::Vector2f &i_TargetLocation);
    void Blink(std::shared_ptr<Player> i_Player);
    void Blaze(std::shared_ptr<Player> i_Player);
    void VoidChain(std::shared_ptr<Player> i_Player, const sf::Vector2f &i_TargetLocation);
    void CheckIfPlayerShouldUseAbility(std::shared_ptr<Player> i_Player);
    std::vector<AreaEffect> AreaOfEffectContainer;
    void EraseLastAreaOfEffect();
    void CheckForAreaOfEffectCollsion();
    void CheckForLooseBallPickup();
    void StopBall();
    float BallVelocityDropOffPerSecond;
    float BallMoveDirection;
    float CurveIntensity;
    float BallVelocity;
    void MoveBall(float DeltaTimeAsSeconds);
    void CheckForGoal();
    void CheckForBallWallCollision();
    bool CheckForWallCollision(const sf::FloatRect &i_Entity);
    bool CheckForWallCollision(std::shared_ptr<Player> i_Player);
    bool CheckIfOffMap(const sf::FloatRect &i_Entity);


   ///////////////// Map objects///////////////////
    sf::RectangleShape LeftGoal;
    sf::RectangleShape RightGoal;
    sf::RectangleShape NorthWall;
    sf::RectangleShape SouthWall;
    sf::RectangleShape WestWallNorthHalf;
    sf::RectangleShape WestWallSouthHalf;
    sf::RectangleShape EastWallNorthHalf;
    sf::RectangleShape EastWallSouthHalf;
    sf::RectangleShape MiddleThird;

    /////// Map object functions /////////
    sf::RectangleShape GetDefensiveThird(int i_TeamNumber);
    sf::RectangleShape GetAttackingThird(int i_TeamNumber);
    sf::RectangleShape GetAreaInFrontOfDefensiveGoal(int i_TeamNumber);

    //Lua Manager
    float TimeBetweenUpdates;
    float TimeSinceLastUpdate;
    LuaManager m_LuaManager;
    std::shared_ptr<Player> GetPlayerByID(int i_ID);


    //ai, prob move to lua at some point
    bool CheckIfPlayerIsMarked(std::shared_ptr<Player> i_Player, sf::Vector2f MarkedBoxSize);
    void MarkAPlayer(std::shared_ptr<Player> i_Player);
    void GetOpenInMidField(std::shared_ptr<Player> i_Player);
    void GetOpenInAttackingThird(std::shared_ptr<Player> i_Player);
    bool CheckIfShouldMarkPlayerInScoringPosition();
    std::shared_ptr<Player> BallCarrierPointer;
    std::shared_ptr<Player> AISavedPlayerPointer;
    bool DoesPlayerHaveShot(std::shared_ptr<Player> i_Player, int ShotRange);
    void PassBallToPlayer(std::shared_ptr<Player> i_Player);
    void SetGoaliePosition(std::shared_ptr<Player> i_Goalie);
    sf::Vector2f GetBlockShotLocation(std::shared_ptr<Player> i_Player);

    // prob could go in another cateogry
    void Tackle(std::shared_ptr<Player> i_Player);
    void FoulCommited(std::shared_ptr<Player> i_Player);
    void ClearMapOutlineAlerts();
    int PlayerStrengthBattle(std::shared_ptr<Player> i_Player1, std::shared_ptr<Player> i_Player2);
    void CreateFloatingText(float i_AnimationDuration, std::string i_AnimatedText, const sf::Vector2f &i_Position, const sf::Color &i_Color, int i_CharSize);
    void CreateFloatingText(float i_AnimationDuration, std::string i_AnimatedText, const sf::Vector2f &i_Position, const sf::Color &i_Color, int i_CharSize, bool i_isTimerText);


    //fix when passed to player is fixed
    void ResetIsBallBeingPassedToYou();


    //helpers
    bool RayShotCheck(std::shared_ptr<Player> i_Player, int ShotRange, const sf::Vector2f &TargetLocation);
    sf::Vector2f AIShotTargetLocation;
    bool CheckForBetterPlayerPriroity(std::shared_ptr<Player> Contender, std::shared_ptr<Player> Champion);


    //shot power text
    sf::Font MainFont;
    sf::Text ShotVelocityText[1];

    //In Match UI stuff
    //Draws Players cards for both teams at the bottom of the screen
    void DrawInMatchUI(sf::RenderWindow &MainWindow);


    sf::RectangleShape ShotRayCastBuffer;

    //end of match menu
    PopUpMenu EndOfMatchPopUpMenu;
    PopUpMenu MatchSessionMenuPopUp;
    sf::Text EndOfMatchText[5];

    // quit match menu
    void AbortMatch();

    sf::Vector2f MatchSessionScale;

    sf::Vector2f Scale(const sf::Vector2f &i_Input);
    sf::Vector2f Scale(float xInput, float yInput);
    float Scale(float Input);

    void CheckForPlayerInZone(std::shared_ptr<Player> i_Player, const sf::RectangleShape &DefensiveZone);

};

