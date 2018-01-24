#include "ProcessManager.h"



ProcessManager::ProcessManager()
{
}


ProcessManager::~ProcessManager()
{
}

void ProcessManager::UpdateProcesses(float DeltaTime_Seconds)
{

    ProcessList::iterator it = MainProcessList.begin();
    while (it != MainProcessList.end())
    {
        //Grab the next Process
        StrongProcessPtr CurrentProcess = (*it);

        //Save the iterator and increment the old one
        ProcessList::iterator thisIt = it;
        ++it;

        //initalize Process if it is not already
        if (CurrentProcess->GetState() == Process::State::Uninitialized)
        {
            CurrentProcess->Initalize();
        }

        // if process is running, progress it
        if (CurrentProcess->GetState() == Process::State::Running)
        {
            CurrentProcess->OnUpdate(DeltaTime_Seconds);
        }

        //if Process is dead, clean it up
        if (CurrentProcess->isDead())
        {
            switch (CurrentProcess->GetState())
            {
            case Process::State::Succeeded:
            {
                CurrentProcess->OnSuccess();

                // If child exists, remove it and insert it into current process list
                StrongProcessPtr pChild = CurrentProcess->RemoveChild();
                if (pChild)
                {
                    AttachProcess(pChild);
                }
            }
            break;
            case Process::State::Failed:
            {
                CurrentProcess->OnFail();
            }
            break;
            case Process::State::Aborted:
            {
                CurrentProcess->OnAbort();
            }
            break;
            }

            MainProcessList.erase(thisIt);
        }
    }

    return;
}

WeakProcessPtr ProcessManager::AttachProcess(StrongProcessPtr iProcess)
{
    MainProcessList.push_front(iProcess);
    return WeakProcessPtr(iProcess);
}

void ProcessManager::DrawAnimations(sf::RenderWindow & MainWindow)
{
    ProcessList::iterator it = MainProcessList.begin();
    while (it != MainProcessList.end())
    {
        //Grab the next Process
        StrongProcessPtr CurrentProcess = (*it);

        //Save the iterator and increment the old one
        ProcessList::iterator thisIt = it;
        ++it;

        CurrentProcess->DrawAnimation(MainWindow);

    }
}
