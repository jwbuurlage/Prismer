#include <Arya.h>

#include "Faction.h"
#include "FactionInput.h"

namespace Prismer {

void FactionInput::activate()
{
    auto input = Arya::Locator::getRoot().getInputSystem();

    _bindings.push_back(input->bind("backspace",
        [this](bool down, const Arya::MousePos&) {
            auto sfaction = _faction.lock();
            if (down && sfaction)
                sfaction->endTurn();
            return true;
        }));
}

void FactionInput::deactivate()
{
    _bindings.clear();
}

} // namespace Prismer
