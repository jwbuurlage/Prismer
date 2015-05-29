#include <memory>

#include <glm/glm.hpp>
#include "Arya.h"

namespace Prismer {

using std::shared_ptr;

class GameSession;

class GameSessionInput
{
    public:
        GameSessionInput(shared_ptr<GameSession> session);
        ~GameSessionInput() { };

        void init();

        void update(float elapsedTime);

    private: 
        shared_ptr<GameSession> _session;

        Arya::InputBinding bindingC, bindingL;
};

} // namespace Prismer
