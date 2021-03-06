#include <Arya.h>

#include "Unit.h"
#include "GridGraphics.h"
#include "Shapes/CircleGraphics.h"

namespace Prismer {

CircleEntity::CircleEntity(shared_ptr<Unit> unit,
        shared_ptr<GridEntity> grid_entity)
    : ShapeEntity(unit, grid_entity)
{
    // we make a 'shapeEntity' and delegate most things to that
    auto& root = Arya::Locator::getRoot();
    auto triangle = root.getModelManager()->getModel("circle");
    triangle->setMaterial(root.getMaterialManager()->createMaterial(vec4(0.6f, 0.6f, 0.6f, 0.5f)));

    _entity = Arya::Entity::create();
    _entity->setPosition(vec3(_grid_entity->boardToWorld(_unit->getX(), _unit->getY()), 1.0f));

    _entity->setGraphics(triangle);
    _entity->getGraphics()->setScale(5.0f);
}

void CircleEntity::updateState()
{
    // change colors or whatever
}

void CircleEntity::update(float elapsed_time, float total_time)
{
    // check where unit is, and move towards that
    auto world_pos = _grid_entity->boardToWorld(_unit->getX(), _unit->getY());
    auto diff = vec3(world_pos, 1.0) - _current_position;

    if(glm::length(diff) < 0.1)
        _current_position = vec3(world_pos, 1.0);
    else
        _current_position += glm::normalize(diff) * _unit->getSpeed() * _grid_entity->getScale() * elapsed_time;

    _entity->setPosition(_current_position);
}

} // namespace Prismer
