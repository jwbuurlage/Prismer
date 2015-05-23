#pragma once

#include <memory>

// FIXME: move TileDireftion to separate file
#include "Tile.h"

namespace Prismer {

using std::weak_ptr;
using std::shared_ptr;

class Grid;
class Tile;

class GridInput {
    public:
        GridInput(weak_ptr<Grid> grid);
        ~GridInput();

        void activate();
        void deactivate();

        
    private:
        weak_ptr<Grid> _grid;
        
        void setActive(shared_ptr<Tile> tile);
        void setHovered(shared_ptr<Tile> tile);
        void setActive(TileDirection dir);
        void setHovered(TileDirection dir);


        shared_ptr<Tile> active = nullptr;
        shared_ptr<Tile> hovered = nullptr;
};

} // namespace Prismer
