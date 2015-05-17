#include "UnitInfo.h"
#include <map>

using std::map;

map<int,UnitInfo*> unitInfoList;

UnitInfo* getUnitInfo(int type)
{
    auto iter = unitInfoList.find(type);
    if (iter == unitInfoList.end() ) return 0;
    return iter->second;
}

UnitInfo* getUnitInfo(const string& name)
{
    for (auto info : unitInfoList)
        if (info.second->displayname == name || info.second->modelname == name)
            return info.second;
    return 0;
}

void registerNewUnitInfo(UnitInfo* info)
{
    unitInfoList[info->typeId] = info;
}

UnitInfo::UnitInfo(int type) : typeId(type)
{
    registerNewUnitInfo(this);
    //default values
    animationIdle = "stand";
    animationMove = "run";
    animationAttack = "attack";
    animationAttackOutOfRange = "crouch_walk";
    animationDie = "death_fallback";
}

UnitInfo::~UnitInfo()
{
    //TODO: remove from unitInfoList
}

