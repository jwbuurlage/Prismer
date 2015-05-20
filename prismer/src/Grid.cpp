#include "Grid.h"

namespace Prismer {

Grid::Grid(int width, int height)
    : _width(width), _height(height)
{ }

void Grid::init() {
    for (int i = 0; i < _width; ++i) {
        for (int j = 0; j < _width; ++j) {
            _tiles.push_back(Tile(i, j - i / 2, shared_from_this()));
        }
    }
}

} // namespace Prismer
