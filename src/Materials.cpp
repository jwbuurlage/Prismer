#include "Materials.h"
#include "Locator.h"
#include "Textures.h"
#include "Files.h"
#include "common/Logger.h"

namespace Arya
{
    shared_ptr<Material> Material::create(string filename)
    {
        return Locator::getMaterialManager().getMaterial(filename);
    }

    shared_ptr<Material> Material::create(const vec4& color)
    {
        return make_shared<Material>(Locator::getTextureManager().createTexture(color));
    }

    shared_ptr<Material> Material::createFromHandle(unsigned int handle)
    {
        return make_shared<Material>(Texture::createFromHandle(handle));
    }

    bool MaterialManager::init()
    {
        loadResource("default");
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

    shared_ptr<Material> MaterialManager::createMaterial(const vec4& color)
    {
        return Material::create(color);
    }

    void MaterialManager::cleanup()
    {
        unloadAll();
    }

}
