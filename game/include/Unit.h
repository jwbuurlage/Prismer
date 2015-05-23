#pragma once 

#include "Arya.h"
#include <vector>
using std::vector;

using Arya::Entity;

typedef enum
{
    UNIT_IDLE,
    UNIT_RUNNING,
    UNIT_ATTACKING_OUT_OF_RANGE,
    UNIT_ATTACKING,
    UNIT_DYING
} UnitState;

typedef enum
{
    STANCE_AGGRESSIVE,
    STANCE_DEFENSIVE,
    STANCE_PASSIVE
} UnitStance;

class Packet;
class Unit;
class Map;
class UnitInfo;
class GameSession;
class ServerGameSession;
//class LuaScriptData;
struct CellList;
struct Cell;

class Unit
{
    private:
        //Only GameSession can create units
        //This ensures that every unit is registered
        //at the session and that every unit has a pointer
        //to the session
        friend class GameSession;
        GameSession* const session;

        Unit(int _type, int id, GameSession* session);
    public:
        ~Unit(); //unregisters itself at session

        GameSession* getSession() const { return session; }

        //! Set the position of the unit
        //! This updates the cell list and so on
        void setPosition(const vec3& pos);
        //! Get the full position or the 2d position
        inline vec3 getPosition() const { return position; }
        inline vec2 getPosition2() const { return vec2(position.x,position.y); }

        //! Set and get the yaw of the unit
        void setYaw(float y){ yaw = y; if(entity) entity->setYaw(y); }
        float getYaw() const { return yaw; }

        //! Update the unit up to a specified game time
        void update(int gameTime); //in ms

        //! Attach a visible entity to the unit
        void setEntity(shared_ptr<Entity> e) { entity = e; }
        Entity* getEntity() const { return entity.get(); }

        //! Add the unit to the appropriate cell
        void setCellFromList(CellList* cl);

        //! Get the current cell that the unit is in
        Cell* getCell() const { return currentCell; }

        //! Call this only for local units:
        //! It checks for enemies and if they are close it
        //! will auto attack them if needed
        void checkForEnemies();

        int getType() const { return type; }
        void setType(int _type);
        UnitInfo* getInfo() const { return unitInfo; }

        void setSelected(bool sel);
        bool isSelected() const { return selected; }

        //! Set the movement of a unit
        void setUnitMovement(int startTime, const vec2& startPos, float startYaw, const std::vector<vec2>& newPath);

        //! Get the path of a unit
        const std::vector<vec2>& getTargetPath() const { return pathNodes; }

        //! Get the target unit (is zero if not targetting any unit)
        Unit* getTargetUnit() const { return targetUnit; }

        //! Set a target unit, for following and attacking
        void setTargetUnit(int startTime, Unit* unit);

        //! Set the unit state
        void setUnitState(UnitState state);
        
        //! Get the unit state
        UnitState getUnitState() const { return unitState; }

        //! Get the health of a unit
        float getHealth() const { return health; }
        //float getHealthRatio() const { return health / unitInfo->maxHealth; }

        //! Check if the unit is alive
        bool isAlive() const { return (!obsolete && health > 0); }

        //! Kill the unit and display dying animations before actually deleting it
        void makeDead(){ health = 0; setUnitState(UNIT_DYING); dyingTime = 0.0f; };

        //! If this returns true the unit will be deleted by game session
        bool readyToDelete() const { return obsolete && refCount <= 0; }

        //! After calling this, the unit will be deleted as soon as there are no more references to it
        void markForDelete(){ obsolete = true; }

        //! Add a reference. Used if a unit targets another unit
        void addRef() { ++refCount; }

        //! Delete the reference
        void release() { --refCount; }

        //void setScreenPosition(vec2 sPos) { screenPosition = sPos; }
        //vec2 getScreenPosition() const { return screenPosition; }
        void setTintColor(vec3 tC);

        //! Get the unit id
        int getId() const { return id; }

        //! Get the faction id that the unit belongs to
        int getFactionId() const { return factionId; }
        //! Set the faction id
        void setFactionId(int id) { factionId = id; }

        //! Set if this unit is the unit of a local player
        void setLocal(bool value = true){ local = value; }
        //! Check if the unit is owned by the local player
        bool isLocal() const { return local; }

        //float getRadius() const { return unitInfo->radius; }

        //void serialize(Packet& pk);
        //void deserialize(Packet& pk);

        //Must be public because it must be accessed by some
        //scripting functions
        //LuaScriptData* customData;
    private:
        //entity->position and entity->yaw are predicted onscreen values, not always the same as Unit::position and Unit::yaw
        shared_ptr<Entity> entity;
        //The position after the last update (gameTime) is stored here
        vec3 position; 
        float yaw;

        UnitInfo* unitInfo;
        int type;
        const int id;
        int factionId;
        bool local; //This must be false on the server. When true the update functions will do auto-attack requests
        bool obsolete;
        int refCount;

        //! If the unit gets a new action (walk, attack, etc) by an incoming Event
        //! then lastActionTime holds the starting time of that event
        //! Unit::update will update from that time till the current time
        //! This way we can (sort of) handle incoming events with timestamps in the past
        int lastActionTime;
        vector<vec2> pathNodes;
        Unit* targetUnit;
        UnitState unitState;

        bool selected;
        Cell* currentCell;

        float health;
        float timeSinceLastAttackRequest; //to prevent spamming the server
        float timeSinceLastAttack;
        float dyingTime;
        float timeSinceLastScriptUpdate;

        //Rect* healthBar;
        //Decal* selectionDecal;
        //Visionary* unitVisionary;
        //vec2 screenPosition;
        vec3 tintColor;

        //These functions are implemented in Scripting.cpp and it just calls 'new LuaScriptData' but it needs luabind headers
        //It is called in the Unit constructor, and in the deconstructor we call the delete one
        //void createScriptData();
        //void deleteScriptData();

        //! Remove the unit from the old cell and add it to the new one
        void setCell(Cell* newCell);

        //! Check if the unit can walk here
        bool collides(vec2 checkPosition, float checkHeight);

        //! Takes damage and calls the scripts OnDamage
        //! Called by Unit::update
        void receiveDamage(float dmg, Unit* attacker);
};
