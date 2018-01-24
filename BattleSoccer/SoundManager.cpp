#include "SoundManager.h"

int SoundManager::MusicVolume = 25;
int SoundManager::EffectsVolume = 25;

SoundManager::SoundManager()
{
    //default music settings
    SoundManager::MusicVolume = 25;
    SoundManager::EffectsVolume = 25;
}


SoundManager::~SoundManager()
{
}

void SoundManager::LoadMenuSounds()
{

    // load sound settings from file
    FileManager::GameSettingsDataPack SavedSettingsData = FileManager::LoadSettings();
    SoundManager::MusicVolume = SavedSettingsData.SavedMusicVolume;
    SoundManager::EffectsVolume = SavedSettingsData.SavedGameVolume;

    CurrentTrackNumber = RNGClass::GetRandomInteger(1, 4);
    if (!MenuMusic1.openFromFile("Data/Audio/Chance_is_music_PSG2.ogg")) {}; MenuMusic1.setVolume(SoundManager::MusicVolume);
    if (!MenuMusic2.openFromFile("Data/Audio/Dear_Snabisch_PSG.ogg")) {}; MenuMusic2.setVolume(SoundManager::MusicVolume);
    if (!MenuMusic3.openFromFile("Data/Audio/Do_not_move_PSG.ogg")) {}; MenuMusic3.setVolume(SoundManager::MusicVolume);
    if (!MenuMusic4.openFromFile("Data/Audio/The_Most_beautiful_PSG.ogg")) {}; MenuMusic4.setVolume(SoundManager::MusicVolume);
    SetDoPlayerMenuMusic(true);

}

void SoundManager::SetDoPlayerMenuMusic(bool i_Toggle)
{
    if (doPlayMusic != i_Toggle)
    {
        doPlayMusic = i_Toggle;
    }
    else
    {
        //dont tell music to start/stop if it was already playing, it will cause songs to restart
        return;
    }

    if (i_Toggle == false)
    {
        switch (CurrentTrackNumber)
        {
        case 1:
            MenuMusic1.stop();
            break;
        case 2:
            MenuMusic2.stop();
            break;
        case 3:
            MenuMusic3.stop();
            break;
        case 4:
            MenuMusic4.stop();
            break;
        }
    }
    else
    {
        switch (CurrentTrackNumber)
        {
        case 1:
            MenuMusic1.play();
            break;
        case 2:
            MenuMusic2.play();
            break;
        case 3:
            MenuMusic3.play();
            break;
        case 4:
            MenuMusic4.play();
            break;
        }
    }
}

void SoundManager::LoadMatchSessionSounds()
{
    if (!CheerSoundBuffer.loadFromFile("Data/Audio/MatchSounds/CheerSound.wav")) {};
    CheerSound.setBuffer(CheerSoundBuffer); CheerSound.setVolume(SoundManager::EffectsVolume);

    if (!GoalSoundBuffer.loadFromFile("Data/Audio/MatchSounds/GoalSound.wav")) {};
    GoalSound.setBuffer(GoalSoundBuffer); GoalSound.setVolume(SoundManager::EffectsVolume);

    if (!ShotSoundBuffer.loadFromFile("Data/Audio/MatchSounds/ShotSound.wav")) {};
    ShotSound.setBuffer(ShotSoundBuffer); ShotSound.setVolume(SoundManager::EffectsVolume);

    if (!PassSoundBuffer.loadFromFile("Data/Audio/MatchSounds/PassSound.wav")) {};
    PassSound.setBuffer(PassSoundBuffer); PassSound.setVolume(SoundManager::EffectsVolume);

    if (!HitSound1Buffer.loadFromFile("Data/Audio/MatchSounds/HitSound_1.wav")) {};
    HitSound1.setBuffer(HitSound1Buffer); HitSound1.setVolume(SoundManager::EffectsVolume);

    if (!HitSound2Buffer.loadFromFile("Data/Audio/MatchSounds/HitSound_2.wav")) {};
    HitSound2.setBuffer(HitSound2Buffer); HitSound2.setVolume(SoundManager::EffectsVolume);

    if (!HitSound3Buffer.loadFromFile("Data/Audio/MatchSounds/HitSound_3.wav")) {};
    HitSound3.setBuffer(HitSound3Buffer); HitSound3.setVolume(SoundManager::EffectsVolume);

    if (!MissSoundBuffer.loadFromFile("Data/Audio/MatchSounds/MissSound.wav")) {};
    MissSound.setBuffer(MissSoundBuffer); MissSound.setVolume(SoundManager::EffectsVolume);

    if (!BallHittingWallSoundBuffer.loadFromFile("Data/Audio/MatchSounds/BallHittingWallSound.wav")) {};
    BallHittingWallSound.setBuffer(BallHittingWallSoundBuffer); BallHittingWallSound.setVolume(SoundManager::EffectsVolume);

    if (!CountDownSoundBuffer.loadFromFile("Data/Audio/MatchSounds/CountDownSound.wav")) {};
    CountDownSound.setBuffer(CountDownSoundBuffer); CountDownSound.setVolume(SoundManager::EffectsVolume);
}

void SoundManager::UpdateSoundManager()
{
    if (doPlayMusic)
    {
        PlayMenuMusic();
    }
}

void SoundManager::PlayRandomHitSound()
{
    int RandomNumber = RNGClass::GetRandomInteger(1, 3);
    switch (RandomNumber)
    {
    case 1:
        HitSound1.play();
        break;
    case 2:
        HitSound2.play();
        break;
    case 3:
        HitSound3.play();
        break;
    default:
        HitSound3.play();
        break;
    }
}

void SoundManager::PlayGoalSounds()
{
    GoalSound.play();
    CheerSound.play();
}

void SoundManager::PlayBallShotSound()
{
    ShotSound.play();
}

void SoundManager::PlayPassSound()
{
    if (PassSound.getStatus() != sf::Sound::Playing)
    {
        PassSound.play();
    }
}

void SoundManager::PlayTackleMissedSound()
{
    MissSound.play();
}

void SoundManager::PlayBallHitWallSound()
{
    if (BallHittingWallSound.getStatus() != sf::Sound::Playing)
    {
        BallHittingWallSound.play();
    }

}

void SoundManager::PlayCountdownSound()
{
    CountDownSound.play();
}

void SoundManager::AbortMatchSounds()
{
    CheerSound.stop();
    GoalSound.stop();
    ShotSound.stop();
    PassSound.stop();
    HitSound1.stop();
    HitSound2.stop();
    HitSound3.stop();
    MissSound.stop();
    BallHittingWallSound.stop();
    CountDownSound.stop();
}

void SoundManager::GameVolumeUp()
{
    SoundManager::EffectsVolume += 5;
    if (SoundManager::EffectsVolume > 50)
    {
        SoundManager::EffectsVolume = 50;
    }
    SoundManager::SetEffectsVolume();
}

void SoundManager::GameVolumeDown()
{
    SoundManager::EffectsVolume -= 5;
    if (SoundManager::EffectsVolume < 0)
    {
        SoundManager::EffectsVolume = 0;
    }
    SoundManager::SetEffectsVolume();
}

void SoundManager::MusicVolumeUp()
{
    SoundManager::MusicVolume += 5;
    if (SoundManager::MusicVolume > 50)
    {
        SoundManager::MusicVolume = 50;
    }
    SoundManager::SetMusicVolume();
}

void SoundManager::MusicVolumeDown()
{
    SoundManager::MusicVolume -= 5;
    if (SoundManager::MusicVolume < 0)
    {
        SoundManager::MusicVolume = 0;
    }
    SoundManager::SetMusicVolume();

}

void SoundManager::SetMusicVolume()
{
    MenuMusic1.setVolume(SoundManager::MusicVolume);
    MenuMusic2.setVolume(SoundManager::MusicVolume);
    MenuMusic3.setVolume(SoundManager::MusicVolume);
    MenuMusic4.setVolume(SoundManager::MusicVolume);
}

void SoundManager::SetEffectsVolume()
{
    CheerSound.setVolume(SoundManager::EffectsVolume);
    GoalSound.setVolume(SoundManager::EffectsVolume);
    ShotSound.setVolume(SoundManager::EffectsVolume);
    PassSound.setVolume(SoundManager::EffectsVolume);
    HitSound1.setVolume(SoundManager::EffectsVolume);
    HitSound2.setVolume(SoundManager::EffectsVolume);
    HitSound3.setVolume(SoundManager::EffectsVolume);
    MissSound.setVolume(SoundManager::EffectsVolume);
    BallHittingWallSound.setVolume(SoundManager::EffectsVolume);
    CountDownSound.setVolume(SoundManager::EffectsVolume);
}

void SoundManager::PlayMenuMusic()
{
    if (doPlayMusic)
    {
        switch (CurrentTrackNumber)
        {
        case 1:
            if (MenuMusic1.getStatus() == sf::Music::Stopped)
            {
                //increment to next track
                CurrentTrackNumber = 2;

                //stop current song
                MenuMusic1.stop();

                //play next song
                MenuMusic2.play();
            }

            break;
        case 2:
            if (MenuMusic2.getStatus() == sf::Music::Stopped)
            {
                CurrentTrackNumber = 3;
                MenuMusic2.stop();
                MenuMusic3.play();
            }
            break;
        case 3:
            if (MenuMusic3.getStatus() == sf::Music::Stopped)
            {
                CurrentTrackNumber = 4;
                MenuMusic3.stop();
                MenuMusic4.play();
            }
            break;
        case 4:
            if (MenuMusic4.getStatus() == sf::Music::Stopped)
            {
                CurrentTrackNumber = 1;
                MenuMusic4.stop();
                MenuMusic1.play();
            }
            break;
        }
    }
}
