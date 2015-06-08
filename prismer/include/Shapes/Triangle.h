#include <Arya.h>

#include "Shape.h"

namespace Prismer {

class Triangle : public Shape
{
    public:
        Triangle(int id,
            shared_ptr<GameSession> session,
            vector<ColorID> colors);
        
        virtual ~Triangle() { }
};

}// namespace Prismer
