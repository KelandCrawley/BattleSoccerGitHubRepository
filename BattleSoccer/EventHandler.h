#pragma once
#include <map>
#include <vector>
#include <functional>
#include <iostream>

#include "EventTypes.h"


class EventHandler
{
public:

    EventHandler();
    ~EventHandler();

    //Loads all the events into the map and gives them a clean container to hold delegates
    void Initialize();


    //Takes an event and checks for any registered delegates
    //Arg is a param that is used by the delegate function
    void HandleEvent(EventType, int Arg);

    // Regeisterd a delegate function with a given event
    void RegisterDelegate(std::function<void(int)> i_Delegate, EventType i_Type);


private:
    //////
    // Container that holds events and its related vector of delegates
    std::map<EventType, std::vector<std::function<void(int)>>> EventsMap;
    

};
