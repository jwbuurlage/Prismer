#pragma once

#include <memory>

namespace Arya {
    class Entity;
    class EntityUserData;
}

namespace Prismer {

using std::weak_ptr;

class GridEntity;
class Tile;

class TileEntity
    : public Arya::EntityUserData {
    public:
        TileEntity(weak_ptr<Tile> tile,
                weak_ptr<GridEntity> grid_entity);
        
        void update();

        weak_ptr<Tile> getTile() const {
            return _tile;
        };

    private:
        weak_ptr<Tile> _tile;
        weak_ptr<GridEntity> _grid_entity;
        shared_ptr<Arya::Entity> _entity;
};

} // namespace Prismer
