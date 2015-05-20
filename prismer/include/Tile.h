#pragma once

#include <vector>
#include <memory>

#include "Colors.h"

namespace Prismer {

using std::vector;
using std::shared_ptr;
using std::weak_ptr;

class Grid;
class Unit;

class TileInfo {
    public:
        TileInfo() {
            _resources.push_back(ColorID::red);
        }

        bool hasResource() {
            return !_resources.empty();
        }

        ColorID popResource() {
            if (_resources.empty())
                return ColorID::na;

            auto color = _resources.back();
            _resources.pop_back();

            return color;
        }

    private:
        vector<ColorID> _resources;
        shared_ptr<Unit> _unit = 0;
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

    private:
        int _x;
        int _y;

        shared_ptr<TileInfo> _info;
        weak_ptr<Grid> _grid;
};



} // namespace Prismer
