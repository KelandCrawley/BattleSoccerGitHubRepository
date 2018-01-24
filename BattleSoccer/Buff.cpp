#include "Buff.h"


Buff::Buff()
{
    BuffDuration = 0.0f;
    SpeedChangePercentage = 0.0f;
    TimeExpired = 0.0f;
}

Buff::Buff(float i_BuffDuration, float i_SpeedChangePercentage, bool i_DoesStun)
{
    SpeedChangePercentage = i_SpeedChangePercentage;
    DoesStun = i_DoesStun;

    BuffDuration = i_BuffDuration;
    TimeExpired = 0.0f;

}

Buff::~Buff()
{
}

bool Buff::TickBuff(float DeltaTimeAsSeconds)
{
    TimeExpired += DeltaTimeAsSeconds;
    if (TimeExpired > BuffDuration)
    {
        return true;
    }
    return false;
}

float Buff::GetSpeedChangePercentage()
{
    return SpeedChangePercentage;
}

bool Buff::GetDoesStun()
{
    return DoesStun;
}
