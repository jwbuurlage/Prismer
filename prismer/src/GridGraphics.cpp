#include <Arya.h>
#include <cmath>

#include "GameLogger.h"
#include "GridGraphics.h"
#include "Grid.h"

namespace Prismer {

using std::shared_ptr;

GridEntity::GridEntity(weak_ptr<Grid> grid)
    : _grid(grid)
{
    _scale = 10.0f;

    auto& root = Arya::Locator::getRoot();

    auto hexagon = root.getModelManager()->getModel("hexagon");
    hexagon->setMaterial(root.getMaterialManager()->createMaterial(vec4(0.25f, 0.0f, 0.5f, 0.5f)));

    auto hexagon_clone = hexagon->clone();
    hexagon_clone->setMaterial(root.getMaterialManager()->createMaterial(vec4(1.0f, 0.0f, 0.5f, 0.5f)));

    shared_ptr<Grid> gridl = _grid.lock();
    if (gridl) {
        for (const auto& tile : gridl->getTiles())
        {
            auto ent = root.getWorld()->createEntity();
            ent->setPosition(vec3(boardToWorld(tile.getX(), tile.getY()), 0.0f));
            ent->setYaw(M_PI / 6);

            if (tile.getX() == 3)
                ent->setGraphics(hexagon_clone);
            else
                ent->setGraphics(hexagon);

            ent->getGraphics()->setScale(0.95f * _scale);
        }
    }
}

vec2 GridEntity::boardToWorld(int row, int col)
{
    float x = 0.0;
    float y = 0.0;
    shared_ptr<Grid> gridl = _grid.lock();
    if (gridl) {

        float offsetX = sqrt(3) * 0.5 * gridl->getWidth() * _scale;
        float offsetY = gridl->getHeight() * _scale;

        x = -offsetX + _scale * sqrt(3) * (row + 0.5f * col);
        y = -offsetY + _scale * 1.5f * col;
    } else {
        // FIXME: error
    }

    return vec2(x, y);
}

} // namespace Prismer
