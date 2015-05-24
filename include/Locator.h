#pragma once

namespace Arya
{
    class Root;
    class World;
    class Console;
    class FileSystem;
    class InputSystem;
    class ModelManager;
    class MaterialManager;
    class TextureManager;
    class Audio;

    class Locator
    {
        public:
            static Root& getRoot() { return *root; }
            static World& getWorld() { return *world; }
            static Console& getConsole() { return *console; }
            static FileSystem& getFileSystem() { return *fileSystem; }
            static InputSystem& getInputSystem() { return *inputSystem; }
            static ModelManager& getModelManager() { return *modelManager; }
            static MaterialManager& getMaterialManager() { return *materialManager; }
            static TextureManager& getTextureManager() { return *textureManager; }
            static Audio& getAudio() { return *audio; }

            static void provide(Root* r) { root = r; }
            static void provide(World* r) { world = r; }
            static void provide(Console* r) { console = r; }
            static void provide(FileSystem* f) { fileSystem = f; }
            static void provide(InputSystem* i) { inputSystem = i; }
            static void provide(ModelManager* m) { modelManager = m; }
            static void provide(MaterialManager* m) { materialManager = m; }
            static void provide(TextureManager* t) { textureManager = t; }
            static void provide(Audio* a) { audio = a; }
        private:
            static Root* root;
            static World* world;
            static Console* console;
            static FileSystem* fileSystem;
            static InputSystem* inputSystem;
            static ModelManager* modelManager;
            static MaterialManager* materialManager;
            static TextureManager* textureManager;
            static Audio* audio;
    };
}
