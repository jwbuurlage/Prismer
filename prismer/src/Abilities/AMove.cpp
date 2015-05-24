#include "Abilities/AMove.h"
#include "Tile.h"
#include "GridInput.h"
#include "Unit.h"

#include "GameLogger.h"

namespace Prismer {

void AMove::perform()
{
    _grid_input->setActive(nullptr);
    _actor->setTile(_grid_input->getHovered());
}

bool AMove::isValid()
{
    auto hover= _grid_input->getHovered();
    if (!hover) {
        GameLogInfo << "No hovered tile" << endLog;
        return false;
    }

    if(auto origin = _actor->getTile().lock()) {
        if (!(_actor->getMovePoints() >= origin->distance(hover))) {
            GameLogInfo << "Not enough MP" << endLog;
            return false;
        }

        if (hover->getInfo()->hasUnit()) {
            GameLogInfo << "Unit at target" << endLog;
            return false;
        }

        return true;
    }

    GameLogInfo << "Could not lock tile" << endLog;
    return false;
}

void AMove::activate(shared_ptr<Unit> actor,
                shared_ptr<GridInput> grid_input)
{
    Ability::activate(actor, grid_input);

    auto input = Arya::Locator::getRoot().getInputSystem();

    keyBinding = input->bind("g", [this](bool down, const Arya::MousePos&) {
            if (down && this->isValid()) {
                this->perform();
                return true;
            }
            return false;
        });
}

void AMove::deactivate()
{
    keyBinding = nullptr;
}

} // namespace Prismer
