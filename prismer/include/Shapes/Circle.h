#include <Arya.h>

#include "Shape.h"

namespace Prismer {

class Circle : public Shape
{
    public:
        Circle(int id,
            shared_ptr<GameSession> session,
            vector<ColorID> colors);
        
        virtual ~Circle() { }
};

}// namespace Prismer
