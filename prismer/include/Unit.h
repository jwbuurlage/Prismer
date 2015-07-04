#pragma once

#include <Arya.h>

#include "GameLogger.h"

#include <memory>

namespace Prismer {

using std::shared_ptr;
using std::weak_ptr;

class GridInput;
class GameSession;
class UnitEntity;
class Tile;
class Faction;

class Unit
    : public std::enable_shared_from_this<Unit>
{
    public:
        Unit(int id,
            weak_ptr<Faction> faction);
        virtual ~Unit() { }

        int getId() const {
            return _id;
        }

        void update(float dt, float t);

        void setEntity(shared_ptr<UnitEntity> entity) {
            _entity = entity;
        }

        shared_ptr<UnitEntity> getEntity() const {
            return _entity;
        }

        int getX() const {
            return _x;
        }

        int getY() const {
            return _y;
        }

        int getMovePoints() const {
            return _mp;
        }

        float getSpeed() const {
            return _speed;
        }

        virtual void activate(shared_ptr<GridInput> grid_input);
        virtual void deactivate();

        void setTile(shared_ptr<Tile> tile);

        weak_ptr<Tile> getTile() {
            return _tile;
        }

        void resetPoints()
        {
            // FIXME: implement
        }

    protected:
        int _mp = 2;
        float _speed = 2.0f;

    private:
        int _id;
        int _x = 0;
        int _y = 0;

        weak_ptr<Faction> _faction;
        shared_ptr<UnitEntity> _entity;
        weak_ptr<Tile> _tile;
};

} // namespace Prismer
