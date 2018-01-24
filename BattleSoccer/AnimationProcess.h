#pragma once
#include <SFML/Graphics.hpp>

#include "Process.h"
class AnimationProcess : public Process
{
public:
    AnimationProcess(float i_AnimationDuration);
    ~AnimationProcess();

    virtual void OnUpdate(float DeltaTime) override;

    virtual void OnSuccess() override;

    virtual void DrawAnimation(sf::RenderWindow &MainWindow) override;

    virtual void SetAnimationPosition(sf::Vector2f i_Position);
    virtual void SetRotation(float Rotation);
    void SetAnimationMoveSpeed(float i_Speed);

protected:
    
    sf::Sprite AnimationSprite;
    sf::Texture AnimationTexture;

    virtual void ProgressAnimation(float DeltaTimeAsSeconds);

    float AnimationMoveSpeed;
 
    float ExpiredTime = 0.0f;
    float AnimationDuration;

    sf::Vector2f AnimationScale;
};

