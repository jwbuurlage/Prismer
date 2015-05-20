#pragma once

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


// rgbColors = map<ColorID, tuple(int, 3)>; //rgbColors[ColorID::red) 

} // namespace Prismer
