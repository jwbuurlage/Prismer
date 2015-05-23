#include <memory>

#include "Shapes/Circle.h"
#include "Abilities/AGather.h"
#include "Abilities/AMove.h"

#include "GameLogger.h"
#include "Colors.h"

namespace Prismer {

using std::make_shared;

Circle::Circle(vector<ColorID> colors) : Shape(colors)
{
    _abilities.push_back(make_shared<AGather>());
    _abilities.push_back(make_shared<AMove>());

    GameLogInfo << "Circle() -- colors:" << endLog;
    for (auto& color_bag : colors) {
        GameLogInfo << "(" << color_bag << ")" << endLog;
    }
}

} // namespace Prismer
