#include "Circle.h"
#include "GameLogger.h"

namespace Prismer {

Circle::Circle(multiset<Color> colors)
{
    GameLogInfo << "Circle() -- colors:" << endLog;
    for (auto& color_bag : colors) {
        GameLogInfo << "(" << color_bag.first.id << ", " <<
            color_bag.second << ")" endLog;
    }
}

} // namespace Prismer
