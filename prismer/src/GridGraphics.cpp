#include <Arya.h>
#include <cmath>

#include "GameLogger.h"
#include "GridGraphics.h"
#include "Grid.h"

namespace Prismer {

GridEntity::GridEntity(shared_ptr<Grid> grid)
    : _grid(grid)
{
    _scale = 10.0f;

    auto& root = Arya::Locator::getRoot();

    auto hexagon = root.getModelManager()->getModel("hexagon");
    hexagon->setMaterial(root.getMaterialManager()->createMaterial(vec4(0.25f, 0.0f, 0.5f, 0.5f)));

    auto hexagon_clone = hexagon->clone();
    hexagon_clone->setMaterial(root.getMaterialManager()->createMaterial(vec4(1.0f, 0.0f, 0.5f, 0.5f)));

    for (const auto& tile : _grid->getTiles())
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

vec2 GridEntity::boardToWorld(int row, int col)
{
    float offsetX = sqrt(3) * 0.5 * _grid->getWidth() * _scale;
    float offsetY = _grid->getHeight() * _scale;

    float x = -offsetX + _scale * sqrt(3) * (row + 0.5f * col);
    float y = -offsetY + _scale * 1.5f * col;

    return vec2(x, y);
}

} // namespace Prismer
