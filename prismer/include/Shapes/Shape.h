#pragma once

#include <Arya.h>
#include <vector>
#include <memory>

#include "../Unit.h"
#include "../Colors.h"
#include "../Abilities/Ability.h"

namespace Prismer {

using std::vector;
using std::shared_ptr;

//class Arya::RenderableComponent;

class Shape
    : public Unit
{
    public:
        Shape(int id,
            weak_ptr<Faction> faction,
            vector<ColorID> colors) : Unit(id, faction), _colors(colors)
        { }

        virtual ~Shape() { }

        const vector<shared_ptr<Ability>>& getAbilities() const {
            return _abilities;
        }

        void addColor(ColorID color);

        virtual void activate(shared_ptr<GridInput> grid_input) override;
        virtual void deactivate() override;

    protected:
        int _max_health = 0;
        int _movement_points = 0;
        vector<shared_ptr<Ability>> _abilities;
        vector<ColorID> _colors;
        Arya::GraphicsComponent _graphics;
};

} // namespace Prismer
