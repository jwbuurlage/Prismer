#include <cmath>
#include <Arya.h>

#include "GridGraphics.h"
#include "TileGraphics.h"
#include "Tile.h"

namespace Prismer {

TileEntity::TileEntity(weak_ptr<Tile> tile,
        weak_ptr<GridEntity> grid_entity)
    : _tile(tile), _grid_entity(grid_entity)
{
    auto l_grid = _grid_entity.lock();
    auto l_tile = _tile.lock();

    _entity = Arya::Entity::create();
    _entity->setPosition(vec3(l_grid->boardToWorld(l_tile->getX(), l_tile->getY()), 0.0f));
    _entity->setYaw(M_PI / 6);
    _entity->setGraphics(l_grid->getBaseTile());
    _entity->getGraphics()->setScale(0.95f * l_grid->getScale());
    _entity->setUserData(this);
}

void TileEntity::update()
{
    // FIXME: error
    if (_tile.expired() || _grid_entity.expired())
        return;

    auto l_tile = _tile.lock();
    auto l_grid = _grid_entity.lock();

    // depending on state setGraphics:
    if (l_tile->getInfo()->isActive() ||
            l_tile->getInfo()->isHovered()) {
        _entity->setGraphics(l_grid->getActiveTile());
        _entity->getGraphics()->setScale(0.95f * l_grid->getScale());
    } else {
        _entity->setGraphics(l_grid->getBaseTile());
        _entity->getGraphics()->setScale(0.95f * l_grid->getScale());
    }
}

} // namespace Prismer
