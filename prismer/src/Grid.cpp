#include <memory>

#include "Grid.h"

namespace Prismer {

using std::make_shared;

Grid::Grid(int width, int height)
    : _width(width), _height(height)
{ }

void Grid::init() {
    // create using grid system
    // width is #x
    // height is #y
    // z is used 'as implied'

    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            auto tile = make_shared<Tile>(x, y, shared_from_this());
            _tiles.push_back(tile);
        }
    }
}


shared_ptr<Tile> Grid::getTile(int x, int y)
{
    if (x < 0 || y < 0)
        return nullptr;

    auto index = x * _height + y;

    if (_tiles.size() < (unsigned int)(index + 1))
        return nullptr;

    return _tiles[index];
}

shared_ptr<Tile> Grid::getNeighbor(int x, int y, TileDirection dir)
{
    switch(dir) {
        case TileDirection::left:
            return getTile(x - 1, y + 1);
            break;

        case TileDirection::top_left:
            return getTile(x, y + 1);
            break;

        case TileDirection::top_right:
            return getTile(x + 1, y);
            break;

        case TileDirection::right:
            return getTile(x + 1, y - 1);
            break;

        case TileDirection::bottom_right:
            return getTile(x, y - 1);
            break;

        case TileDirection::bottom_left:
            return getTile(x - 1, y);
            break;

        default:
            return nullptr;
            break;
    }
}

} // namespace Prismer
