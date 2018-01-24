#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <memory>
#include <functional>

class Process;
//smart pointer alias
using StrongProcessPtr = std::shared_ptr<Process>;
using WeakProcessPtr = std::weak_ptr<Process>;

class Process
{


    friend class ProcessManager;

public:
    Process();
    virtual ~Process();

    // state of the process
    enum class State
    {     Uninitialized = 0, Removed, Running, Paused, Succeeded, Failed, Aborted,   };


    //setter
    void SetSuccessFunction(std::function<void()> i_Function);


    // If the process has an animation it draws it
    // not all child classes will have an animation, in those cases this does nothing
    // all child classes with animation will need to overwite this funciton
    virtual void DrawAnimation(sf::RenderWindow &MainWindow);

private:
    State CurrentState;
    StrongProcessPtr ChildProcess;

    void SetState(State NewState);
    State GetState(void) const { return CurrentState; };

    bool const isDead(void);

protected:
    // Child class Interface Functions
    virtual void Initalize(void) { CurrentState = State::Running; };
    virtual void OnUpdate(float DeltaTime) = 0;
    virtual void OnSuccess(void) {};
    virtual void OnFail(void) {};
    virtual void OnAbort(void) {};

    void Succeed();
    void Fail();

    // function to be executed on sucess on the funtion
    std::function<void()> SuccessFunction;

    // child functions
    inline void AttachChild(StrongProcessPtr pChild);
    StrongProcessPtr RemoveChild(void); // Releases child
    StrongProcessPtr PeekChild(void) { return ChildProcess; };

};