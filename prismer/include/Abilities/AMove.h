#include <Abilities/Ability.h>
#include <AryaBinding.h>

namespace Prismer {

class AMove : public Ability
{
    public:
        AMove() { }

        virtual void perform() override;
        virtual bool isValid() override;

        virtual void activate(shared_ptr<Unit> actor,
                shared_ptr<GridInput> grid_input) override;
        virtual void deactivate() override;

    private:
        Arya::InputBinding keyBinding;
};

} // namespace Prismer
