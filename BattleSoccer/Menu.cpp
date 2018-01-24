#include "Menu.h"

Menu::Menu()
{
}


Menu::~Menu()
{
}

bool Menu::Initialize(const sf::Vector2f &i_Scale)
{
    // this is a pure virtual function and should never be called
    // it acts as a minimum prototype for any derived classes
    SetMenuScale(i_Scale);
    return false;
}

void Menu::DrawMenu(sf::RenderWindow &MainWindow)
{
    MainWindow.draw(MenuSprite);
}

void Menu::UpdateMenu(const sf::Vector2f &MousePosition)
{
    SavedMousePosition = MousePosition;
    LookForButtonHovering();
}



void Menu::LeftClickEvent()
{
    for (auto i = 0; i < ButtonsContainer.size(); i++)
    {
        if (ButtonsContainer.at(i).ButtonRect.getGlobalBounds().contains(SavedMousePosition))
        {
            if (HandleEvent_CallBack)
            {
                HandleEvent_CallBack(ButtonsContainer.at(i).EventOnClick, ButtonsContainer.at(i).PassedArgumentValue);

            }
            if (ButtonsContainer.at(i).Button_CallBack)
            {
                ButtonsContainer.at(i).Button_CallBack(ButtonsContainer.at(i).PassedArgumentValue);
            }
        }
    }
}

void Menu::RightClickEvent()
{
    //no default implementation
}

void Menu::LookForButtonHovering()
{
    for (auto i = 0; i < ButtonsContainer.size(); i++)
    {
        if (ButtonsContainer.at(i).ButtonRect.getGlobalBounds().contains(SavedMousePosition))
        {
            ButtonsContainer.at(i).isHoverd = true;
        }
        else
        {
            ButtonsContainer.at(i).isHoverd = false;
        }
    }
}

void Menu::SetMenuScale(const sf::Vector2f &i_Scale)
{
    MenuSprite.setScale(i_Scale);
    for (auto i = 0; i < ButtonsContainer.size(); i++)
    {
        ButtonsContainer.at(i).ButtonRect.setScale(i_Scale);
        ButtonsContainer.at(i).ButtonRect.setPosition(ButtonsContainer.at(i).ButtonRect.getPosition().x * i_Scale.x, ButtonsContainer.at(i).ButtonRect.getPosition().y * i_Scale.y);
    }
}

void Menu::AddButton(sf::RectangleShape i_ButtonRect, sf::Vector2f i_position, int i_PassedArgumentValue, EventType i_EventOnClick, std::function<void(int)> i_Button_CallBack)
{

    i_ButtonRect.setFillColor(sf::Color::Transparent);
    i_ButtonRect.setOutlineColor(sf::Color::Red);
    i_ButtonRect.setOutlineThickness(2);

    Button NewButton;
    NewButton.ButtonRect = i_ButtonRect;
    NewButton.ButtonRect.setPosition(i_position);
    NewButton.EventOnClick = i_EventOnClick;
    NewButton.PassedArgumentValue = i_PassedArgumentValue;
    NewButton.Button_CallBack = i_Button_CallBack;
    ButtonsContainer.push_back(NewButton);

}

void Menu::AddButton(Menu::Button &i_button)
{
    ButtonsContainer.push_back(i_button);
}
