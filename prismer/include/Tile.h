#pragma once

#include <vector>
#include <memory>

#include "GameLogger.h"
#include "Colors.h"

namespace Prismer {

// asume pointy tops
enum TileDirection {
    left,
    top_left,
    top_right,
    right,
    bottom_right,
    bottom_left
};

using std::vector;
using std::shared_ptr;
using std::weak_ptr; 

class Grid;
class Unit;
class Tile;
class TileEntity;

class TileInfo {
    public:
        TileInfo() {
            _resources.push_back(ColorID::red);
        }

        bool hasResource() {
            return !_resources.empty();
        }

        bool hasUnit() {
            if(_unit)
                return true;
            return false;
        }

        void setUnit(shared_ptr<Unit> unit) {
            if(unit && hasUnit()) {
                GameLogError << "Tile " << this << " already has unit." << endLog;
                return;
            }
            _unit = unit;
        }

        shared_ptr<Unit> getUnit() const {
            return _unit;
        }

        ColorID popResource() {
            if (_resources.empty())
                return ColorID::na;

            auto color = _resources.back();
            _resources.pop_back();

            return color;
        }

        bool isActive() const {
            return _active;
        }

        bool isHovered() const {
            return _hovered;
        }

    friend class Tile;
    protected:
        bool _active = false;
        bool _hovered = false;
        shared_ptr<Unit> _unit = nullptr;
        vector<ColorID> _resources;
};

class Tile {
    public:
        Tile(int x, int y, weak_ptr<Grid> grid);

        int getX() const {
            return _x;
        }

        int getY() const {
            return _y;
        }

        int getZ() const {
            return - (_x + _y);
        }

        shared_ptr<TileInfo> getInfo() const {
            return _info;
        }

        shared_ptr<Tile> getNeighbor(TileDirection dir);

        int distance(shared_ptr<Tile> rhs);

        void setEntity(shared_ptr<TileEntity> entity) {
            _entity = entity;
        }

        // FIXME: Technically this is stuff for graphics, options:
        // 1. make tile larger than necessary (and info)
        // 2. couple input and graphics..
        // not sure which is better
        void setActive(bool active);
        void setHovered(bool hovered);

    private:
        int _x;
        int _y;

        shared_ptr<TileInfo> _info;
        shared_ptr<TileEntity> _entity;
        weak_ptr<Grid> _grid;
};

} // namespace Prismer
