#include <memory>

#include "Shapes/Triangle.h"
#include "Abilities/AMove.h"

#include "GameLogger.h"
#include "Colors.h"

namespace Prismer {

using std::make_shared;

Triangle::Triangle(int id,
            shared_ptr<GameSession> session,
            vector<ColorID> colors)
     : Shape(id, session, colors)
{
    _abilities.push_back(make_shared<AMove>());

    _mp = 5;

    GameLogInfo << "Triangle() -- colors:" << endLog;
    for (auto& color_bag : colors) {
        GameLogInfo << "(" << color_bag << ")" << endLog;
    }
}

} // namespace Prismer
