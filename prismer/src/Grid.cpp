#include <memory>
#include <vector>
#include <algorithm>
#include <queue>

//TODO Make the functions:
//axialToRadius:(x,y)-> R,
//axialToAngle:(x,y)-> phi,
//Make the object type:
//PolarCoordinate, consisting of a Radius and an Angle, ordered first by angle and then by Radius
//WallSegment, consisting of a pair of PolarCoordinates representing the begin and endpoints and a float that represents the distance.
//Hardcode an array:
//axialX,YOffset
//In Tile.h add walls!!!
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

/* vector<float> Grid::getVision(shared_ptr<Tile> origin)
{
    int range = 3;
    int originX = origin->getX();
    int originY = origin->getY();
    vector<Tile> nonemptytiles;
    for(int k = -range; k + 1 < range; k++) //First make a list of Tiles that have nontrivial vision information.
    {
        for(int l = std::max(-range, -k-range); l + 1 < std::min(range, range-k); l++)
        {
            Tile targetTile = getTile(k + originX, l + originY);
            if( targetTile == nullptr)
                continue;
            if(target->getInfo()->isVisible()==false)
            {
                nonemptytiles.push_back(targetTile);
            }
        }
    }
    priority_queue<WallSegment> walls;
    while (! nonemptytiles.empty()) //Make a priority queue filled with the edges that block view.
    {
        Tile target = nonemptytiles.back();
        nonemptytiles.pop_back();
        for (int i = 0; i < 6; i++)
        {
            relativeX = target->getX() - originX;
            relativeY = target->getY() - originY;
            if (target->getInfo->wall(i))
            { //CW for clockwise and CCW for counterclockwise
                float targetRadiusCW = axialToRadius(relativeX+axialXOffsetCW(i),relativeY+axialYOffsetCW(i));
                float targetAngleCW = axialToAngle(relativeX+axialXOffsetCW(i),relativeY+axialYOffsetCW(i));
                PolarCoordinateCW targetPolarCoordinateCW = (targetAngleCW, targetRadiusCW);
                float targetRadiusCCW = axialToRadius(relativeX+axialXOffsetCCW(i),relativeY+axialYOffsetCCW(i));
                float targetAngleCCW = axialToAngle(relativeX+axialXOffsetCCW(i),relativeY+axialYOffsetCCW(i));
                PolarCoordinateCCW targetPolarCoordinateCCW = (targetAngleCCW, targetRadiusCCW);
                float targetDistance = (targetRadiusCW + targetRadiusCCW)*0.5;
                WallSegment targetWall = (targetPolarCoordinateCW,targetPolarCoordinateCCW,targetDistance);
                walls.push(targetWall);
            }
        }
    }
    vector<PolarCoordinate> result;
    while (!walls.empty()); //Build the result.
    {
    }
} */

///Old version of getVision
//! Get the visibility for the desired object
//! @param origin The starting tile.
//! @param tileVisionInfo The additional information, such as the maximum visible range.
//! @return List of visible tiles.
//! A tile is visible if the line between the origin and the tile in question does not intersect any tile that blocks visibility.
//! The origin itself is always included in the list of visible tiles.
//! TODO: get vision range from unit on origin.
///vector<shared_ptr<Tile>> Grid::getVision(shared_ptr<Tile> origin)
///{
///    vector<shared_ptr<Tile>> result;
///    int range = 3; 
///    for(int k = -range; k + 1 < range; k++)
///    {
///        for(int l = std::max(-range,-k-range); l + 1 < std::min(range, range -k) ; l++)
///        {
///            int originX = origin->getX();
///            int originY = origin->getY();
///            auto targetTile = getTile(k + originX, l + originY);
///            if( targetTile == nullptr)
///            {
///                continue;
///            }
///            int distance = (abs(k) + abs(l) + abs(k-l)/2);
///            GameLogDebug << "distance = " << distance << endLog;
///            bool isTargetVisible = true;
///            float step = 1.0/float(distance);
///            for( int n = 1; n < distance + 1; n++) //Change this if tiles that block visibility should be invisible themselves.
///            {
///                GameLogDebug << "Target x coordinate is: " << step*float(k)*float(n) + float(originX) << endLog;
///                GameLogDebug << "Target y coordinate is: " << step*float(l)*float(n) + float(originY) << endLog;
///                auto target = getEntity()->worldToBoard(step*float(k)*float(n)+float(originX), step*float(l)*float(n) + float(originY));
///                GameLogDebug << "The tile now being checked is: " << target->getX() << ", " << target->getY() << endLog;
///                if(target->getInfo()->isVisible()==false)
///                {
///                    isTargetVisible=false;
///                    break;
///                }
///            }
///            GameLogDebug << "The target is visible: " << isTargetVisible << endLog;
///            if(isTargetVisible==true)
///            {
///                GameLogDebug << "Target tile coordinates are: " << targetTile->getX() << "  " << targetTile->getY() << endLog;
///                result.push_back(targetTile);
///            }
///        }
///    }
///    return result;
///}

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
