#include <memory>

#include <glm/glm.hpp>
#include "Arya.h"

namespace Prismer {

using std::shared_ptr;

class GameSessionClient;

class GameSessionInput
{
    public:
        GameSessionInput(shared_ptr<GameSessionClient> session);
        ~GameSessionInput() { };

        void init();

        void update(float elapsedTime);

    private: 
        shared_ptr<GameSessionClient> _session;

        Arya::InputBinding bindingC, bindingL;
};

} // namespace Prismer
