#include <Arya.h>
#include <cmath>

#include "GameLogger.h"
#include "Tile.h"
#include "TileGraphics.h"
#include "GridGraphics.h"
#include "Grid.h"

namespace Prismer {

using std::shared_ptr;

GridEntity::GridEntity(weak_ptr<Grid> grid)
    : _grid(grid)
{
    _scale = 10.0f;

    auto& root = Arya::Locator::getRoot();

    baseTile = root.getModelManager()->getModel("hexagon");
    baseTile->setMaterial(root.getMaterialManager()->createMaterial(vec4(0.25f, 0.0f, 0.5f, 0.5f)));

    activeTile = baseTile->clone();

    using Arya::ShaderProgram;
    auto myShader = make_shared<ShaderProgram>(
            "../prismer/shaders/custom.vert",
            "../prismer/shaders/custom.frag");
    if (!myShader->isValid()) {
        myShader = nullptr;
        LogError << "Could not load custom shader." << Arya::endLog;
    }
    else
    {
        myShader->enableUniform(Arya::UNIFORM_MOVEMATRIX | Arya::UNIFORM_VPMATRIX | Arya::UNIFORM_TEXTURE);
        myShader->addUniform4fv("customUniform", [this](Arya::Entity* ent){
                TileEntity* t = (TileEntity*)ent->getUserData();
                shared_ptr<Tile> tile = t->getTile().lock();

                if (!tile)
                    return vec4(1.0f, 0.0f, 0.0f, 1.0f);

                auto ans = vec4(0.0);
                if (tile->getInfo()->isActive())
                    ans += vec4(1.0f, 0.0f, 0.0f, 1.0f);
                if (tile->getInfo()->isHovered())
                    ans += vec4(0.5f, 0.5f, 0.5f, 1.0f);

                return ans;

                });
        activeTile->setShaderProgram(myShader);
    }
}

void GridEntity::init()
{
    shared_ptr<Grid> gridl = _grid.lock();
    if (gridl) {
        for (auto& tile : gridl->getTiles())
        {
            // create tile graphic
            auto t_ent = make_shared<TileEntity>(weak_ptr<Tile>(tile), 
                    weak_ptr<GridEntity>(shared_from_this()));
            tile_entities.push_back(t_ent);

            // link tile graphic to tile
            tile->setEntity(t_ent);
        }
    }
}

vec2 GridEntity::boardToWorld(int x, int y)
{
    int col = -y;
    int row = x + y;

    float x_world = 0.0;
    float y_world = 0.0;
    shared_ptr<Grid> l_grid = _grid.lock();
    if (l_grid) {

        auto col_mid = -l_grid->getHeight() / 2;
        auto row_mid = (l_grid->getHeight() + l_grid->getWidth()) / 2;

        float h = _scale * 2;
        float w = 0.5 * sqrt(3) * h;
        x_world = (col - col_mid + 0.5 * (row - row_mid)) * w;
        y_world = 0.75 * (row - row_mid) * h;
    } else {
        // FIXME: error
    }

    return vec2(x_world, y_world);
}

} // namespace Prismer
