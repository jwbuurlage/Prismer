#include "Tile.h"
#include "Grid.h"

namespace Prismer
{

Tile::Tile(int x, int y, weak_ptr<Grid> grid)
    : _x(x), _y(y), _grid(grid)
{

}

} // namespace Prismer
