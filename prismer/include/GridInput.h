#pragma once

#include <AryaBinding.h>
#include <vector>
#include <memory>

// FIXME: move TileDireftion to separate file
#include "Tile.h"

namespace Prismer {

using std::vector;
using std::weak_ptr;
using std::shared_ptr;

class Grid;
class Tile;

class GridInput 
    : public std::enable_shared_from_this<GridInput> {
    public:
        GridInput(weak_ptr<Grid> grid);
        ~GridInput();

        void activate();
        void deactivate();

        shared_ptr<Tile> getActive() const {
            return _active;
        }

        shared_ptr<Tile> getHovered() const {
            return _hovered;
        }
        
        void setActive(shared_ptr<Tile> tile);

    private:
        weak_ptr<Grid> _grid;
        
        void setHovered(shared_ptr<Tile> tile);
        void setHovered(TileDirection dir);


        shared_ptr<Tile> _active = nullptr;
        shared_ptr<Tile> _hovered = nullptr;

        vector<Arya::InputBinding> keyBindings;
};

} // namespace Prismer
