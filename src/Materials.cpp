#include "Materials.h"
#include "Locator.h"
#include "Textures.h"
#include "Files.h"
#include "common/Logger.h"

namespace Arya
{
    bool MaterialManager::init()
    {
        return true;
    }

    void MaterialManager::loadMaterials(const vector<string>& filenames)
    {
        for(unsigned int i=0; i<filenames.size(); i++)
            getResource(filenames[i]);
    }

    shared_ptr<Material> MaterialManager::loadResource(string filename)
    {
        shared_ptr<Material> mat = make_shared<Material>(Locator::getTextureManager().getTexture(filename));
        addResource(filename, mat);
        return mat;
    }

    void MaterialManager::cleanup()
    {
        unloadAll();
    }

}
