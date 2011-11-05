/*
Copyright (C) 2005 Wolfgang Becker <uafr@gmx.de>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <config.h>

#include "Sound.hpp"

#include <assert.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "PhysfsStream/PhysfsSDL.hpp"

#include <SDL_mixer.h>
#include <physfs.h>
#include "Config.hpp"

#include <android/log.h>
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "JNI_DEBUGGING", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,   "JNI_DEBUGGING", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,    "JNI_DEBUGGING", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,    "JNI_DEBUGGING", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,   "JNI_DEBUGGING", __VA_ARGS__)

Sound* soundPtr = 0;

Sound *getSound()
{
  return soundPtr;
}

int
Sound::soundThread(void* ptr)
{
    Sound* sound = (Sound*) ptr;
    sound->loadWaves();
    return 0;
}
        
void
Sound::loadWaves() {
    //Load Waves
    std::string filename;
    std::string directory = "sounds/";
    std::string fullname;
    Mix_Chunk *chunk;
    SDL_RWops* file;
    char **rc = PHYSFS_enumerateFiles( directory.c_str() );
    char **i;
    for (i = rc; *i != NULL; i++) {
        fullname = directory;
        fullname.append( *i );
        filename.assign( *i );

        if(PHYSFS_isDirectory(fullname.c_str()))
            continue;

        try {
            //LOGD("[Lincity]Sound::loadWaves() - Loading %s\n",fullname.c_str());
            file = getPhysfsSDLRWops( fullname.c_str() );
            chunk = Mix_LoadWAV_RW( file, 1);
            if(!chunk) {
                std::stringstream msg;
                msg << "Couldn't read soundfile '" << fullname
                    << "': " << SDL_GetError();
                LOGD("[Lincity]Sound::loadWaves() Error : could not load %s\n",fullname.c_str());
                throw std::runtime_error(msg.str());
            }

            std::string idName = getIdName( filename );
            waves.insert( std::pair<std::string,Mix_Chunk*>(idName, chunk) );
        } catch(std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    PHYSFS_freeList(rc);
    LOGD("[Lincity]Sound::loadWaves() - Exiting\n");
}

Sound::Sound()
    : currentMusic(0)
{
    LOGD("[Lincity]Sound::Sound()\n");
    assert( soundPtr == 0);
    soundPtr = this;
    loaderThread = 0;

    //Load Sound
    audioOpen = false;
    /* Open the audio device */
    if (Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        return;
    } else {
        audioOpen = true;
        loaderThread = SDL_CreateThread(soundThread, this);
    }

    setMusicVolume(getConfig()->musicVolume);
    setSoundVolume(getConfig()->soundVolume);

    // for now...
    //playMusic("01 - pronobozo - lincity.ogg");
    LOGD("[Lincity]Sound::Sound() - Before playMusic()\n");
    //playMusic( getConfig()->playSongName );
    LOGD("[Lincity]Sound::Sound() - After playMusic()\n");
}

Sound::~Sound()
{
    //SDL_KillThread( loaderThread );
    SDL_WaitThread( loaderThread, NULL );
    if(currentMusic)
        Mix_FreeMusic(currentMusic);

    if( soundPtr == this )
    {
        soundPtr = 0;
    }
    for (chunks_t::iterator i = waves.begin(); i != waves.end(); i++) {
        Mix_FreeChunk( i->second );
    }
    if ( audioOpen ) {
        Mix_CloseAudio();
        audioOpen = false;
    }
}

/*
 *  Playback an Audio-Effect.
 *  Name is the Name of an Audiofile from sounds/ minus .wav
 *  and without trailing Numbers. If there are eg.
 *  beep1.wav, beep2.wav, beep3.wav
 *  playSound( "beep" ) would pick one of the three Files randomly
 */
void
Sound::playSound(const std::string& name) {
    LOGD("[Lincity]Sound::playSound() %s\n",name.c_str());
    if( !getConfig()->soundEnabled ){
        return;
    }
    if( !audioOpen ){
        return;
    }

    chunks_t::size_type count = waves.count( name );
    if ( count == 0 ) {
        std::cout << "Couldn't find audio file '" << name << "'\n";
        return;
    }

    chunks_t::iterator it = waves.find(name);
    for (int i = rand() % count; i > 0; i--) {
        it++;
    }

    Mix_Volume( 0, getConfig()->soundVolume );

    LOGD("[Lincity]Sound::playSound() Before Mix_PlayChannel() %d %d\n", getConfig()->soundVolume,it->second);
    if (Mix_PlayChannel( 0, it->second, 0 ) == -1)
    {
       LOGD("[Lincity]Sound::playSound() Error : unable to play sound %s\n",SDL_GetError());
    }
    LOGD("[Lincity]Sound::playSound() After Mix_PlayChannel()\n");
}

/*
 * Get ID-String for a given Filename.
 */
std::string
Sound::getIdName(const std::string& filename)
{
    std::string::size_type pos = filename.find_first_of(".0123456789");

    return filename.substr(0, pos);
}

void
Sound::playMusic(const std::string& name)
{
    LOGD("[Lincity]Sound::playMusic() %s\n",name.c_str());
    getConfig()->playSongName = name;
    if(!audioOpen)
        return;

    musicFile = name;
    if(getConfig()->musicEnabled) {
        if(currentMusic) {
            Mix_FreeMusic(currentMusic);
            currentMusic = 0;
        }
        if(musicFile == "")
            return;

        // transform filename... because the music commands in SDL_Mixer don't
        // support reading callbacks to read from physfs directly
        std::string filename = "music/";
        filename += name;
        const char* dir = PHYSFS_getRealDir(filename.c_str());
        if(dir == 0) {
            std::cerr << "Warning couldn't find music file '" << name << "'.\n";
            return;
        }
        filename = dir;
        filename += "/music/";
        filename += name;
        
        LOGD("[Lincity]Sound::playMusic() Before Mix_LoadMUS() %s\n",filename.c_str());
        currentMusic = Mix_LoadMUS(filename.c_str());
        if(currentMusic == 0) {
            std::cerr << "Couldn't load music file '" << filename << "': "
                << SDL_GetError() << "\n";
            return;
        }

        LOGD("[Lincity]Sound::playMusic() Before Mix_PlayMusic()\n");
        Mix_PlayMusic(currentMusic, -1);
        LOGD("[Lincity]Sound::playMusic() After Mix_PlayMusic()\n");
    }
}

void
Sound::enableMusic(bool enabled)
{
    if(getConfig()->musicEnabled == enabled)
        return;
    getConfig()->musicEnabled = enabled;

    if(!audioOpen)
        return;

    if(enabled) {
        playMusic(musicFile);
    } else {
        if(Mix_PlayingMusic()) {
            Mix_FadeOutMusic(1000);
        }
    }
}

void
Sound::setMusicVolume(int vol)
{
    if(vol < 0 || vol > 100)
        throw std::runtime_error("Music volume out of range (0..100)");

    getConfig()->musicVolume = vol;
    float volvalue = vol * MIX_MAX_VOLUME / 100.0;
    Mix_VolumeMusic(static_cast<int>(volvalue));
}

void
Sound::setSoundVolume(int vol)
{
    if(vol < 0 || vol > 100)
        throw std::runtime_error("Music volume out of range (0..100)");

    getConfig()->soundVolume = vol;
    float volvalue = vol * MIX_MAX_VOLUME / 100.0;
    Mix_Volume(-1, static_cast<int>(volvalue));
}

