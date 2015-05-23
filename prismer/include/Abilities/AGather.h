#include <Abilities/Ability.h>

namespace Prismer {

class AGather : public Ability
{
    public:
        AGather() { }

        virtual void perform(shared_ptr<Unit> actor);
        virtual bool isValid(shared_ptr<Unit> actor);

        virtual void activate(shared_ptr<Unit> actor);
        virtual void deactivate();
};

} // namespace Prismer
