#include "TimerProcess.h"



TimerProcess::TimerProcess()
{
    ExpiredTime = 0.0f;
    TimerDuration = 1.0f;
}


TimerProcess::~TimerProcess()
{
}

void TimerProcess::OnUpdate(float DeltaTime)
{
    ExpiredTime += DeltaTime;
    if (ExpiredTime >= TimerDuration)
    {
        Succeed();
    }
}

void TimerProcess::OnSuccess()
{
    if (SuccessFunction)
    {
        SuccessFunction();
    }
}

void TimerProcess::SetTimer(float Timer)
{
    TimerDuration = Timer;
}
