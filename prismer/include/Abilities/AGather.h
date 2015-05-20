#include <Abilities/Ability.h>

namespace Prismer {

class AGather : public Ability
{
    public:
        AGather() { }

        virtual void perform(shared_ptr<TileInfo> tile,
                shared_ptr<Unit> actor) override;

        virtual bool isValid(shared_ptr<TileInfo> tile) override;
};

} // namespace Prismer
