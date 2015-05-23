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
    activeTile->setMaterial(root.getMaterialManager()->createMaterial(vec4(0.5f, 0.0f, 0.0f, 0.5f)));
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
        
        auto col_mid = -l_grid->getWidth() / 2;
        auto row_mid = (l_grid->getHeight() + l_grid->getWidth()) / 2;

        float offsetX = _scale * sqrt(3) * (row_mid + 0.5f * col_mid);
        float offsetY = -_scale * 1.5f * col_mid;

        x_world = -offsetX + _scale * sqrt(3) * (row + 0.5f * col);
        y_world = -offsetY - _scale * 1.5f * col;

        float h = _scale * 2;
        float w = 0.5 * sqrt(3) * h;
        x_world = (col + 0.5 * row) * w;
        y_world = 0.75 * row * h;
    } else {
        // FIXME: error
    }

    return vec2(x_world, y_world);
}

} // namespace Prismer
