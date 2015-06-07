#include <memory>

#include "Shapes/Circle.h"
#include "Abilities/AGather.h"
#include "Abilities/AMove.h"

#include "GameLogger.h"
#include "Colors.h"

namespace Prismer {

using std::make_shared;

Circle::Circle(int id,
            shared_ptr<GameSession> session,
            vector<ColorID> colors)
     : Shape(id, session, colors)
{
    _abilities.push_back(make_shared<AGather>());
    _abilities.push_back(make_shared<AMove>());

    GameLogInfo << "Circle() -- colors:" << endLog;
    for (auto& color_bag : colors) {
        GameLogInfo << "(" << color_bag << ")" << endLog;
    }
}

} // namespace Prismer
