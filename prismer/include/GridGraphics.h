#pragma once

#include <memory>

namespace Prismer {

using std::weak_ptr;

class Grid;

class GridEntity {
    public:
        GridEntity(weak_ptr<Grid> grid);
        
        vec2 boardToWorld(int row, int col);

        void update();

    private:
        weak_ptr<Grid> _grid;
        float _scale = 0.0f;
        //vector<Tile> tiles;
};

} // namespace Prismer
