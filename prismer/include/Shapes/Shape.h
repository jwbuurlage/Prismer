#pragma once

#include <Arya.h>
#include <vector>
#include <memory>

#include "../Colors.h"
#include "../Abilities/Ability.h"

namespace Prismer {

using std::vector;
using std::shared_ptr;

//class Arya::RenderableComponent;

class Shape
{
    public:
        Shape(vector<ColorID> colors) : _colors(colors) { };

        const vector<shared_ptr<Ability>>& getAbilities() const {
            return _abilities;
        }

    protected:
        int _max_health;
        int _movement_points;
        vector<shared_ptr<Ability>> _abilities;
        vector<ColorID> _colors;
        Arya::GraphicsComponent _graphics;
};

} // namespace Prismer
