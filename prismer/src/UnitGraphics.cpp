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

    auto ent = Arya::Entity::create();
    ent->setPosition(vec3(_grid_entity->boardToWorld(unit->getX(), unit->getY()), 1.0f));

    ent->setGraphics(triangle);
    ent->getGraphics()->setScale(5.0f);
}

} // namespace Prismer
