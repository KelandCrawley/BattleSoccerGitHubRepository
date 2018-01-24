#pragma once
#include "AnimationProcess.h"
#include "Buff.h"

class ProjectileProcess : public AnimationProcess
{
public:
    ProjectileProcess(float i_AnimationDuration, sf::Vector2f i_Scale);
    ~ProjectileProcess();

    virtual void OnUpdate(float DeltaTime) override;

    virtual void OnSuccess() override;

    virtual void DrawAnimation(sf::RenderWindow &MainWindow) override;

    virtual void SetAnimationPosition(sf::Vector2f i_Position) override;
    virtual void SetRotation(float Rotation) override;

    std::function<bool(sf::FloatRect, Buff)> CheckForCollison_CallBack;
    Buff AppliedBuff;


private:

    void ProgressProjectile(float DeltaTime);
    sf::RectangleShape ProjectileRectangle;
    float CheckFrequency;
};

