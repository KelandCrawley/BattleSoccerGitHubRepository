#pragma once
#include <list>
#include <iostream>


#include "Process.h"


class ProcessManager
{
    //smart pointer typedefs
    using ProcessList = std::list<StrongProcessPtr>;



public:
    ProcessManager();
    ~ProcessManager();

    // Updates process list by incrementing through each process and calling its update funciton
    //  @Param DeltaTime should always be pased in Seconds
    void UpdateProcesses(float DeltaTime_Seconds);

    // attachess a new process to MainProcessList
    // @iprocess is a shared_ptr<Process>
    // @return is a weak_ptr<Process>
    WeakProcessPtr AttachProcess(StrongProcessPtr iProcess);


    // Increments through the main Process list and draws and animations associated with a process
    void DrawAnimations(sf::RenderWindow &MainWindow);


private:
    // List of Current processes
    ProcessList MainProcessList;
};


