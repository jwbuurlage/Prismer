#include <memory>

#include "Shapes/Shape.h"

namespace Prismer {

void Shape::addColor(ColorID color)
{

}

void Shape::activate(shared_ptr<GridInput> grid_input)
{
    for (auto& ability : _abilities) {
        ability->activate(shared_from_this(), grid_input);
    }
}

void Shape::deactivate()
{
    for (auto& ability : _abilities) {
        ability->deactivate();
    }
}

} // namespace Prismer
