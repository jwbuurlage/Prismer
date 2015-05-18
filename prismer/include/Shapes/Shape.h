#include <Arya.h>
#include <set>
#include <vector>

namespace Prismer {

using std::multiset;
using std::vector;

class Color;
class Arya::RenderableComponent;

class Shape
{
    public:
        Shape(multiset<Color> colors);

    private:
//        Arya::RenderableComponent _graphic;
//        vector<Ability> _abilities;
};

} // namespace Prismer
