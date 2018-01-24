#pragma once
#include <SFML/Audio.hpp>

#include "RNGClass.h"
#include "FileManager.h"

class SoundManager
{
public:
    SoundManager();
    ~SoundManager();

    //loads menu sounds from file
    void LoadMenuSounds();

    //Toggle menu music
    void SetDoPlayerMenuMusic(bool i_Toggle);

    //loads match sessions sounds from file
    void LoadMatchSessionSounds();

    // sets changes to sound manager every loop
    void UpdateSoundManager();

         // event funcitnos
    void PlayRandomHitSound();
    void PlayGoalSounds();
    void PlayBallShotSound();
    void PlayPassSound();
    void PlayTackleMissedSound();
    void PlayBallHitWallSound();
    void PlayCountdownSound();
    void AbortMatchSounds();

         //volume setting's functions and variables
        //volume is always changed in increments of 5
    void GameVolumeUp();
    void GameVolumeDown();

    void MusicVolumeUp();
    void MusicVolumeDown();

    static int MusicVolume;
    static int EffectsVolume;


private:

        //Not interface setters, these set the volume of the individual sound effects associated with each category
    void SetMusicVolume();
    void SetEffectsVolume();

        // Menu Music
    void PlayMenuMusic();
    int CurrentTrackNumber;
    sf::Music MenuMusic1;
    sf::Music MenuMusic2;
    sf::Music MenuMusic3;
    sf::Music MenuMusic4;
    bool doPlayMusic;


        //Match Session sounds
    sf::SoundBuffer CheerSoundBuffer;
    sf::Sound CheerSound;

    sf::SoundBuffer GoalSoundBuffer;
    sf::Sound GoalSound;

    sf::SoundBuffer ShotSoundBuffer;
    sf::Sound ShotSound;

    sf::SoundBuffer PassSoundBuffer;
    sf::Sound PassSound;

    sf::SoundBuffer HitSound1Buffer;
    sf::Sound HitSound1;

    sf::SoundBuffer HitSound2Buffer;
    sf::Sound HitSound2;

    sf::SoundBuffer HitSound3Buffer;
    sf::Sound HitSound3;

    sf::SoundBuffer MissSoundBuffer;
    sf::Sound MissSound;

    sf::SoundBuffer BallHittingWallSoundBuffer;
    sf::Sound BallHittingWallSound;

    sf::SoundBuffer CountDownSoundBuffer;
    sf::Sound CountDownSound;



};

