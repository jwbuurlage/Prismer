#include <Arya.h>

#include "Shape.h"

namespace Prismer {

class Triangle : public Shape
{
    public:
        Triangle(int id,
            weak_ptr<Faction> faction,
            vector<ColorID> colors);
        
        virtual ~Triangle() { }
};

}// namespace Prismer
