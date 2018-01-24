#include "ProjectileProcess.h"



ProjectileProcess::ProjectileProcess(float i_AnimationDuration, sf::Vector2f i_Scale)
    :AnimationProcess(i_AnimationDuration)
    ,ProjectileRectangle(sf::Vector2f(100 * i_Scale.x, 4 * i_Scale.y))
{
    //default settings
    ProjectileRectangle.setFillColor(sf::Color::Magenta);
    ProjectileRectangle.setOutlineColor(sf::Color::Black);
    ProjectileRectangle.setOutlineThickness(1);
    ProjectileRectangle.setOrigin(ProjectileRectangle.getGlobalBounds().width / 2, ProjectileRectangle.getGlobalBounds().height / 2);

    AnimationScale = i_Scale;

    AnimationMoveSpeed = 0.0f;
    CheckFrequency = 0.1f;
}


ProjectileProcess::~ProjectileProcess()
{
}

void ProjectileProcess::OnUpdate(float DeltaTime)
{
    ProgressProjectile(DeltaTime);
}

void ProjectileProcess::OnSuccess()
{
}

void ProjectileProcess::ProgressProjectile(float DeltaTime)
{
    ProjectileRectangle.move(sf::Vector2f(cos(ProjectileRectangle.getRotation() * 3.1415 / 180.0) * DeltaTime * AnimationMoveSpeed * AnimationScale.x, sin(ProjectileRectangle.getRotation() * 3.1415 / 180.0) * DeltaTime * AnimationMoveSpeed * AnimationScale.y));


    ExpiredTime += DeltaTime;

    if (ExpiredTime > CheckFrequency)
    {
        ExpiredTime -= CheckFrequency;

        if (CheckForCollison_CallBack)
        {
            if (CheckForCollison_CallBack(ProjectileRectangle.getGlobalBounds(), AppliedBuff))
            {
                Succeed();
            }
        }
        else
        {
            Fail();
        }
    }

}

void ProjectileProcess::DrawAnimation(sf::RenderWindow & MainWindow)
{
    MainWindow.draw(ProjectileRectangle);
}

void ProjectileProcess::SetAnimationPosition(sf::Vector2f i_Position)
{
    ProjectileRectangle.setPosition(i_Position);
}

void ProjectileProcess::SetRotation(float Rotation)
{
    ProjectileRectangle.setRotation(Rotation);
}
