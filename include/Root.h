#pragma once

#include <functional>

namespace Arya
{
    class World;
    class InputSystem;
    class Graphics;
    class CommandHandler;
    class Console;
    class Interface;
    class FileSystem;
    class ModelManager;
    class MaterialManager;
    class TextureManager;
    class AudioManager;

    struct SDLValues; //This prevents including SDL headers here

    class Root
    {
        public:
            Root();
            ~Root();

            //! If init returns false, Root or one of its subsystems failed to initialize
            //! In this case, the Root class must be deleted
            bool init(const char* windowTitle, int _width, int _height, bool _fullscreen);

            void gameLoop( std::function<void(float)> callback );
            void stopGameLoop();

            bool getFullscreen() const { return fullscreen; }
            void setFullscreen(bool fullscreen = true);

            int getWindowWidth() const { return windowWidth; }
            int getWindowHeight() const { return windowHeight; }

            World*       getWorld() const { return world; }
            Interface*   getInterface() const { return interface; }
            Graphics*    getGraphics() const { return graphics; }
            InputSystem* getInputSystem() const { return inputSystem; }
            FileSystem*  getFileSystem() const { return fileSystem; }
            ModelManager* getModelManager() const { return modelManager; }
            MaterialManager* getMaterialManager() const { return materialManager; }
            TextureManager* getTextureManager() const { return textureManager; }

        private:
            World*       world;
            Interface*   interface;
            Graphics*    graphics;
            CommandHandler* commandHandler;
            Console*     console;
            InputSystem* inputSystem;
            FileSystem*  fileSystem;
            ModelManager* modelManager;
            MaterialManager* materialManager;
            TextureManager* textureManager;
            AudioManager* audioManager;

            bool loopRunning;

            void render();
            void handleEvents();

            int windowWidth;
            int windowHeight;
            bool fullscreen;

            void windowResized(int newWidth, int newHeight);

            SDLValues* sdlValues;

            int timer;
    };
}

