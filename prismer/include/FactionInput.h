#pragma once

namespace Prismer {

class Faction;

class FactionInput
{
    public:
        FactionInput() { }
        ~FactionInput() = default;

        // bindings
        void activate();
        void deactivate();

    private:
        vector<Arya::InputBinding> _binds;
        weak_ptr<Faction> _faction;
};

} // namespace Prismer

