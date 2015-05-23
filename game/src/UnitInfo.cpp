#include "UnitInfo.h"
#include <map>

using std::map;

map<int,UnitInfo*> unitInfoList;

UnitInfo* UnitInfo::getFromType(int type)
{
    auto iter = unitInfoList.find(type);
    if (iter == unitInfoList.end() ) return 0;
    return iter->second;
}

UnitInfo* UnitInfo::getFromType(const string& name)
{
    for (auto info : unitInfoList)
        if (info.second->displayname == name || info.second->modelname == name)
            return info.second;
    return 0;
}

UnitInfo::UnitInfo(int type) : typeId(type)
{
    unitInfoList[typeId] = this;

    //default values
    animationIdle = "stand";
    animationMove = "run";
    animationAttack = "attack";
    animationAttackOutOfRange = "crouch_walk";
    animationDie = "death_fallback";
}

UnitInfo::~UnitInfo()
{
    auto iter = unitInfoList.find(typeId);
    if (iter != unitInfoList.end() ) unitInfoList.erase(iter);
}

