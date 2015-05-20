#pragma once

#include "Tile.h"

namespace Prismer {

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

        const vector<Tile>& getTiles() const {
            return _tiles;
        }

    private:
        int _width;
        int _height;
        vector<Tile> _tiles;
};

} // namespace Prismer
