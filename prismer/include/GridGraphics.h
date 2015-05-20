#pragma once

#include <memory>

namespace Prismer {

using std::shared_ptr;

class Grid;

class GridEntity {
    public:
        GridEntity(shared_ptr<Grid> grid);
        
        vec2 boardToWorld(int row, int col);

    private:
        shared_ptr<Grid> _grid;
        float _scale = 0.0f;
        //vector<Tile> tiles;
};

} // namespace Prismer
