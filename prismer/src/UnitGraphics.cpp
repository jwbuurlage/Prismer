#include <Arya.h>

#include "Unit.h"
#include "UnitGraphics.h"
#include "GridGraphics.h"

namespace Prismer {

UnitEntity::UnitEntity(shared_ptr<Unit> unit,
        shared_ptr<GridEntity> grid_entity)
    : _unit(unit), _grid_entity(grid_entity)
{
    auto& root = Arya::Locator::getRoot();
    auto triangle = root.getModelManager()->getModel("triangle");
    triangle->setMaterial(root.getMaterialManager()->createMaterial(vec4(0.6f, 0.6f, 0.6f, 0.5f)));

    _entity = Arya::Entity::create();
    _entity->setPosition(vec3(_grid_entity->boardToWorld(_unit->getX(), _unit->getY()), 1.0f));

    _entity->setGraphics(triangle);
    _entity->getGraphics()->setScale(5.0f);
}

void UnitEntity::update()
{
    _entity->setPosition(vec3(_grid_entity->boardToWorld(_unit->getX(), _unit->getY()), 1.0f));
}

} // namespace Prismer
