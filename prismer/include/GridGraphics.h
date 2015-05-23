#pragma once

#include <memory>
#include <map>

namespace Arya {
    class Model;
}

namespace Prismer {

using std::weak_ptr;
using std::shared_ptr;

class Grid;
class TileEntity;

class GridEntity :
    public std::enable_shared_from_this<GridEntity>
    {
    public:
        GridEntity(weak_ptr<Grid> grid);
        
        vec2 boardToWorld(int row, int col);

        void update();

        void init();

        float getScale() const {
            return _scale;
        }

        shared_ptr<Arya::Model> getBaseTile() const {
            return baseTile;
        }

        shared_ptr<Arya::Model> getActiveTile() const {
            return activeTile;
        }

    private:
        weak_ptr<Grid> _grid;
        float _scale = 0.0f;

        vector<shared_ptr<TileEntity>> tile_entities;
        shared_ptr<Arya::Model> baseTile;
        shared_ptr<Arya::Model> activeTile;
};

} // namespace Prismer
