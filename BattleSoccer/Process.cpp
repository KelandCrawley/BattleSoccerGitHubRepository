#include "Process.h"



Process::Process()
{
    CurrentState = State::Uninitialized;
}


Process::~Process()
{
}

void Process::SetSuccessFunction(std::function<void()> i_Function)
{
    SuccessFunction = i_Function;
}

void Process::DrawAnimation(sf::RenderWindow & MainWindow)
{
    return;
}

void Process::SetState(State NewState)
{
    CurrentState = NewState;
}

bool const Process::isDead(void)
{
    return(CurrentState == State::Succeeded || CurrentState == State::Failed || CurrentState == State::Aborted);
}

void Process::Succeed()
{
    CurrentState = State::Succeeded;
}

void Process::Fail()
{
    CurrentState = State::Failed;
}

inline void Process::AttachChild(StrongProcessPtr pChild)
{
    ChildProcess = pChild;
}

StrongProcessPtr Process::RemoveChild(void)
{
    StrongProcessPtr pChild = ChildProcess;
    ChildProcess = nullptr;
    return pChild;
}