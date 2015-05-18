#include <Arya.h>

// TODO: discuss moving this to resource file

namespace Prismer {

enum ColorID : int
{
    na,
    red,
    blue,
    green
};

struct Color
{
    Color(int r, int g, int b) {
        red = r;
        blue = b;
        green = g;
        id = ColorID::na;
    };

    int red;
    int blue;
    int green;
    ColorID id;
};

struct Red : Color
{
    Red() {
        red = 1.0;
        blue = 0.0;
        green = 0.0;
        id = ColorID::red;
    }
};

struct Blue : Color
{
    Red() {
        red = 0.0;
        blue = 1.0;
        green = 0.0;
        id = ColorID::blue;
    }
};

} // namespace Prismer
