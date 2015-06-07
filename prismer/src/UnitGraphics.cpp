#include <Arya.h>

#include "Unit.h"
#include "UnitGraphics.h"
#include "GridGraphics.h"
#include "GameLogger.h"

namespace Prismer {

UnitEntity::UnitEntity(shared_ptr<Unit> unit,
        shared_ptr<GridEntity> grid_entity)
    : _unit(unit), _grid_entity(grid_entity)
{
//    // we make a 'shapeEntity' and delegate most things to that
//
//    auto& root = Arya::Locator::getRoot();
//    auto triangle = root.getModelManager()->getModel("triangle");
//    triangle->setMaterial(root.getMaterialManager()->createMaterial(vec4(0.6f, 0.6f, 0.6f, 0.5f)));
//
//    _entity = Arya::Entity::create();
//    _entity->setPosition(vec3(_grid_entity->boardToWorld(_unit->getX(), _unit->getY()), 1.0f));
//
//    _entity->setGraphics(triangle);
//    _entity->getGraphics()->setScale(5.0f);
}

void UnitEntity::updateState()
{
    // change colors or whatever
}

void UnitEntity::update(float elapsed_time, float total_time)
{
//    // check where unit is, and move towards that
//    auto world_pos = _grid_entity->boardToWorld(_unit->getX(), _unit->getY());
//    auto diff = vec3(world_pos, 1.0) - _current_position;
//
//    if(glm::length(diff) < 0.1)
//        _current_position = vec3(world_pos, 1.0);
//    else
//        _current_position += glm::normalize(diff) * _unit->getSpeed() * _grid_entity->getScale() * elapsed_time;
//
//    _entity->setPosition(_current_position);
//    _entity->setYaw(total_time);
}

} // namespace Prismer
