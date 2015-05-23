#include "Locator.h"

namespace Arya
{
    Root* Locator::root = 0;
    World* Locator::world = 0;
    FileSystem* Locator::fileSystem = 0;
    InputSystem* Locator::inputSystem = 0;
    ModelManager* Locator::modelManager = 0;
    MaterialManager* Locator::materialManager = 0;
    TextureManager* Locator::textureManager = 0;
    Audio* Locator::audio = 0;
}
