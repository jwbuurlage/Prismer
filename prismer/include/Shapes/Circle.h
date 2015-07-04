#include <Arya.h>

#include "Shape.h"

namespace Prismer {

class Circle : public Shape
{
    public:
        Circle(int id,
            weak_ptr<Faction> faction,
            vector<ColorID> colors);
        
        virtual ~Circle() { }
};

}// namespace Prismer
