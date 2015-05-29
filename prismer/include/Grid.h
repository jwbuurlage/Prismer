#pragma once

#include <memory>

#include "Tile.h"

namespace Prismer {

using std::shared_ptr;

class GridEntity;
class GridInput;

class Grid
    : public std::enable_shared_from_this<Grid>
{
    public:
        Grid(int width, int height);

        void init();

        int getWidth() const {
            return _width;
        }

        int getHeight() const {
            return _height;
        }

        vector<shared_ptr<Tile>>& getTiles() {
            return _tiles;
        }

        shared_ptr<Tile> getTile(int x, int y);

        shared_ptr<Tile> getNeighbor(int x, int y, TileDirection dir);

        int distance(int x1, int y1, int x2, int y2) {
            int z1 = -x1 - y1;
            int z2 = -x2 - y2;
            return (abs(x1 - x2) + abs(y2 - y1) + abs(z2 - z1)) / 2;
        }

        void setEntity(shared_ptr<GridEntity> entity) {
            _entity = entity;
        }

        shared_ptr<GridEntity> getEntity() const {
            return _entity;
        }

        void setInput(shared_ptr<GridInput> input) {
            _input = input;
        }

        vector<shared_ptr<Tile>> getVision(shared_ptr<Tile> origin);

    private:
        int _width;
        int _height;

        vector<shared_ptr<Tile>> _tiles;

        shared_ptr<GridEntity> _entity;
        shared_ptr<GridInput> _input;
};

} // namespace Prismer
