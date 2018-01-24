#include "AnimationProcess.h"



AnimationProcess::AnimationProcess(float i_AnimationDuration)
{
    AnimationMoveSpeed = 0.0f;
    AnimationDuration = i_AnimationDuration;
}


AnimationProcess::~AnimationProcess()
{
}

void AnimationProcess::OnUpdate(float DeltaTime)
{
    ProgressAnimation(DeltaTime);
    ExpiredTime += DeltaTime;
    if (ExpiredTime >= AnimationDuration)
    {
        Succeed();
    }
}

void AnimationProcess::OnSuccess()
{

}

void AnimationProcess::DrawAnimation(sf::RenderWindow & MainWindow)
{
    MainWindow.draw(AnimationSprite);
}

void AnimationProcess::SetAnimationPosition(sf::Vector2f i_Position)
{
    AnimationSprite.setPosition(i_Position);
}

void AnimationProcess::SetRotation(float Rotation)
{
    AnimationSprite.setRotation(Rotation);
}

void AnimationProcess::SetAnimationMoveSpeed(float i_Speed)
{
    AnimationMoveSpeed = i_Speed;
}

void AnimationProcess::ProgressAnimation(float DeltaTimeAsSeconds)
{
    AnimationSprite.move(sf::Vector2f(cos(AnimationSprite.getRotation() * 3.1415 / 180.0) * DeltaTimeAsSeconds * AnimationMoveSpeed, sin(AnimationSprite.getRotation() * 3.1415 / 180.0) * DeltaTimeAsSeconds * AnimationMoveSpeed));
}
