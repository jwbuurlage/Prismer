#pragma once
#include <memory>

namespace Arya
{
    using std::unique_ptr;
    using std::make_unique;

    class Sound
    {

    };


    class AudioManager
    {
        public:
            AudioManager();
            ~AudioManager();

            bool init();

            void playSound(Sound* snd);
        private:
            //pimpl
            struct Impl;
            unique_ptr<Impl> impl;
    };
}
