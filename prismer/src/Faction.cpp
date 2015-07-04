#include <Arya.h>

#include "Faction.h"

namespace Prismer {

Faction::Faction()
{

}

Faction::~Faction()
{

}

void Faction::beginTurn()
{
    if (_input)
        _input->activate();

    for (auto& unit : _units) {
        unit.resetPoints();
    }
}

void Faction::endTurn()
{
    if (_input)
        _input->deactivate();
}

} //namespace Prismer
