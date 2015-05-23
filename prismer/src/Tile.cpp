#include "Tile.h"
#include "TileGraphics.h"
#include "Grid.h"
#include "Unit.h"

namespace Prismer
{

Tile::Tile(int x, int y, weak_ptr<Grid> grid)
    : _x(x), _y(y), _grid(grid)
{
    _info = make_shared<TileInfo>();
}

void Tile::setActive(bool active) {
    _info->_active = active;

    // if info has unit, we want to activate it
    if (_info->_unit) {
        if(active)
            _info->_unit->activate();
        else
            _info->_unit->deactivate();
    }

    _entity->update();
}

void Tile::setHovered(bool hovered) {
    _info->_hovered = hovered;
    _entity->update();
}

shared_ptr<Tile> Tile::getNeighbor(TileDirection dir)
{
    if (_grid.expired())
        return nullptr;

    auto l_grid = _grid.lock();
    return l_grid->getNeighbor(_x, _y, dir);
}

int Tile::distance(shared_ptr<Tile> tile)
{
    if(auto l_grid = _grid.lock())  {
        return l_grid->distance(_x, _y, tile->getX(), tile->getY());
    } else {
        return 0;
    }
}


} // namespace Prismer
