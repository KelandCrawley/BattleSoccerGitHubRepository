#pragma once
#include <SFML/Graphics.hpp>
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include <string>
#include <cassert>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <memory>


#include <windows.h>

#include "Team.h"
#include "Player.h"

class FileManager
{
public:

    //data pack that holds key binding and volume information
    struct GameSettingsDataPack
    {
        int SavedGameVolume;
        int SavedMusicVolume;

        int MoveUp_KeyCode;
        int MoveDown_KeyCode;
        int MoveRight_KeyCode;
        int MoveLeft_KeyCode;
        int SwitchToClosestPlayer_KeyCode;
        int UseAbility_KeyCode;

    };

    FileManager();
    ~FileManager();
    


    /////////////loads////////////////

    //Loads UserTeam from save file
    static std::shared_ptr<Team> LoadUserTeam();

    //Loads the free agents for a specific league and returns them in a vector of shared_ptr<Player>
    static std::vector<std::shared_ptr<Player>> LoadFreeAgents(std::string FileName);

    //Loads any team and returns a shared_ptr<Team>, will also work for UserTeam
    //@Param FileName is the name of the teams file, should be the name of team itself
    static std::shared_ptr<Team> LoadTeam(std::string FileName);

    //Loads the game state from file and returns a vector<int> with the data values
    // The Data values are in a pre-determined order that must be known to any function that wants them
    static std::vector<int> LoadGameState();

    //Loads Key Bindings and volume settings and returns them in a custom data structure GameSettingsDataPack
    static GameSettingsDataPack LoadSettings();

    //converts integer to c style char pointer
    // Used To convert integer data into char style data for save funtions
    //@param num is the integer to be converted, can be negative or postive
    static char* itoa_base10(int num, char* str);
   
    ///////////saves/////////////////////
    static void SaveTeam(std::shared_ptr<Team> i_Team, std::string FileName);

    //Saves Key Bindings and volume settings
    //@param i_Datapack is a custom data structure GameSettingsDataPack
    static void SaveSettings(GameSettingsDataPack i_DataPack);

    //Saves free agents by creating individual save files for each player and writing the name of the file inside the FreeAgents save file
    //@Param FileName is the name of the speicifc free agent list you want to save too, should be specific to the league the free agents are a part of
    //@Param FA_Container is the vector containing pointers to the individual Free Agent Players
    //Function will call SavePlayer Function on each individual player
    static void SaveFreeAgents(std::string FileName, std::vector<std::shared_ptr<Player>> FA_Container);


    static void SaveGameState(std::vector<int> GameStateNumbers);

    //////////////reloads//////////////
    //All resets work by opening the default data files and overriddeing the user save file

    static std::shared_ptr<Team> ResetUserTeam();
    static void ResetFreeAgents(std::string FileName);
    static void ResetGameState();

private:

    // These are individual load/save for players. They are exclusivly called from save/load team functions and work in the same manner

    //@param isUserTeam is used to flag that is only pass to the shared_ptr in this function. It can be used by the calling function to determine if it will load blue or red colors
    static std::shared_ptr<Player> LoadPlayer(std::string i_PlayerFileName, bool isUserTeam);

    static void SavePlayer(std::shared_ptr<Player> i_Player);


   

};

