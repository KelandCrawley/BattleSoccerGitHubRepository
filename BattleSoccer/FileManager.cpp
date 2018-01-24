#include "FileManager.h"



FileManager::FileManager()
{
}


FileManager::~FileManager()
{
}

FileManager::GameSettingsDataPack FileManager::LoadSettings()
{

    GameSettingsDataPack NewSettingsPack;
    
    rapidxml::xml_document<> doc;
    std::ifstream file("Saves/SaveGameOne/Settings/GameSettings.xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    //game volume
    rapidxml::xml_node<> *pRoot = doc.first_node();
    rapidxml::xml_node<> *pNode = pRoot->first_node("GameVolume");
    rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");

    NewSettingsPack.SavedGameVolume = atoi(pAttr->value());

    //music volume
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    NewSettingsPack.SavedMusicVolume = atoi(pAttr->value());

    //Move up
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    NewSettingsPack.MoveUp_KeyCode = atoi(pAttr->value());

    //Move down
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    NewSettingsPack.MoveDown_KeyCode = atoi(pAttr->value());

    //Move right
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    NewSettingsPack.MoveRight_KeyCode = atoi(pAttr->value());

    //Move left
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    NewSettingsPack.MoveLeft_KeyCode = atoi(pAttr->value());

    //switch to closest player
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    NewSettingsPack.SwitchToClosestPlayer_KeyCode = atoi(pAttr->value());

    //Use ability
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    NewSettingsPack.UseAbility_KeyCode = atoi(pAttr->value());

    doc.clear();

    return NewSettingsPack;
}

void FileManager::SaveSettings(GameSettingsDataPack i_DataPack)
{
    rapidxml::xml_document<> doc;
    std::ifstream file("Saves/SaveGameOne/Settings/GameSettings.xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    rapidxml::xml_node<> *pRoot = doc.first_node();
    rapidxml::xml_node<> *pNode = pRoot->first_node("GameVolume");
    rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");
    char pbuff[32];

    // save game volume
    itoa_base10(i_DataPack.SavedGameVolume, pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    // get music volume node
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");

    // save music volume
    itoa_base10(i_DataPack.SavedMusicVolume, pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    // MoveUp hotkey
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    itoa_base10(i_DataPack.MoveUp_KeyCode, pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    // MoveDown hotkey
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    itoa_base10(i_DataPack.MoveDown_KeyCode, pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    // Move Right hotkey
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    itoa_base10(i_DataPack.MoveRight_KeyCode, pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    // Move Left hotkey
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    itoa_base10(i_DataPack.MoveLeft_KeyCode, pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    // Switch To Closest Player hotkey
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    itoa_base10(i_DataPack.SwitchToClosestPlayer_KeyCode, pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    // Use ability hotkey
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    itoa_base10(i_DataPack.UseAbility_KeyCode, pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    std::ofstream file_stored("Saves/SaveGameOne/Settings/GameSettings.xml");
    file_stored << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    file_stored << doc;
    file_stored.close();
    doc.clear();
}

void FileManager::SaveFreeAgents(std::string FileName, std::vector<std::shared_ptr<Player>> FA_Container)
{
    
    std::ofstream GameSave;
    GameSave.open("Saves/SaveGameOne/Teams/" + FileName + ".xml");
    GameSave.close();

    rapidxml::xml_document<> doc;
    std::ifstream file("Saves/SaveGameOne/Teams/" + FileName + ".xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());



    //this needs to be variable in size, currently does not allow for longer player names
    char Player1Buff[32];
    char Player2Buff[32];
    char Player3Buff[32];
    char Player4Buff[32];
    char Player5Buff[32];
    char Player6Buff[32];
    char Player7Buff[32];
    char Player8Buff[32];
    char Player9Buff[32];
    char Player10Buff[32];
    char Player11Buff[32];
    char Player12Buff[32];
    char Player13Buff[32];


    //for each, check if FA_Container is sufficently big enough to have another player in it
    // Then save each player name through parsing by RAPID_XML parser
    // Then call SavePlayer function and pass the player, this function will save the players individual information in the file we created under its name
    if (FA_Container.size() > 0)
    {
        strcpy_s(Player1Buff, std::string(FA_Container.at(0)->GetPlayerName() + ".xml").c_str());
    }
    else
    {
        return;
    }
    // player 1
    rapidxml::xml_node<> *node = doc.allocate_node(rapidxml::node_element, "Player", 0);
    doc.append_node(node);
    rapidxml::xml_attribute<> *attr = doc.allocate_attribute("value", Player1Buff);
    node->append_attribute(attr);
    SavePlayer(FA_Container.at(0));

    if (FA_Container.size() > 1)
    {
        strcpy_s(Player2Buff, std::string(FA_Container.at(1)->GetPlayerName() + ".xml").c_str());
        // player 2
        node = doc.allocate_node(rapidxml::node_element, "Player", 0);
        doc.append_node(node);
        attr = doc.allocate_attribute("value", Player2Buff);
        node->append_attribute(attr);
        SavePlayer(FA_Container.at(1));
    }
    if (FA_Container.size() > 2)
    {
        strcpy_s(Player3Buff, std::string(FA_Container.at(2)->GetPlayerName() + ".xml").c_str());
        // player 3
        node = doc.allocate_node(rapidxml::node_element, "Player", 0);
        doc.append_node(node);
        attr = doc.allocate_attribute("value", Player3Buff);
        node->append_attribute(attr);
        SavePlayer(FA_Container.at(2));
    }
    if (FA_Container.size() > 3)
    {
        strcpy_s(Player4Buff, std::string(FA_Container.at(3)->GetPlayerName() + ".xml").c_str());
        // player 4
        node = doc.allocate_node(rapidxml::node_element, "Player", 0);
        doc.append_node(node);
        attr = doc.allocate_attribute("value", Player4Buff);
        node->append_attribute(attr);
        SavePlayer(FA_Container.at(3));
    }
    if (FA_Container.size() > 4)
    {
        strcpy_s(Player5Buff, std::string(FA_Container.at(4)->GetPlayerName() + ".xml").c_str());
        // player 5
        node = doc.allocate_node(rapidxml::node_element, "Player", 0);
        doc.append_node(node);
        attr = doc.allocate_attribute("value", Player5Buff);
        node->append_attribute(attr);
        SavePlayer(FA_Container.at(4));
    }
    if (FA_Container.size() > 5)
    {
        strcpy_s(Player6Buff, std::string(FA_Container.at(5)->GetPlayerName() + ".xml").c_str());
        // player 6
        node = doc.allocate_node(rapidxml::node_element, "Player", 0);
        doc.append_node(node);
        attr = doc.allocate_attribute("value", Player6Buff);
        node->append_attribute(attr);
        SavePlayer(FA_Container.at(5));
    }
    if (FA_Container.size() > 6)
    {
        strcpy_s(Player7Buff, std::string(FA_Container.at(6)->GetPlayerName() + ".xml").c_str());
        // player 7
        node = doc.allocate_node(rapidxml::node_element, "Player", 0);
        doc.append_node(node);
        attr = doc.allocate_attribute("value", Player7Buff);
        node->append_attribute(attr);
        SavePlayer(FA_Container.at(6));
    }   
    if (FA_Container.size() > 7)
    {
        strcpy_s(Player8Buff, std::string(FA_Container.at(7)->GetPlayerName() + ".xml").c_str());
        // player 8
        node = doc.allocate_node(rapidxml::node_element, "Player", 0);
        doc.append_node(node);
        attr = doc.allocate_attribute("value", Player8Buff);
        node->append_attribute(attr);
        SavePlayer(FA_Container.at(7));
    }
    if (FA_Container.size() > 8)
    {
        strcpy_s(Player9Buff, std::string(FA_Container.at(8)->GetPlayerName() + ".xml").c_str());
        // player 9
        node = doc.allocate_node(rapidxml::node_element, "Player", 0);
        doc.append_node(node);
        attr = doc.allocate_attribute("value", Player9Buff);
        node->append_attribute(attr);
        SavePlayer(FA_Container.at(8));
    }
    if (FA_Container.size() > 9)
    {
        strcpy_s(Player10Buff, std::string(FA_Container.at(9)->GetPlayerName() + ".xml").c_str());
        // player 10
        node = doc.allocate_node(rapidxml::node_element, "Player", 0);
        doc.append_node(node);
        attr = doc.allocate_attribute("value", Player10Buff);
        node->append_attribute(attr);
        SavePlayer(FA_Container.at(9));
    }
    if (FA_Container.size() > 10)
    {
        strcpy_s(Player11Buff, std::string(FA_Container.at(10)->GetPlayerName() + ".xml").c_str());
        // player 11
        node = doc.allocate_node(rapidxml::node_element, "Player", 0);
        doc.append_node(node);
        attr = doc.allocate_attribute("value", Player11Buff);
        node->append_attribute(attr);
        SavePlayer(FA_Container.at(10));
    }
    if (FA_Container.size() > 11)
    {
        strcpy_s(Player12Buff, std::string(FA_Container.at(11)->GetPlayerName() + ".xml").c_str());
        // player 12
        node = doc.allocate_node(rapidxml::node_element, "Player", 0);
        doc.append_node(node);
        attr = doc.allocate_attribute("value", Player12Buff);
        node->append_attribute(attr);
        SavePlayer(FA_Container.at(11));
    }
    if (FA_Container.size() > 12)
    {
        strcpy_s(Player13Buff, std::string(FA_Container.at(12)->GetPlayerName() + ".xml").c_str());
        // player 13
        node = doc.allocate_node(rapidxml::node_element, "Player", 0);
        doc.append_node(node);
        attr = doc.allocate_attribute("value", Player13Buff);
        node->append_attribute(attr);
        SavePlayer(FA_Container.at(12));
    }


    std::ofstream file_stored("Saves/SaveGameOne/Teams/" + FileName + ".xml");
    file_stored << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    file_stored << doc;
    file_stored.close();
    doc.clear();

}

void FileManager::SaveGameState(std::vector<int> GameStateNumbers)
{
    rapidxml::xml_document<> doc;
    std::ifstream file("Saves/SaveGameOne/GameState/GameState.xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    rapidxml::xml_node<> *pRoot = doc.first_node();
    rapidxml::xml_node<> *pNode = pRoot->first_node("CurrentWeek");
    rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");
    char pbuff[32];

    // CurrentWeek
    itoa_base10(GameStateNumbers.at(0), pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    // CurrentLeague Level
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    itoa_base10(GameStateNumbers.at(1), pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    // UserCash
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    itoa_base10(GameStateNumbers.at(2), pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    std::ofstream file_stored("Saves/SaveGameOne/GameState/GameState.xml");
    file_stored << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    file_stored << doc;
    file_stored.close();
    doc.clear();
}

std::shared_ptr<Team> FileManager::ResetUserTeam()
{
    std::shared_ptr<Team> i_UserTeam = std::make_shared<Team>();
    i_UserTeam->SetTeamName("Fail"); //presets team name to "fail" but this will be overwritten if reset is succesful

    //open default UserTeam data
    rapidxml::xml_document<> doc;
    std::ifstream file("Saves/Default/Teams/UserTeam.xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    rapidxml::xml_node<> *pRoot = doc.first_node();
    rapidxml::xml_node<> *pNode = doc.first_node("TeamName");
    rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");

    std::string strValue = pAttr->value();
    i_UserTeam->SetTeamName(strValue);

    //Start Overwriting individual pieces of data

    //Leauge wins
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    i_UserTeam->LeagueWinTotal = atoi(pAttr->value());

    //Leauge Loses
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    i_UserTeam->LeagueLossTotal = atoi(pAttr->value());

    //total wins
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    i_UserTeam->LifetimeWinTotal = atoi(pAttr->value());

    //total losses
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    i_UserTeam->LifetimeLossTotal = atoi(pAttr->value());

    for (rapidxml::xml_node<> *pNode = doc.first_node("Player"); pNode; pNode = pNode->next_sibling())
    {
        rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");
        strValue = pAttr->value();
        if (strValue != "NoValue" && strValue != "No Value" && strValue != "Novalue" && strValue != "No value" && strValue != "novalue" && strValue != "no value")
        {
            i_UserTeam->AddPlayer(LoadPlayer(strValue, true));
        }
    }



    FileManager::SaveTeam(i_UserTeam, "UserTeam");
    doc.clear();

    return i_UserTeam;
}

void FileManager::ResetFreeAgents(std::string FileName)
{

    std::vector<std::shared_ptr<Player>> FA_Container;

    rapidxml::xml_document<> doc;
    std::ifstream file("Saves/Default/Teams/" + FileName + ".xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    rapidxml::xml_node<> *pRoot = doc.first_node();
    std::string strValue = "";

    //prob need to look if node exists
    for (rapidxml::xml_node<> *pNode = doc.first_node("Player"); pNode; pNode = pNode->next_sibling())
    {
        rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");
        strValue = pAttr->value();
        FA_Container.push_back(FileManager::LoadPlayer(strValue, false));
    }

    SaveFreeAgents(FileName, FA_Container);

    return;
}

void FileManager::ResetGameState()
{
    std::vector<int> GameStateNumbers;

    rapidxml::xml_document<> doc;
    std::ifstream file("Saves/Default/GameState/GameState.xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    //Current Week
    rapidxml::xml_node<> *pRoot = doc.first_node();
    rapidxml::xml_node<> *pNode = pRoot->first_node("CurrentWeek");
    rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");
    GameStateNumbers.push_back(atoi(pAttr->value()));

    //CurrentLeagueLevel
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    GameStateNumbers.push_back(atoi(pAttr->value()));

    //User Cash
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    GameStateNumbers.push_back(atoi(pAttr->value()));



    doc.clear();

    SaveGameState(GameStateNumbers);
}

std::shared_ptr<Team> FileManager::LoadUserTeam()
{
    std::shared_ptr<Team> i_UserTeam = std::make_shared<Team>();
    i_UserTeam->SetTeamName("Fail");

    rapidxml::xml_document<> doc;
    std::ifstream file("Saves/SaveGameOne/Teams/UserTeam.xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    rapidxml::xml_node<> *pRoot = doc.first_node();
    rapidxml::xml_node<> *pNode = doc.first_node("TeamName");
    rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");

    std::string strValue = pAttr->value();
    i_UserTeam->SetTeamName(strValue);

    //Leauge wins
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    i_UserTeam->LeagueWinTotal = atoi(pAttr->value());

    //Leauge Loses
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    i_UserTeam->LeagueLossTotal = atoi(pAttr->value());

    //total wins
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    i_UserTeam->LifetimeWinTotal = atoi(pAttr->value());

    //total losses
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    i_UserTeam->LifetimeLossTotal = atoi(pAttr->value());

    for (rapidxml::xml_node<> *pNode = doc.first_node("Player"); pNode; pNode = pNode->next_sibling())
    {
        rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");
        strValue = pAttr->value();
        if (strValue != "NoValue" && strValue != "No Value" && strValue != "Novalue" && strValue != "No value" && strValue != "novalue" && strValue != "no value")
        {
            i_UserTeam->AddPlayer(LoadPlayer(strValue, true));
        }
    }


    doc.clear();

    return i_UserTeam;
}

std::vector<std::shared_ptr<Player>> FileManager::LoadFreeAgents(std::string FileName)
{
    std::vector<std::shared_ptr<Player>> FA_Container;

    rapidxml::xml_document<> doc;
    std::ifstream file("Saves/SaveGameOne/Teams/" + FileName + ".xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    rapidxml::xml_node<> *pRoot = doc.first_node();
    std::string strValue = "";

    //prob need to look if node exists
    for (rapidxml::xml_node<> *pNode = doc.first_node("Player"); pNode; pNode = pNode->next_sibling())
    {
        rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");
        strValue = pAttr->value();
        FA_Container.push_back(FileManager::LoadPlayer(strValue, false));
    }

    return FA_Container;
}

std::shared_ptr<Team> FileManager::LoadTeam(std::string FileName)
{
    std::shared_ptr<Team> i_Team = std::make_shared<Team>();
    i_Team->SetTeamName("Fail"); // Presets team name to "fail" but will overwrite when loaded properly

    rapidxml::xml_document<> doc;
    std::ifstream file("Saves/SaveGameOne/Teams/" + FileName + ".xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    rapidxml::xml_node<> *pRoot = doc.first_node();
    rapidxml::xml_node<> *pNode = doc.first_node("TeamName");
    rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");

    std::string strValue = pAttr->value();
    i_Team->SetTeamName(strValue);

    //Leauge wins
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    i_Team->LeagueWinTotal = atoi(pAttr->value());

    //Leauge Loses
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    i_Team->LeagueLossTotal = atoi(pAttr->value());

    //total wins
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    i_Team->LifetimeWinTotal = atoi(pAttr->value());

    //total losses
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    i_Team->LifetimeLossTotal = atoi(pAttr->value());

    for (rapidxml::xml_node<> *pNode = doc.first_node("Player"); pNode; pNode = pNode->next_sibling())
    {
        rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");
        strValue = pAttr->value();
        if (strValue != "NoValue" && strValue != "No Value" && strValue != "Novalue" && strValue != "No value" && strValue != "novalue" && strValue != "no value")
        {
            i_Team->AddPlayer(LoadPlayer(strValue, false));
        }
    }


    doc.clear();

    return i_Team;

}

std::vector<int> FileManager::LoadGameState()
{
    std::vector<int> GameStateNumbers;

    rapidxml::xml_document<> doc;
    std::ifstream file("Saves/SaveGameOne/GameState/GameState.xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    //Current Week
    rapidxml::xml_node<> *pRoot = doc.first_node();
    rapidxml::xml_node<> *pNode = pRoot->first_node("CurrentWeek");
    rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");
    GameStateNumbers.push_back(atoi(pAttr->value()));

    //CurrentLeagueLevel
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    GameStateNumbers.push_back(atoi(pAttr->value()));

    //User Cash
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    GameStateNumbers.push_back(atoi(pAttr->value()));



    doc.clear();

    return GameStateNumbers;
}

char * FileManager::itoa_base10(int num, char *str)
{
    int i = 0;
    bool isNegative = false;

    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // if num is negative, multiply by -1 and flip isNegatie flag
    if (num < 0)
    {
        isNegative = true;
        num = -num;
    }

    // Process digits
    while (num != 0)
    {
        // get last digit
        int rem = num % 10; 

        //change didit to char
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';

        // drop last digit
        num = num / 10;
    }

    // If number is negative, append '-'
    if (isNegative)
    {
        str[i++] = '-';
    }

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    std::reverse(str, str + strlen(str));

    return str;
}

void FileManager::SaveTeam(std::shared_ptr<Team> i_Team, std::string FileName)
{
    rapidxml::xml_document<> doc;
    std::ifstream file("Saves/SaveGameOne/Teams/" + FileName + ".xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    rapidxml::xml_node<> *pNode = doc.first_node("TeamName");
    rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");
    char pbuff[32];


    //skip team name and get league wins
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");

    // save league wins
    itoa_base10(i_Team->LeagueWinTotal, pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    // League Loses
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    itoa_base10(i_Team->LeagueLossTotal, pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    // total wins
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    itoa_base10(i_Team->LifetimeWinTotal, pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    // Total Loses
    pNode = pNode->next_sibling();
    pAttr = pNode->first_attribute("value");
    itoa_base10(i_Team->LifetimeLossTotal, pbuff);
    pAttr->value(doc.allocate_string(pbuff));
    pNode->append_attribute(doc.allocate_attribute("value"));

    for (auto i = 0; i < 5; i++)
    {
        pNode = pNode->next_sibling();
        pAttr = pNode->first_attribute("value");
        if(i_Team->TeamRoster->size() > i)
        {
            strcpy_s(pbuff, std::string(i_Team->TeamRoster->at(i)->GetPlayerName() + ".xml").c_str());
            pAttr->value(doc.allocate_string(pbuff));
            pNode->append_attribute(doc.allocate_attribute("value"));
            SavePlayer(i_Team->TeamRoster->at(i));
        }
        else
        {
            strcpy_s(pbuff, std::string("NoValue").c_str());
            pAttr->value(doc.allocate_string(pbuff));
            pNode->append_attribute(doc.allocate_attribute("value"));
        }

    }





    std::ofstream file_stored("Saves/SaveGameOne/Teams/" + FileName + ".xml");
    file_stored << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    file_stored << doc;
    file_stored.close();
    doc.clear();
}

std::shared_ptr<Player> FileManager::LoadPlayer(std::string i_PlayerFileName, bool isUserTeam)
{

    std::string i_Name = "Dummy";
    int i_str = 50;
    int i_spd = 325;
    int i_sht = 50;
    int i_cat = 50;
    std::string i_AbilityString;
    std::string i_PositionString;




    rapidxml::xml_document<> doc;
    std::ifstream file("Saves/SaveGameOne/Players/" + i_PlayerFileName);
    if (file.is_open())
    {
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        std::string content(buffer.str());
        doc.parse<0>(&content[0]);

        rapidxml::xml_node<> *pNode = doc.first_node();
        rapidxml::xml_attribute<> *pAttr = pNode->first_attribute("value");

        //name
        i_Name = pAttr->value();

        //strength
        pNode = pNode->next_sibling();
        pAttr = pNode->first_attribute("value");
        i_str = atoi(pAttr->value());

        //Spd
        pNode = pNode->next_sibling();
        pAttr = pNode->first_attribute("value");
        i_spd = atoi(pAttr->value());

        //shoot
        pNode = pNode->next_sibling();
        pAttr = pNode->first_attribute("value");
        i_sht = atoi(pAttr->value());

        //str
        pNode = pNode->next_sibling();
        pAttr = pNode->first_attribute("value");
        i_cat = atoi(pAttr->value());

        //Player Position
        pNode = pNode->next_sibling();
        pAttr = pNode->first_attribute("value");
        i_PositionString = (pAttr->value());

        //Special Abiliity
        pNode = pNode->next_sibling();
        pAttr = pNode->first_attribute("value");
        i_AbilityString = (pAttr->value());


    }
    else
    {
        std::cout << "Error Opening player file: " + i_PlayerFileName << std::endl;
    }
    doc.clear();

    std::shared_ptr<Player> i_Player = std::make_shared<Player>(i_Name, i_str, i_spd, i_sht, i_cat, Player::GetPositionByName(i_PositionString), Player::GetAbilityByName(i_AbilityString), isUserTeam);
    return i_Player;
}

void FileManager::SavePlayer(std::shared_ptr<Player> i_Player)
{
    std::string StringBuffer = i_Player->GetPlayerName();
    std::ofstream GameSave;

    GameSave.open("Saves/SaveGameOne/Players/" + StringBuffer + ".xml");
    GameSave.close();

    rapidxml::xml_document<> doc;
    std::ifstream file("Saves/SaveGameOne/Players/" + i_Player->GetPlayerName() + ".xml");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content(buffer.str());

    char NameBuff[32];
    char StrBuff[32];
    char SpdBuff[32];
    char ShtBuff[32];
    char CatBuff[32];
    char PosBuff[32];
    char AbiBuff[32];

    strcpy_s(NameBuff, i_Player->GetPlayerName().c_str());


    // name
    rapidxml::xml_node<> *node = doc.allocate_node(rapidxml::node_element, "Name", 0);
    doc.append_node(node);
    rapidxml::xml_attribute<> *attr = doc.allocate_attribute("value", NameBuff);
    node->append_attribute(attr);

    // add player strength
    itoa_base10(i_Player->GetPlayerStrength(), StrBuff);
    node = doc.allocate_node(rapidxml::node_element, "PlayerStrength", 0);
    doc.append_node(node);
    attr = doc.allocate_attribute("value", StrBuff);
    node->append_attribute(attr);

    // add player Speed
    itoa_base10(i_Player->GetPlayerSpeed(), SpdBuff);
    node = doc.allocate_node(rapidxml::node_element, "PlayerSPeed", 0);
    doc.append_node(node);
    attr = doc.allocate_attribute("value", SpdBuff);
    node->append_attribute(attr);

    // add player Shot
    itoa_base10(i_Player->GetPlayerShotPower(), ShtBuff);
    node = doc.allocate_node(rapidxml::node_element, "PlayerShoot", 0);
    doc.append_node(node);
    attr = doc.allocate_attribute("value", ShtBuff);
    node->append_attribute(attr);

    // add player strength
    itoa_base10(i_Player->GetPlayerCatch(), CatBuff);
    node = doc.allocate_node(rapidxml::node_element, "PlayerCatch", 0);
    doc.append_node(node);
    attr = doc.allocate_attribute("value", CatBuff);
    node->append_attribute(attr);

    // add player strength
    strcpy_s(PosBuff, i_Player->GetPlayerPositionString().c_str());
    node = doc.allocate_node(rapidxml::node_element, "PlayerPosition", 0);
    doc.append_node(node);
    attr = doc.allocate_attribute("value", PosBuff);
    node->append_attribute(attr);

    // add player strength
    strcpy_s(AbiBuff, i_Player->GetPlayerAbilityString().c_str());
    node = doc.allocate_node(rapidxml::node_element, "PlayerAbility", 0);
    doc.append_node(node);
    attr = doc.allocate_attribute("value", AbiBuff);
    node->append_attribute(attr);

    std::ofstream file_stored("Saves/SaveGameOne/Players/" + StringBuffer + ".xml");
    file_stored << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    file_stored << doc;
    file_stored.close();
    doc.clear();

}
