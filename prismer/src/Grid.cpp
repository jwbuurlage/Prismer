#include <memory>
#include <vector>
#include <algorithm>
#include <queue>

using std::vector;

#include "Grid.h"
#include "GridGraphics.h"

namespace Prismer {

using std::make_shared;

Grid::Grid(int width, int height)
    : _width(width), _height(height)
{ }

void Grid::init() {
    // create using grid system
    // width is #x
    // height is #y
    // z is used 'as implied'

    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            auto tile = make_shared<Tile>(x, y, shared_from_this());
            _tiles.push_back(tile);
        }
    }
}


shared_ptr<Tile> Grid::getTile(int x, int y)
{
    if (x < 0 || y < 0 || x >= _width || y >= _height)
        return nullptr;

    auto index = x * _height + y;

    if (_tiles.size() < (unsigned int)(index + 1))
        return nullptr;

    return _tiles[index];
}

shared_ptr<Tile> Grid::getNeighbor(int x, int y, TileDirection dir)
{
    switch(dir) {
        case TileDirection::left:
            return getTile(x - 1, y + 1);
            break;

        case TileDirection::top_left:
            return getTile(x, y + 1);
            break;

        case TileDirection::top_right:
            return getTile(x + 1, y);
            break;

        case TileDirection::right:
            return getTile(x + 1, y - 1);
            break;

        case TileDirection::bottom_right:
            return getTile(x, y - 1);
            break;

        case TileDirection::bottom_left:
            return getTile(x - 1, y);
            break;

        default:
            return nullptr;
            break;
    }
}

//! Get the visibility for the desired object
//! @param origin The starting tile.
//! @param tileVisionInfo The additional information, such as the maximum visible range.
//! @return List of visible tiles.
//! A tile is visible if the line between the origin and the tile in question does not intersect any tile that blocks visibility.
//! The origin itself is always included in the list of visible tiles.
//! TODO: get vision range from unit on origin.
vector<shared_ptr<Tile>> Grid::getVision(shared_ptr<Tile> origin)
{
    vector<shared_ptr<Tile>> result;
    int range = 3; 
    for(int k = -range; k + 1 < range; k++)
    {
        for(int l = std::max(-range,-k-range); l + 1 < std::min(range, range -k) ; l++)
        {
            int originX = origin->getX();
            int originY = origin->getY();
            if( getTile(k + originX, l + originY) == nullptr)
            {
                continue;
            }
            int distance = (abs(k) + abs(l) + abs(k-l)/2);
            float step = 1/(float(distance) + 1);
            //GameLogDebug << "distance = " << distance << ",  and step = " << step << endLog;
            bool isTargetVisible = true;
            for( int n = 1; n < distance + 1; n++) //Change this if tiles that block visibility should be invisible themselves.
            {
                //GameLogDebug << "x = " << targetX << ",  y = " << targetY << endLog;
                auto target = getEntity()->worldToBoard(n*k*step + originX, n*l*step + originY);
                if(target->getInfo()->isVisible()==false)
                {
                    isTargetVisible=false;
                    break;
                }
            }
            if(isTargetVisible==true)
            {
                result.push_back(getTile(k + originX, l + originY));
            }
        }
    }
    //GameLogDebug << "Ben ik weer" << endLog;
    return result;
}

//! Get the movement range and cost for each tile in range for the desired unit.
//vector<TileCost*> getMoves(Tile origin, TileMovesInfo tileMovesInfo);
//{
//    vector<TileCost> result;
//    priority_queue<TileCost> fringes(1,Tilecost(origin,0));
//    int moveRange = tileMovesInfo.maxRange;
//    while(!fringes.empty) //As long as the fringes are not empty keep considering neighbours.
//    {
//        TileCost current = fringes.top(); //Get a highest priority element.
//        fringes.pop(); //Remove the highest priority element just obtained.
//        for(int n = 1; n <= 6; n++)
//        {
//            
//        }
//    }
//}
} // namespace Prismer
