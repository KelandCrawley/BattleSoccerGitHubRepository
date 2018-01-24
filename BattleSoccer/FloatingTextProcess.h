#pragma once

#include "AnimationProcess.h"
#include <iostream>
class FloatingTextProcess : public AnimationProcess
{

public:
    FloatingTextProcess(float i_AnimationDuration, std::string i_AnimatedText, sf::Vector2f i_Position, sf::Color i_Color, int i_CharSize, bool i_isTimerText);
    ~FloatingTextProcess();

    void DrawAnimation(sf::RenderWindow &MainWindow) override;

private:
    sf::Font MainFont;
    sf::Text AnimatedNumberText;
    

    // wierd work around to avoid making a seperate class for countdown text
    bool isTimerText;
};

