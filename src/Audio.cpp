#include "Audio.h"
#include "common/Logger.h"
#include <SDL2/SDL_mixer.h>

namespace Arya
{
    struct AudioManager::Impl
    {
        bool mixerInitialized;
        bool mixerOpened;
    };

    AudioManager::AudioManager() : impl(make_unique<Impl>())
    {
        impl->mixerInitialized = false;
        impl->mixerOpened = false;
    }

    AudioManager::~AudioManager()
    {
        if (impl->mixerOpened)
            Mix_CloseAudio();
        if (impl->mixerInitialized)
            Mix_Quit();
    }

    bool AudioManager::init()
    {
        //Mix_SetSoundFonts("./");
        if (!impl->mixerInitialized)
        {
            //int flags = (MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC);
            int flags = 0;
            int loaded = Mix_Init(flags);
            if ((loaded & flags) != flags)
            {
                LogError << "Unable to load audio: Mix_Init failed. Error message: " << Mix_GetError() << endLog;
                return false;
            }
            impl->mixerInitialized = true;
        }

        if (!impl->mixerOpened)
        {
            if (!Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024))
            {
                LogError << "Unable to load audio: Mix_OpenAudio failed. Error message: " << Mix_GetError() << endLog;
                return false;
            }
            impl->mixerOpened = true;
        }

        return true;
    }

    void AudioManager::playSound(Sound* snd)
    {
        (void)snd;
        return;
    }

}
