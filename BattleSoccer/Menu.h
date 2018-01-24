#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>
#include <iostream>

#include "EventTypes.h"

// virtual base class for all menus


class Menu
{
public:
    Menu();

    //button
    // Non-templated button that serves the needs of only this program
    // TODO: make this button more variable and tempalted for use in future projects
    struct Button
    {
        sf::RectangleShape ButtonRect;
        bool isHoverd = false;
        EventType EventOnClick = EventType::NoEvent;            // Default no event
        int PassedArgumentValue = 0;                            // Not every button will have a function that requires an argument value
        std::function<void(int)> Button_CallBack;
    };

    bool virtual Initialize(const sf::Vector2f &i_Scale) = 0;

    void virtual DrawMenu(sf::RenderWindow &MainWindow);

    void virtual LeftClickEvent();
    void virtual RightClickEvent();

    //default implementation of update function
    // update menu is called very cycle while the menu is active
    void virtual UpdateMenu(const sf::Vector2f &MousePosition);

    //callback to the HandleEvent funciton for the Main Event Manager
    std::function<void(EventType, int)> HandleEvent_CallBack;

    sf::Vector2f SavedMousePosition;

    //creates and adds a button to the button container
    void AddButton(sf::RectangleShape i_ButtonRect, sf::Vector2f i_position, int i_PassedArgumentValue, EventType i_EventOnClick, std::function<void(int)> Button_CallBack);

    //adds an exisitng button to the button container
    void AddButton(Menu::Button &i_button);

protected:
    // protected deconstructor to avoid deconstruction by calling the virtual base class destructor
    ~Menu();

    sf::Sprite MenuSprite;
    sf::Texture MenuTexture;

    // sets hovering for all the buttons on the menu, does not implement any funciton calls of the buttons
    void virtual LookForButtonHovering();

    //scaling
    // used when the screen resolution is not 1920/1080
    virtual void SetMenuScale(const sf::Vector2f &i_Scale);



    std::vector<Button> ButtonsContainer;
};

