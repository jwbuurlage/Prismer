#pragma once

#include <memory>
#include <vector>

#include <AryaBinding.h>

namespace Prismer {

using std::weak_ptr;
using std::vector;

class Faction;

class FactionInput
{
    public:
        FactionInput(weak_ptr<Faction> faction)
            : _faction(faction)
        { }
        ~FactionInput() = default;

        // bindings
        void activate();
        void deactivate();

    private:
        vector<Arya::InputBinding> _bindings;
        weak_ptr<Faction> _faction;
};

} // namespace Prismer

