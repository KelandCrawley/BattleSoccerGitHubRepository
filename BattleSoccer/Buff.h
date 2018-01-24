#pragma once
class Buff
{
public:
    Buff();
    Buff(float i_BuffDuration, float i_SpeedChangePercentage, bool i_DoesStun);
    ~Buff();

    bool TickBuff(float DeltaTimeAsSeconds);

    float GetSpeedChangePercentage();
    bool GetDoesStun();

private:

    float BuffDuration;
    float TimeExpired;

    // default 0.0f for no change, 0.2f would be a 20% increase, -0.2f would be a 20% decrase in speed
    float SpeedChangePercentage;

    bool DoesStun;
};

