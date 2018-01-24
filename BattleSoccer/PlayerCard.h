#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>


class PlayerCard
{
    friend class Player;
public:

    PlayerCard();
    ~PlayerCard();

    void Initialize(int i_PlayerSpriteType, bool isUserTeam);

    //Draws player card and text at a given position
    //@Param CardPosition is where you want the top left corner of the card sprite to be
    void DrawPlayerCard(sf::RenderWindow &MainWindow, const sf::Vector2f &CardPosition);
    
    //Draws a smaller icon that represents the players ability being avaliable, usually drawn on the player during a match
    //@parm i_Position is the where you want the top left corner of the icon to be
    void DrawPlayerAbilityMiniIcon(sf::RenderWindow &MainWindow, const sf::Vector2f &i_Position);

    //Sets the card highlight around the player card
    void SetIsHighlighted(bool i_isHighlighted);

    //scales the card and all text / images
    // @param multiplies x and y dimensions and positions by x,y corrdinates of sf::vector2f
    void SetPlayerCardScale(const sf::Vector2f &i_Scale);


    // sets each stat/data point for the card
    void UpdatePlayerCardStats(int i_sht, int i_str, int i_spd, int i_cat, std::string i_PlayerName, std::string i_PlayerPosition, int i_SpecialAbility, int i_WeeklySalary, int i_TeamNumber);

private:

    sf::Sprite PlayerCardSprite;
    sf::Texture PlayerCardTexture;
    sf::Sprite PlayerSprite;
    sf::Texture PlayerTexture;
    sf::RectangleShape CardHighLight;
    sf::RectangleShape CooldownBox;

    sf::Texture PlayerAbilityIconTexture;
    sf::Sprite PlayerAbilityIconSprite;
    sf::Sprite PlayerAbilityMiniIconSprite;

    sf::Font MainFont;
    sf::Text CardText[6][2];



    //updates the color/sprite variation of the player icon on the card
    //Team color and sprite position are based on saved values from the associated player
    void UpdatePlayerSprite();

    int CurrentTeamNumber;
    int PlayerSpriteType;

    
};

