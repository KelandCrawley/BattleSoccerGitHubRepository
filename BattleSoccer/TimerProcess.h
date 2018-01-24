#pragma once
#include "Process.h"

class TimerProcess : public Process
{
public:
    TimerProcess();
    ~TimerProcess();

    virtual void OnUpdate(float DeltaTime) override;

    virtual void OnSuccess() override;

    void SetTimer(float Timer);

public:
    float TimerDuration;
    float ExpiredTime;

};

