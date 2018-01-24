#include "FloatingTextProcess.h"



FloatingTextProcess::FloatingTextProcess(float i_AnimationDuration, std::string i_AnimnatedText, sf::Vector2f i_Position, sf::Color i_Color, int i_CharSize, bool i_isTimerText)
    :AnimationProcess(i_AnimationDuration)
{
    isTimerText = i_isTimerText;
    if (!MainFont.loadFromFile("Data/ArialFont/arial.ttf"))
    {
        Fail();
        return;
    }
    AnimatedNumberText.setCharacterSize(i_CharSize);
    AnimatedNumberText.setFillColor(i_Color);
    AnimatedNumberText.setFont(MainFont);
    AnimatedNumberText.setString(i_AnimnatedText);
    AnimatedNumberText.setOutlineThickness(1);
    AnimatedNumberText.setPosition(i_Position.x - (AnimatedNumberText.getString().getSize() * i_CharSize / 4), i_Position.y + (i_CharSize / 2));


    if (i_Color == sf::Color::Black)
    {
        AnimatedNumberText.setOutlineColor(sf::Color::White);
    }
    else
    {
        AnimatedNumberText.setOutlineColor(sf::Color::Black);
    }
}


FloatingTextProcess::~FloatingTextProcess()
{
}

void FloatingTextProcess::DrawAnimation(sf::RenderWindow & MainWindow)
{
    // if it is timer text then set the string to be the remaining time
    if (isTimerText)
    {
        AnimatedNumberText.setString(std::to_string((int)(AnimationDuration - ExpiredTime) + 1));

    }
    MainWindow.draw(AnimatedNumberText);
}