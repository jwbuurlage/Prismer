#include "GameLogger.h"
#include "Unit.h"
#include "UnitInfo.h"
#include "GameSession.h"
#include "Cells.h"
#include <cmath>

#ifdef _WIN32
#define M_PI 3.14159265358979323846
#endif

using Arya::Root;

Unit::Unit(int _type, int _id, GameSession* _session) : session(_session), id(_id)
{
    entity = 0;
    position = vec3(0.0f);
    yaw = 0;

    setType(_type);
    factionId = -1;
    local = false;
    obsolete = false;
    refCount = 0;

    lastActionTime = 0;
    targetUnit = 0;
    unitState = UNIT_IDLE;

    selected = false;
    currentCell = 0;

    health = unitInfo->maxHealth;
    timeSinceLastAttack = unitInfo->attackSpeed + 1.0f;
    timeSinceLastAttackRequest = 2.0f;
    dyingTime = 0.0f;
    timeSinceLastScriptUpdate = 0.0f;
}

Unit::~Unit()
{
    if(targetUnit) targetUnit->release();
    //if(entity) entity->setObsolete();
    //deleteScriptData();
    setCell(0);
    session->destroyUnit(id);
}

void Unit::setEntity(Entity* ent)
{
    entity = ent;
}

void Unit::setSelected(bool _sel)
{
//    if(!(selected == _sel))
//    {
//        if(selectionDecal)
//        {
//            if(_sel)
//                Arya::Decals::shared().addDecal(selectionDecal);
//            else
//                Arya::Decals::shared().removeDecal(selectionDecal);
//        }
//    }
    selected = _sel;
}

void Unit::setType(int _type)
{
    type = _type;
    unitInfo = getUnitInfo(_type);
    if(unitInfo == 0)
        GameLogError << "UnitInfo for type " << type << " not found! This will crash" << endLog;
}

void Unit::setPosition(const vec3& pos)
{
    position = pos;
    if(entity) entity->setPosition(pos);
    if(currentCell) setCell(currentCell->cellList->cellForPosition(getPosition2()));
    //if(selectionDecal) selectionDecal->setPos(getPosition2());
}

void Unit::setCell(Cell* newCell)
{
    if(currentCell != newCell)
    {
        if(currentCell) currentCell->remove(id);
        currentCell = newCell;
        if(currentCell) currentCell->add(id);
    }
}

void Unit::setCellFromList(CellList* cl)
{
    setCell(cl->cellForPosition(getPosition2()));
}

void Unit::update(int gameTime)
{
    //For any units referenced by this unit we must check if they are obsolete
    //Currently the only referenced unit is targetUnit
    if(targetUnit && !targetUnit->isAlive())
    {
        targetUnit->release();
        targetUnit = 0;
        if(unitState == UNIT_ATTACKING || unitState == UNIT_ATTACKING_OUT_OF_RANGE)
            setUnitState(UNIT_IDLE);
    }

    if( gameTime <= lastActionTime ) return;
    float timeElapsed = 0.001f*(gameTime - lastActionTime);
    lastActionTime = gameTime;

    //the current values of position and other states
    //are valid at the timestamp 'lastActionTime' when this function was called
    //timeElapsed is the time since these variables were 'valid'
    //this update function must now update all variables up to gameTime

    //TODO
    //This is not really correct: timeElapsed is not the actual elapsed time
    timeSinceLastAttackRequest += timeElapsed;
    timeSinceLastAttack += timeElapsed;
    timeSinceLastScriptUpdate += timeElapsed;

    //if(timeSinceLastScriptUpdate > 1.0f)
    //{
    //    if(session->isServer())
    //    {
    //        getInfo()->onUpdate(this, timeSinceLastScriptUpdate);
    //    }
    //    timeSinceLastScriptUpdate -= 1.0f;
    //}

    if(unitState == UNIT_IDLE)
        return;

    if(unitState == UNIT_DYING)
    {
        dyingTime += timeElapsed;
        //TODO: Use death animation time here
        if(dyingTime > 0.8f) markForDelete();
        return;
    }

    if(obsolete)
    {
        GameLogDebug << "Unit " << id << " is obsolete but has state " << unitState << endLog;
        setUnitState(UNIT_DYING);
        return;
    }

    //If we are attacking, we must always set the 'path' to the position of the target unit.
    if(unitState == UNIT_ATTACKING || unitState == UNIT_ATTACKING_OUT_OF_RANGE)
    {
        if(!targetUnit)
        {
            GameLogWarning << "Unit (" << id << ") is attacking, but no target unit" << endLog;
            setUnitState(UNIT_IDLE);
            return;
        }
        pathNodes.clear();
        pathNodes.push_back(targetUnit->getPosition2());
    }

    // Rotation and movement
    // Even when we are in attacking mode we must still
    // check if we are faced towards the target
    // so these calculations are always done first
    // before checking what to do in the current state

    vec2 targetPosition(getPosition2());
    if(pathNodes.empty())
    {
        GameLogWarning << "Unit (" << id << ") is in state " << unitState << " and has empty pathNodes array" << endLog;
    }
    else
    {
        targetPosition = pathNodes[0];
    }

    vec2 diff = targetPosition - getPosition2();
    float difflength = glm::length(diff);

    float newYaw = (difflength < 0.1 ? getYaw() : (180.0f/M_PI)*atan2(-diff.x, -diff.y));
    float yawDiff = newYaw - getYaw();
    //make sure it is in the [-180,180] range
    //so that a small rotation is always a small number
    //instead of 359 degrees
    if(yawDiff > 180.0f) yawDiff -= 360.0f;
    else if(yawDiff < -180.0f) yawDiff += 360.0f;

    //At this point we can have a switch or some if statements
    //to decide what to do for each unit state.
    //After this the unit will always be rotated to their target
    //angle (we assume this can be done in any state, even during attacks)
    //If canMove is left to true it will also move when fully rotated
    bool canMove = true;
    if(unitState == UNIT_ATTACKING || unitState == UNIT_ATTACKING_OUT_OF_RANGE)
    {
        bool inRange = glm::distance(getPosition2(), targetUnit->getPosition2())
            < targetUnit->getInfo()->radius + unitInfo->attackRadius;
        bool inAngleRange = yawDiff > -20.0f && yawDiff < 20.0f;

        if(inRange && inAngleRange)
        {
            if(!unitInfo->canMoveWhileAttacking)
                canMove = false;

            if(unitState != UNIT_ATTACKING)
            {
                //If the time since last attack is LESS we do not want to reset
                //it because then the unit could attack much faster by going
                //in-out-in-out of range. When the time is MORE then we want to
                //cap it because otherwise it could build up attacks

                if(timeSinceLastAttack > unitInfo->attackSpeed)
                    timeSinceLastAttack = unitInfo->attackSpeed;
                setUnitState(UNIT_ATTACKING);
            }

            while(timeSinceLastAttack >= unitInfo->attackSpeed)
            {
                timeSinceLastAttack -= unitInfo->attackSpeed;

                targetUnit->receiveDamage(unitInfo->damage, this);

                //When the unit dies the server sends a packet
                //The client leaves the unit alive untill it receives the packet
//                if(session->isServer())
//                {
//                    //TODO: just call session->onUnitDied() which handles this!!
//                    ServerGameSession* serverSession = (ServerGameSession*)session;
//                    if(!targetUnit->isAlive())
//                    {
//                        targetUnit->getInfo()->onDeath(targetUnit);
//
//                        //Note that the unit was alive before this damage so this must have killed it
//                        //Therefore we can send the death packet here
//                        Packet* pak = serverSession->createPacket(EVENT_UNIT_DIED);
//                        *pak << targetUnit->id;
//                        serverSession->sendToAllClients(pak);
//                        targetUnit->markForDelete();
//                        targetUnit->release();
//                        targetUnit = 0;
//                        setUnitState(UNIT_IDLE);
//                        return;
//                    }
//                }
            }
        }
        else
        {
            if(unitState != UNIT_ATTACKING_OUT_OF_RANGE)
                setUnitState(UNIT_ATTACKING_OUT_OF_RANGE);
        }
    }

    float deltaYaw = timeElapsed * unitInfo->yawSpeed;
    //check if we reached target angle
    if( abs(yawDiff) < deltaYaw )
    {
        //Target angle reached
        setYaw(newYaw);

        //A part of the current frametime went into rotating
        //we must now calculate how many frametime is left for moving
        float remainingTime = (deltaYaw - abs(yawDiff)) / unitInfo->yawSpeed;

        if(canMove)
        {
            //When we are close to the target, we might go past the target because
            //of high speed or high frametime so we have to check for this
            float distanceToTravel = remainingTime * unitInfo->speed;

            vec2 newPosition;
            if( distanceToTravel >= difflength )
            {
                newPosition = targetPosition;
                setUnitState(UNIT_IDLE);
                //TODO: Keep looping this complete movement code untill remainingTime is zero!!!!
                if(!pathNodes.empty()) pathNodes.erase(pathNodes.begin());
            }
            else
                newPosition = getPosition2() + distanceToTravel * glm::normalize(diff);

            //TODO
            //Map* map = session->getMap();
            //float height = (map ? map->heightAtGroundPosition(newPosition.x, newPosition.y) : 0.0f);
            float height = 0.0;
            //if(!collides(newPosition,height))
            //{
                setPosition(vec3(newPosition.x, newPosition.y, height));
            //}
        }
    }
    else
    {
        //Target angle not reached, rotate
        if(yawDiff < 0) deltaYaw = -deltaYaw;
        setYaw( getYaw() + deltaYaw );
    }
}

void Unit::setUnitState(UnitState state)
{
    if(unitState == UNIT_DYING)
        return;

    unitState = state;

#ifndef SERVERONLY
    if(!entity) return; //server
    Arya::GraphicsComponent* gr = entity->getGraphics();
    if(!gr) return;

    switch(unitState)
    {
        case UNIT_IDLE:
            setTintColor(vec3(0.0,0.0,0.0));
            gr->setAnimation(unitInfo->animationIdle.c_str());
            break;

        case UNIT_RUNNING:
            setTintColor(vec3(0.0,1.0,0.0));
            gr->setAnimation(unitInfo->animationMove.c_str());
            break;

        case UNIT_ATTACKING_OUT_OF_RANGE:
            setTintColor(vec3(0.0,0.0,1.0));
            gr->setAnimation(unitInfo->animationAttackOutOfRange.c_str());
            break;

        case UNIT_ATTACKING:
            setTintColor(vec3(1.0,0.0,0.0));
            gr->setAnimation(unitInfo->animationAttack.c_str());
            gr->setAnimationTime(unitInfo->attackSpeed);
            break;

        case UNIT_DYING:
            setTintColor(vec3(1.0,1.0,1.0));
            gr->setAnimation(unitInfo->animationDie.c_str());
            break;
    }
#endif
}

void Unit::setTargetUnit(int startTime, Unit* target)
{
    if(targetUnit)
        targetUnit->release();

    lastActionTime = startTime;
    targetUnit = target;
    targetUnit->addRef();

    if(unitState == UNIT_DYING)
        GameLogDebug << "Unit " << id << " probable error at setTargetUnit" << endLog;

    setUnitState(UNIT_ATTACKING_OUT_OF_RANGE);
}

void Unit::setUnitMovement(int startTime, const vec2& startPos, float startYaw, const std::vector<vec2>& newPath)
{
    if(targetUnit)
        targetUnit->release();
    targetUnit = 0;

    if(unitState == UNIT_DYING)
        GameLogDebug << "Unit " << id << " probable error at setTargetPosition" << endLog;

    //TODO:
    //float height = (session->getMap() ? session->getMap()->heightAtGroundPosition(startPos.x, startPos.y) : 0.0f);
    float height = 0.0f;

    lastActionTime = startTime;
    setPosition(vec3(startPos.x, startPos.y, height));
    setYaw(startYaw);

    pathNodes = newPath;
    if(pathNodes.empty())
        setUnitState(UNIT_IDLE);
    else
        setUnitState(UNIT_RUNNING);
}

void Unit::receiveDamage(float dmg, Unit* attacker)
{
    if(local)
    {
        if(timeSinceLastAttackRequest > 1.0f)
        {
            if(unitState == UNIT_IDLE) //and if not in passive mode
            {
                timeSinceLastAttackRequest = 0;
//TODO
//                Event& ev = Game::shared().getEventManager()->createEvent(EVENT_ATTACK_MOVE_UNIT_REQUEST);
//                ev << 1;
//                ev << id << attacker->getId();
//                ev.send();
            }
        }
    }

    health -= dmg;
    if(health < 0) health = 0;

    unitInfo->onDamage(this, attacker, dmg);

    //healthBar->sizeInPixels = vec2(25.0*getHealthRatio(), 3.0);

    //This is done when the death packet is received
    //if(!isAlive())
    //{
    //    setUnitState(UNIT_DYING);
    //}
}

void Unit::setTintColor(vec3 tC)
{
    tintColor = tC;
    //if(entity) entity->setTintColor(tC);
    //if(selectionDecal) selectionDecal->color = tC;
    //healthBar->fillColor = vec4(tintColor, 1.0);
}

void Unit::checkForEnemies()
{
#ifndef SERVERONLY
    if(unitState != UNIT_IDLE)
        return;

    //TODO: Instead of trying to stop the spam with a timer
    //we should actually check if this specific request has been
    //answered or not
    if(timeSinceLastAttackRequest < 0.3f) return;

    if(!currentCell) return;

    CellList* list = currentCell->cellList;
    if(!list) return;

    int curx = currentCell->cellx;
    int cury = currentCell->celly;

    Cell* c;
    float closestDistance = unitInfo->viewRadius;
    int closestId = -1;

    float d;
    // loop through neighbours
    for(int dx = -1; dx <= 1; ++dx)
        for(int dy = -1; dy <= 1; ++dy)
        {
            if(curx + dx >= list->gridSize || curx + dx < 0) continue;
            if(cury + dy >= list->gridSize || cury + dy < 0) continue;
            c = list->cellForIndex(curx + dx, cury + dy);
            // loop through
            for(unsigned i = 0; i < c->cellPoints.size(); ++i)
            {
                Unit* unit = session->getUnitById(c->cellPoints[i]);
                if(!unit) continue;
                if(unit->factionId == factionId) continue;
                d = glm::distance(position, unit->getPosition());

                if(d < closestDistance)
                {
                    closestDistance = d;
                    closestId = c->cellPoints[i];
                }
            }
        }

    if(closestId >= 0)
    {
        timeSinceLastAttackRequest = 0;
        //TODO: Instead of a single event for each unit we can combine
        //all attacks into a single event (currently there is a '1' as count)
//        Event& ev = Game::shared().getEventManager()->createEvent(EVENT_ATTACK_MOVE_UNIT_REQUEST);
//        ev << 1;
//        ev << id << closestId;
//        ev.send();
    }
#endif
}

//void Unit::serialize(Packet& pk)
//{
//    pk << type;
//    pk << factionId;
//    pk << position;
//    pk << (int)unitState;
//    pk << (int)pathNodes.size();
//    for(unsigned int i = 0; i < pathNodes.size(); ++i)
//        pk << pathNodes[i];
//    pk << (targetUnit ? targetUnit->getId() : 0);
//}
//
//void Unit::deserialize(Packet& pk)
//{
//    int _type;
//    pk >> _type;
//    setType(_type);
//    pk >> factionId;
//    pk >> position;
//    int _unitState;
//    pk >> _unitState;
//    unitState = (UnitState)_unitState;
//    int pathCount;
//    vec2 pos;
//    pk >> pathCount;
//    pathNodes.clear();
//    for(int i = 0; i < pathCount; ++i){ pk >> pos; pathNodes.push_back(pos); }
//    int targetUnitId;
//    pk >> targetUnitId;
//    if(targetUnitId) targetUnit = session->getUnitById(targetUnitId);
//}
//void Unit::getDebugText()
//{
//    GameLogDebug << "Unit id = " << id << endLog;
//    GameLogDebug << "Unit factionId = " << factionId << endLog;
//    GameLogDebug << "Unit obsolete = " << obsolete << endLog;
//    GameLogDebug << "Unit refCount = " << refCount << endLog;
//    GameLogDebug << "Unit health = " << health << endLog;
//    GameLogDebug << "Unit dyingTime = " << dyingTime << endLog;
//    GameLogDebug << "targetUnit = " << (targetUnit ? targetUnit->getId() : 0) << endLog;
//    GameLogDebug << "Unit state = " << unitState << endLog;
//    GameLogDebug << "------------------------------------" << endLog;
//}
